
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVersor.h"
#include "itkChangeInformationImageFilter.h"

int main( int argc, char * argv[] )
{
  if( argc < 3 )
  {
      std::cerr << "Changes FILE2 header using information from FILE1 header. Saves output in FILEOUT." << std::endl;
      std::cerr << "Usage: " << argv[0] << " file1  file2 fileout" << std::endl;
      return EXIT_FAILURE;
  }
    
  const char * inputName1 = argv[1];
  const char * inputName2 = argv[2];
  const char * outputName = argv[3];
  
  
  //Use ImageIO to reade header information since it is much faster to read header information the ImageReader
  itk::ImageIOBase::Pointer imageIO1 = itk::ImageIOFactory::CreateImageIO(inputName1, itk::ImageIOFactory::ReadMode);
  if( !imageIO1 ){
        std::cerr << "Could not CreateImageIO for: " << inputName1 << std::endl;
        return EXIT_FAILURE;
  }
  imageIO1->SetFileName(inputName1);
  imageIO1->ReadImageInformation();
  
  itk::ImageIOBase::Pointer imageIO2 = itk::ImageIOFactory::CreateImageIO(inputName2, itk::ImageIOFactory::ReadMode);
  if( !imageIO2 ){
        std::cerr << "Could not CreateImageIO for: " << inputName2 << std::endl;
        return EXIT_FAILURE;
  }
  imageIO2->SetFileName(inputName2);
  imageIO2->ReadImageInformation();
  
    
  typedef   unsigned char  PixelType;
  const unsigned int Dimension = 3;
  typedef itk::Image< PixelType,  Dimension >   ImageType;
  typedef itk::ImageFileReader< ImageType >  ReaderType;
  typedef itk::ImageFileWriter< ImageType >  WriterType;
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  
  reader->SetFileName(inputName2);
  writer->SetFileName(outputName);

  typedef itk::ChangeInformationImageFilter< ImageType >  FilterType;
  FilterType::Pointer filter = FilterType::New();
  
  std::cout << "FILE 1: " << inputName1 << std::endl;
  std::cout << "FILE 2: " << inputName2 << std::endl;
  std::cout << "FILE_OUT: " << outputName << std::endl;

  //Get header information from example file (FILE1)
  ImageType::PointType origin;
  ImageType::SpacingType spacing;
  ImageType::DirectionType direction;
  origin[0] = imageIO1->GetOrigin(0);
  origin[1] = imageIO1->GetOrigin(1);
  origin[2] = imageIO1->GetOrigin(2);
  
  spacing[0] = imageIO1->GetSpacing(0);
  spacing[1] = imageIO1->GetSpacing(1);
  spacing[2] = imageIO1->GetSpacing(2);
  
  std::vector<double> dX = imageIO1->GetDirection(0);
  std::vector<double> dY = imageIO1->GetDirection(1);
  std::vector<double> dZ = imageIO1->GetDirection(2);
  
  direction[0][0] = dX[0]; direction[0][1] = dX[1]; direction[0][2] = dX[2];
  direction[1][0] = dY[0]; direction[1][1] = dY[1]; direction[1][2] = dY[2];
  direction[2][0] = dX[0]; direction[2][1] = dX[1]; direction[2][2] = dX[2];
  
  
  //Get header information from destination file (FILE2)
  ImageType::PointType origin2;
  ImageType::SpacingType spacing2;
  ImageType::DirectionType direction2;
  origin2[0] = imageIO2->GetOrigin(0);
  origin2[1] = imageIO2->GetOrigin(1);
  origin2[2] = imageIO2->GetOrigin(2);
  
  spacing2[0] = imageIO2->GetSpacing(0);
  spacing2[1] = imageIO2->GetSpacing(1);
  spacing2[2] = imageIO2->GetSpacing(2);
  
  std::vector<double> dX2 = imageIO2->GetDirection(0);
  std::vector<double> dY2 = imageIO2->GetDirection(1);
  std::vector<double> dZ2 = imageIO2->GetDirection(2);
  
  direction2[0][0] = dX2[0]; direction2[0][1] = dX2[1]; direction2[0][2] = dX2[2];
  direction2[1][0] = dY2[0]; direction2[1][1] = dY2[1]; direction2[1][2] = dY2[2];
  direction2[2][0] = dX2[0]; direction2[2][1] = dX2[1]; direction2[2][2] = dX2[2];

  std::cout << std::endl; 
  std::cout << "Original header information :" << std::endl;
  std::cout << "Origin: " << origin2 << std::endl;
  std::cout << "Spacing: " << spacing2 << std::endl;
  std::cout << "Direction: " << direction2 << std::endl;
  std::cout << std::endl;
  std::cout << "New header information:" << std::endl;
  std::cout << "Origin: " << origin << std::endl;
  std::cout << "Spacing: " << spacing << std::endl;
  std::cout << "Direction: " << direction << std::endl;
     
  filter->SetInput( reader->GetOutput() );
  writer->SetInput( filter->GetOutput() );
  
  //filter->SetReferenceImage(reader->GetOutput());
  //filter->SetUseReferenceImage(true);
  filter->SetOutputOrigin(origin);
  filter->ChangeOriginOn();
  //filter->SetOutputDirection(direction);
  filter->ChangeDirectionOn();
  filter->SetOutputSpacing(spacing);
  filter->ChangeSpacingOn();

  try
    {
      writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
      std::cerr << excp << std::endl;
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}