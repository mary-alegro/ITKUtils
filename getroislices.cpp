#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkImageSeriesWriter.h"
#include "itkRegionOfInterestImageFilter.h"


int main( int argc, char ** argv )
{
    if( argc != 9 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << " inputImageFile  outputPrefix " << std::endl;
        std::cerr << " startX startY startZ sizeX sizeY sizeZ" << std::endl;
        return EXIT_FAILURE;
    }
    
    
    //typedef signed short        InputPixelType;
    //typedef signed short        OutputPixelType;
    
    typedef itk::RGBPixel< unsigned char > InputPixelType;
    typedef itk::RGBPixel< unsigned char > OutputPixelType;
    typedef itk::Image< InputPixelType,  3 >    InputImageType;
    typedef itk::Image< InputPixelType,  3 >    ROIImageType;
    typedef itk::Image< OutputPixelType, 2 >    OutputImageType;
    
    typedef itk::ImageFileReader< InputImageType  >  ReaderType;
    typedef itk::ImageSeriesWriter< ROIImageType, OutputImageType > WriterType;
    typedef itk::RegionOfInterestImageFilter< InputImageType, ROIImageType > FilterType;
    
    const char * inputFilename  = argv[1];
    const char * outputFilename = argv[2];
    std::string format =  std::string( outputFilename ) + std::string( "_%d.tif" );
    
    InputImageType::IndexType start;
    start[0] = atoi( argv[3] );
    start[1] = atoi( argv[4] );
    start[2] = atoi( argv[5] );
    
    InputImageType::SizeType size;
    size[0] = atoi( argv[6] );
    size[1] = atoi( argv[7] );
    size[2] = atoi( argv[8] );
    
    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    FilterType::Pointer filter = FilterType::New();
    
    reader->SetFileName(inputFilename);
    
    ROIImageType::RegionType desiredRegion;
    desiredRegion.SetSize(size);
    desiredRegion.SetIndex(start);
    filter->SetRegionOfInterest(desiredRegion);
    filter->SetInput(reader->GetOutput());
    //filter->UpdateLargestPossibleRegion();
    //filter->Update();
    
    
    //ROIImageType::RegionType region = filter->GetOutput()->GetLargestPossibleRegion();
    //ROIImageType::SizeType rsize = region.GetSize();
    itk::NumericSeriesFileNames::Pointer fnames = itk::NumericSeriesFileNames::New();
    fnames->SetStartIndex( 0 );
    
    //std::cout << rsize[0] << " " << rsize[1] << " " << rsize[2] << std::endl;
    
    fnames->SetEndIndex( size[2] - 1 );
    fnames->SetIncrementIndex( 1 );
    fnames->SetSeriesFormat( format.c_str() );
    
    
    writer->SetInput(filter->GetOutput());
    writer->SetFileNames( fnames->GetFileNames() );
    
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


