#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkChangeInformationImageFilter.h"
#include <iostream>


int main( int argc, char * argv[] )
{
  // Verify the number of parameters in the command line
  if( argc < 5 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] 
              << " pattern firstSliceIndex lastSliceIndex spacingX spacingY spacingZ outputImageFile" << std::endl;
    std::cerr << " i.e: makevol \"histo_RGB_%d.tif\" 1 100 0.33 0.33 0.33 histo.mha"
              << std::endl;
    return EXIT_FAILURE;
    }
    
    std::string sFormat = argv[1];
    const unsigned int first = atoi( argv[2] );
    const unsigned int last  = atoi( argv[3] );
    const float dimX = atof(argv[4]);
    const float dimY = atof(argv[5]);
    const float dimZ = atof(argv[6]);
    const char * outputFilename = argv[7];
    const char * inputFilename;
    
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Name format: " << sFormat << std::endl;
    std::cout << "Initial index: " << first << std::endl;
    std::cout << "Final index: " << last << std::endl;
    std::cout << "Dimensions: X = " << dimX << " Y = " << dimY << " Z = " << dimZ << std::endl;
    std::cout << "Output file: " << outputFilename << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    
    typedef itk::NumericSeriesFileNames NameGeneratorType;
    NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
    nameGenerator->SetSeriesFormat(sFormat);
    nameGenerator->SetStartIndex(first);
    nameGenerator->SetEndIndex(last);
    nameGenerator->SetIncrementIndex(1);

    std::vector<std::string> names = nameGenerator->GetFileNames();
    std::vector<std::string>::iterator nit;
    int nfiles = names.size();
    nit = names.begin();
    inputFilename = (*nit).c_str();
    std::cout << "No. file(e) to stack: " << nfiles << std::endl;
    //list files
    for (nit = names.begin(); nit != names.end(); nit++){
        std::cout << "File: " << (*nit).c_str() << std::endl;
    }
    
    //get PixelType and ComponentType
    typedef itk::ImageIOBase::IOPixelType     IOPixelType;
    typedef itk::ImageIOBase::IOComponentType IOComponentType;
    itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(inputFilename,itk::ImageIOFactory::ReadMode);
    
    imageIO->SetFileName(inputFilename);
    imageIO->ReadImageInformation();
    const IOPixelType pixelType = imageIO->GetPixelType();
    const IOComponentType componentType = imageIO->GetComponentType();
    const unsigned int imageDimension = imageIO->GetNumberOfDimensions();
    
    std::cout << "1st Image Pixel Type is " << itk::ImageIOBase::GetPixelTypeAsString( pixelType ) << std::endl;
    std::cout << "1st Image Component Type is " << imageIO->GetComponentTypeAsString( componentType ) << std::endl;
    std::cout << "1st Image Image Dimension is " << imageDimension << std::endl;
 
    typedef unsigned char   PixelType;
    //typedef itk::RGBPixel< unsigned char >   PixelType;
    const unsigned int Dimension = 3;
 
  typedef itk::Image< PixelType, Dimension >  ImageType;
  typedef itk::ImageSeriesReader< ImageType > ReaderType;
  typedef itk::ImageFileWriter<   ImageType > WriterType;
 
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
 
 
  reader->SetFileNames(names);
 
    try{
        reader->UpdateOutputInformation();
        
    }catch(itk::ExceptionObject & err){
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
    
    typedef itk::ChangeInformationImageFilter< ImageType > FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput( reader->GetOutput() );
    
    
    double dims[3];
    dims[0] = dimX;
    dims[1] = dimY;
    dims[2] = dimZ;
    const ImageType::SpacingType spacing(dims);
    filter->SetOutputSpacing(spacing);
    filter->ChangeSpacingOn();


  writer->SetFileName( outputFilename );
    writer->SetInput(filter->GetOutput());
    writer->SetNumberOfStreamDivisions(20);
  //writer->SetInput( reader->GetOutput() );

    
  try{
    
    writer->Update();
      
  }catch( itk::ExceptionObject & err ){
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
  }
    
  return EXIT_SUCCESS;
 }

