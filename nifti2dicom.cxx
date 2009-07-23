//    This file is part of Nifti2Dicom.
//    Copyright (C) 2008,2009 Daniele E, Domenichelli
//
//    Nifti2Dicom is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Nifti2Dicom is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

// $Id$



#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesWriter.h>
#include <itkNumericSeriesFileNames.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkOrientImageFilter.h>

#include <itkMetaDataDictionary.h>
#include <itkMetaDataObject.h>

//#include <itksys/SystemTools.hxx>
#include <gdcm/src/gdcmFile.h>
#include <gdcm/src/gdcmUtil.h>

#include <iostream>
#include <string>
#include <vector>


#define DEBUG



#include "nifti2dicomCommandLineParser.h"




/*
Alcune note:
  1. seriesdescription: l'elenco fisso deve avere questi campi:
    1. fMRI;
    2. DTI-ft;
    3. Image Fusion;
    4. Other/postprocessing. 
  2. accessionNumberb: di default il campo deve essere compilato a mano dall'utente; se
     invece viene data un header di riferimento, per la copia dei dati anagrafici ALLORA
     si prevede la doppia opzione:
     1. (default) da inserire a mano;
     2. copia dall'header DICOM ma dà un warning per avvertire che ci saranno due/o più
        immagini con lo stesso Accession Number.
  3. studyistanceUID e seriesnumber: il filtro li completa già da solo (sono campi univoci
     creati a partire dalla data dell'esame, dall'ora) bisogna capire come vengono creati
     per controllare l'effettiva correttezza di tali campi.
  4. patientName: allo stato attuale di default viene settato a "GDCM" quindi loro lo
     devono cambiare a mano.
  5. alcune volte, non per tutte le immagini, viene inserito un gantrytilt anche quando
     non c'è, quindi bisogna capire come funziona il calcolo del gantrytilt e verificare
     la correttezza delle informazioni relative salvate nell'header. 
*/


void CopyDictionary (itk::MetaDataDictionary &fromDict,
                     itk::MetaDataDictionary &toDict);

void PrintDictionary (itk::MetaDataDictionary &Dict);

bool ReadDICOMTags(std::string file, itk::MetaDataDictionary &dict);



int main(int argc, char* argv[])
{
  try
  {
    nifti2dicomCommandLineParser parser;
    parser.parse(argc,argv);
  }
  catch (...)
  {
    std::cerr << "Unknown Exceprion" << std::endl;
    return EXIT_FAILURE;
  }






  // Base definitions
  const int Dimension = 3;
  const int DICOMDimension = 2;

  typedef unsigned int PixelType;
  typedef signed short DICOMPixelType;

  typedef itk::Image <PixelType, Dimension> ImageType;
  typedef itk::Image<DICOMPixelType, Dimension> DICOM3DImageType;
  typedef itk::Image<DICOMPixelType, DICOMDimension> DICOMImageType;


// -----------------------------------------------------------------------------
// Lettura dell'immagine
// -----------------------------------------------------------------------------


  // Reader
  typedef itk::ImageFileReader< ImageType >  ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( args.inputfile );

  try
  {
    std::cout << "Reading... " << std::flush;
    reader->Update();
    std::cout << "DONE" << std::endl;
  }
  catch ( itk::ExceptionObject & ex )
  {
    std::string message;
    message = ex.GetLocation();
    message += "\n";
    message += ex.GetDescription();
    std::cerr << message << std::endl;
    return EXIT_FAILURE;
  }


// -----------------------------------------------------------------------------
// Orientamento dell'immagine
// -----------------------------------------------------------------------------


  typedef itk::OrientImageFilter<ImageType,ImageType> OrienterType;
  OrienterType::Pointer orienter = OrienterType::New();
  orienter->UseImageDirectionOn();
  orienter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI); //Orient to RAI
  orienter->SetInput(reader->GetOutput());

  try
  {
    std::cout << "Orienting... " << std::flush;
    orienter->Update();
    std::cout << "DONE" << std::endl;
  }
  catch ( itk::ExceptionObject & ex )
  {
    std::string message;
    message = ex.GetLocation();
    message += "\n";
    message += ex.GetDescription();
    std::cerr << message << std::endl;
    return EXIT_FAILURE;
  }




