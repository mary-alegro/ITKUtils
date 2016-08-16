
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkRegionOfInterestImageFilter.h"

#include "itkImage.h"
int main( int argc, char ** argv )
{
    if( argc != 9 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
        std::cerr << " startX startY startZ sizeX sizeY sizeZ" << std::endl;
        return EXIT_FAILURE;
    }
    
    
    //typedef signed short        InputPixelType;
    //typedef signed short        OutputPixelType;
    
    typedef itk::RGBPixel< unsigned char > InputPixelType;
    typedef itk::RGBPixel< unsigned char > OutputPixelType;
    const int Dimension = 3;
    typedef itk::Image< InputPixelType,  Dimension >    InputImageType;
    typedef itk::Image< OutputPixelType, Dimension >    OutputImageType;
    
    typedef itk::ImageFileReader< InputImageType  >  ReaderType;
    typedef itk::ImageFileWriter< OutputImageType >  WriterType;
    
    typedef itk::RegionOfInterestImageFilter< InputImageType, OutputImageType > FilterType;
    FilterType::Pointer filter = FilterType::New();
    
    OutputImageType::IndexType start;
    start[0] = atoi( argv[3] );
    start[1] = atoi( argv[4] );
    start[2] = atoi( argv[5] );
   
    OutputImageType::SizeType size;
    size[0] = atoi( argv[6] );
    size[1] = atoi( argv[7] );
    size[2] = atoi( argv[8] );
    
    OutputImageType::RegionType desiredRegion;
    desiredRegion.SetSize(size);
    desiredRegion.SetIndex(start);
    
    filter->SetRegionOfInterest(desiredRegion);
    
    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    
    const char * inputFilename  = argv[1];
    const char * outputFilename = argv[2];
    
    reader->SetFileName( inputFilename  );
    writer->SetFileName( outputFilename );
    
    filter->SetInput( reader->GetOutput() );
    writer->SetInput( filter->GetOutput() );
   
    try
    {
        writer->Update();
    }
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
   
    return EXIT_SUCCESS;
}

