#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesWriter.h>
#include <itkNumericSeriesFileNames.h>

#define DEBUG

int main(int argc, char* argv[])
{

  // Arguments check
  if (argc < 3 || argc >5)
  {
    std::cout << "Usage: " << argv[0] << " <InputFile> <OutputDirectory> [Suffix = \".dcm\"] [Prefix]" << std::endl;
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

  std::string Format = OutputDirectory + Prefix + std::string("%04d") + Suffix;

#ifdef DEBUG
  std::cout << "InputFile       = " << InputFile       << std::endl;
  std::cout << "OutputDirectory = " << OutputDirectory << std::endl;
  std::cout << "Suffix          = " << Suffix          << std::endl;
  std::cout << "Prefix          = " << Prefix          << std::endl;
  std::cout << "Format          = " << Format          << std::endl;
#endif // DEBUG


  // Base definitions
  typedef unsigned char PixelType;
  const int Dimension = 3;
  typedef itk::Image <PixelType, Dimension> ImageType;

  typedef signed short DICOMPixelType;
  const int DICOMDimension = 2;
  typedef itk::Image<PixelType, DICOMDimension> DICOMImageType;

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


  // Writer
  typedef itk::ImageSeriesWriter<ImageType, DICOMImageType> SeriesWriterType;
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

  seriesWriter->SetInput( reader->GetOutput() );
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