// -----------------------------------------------------------------------------
// Riscalamento o cast dell'immagine
// -----------------------------------------------------------------------------


  typedef itk::RescaleIntensityImageFilter< ImageType, DICOM3DImageType > RescaleType;
  RescaleType::Pointer rescaleFilter;

  typedef itk::CastImageFilter < ImageType, DICOM3DImageType > CastType;
  CastType::Pointer cast;

  DICOM3DImageType::Pointer Image;

  if (args.rescale)
  {
    // Rescale
    rescaleFilter = RescaleType::New();

    rescaleFilter->SetInput(orienter->GetOutput());
    rescaleFilter->SetOutputMinimum( 0 );
    rescaleFilter->SetOutputMaximum( 2^11 -1 );

    try
    {
      std::cout << "Rescaling... " << std::flush;
      rescaleFilter->Update();
      std::cout << "DONE" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
      std::string message;
      message = ex.GetLocation();
      message += "\n";
      message += ex.GetDescription();
      std::cerr << message << std::endl;
      return EXIT_FAILURE;
    }


    Image = rescaleFilter->GetOutput();
  }
  else  
  {
    // Caster
    cast = CastType::New();

    cast->SetInput(orienter->GetOutput());

    try
    {
      std::cout << "Casting... " << std::flush;
      cast->Update();
      std::cout << "DONE" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
      std::string message;
      message = ex.GetLocation();
      message += "\n";
      message += ex.GetDescription();
      std::cerr << message << std::endl;
      return EXIT_FAILURE;
  }


  Image = cast->GetOutput();

  }

  ImageType::RegionType region = Image->GetLargestPossibleRegion();
  ImageType::SizeType dimensions = region.GetSize();

  unsigned int nbSlices = dimensions[2];


