#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkImageSeriesWriter.h"
#include "itkThresholdImageFilter.h"
#include "itkBinaryFillholeImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"


int main( int argc, char* argv[] )
{
    if( argc != 3 )
    {
        std::cerr << "Usage: "<< std::endl;
        std::cerr << argv[0];
        std::cerr << " <InputFileName> <OutputPrefix> <Pixel Type (0:uchar, 1:RGB)>";
        std::cerr << std::endl;
        return EXIT_FAILURE;
    }
    const char * inputFileName = argv[1];
    const char * outputFileName = argv[2];
    //const int threshValue = atoi(argv[3]);
    
    std::string format =  std::string( outputFileName ) + std::string( "_%d.tif" );
    const unsigned int Dimension = 3;
    typedef unsigned char                      PixelType;
    typedef itk::Image< PixelType, Dimension > InputImageType;
    typedef itk::ImageFileReader< InputImageType >  ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( inputFileName );
    
    typedef unsigned char OutputPixelType;
    typedef itk::Image< OutputPixelType, 2 > OutputImageType;
    typedef itk::Image< OutputPixelType, Dimension > RescaleImageType;
    typedef itk::RescaleIntensityImageFilter<InputImageType,RescaleImageType>RescaleFilterType;
    RescaleFilterType::Pointer rescale = RescaleFilterType::New();
    rescale->SetInput( reader->GetOutput() );
    rescale->SetOutputMinimum( 0 );
    rescale->SetOutputMaximum( 255 );
    rescale->UpdateLargestPossibleRegion();
    
    InputImageType::RegionType region = reader->GetOutput()->GetLargestPossibleRegion();
    InputImageType::SizeType size = region.GetSize();
    itk::NumericSeriesFileNames::Pointer fnames = itk::NumericSeriesFileNames::New();
    fnames->SetStartIndex( 0 );
    
    std::cout << size[0] << " " << size[1] << " " << size[2] << std::endl;
    
    fnames->SetEndIndex( size[2] - 1 );
    fnames->SetIncrementIndex( 1 );
    fnames->SetSeriesFormat( format.c_str() );
    
    typedef itk::ImageSeriesWriter< RescaleImageType, OutputImageType > WriterType;
    WriterType::Pointer writer = WriterType::New();
    
    
    writer->SetInput(rescale->GetOutput());
    //writer->SetInput(binaryFilter->GetOutput());
    writer->SetFileNames( fnames->GetFileNames() );
    try
    {
        
        writer->Update();
    }
    catch( itk::ExceptionObject & error )
    {
        std::cerr << "Error: " << error << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}