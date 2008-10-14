#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesWriter.h>
#include <itkNumericSeriesFileNames.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkCastImageFilter.h>



#define DEBUG

int main(int argc, char* argv[])
{

  // Arguments check
  if (argc < 3 || argc >6)
  {
    std::cout << "Usage: " << argv[0] << " <InputFile> <OutputDirectory> [Suffix = \".dcm\"] [Prefix = \"\"] [Rescale = OFF]" << std::endl;
    return EXIT_FAILURE;
  }

  std::string InputFile       = argv[1];
  std::string OutputDirectory = argv[2];
  std::string Suffix;
  std::string Prefix;

  if (argc >= 4)
    Suffix = argv[3];
  else
    Suffix = ".dcm";

  if (argc >= 5)
    Prefix = argv[4];
  else
    Prefix = "";

  std::string Format = OutputDirectory +  std::string("/")+ Prefix + std::string("%04d") + Suffix;

  bool doRescale = argc >= 6 ? true : false;

#ifdef DEBUG
  std::cout << "InputFile       = " << InputFile       << std::endl;
  std::cout << "OutputDirectory = " << OutputDirectory << std::endl;
  std::cout << "Suffix          = " << Suffix          << std::endl;
  std::cout << "Prefix          = " << Prefix          << std::endl;
  std::cout << "Format          = " << Format          << std::endl;
  std::cout << "doRescale       = " << doRescale       << std::endl;
#endif // DEBUG


  // Base definitions
  typedef unsigned char PixelType;
  const int Dimension = 3;
  typedef itk::Image <PixelType, Dimension> ImageType;

  typedef signed short DICOMPixelType;
  const int DICOMDimension = 2;
  typedef itk::Image<DICOMPixelType, DICOMDimension> DICOMImageType;

  typedef itk::Image<DICOMPixelType, Dimension> RescaledImageType;


  // Reader
  typedef itk::ImageFileReader< ImageType >  ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( InputFile );

  try
  {
    reader->Update();
  }
  catch(...)
  {
    std::cout<<"Error Reading"<<std::endl;
    return EXIT_FAILURE;
  }



  typedef itk::RescaleIntensityImageFilter< ImageType, RescaledImageType > RescaleType;
  RescaleType::Pointer rescale;

  typedef itk::CastImageFilter < ImageType, RescaledImageType > CastType;
  CastType::Pointer cast;

  if (doRescale)
  {
    // Rescale
    rescale = RescaleType::New();

    rescale->SetInput(reader->GetOutput());
    rescale->SetOutputMinimum( 0 );
    rescale->SetOutputMaximum( 2^11 -1 );

    try
    {
      rescale->Update();
    }
    catch(...)
    {
      std::cout<<"Error Rescaling"<<std::endl;
      return EXIT_FAILURE;
    }
  }
  else  
  {
    // Caster
    cast = CastType::New();

    cast->SetInput(reader->GetOutput());

    try
    {
      cast->Update();
    }
    catch(...)
    {
      std::cout<<"Error Casting"<<std::endl;
      return EXIT_FAILURE;
    }

  }

  // Writer
  typedef itk::ImageSeriesWriter<RescaledImageType, DICOMImageType> SeriesWriterType;
  typedef itk::GDCMImageIO ImageIOType;
//  typedef itk::GDCMSeriesFileNames NamesGeneratorType;


  typedef itk::NumericSeriesFileNames NameGeneratorType;
  NameGeneratorType::Pointer namesGenerator = NameGeneratorType::New();

  ImageType::RegionType region = reader->GetOutput()->GetLargestPossibleRegion();
  ImageType::SizeType dimensions = region.GetSize();

  namesGenerator->SetStartIndex( 1 );
  namesGenerator->SetEndIndex( dimensions[2] );
  namesGenerator->SetIncrementIndex( 1 );

  itksys::SystemTools::MakeDirectory( OutputDirectory.c_str() ); // Create directory if it does not exist yet

  namesGenerator->SetSeriesFormat( Format.c_str() );



//  NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();
  ImageIOType::Pointer gdcmIO = ImageIOType::New();
  SeriesWriterType::Pointer seriesWriter = SeriesWriterType::New();

//  namesGenerator->SetOutputDirectory( OutputDirectory );

  if(doRescale)
    seriesWriter->SetInput( rescale->GetOutput() );
  else
    seriesWriter->SetInput( cast->GetOutput() );

  seriesWriter->SetImageIO( gdcmIO );
  seriesWriter->SetFileNames( namesGenerator->GetFileNames() );
  //seriesWriter->SetMetaDataDictionaryArray( reader->GetMetaDataDictionaryArray ); //TODO MetaDataDictionary in tutti i files o roba del genere?


  try
  {
    seriesWriter->Update();
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