// -----------------------------------------------------------------------------
// Gestione dell'header DICOM
// -----------------------------------------------------------------------------



  typedef itk::ImageSeriesWriter<DICOM3DImageType, DICOMImageType> SeriesWriterType;
  typedef itk::GDCMImageIO ImageIOType;

  ImageIOType::Pointer gdcmIO = ImageIOType::New();


  // To keep the new series in the same study as the original we need
  // to keep the same study UID. But we need new series and frame of
  // reference UID's.

  itk::MetaDataDictionary& inputDict = reader->GetMetaDataDictionary();

  if (!args.dicomheaderfile.empty())
  {
    if (!ReadDICOMTags(args.dicomheaderfile, inputDict))
      return EXIT_FAILURE;

    std::string seriesUID = gdcm::Util::CreateUniqueUID( gdcmIO->GetUIDPrefix());
    std::string frameOfReferenceUID = gdcm::Util::CreateUniqueUID( gdcmIO->GetUIDPrefix());
    std::string studyUID;
    std::string sopClassUID;
    itk::ExposeMetaData<std::string>(inputDict, "0020|000d", studyUID);
    itk::ExposeMetaData<std::string>(inputDict, "0008|0016", sopClassUID);
    gdcmIO->KeepOriginalUIDOn();

    itk::EncapsulateMetaData<std::string>(inputDict, "0020|000d", studyUID);
    itk::EncapsulateMetaData<std::string>(inputDict, "0020|000e", seriesUID);
    itk::EncapsulateMetaData<std::string>(inputDict, "0020|0052", frameOfReferenceUID);

    std::string sopInstanceUID = gdcm::Util::CreateUniqueUID(gdcmIO->GetUIDPrefix());
    itk::EncapsulateMetaData<std::string>(inputDict, "0008|0018", sopInstanceUID);
    itk::EncapsulateMetaData<std::string>(inputDict, "0002|0003", sopInstanceUID);
  }






  if(!args.accessionnumber.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0008|0050", args.accessionnumber);


  if(!args.studydate.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0008|0020", args.studydate);
  if(!args.seriesdate.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0008|0021", args.seriesdate);
  if(!args.modality.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0008|0060", args.modality);
  if(!args.manufacturer.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0008|0070", args.manufacturer);
  if(!args.istitutionname.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0008|0080", args.istitutionname);
  if(!args.referphysicianname.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0008|0090", args.referphysicianname);
  if(!args.studydescription.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0008|1030", args.studydescription);
  if(!args.seriesdescription.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0008|103e", args.seriesdescription);
  if(!args.patientname.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0010|0010", args.patientname);
  if(!args.patientid.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0010|0020", args.patientid);
  if(!args.patientdob.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0010|0030", args.patientdob);
  if(!args.patientsex.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0010|0040", args.patientsex);
  if(!args.patientage.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0010|1010", args.patientage);
  if(!args.studyistanceuid.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0020|000d", args.studyistanceuid);
  if(!args.seriesistanceuid.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0020|000e", args.seriesistanceuid);
  if(!args.seriesnumber.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0020|0011", args.seriesnumber);
  if(!args.acquisitionnumber.empty())
    itk::EncapsulateMetaData<std::string>( inputDict, "0020|0012", args.acquisitionnumber);


//TODO settare correttamente BITS ALLOCATED (0028,0100)/ BITS STORED (0028,0101) / HIGH BIT (0028,0102)
// (pare impossibile con itk+gdcm)
/*
// Bits Allocated
  itk::EncapsulateMetaData<std::string>( inputDict, "0028|0100", "16");
// Bits Stored
  itk::EncapsulateMetaData<std::string>( inputDict, "0028|0101", "12");
// High Bit
  itk::EncapsulateMetaData<std::string>( inputDict, "0028|0102", "11");
*/

//TODO Controllare che non siano settati Rescale - Slope - Windows
//     o che siano settati, ma correttamente
// Al momento sembra che non si possa http://www.itk.org/Bug/view.php?id=3223
// (si può sempre non copiarli nel CopyDictionary)

  SeriesWriterType::DictionaryRawPointer dictionary[ nbSlices ];
  SeriesWriterType::DictionaryArrayType outputArray;


  PrintDictionary( inputDict );

  for (unsigned int i=0; i<nbSlices; i++)
  {
    dictionary[i] = new SeriesWriterType::DictionaryType;
    CopyDictionary(inputDict, *dictionary[i]);




    // Image Position Patient: This is calculated by computing the
    // physical coordinate of the first pixel in each slice.
    ImageType::PointType position;
    ImageType::SpacingType spacing = Image->GetSpacing();
    ImageType::IndexType index;

    index[0] = 0;
    index[1] = 0;
    index[2] = i;
    Image->TransformIndexToPhysicalPoint(index, position);

    itksys_ios::ostringstream value;

    // Image Number
    value.str("");
    value << i + 1;
    itk::EncapsulateMetaData<std::string>(*dictionary[i],"0020|0013",value.str());


    value.str("");
    value << position[0] << "\\" << position[1] << "\\" << position[2];
    itk::EncapsulateMetaData<std::string>(*dictionary[i],"0020|0032", value.str());

    // Slice Location: For now, we store the z component of the Image
    // Position Patient.
    value.str("");
    value << position[2];
    itk::EncapsulateMetaData<std::string>(*dictionary[i],"0020|1041", value.str());

    // Slice Thickness: For now, we store the z spacing
    value.str("");
    value << spacing[2];
    itk::EncapsulateMetaData<std::string>(*dictionary[i],"0018|0050", value.str());

    // Spacing Between Slices
    itk::EncapsulateMetaData<std::string>(*dictionary[i],"0018|0088", value.str());


    outputArray.push_back(dictionary[i]);
  }


// -----------------------------------------------------------------------------
// Scrittura dell'immagine
// -----------------------------------------------------------------------------


  // Writer
  typedef itk::NumericSeriesFileNames NameGeneratorType;
  NameGeneratorType::Pointer namesGenerator = NameGeneratorType::New();



  namesGenerator->SetStartIndex( 1 );
  namesGenerator->SetEndIndex( nbSlices );
  namesGenerator->SetIncrementIndex( 1 );

  itksys::SystemTools::MakeDirectory( args.outputdirectory.c_str() ); // Create directory if it does not exist yet

  namesGenerator->SetSeriesFormat( args.Format.c_str() );



  SeriesWriterType::Pointer seriesWriter = SeriesWriterType::New();


  seriesWriter->SetInput( Image );

  seriesWriter->SetImageIO( gdcmIO );
  seriesWriter->SetFileNames( namesGenerator->GetFileNames() );


  //seriesWriter->SetMetaDataDictionaryArray( reader->GetMetaDataDictionaryArray ); //TODO MetaDataDictionary in tutti i files o roba del genere?
  seriesWriter->SetMetaDataDictionaryArray( &outputArray );

  try
  {
    std::cout << "Writing... " << std::flush;
    seriesWriter->Update();
    std::cout << "DONE" << std::endl;
  }
  catch ( itk::ExceptionObject & ex )
  {
    std::cout << "Error Writing:" << std::endl;
    std::string message;
    message = ex.GetLocation();
    message += "\n";
    message += ex.GetDescription();
    std::cerr << message << std::endl;
    return EXIT_FAILURE;
  }

return EXIT_SUCCESS;
}







void CopyDictionary (itk::MetaDataDictionary &fromDict, itk::MetaDataDictionary &toDict)
{
  typedef itk::MetaDataDictionary DictionaryType;

  DictionaryType::ConstIterator itr = fromDict.Begin();
  DictionaryType::ConstIterator end = fromDict.End();
  typedef itk::MetaDataObject< std::string > MetaDataStringType;

  while( itr != end )
  {
    itk::MetaDataObjectBase::Pointer  entry = itr->second;

    MetaDataStringType::Pointer entryvalue = dynamic_cast<MetaDataStringType *>( entry.GetPointer() ) ;
    if( entryvalue )
      {
      std::string tagkey   = itr->first;
      std::string tagvalue = entryvalue->GetMetaDataObjectValue();
      itk::EncapsulateMetaData<std::string>(toDict, tagkey, tagvalue); 
      }
    ++itr;
  }
}

void PrintDictionary (itk::MetaDataDictionary &Dict)
{
  typedef itk::MetaDataDictionary DictionaryType;

  DictionaryType::ConstIterator itr = Dict.Begin();
  DictionaryType::ConstIterator end = Dict.End();
  typedef itk::MetaDataObject< std::string > MetaDataStringType;

  while( itr != end )
  {
    itk::MetaDataObjectBase::Pointer  entry = itr->second;

    MetaDataStringType::Pointer entryvalue = dynamic_cast<MetaDataStringType *>( entry.GetPointer() ) ;
    if( entryvalue )
    {
      std::string tagkey   = itr->first;
      std::string tagvalue = entryvalue->GetMetaDataObjectValue();

      std::cout << "(" << tagkey << ") " << tagvalue << std::endl;
    }
    ++itr;
  }
}

bool ReadDICOMTags(std::string file, itk::MetaDataDictionary &dict)
{
  const int DICOMDimension = 2;
  typedef signed short DICOMPixelType;

  typedef itk::Image<DICOMPixelType, DICOMDimension> DICOMImageType;
  typedef itk::ImageFileReader<DICOMImageType> ReaderType;
  typedef itk::GDCMImageIO ImageIOType;

  ReaderType::Pointer reader = ReaderType::New();
  ImageIOType::Pointer dicomIO = ImageIOType::New();
  reader->SetImageIO( dicomIO );

  reader->SetFileName( file );

  try
  {
    std::cout << "Reading DICOM Tags... " << std::flush;
    reader->Update();
    std::cout << "DONE" << std::endl;
  }
  catch ( itk::ExceptionObject & ex )
  {
    std::string message;
    message = ex.GetLocation();
    message += "\n";
    message += ex.GetDescription();
    std::cerr << message << std::endl;
    return false;
  }

  CopyDictionary( reader->GetMetaDataDictionary(), dict );
  return true;
}
