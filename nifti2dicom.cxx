#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesWriter.h>
#include <itkNumericSeriesFileNames.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkOrientImageFilter.h>


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
  reader->SetFileName( InputFile );

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
  RescaleType::Pointer rescale;

  typedef itk::CastImageFilter < ImageType, DICOM3DImageType > CastType;
  CastType::Pointer cast;

  DICOM3DImageType::Pointer Image;

  if (doRescale)
  {
    // Rescale
    rescale = RescaleType::New();

    rescale->SetInput(orienter->GetOutput());
    rescale->SetOutputMinimum( 0 );
    rescale->SetOutputMaximum( 2^11 -1 );

    try
    {
      std::cout << "Rescaling... " << std::flush;
      rescale->Update();
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


    Image = rescale->GetOutput();
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




// -----------------------------------------------------------------------------
// Scrittura dell'immagine
// -----------------------------------------------------------------------------


  // Writer
  typedef itk::ImageSeriesWriter<DICOM3DImageType, DICOMImageType> SeriesWriterType;
  typedef itk::GDCMImageIO ImageIOType;
//  typedef itk::GDCMSeriesFileNames NamesGeneratorType;


  typedef itk::NumericSeriesFileNames NameGeneratorType;
  NameGeneratorType::Pointer namesGenerator = NameGeneratorType::New();

  ImageType::RegionType region = Image->GetLargestPossibleRegion();
  ImageType::SizeType dimensions = region.GetSize();

#ifdef DEBUG
  std::cout << "Dimensions: [" << dimensions[0] << "," << dimensions[1] << "," << dimensions[2] << "]" << std::endl;
#endif

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

