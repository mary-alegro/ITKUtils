#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkChangeInformationImageFilter.h"
#include "itkComposeImageFilter.h"
#include <iostream>

#include "itkRawImageIO.h"
#include "itkNumericSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"
#include "itkShrinkImageFilter.h"
#include "itkComposeImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkShrinkImageFilter.h"
#include "itkMeanImageFilter.h"


int main( int argc, char * argv[] )
{
    // Verify the number of parameters in the command line
    if( argc < 5 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0]
        << " redVolume greenVoluem blueVoluem outputImageFile" << std::endl;
        std::cerr << " i.e: combinergb2 RED.nii GREEN.nii BLUE.nii histo_rgb.nrrd"
        << std::endl;
        
        return EXIT_FAILURE;
    }
    
    std::string input_red = argv[1];
    std::string input_green = argv[2];
    std::string input_blue = argv[3];

    const char * outputFilename = argv[4];
    

    typedef unsigned char PixelType;
    typedef itk::RGBPixel<unsigned char> PixelTypeRGB;
    const unsigned int Dimension = 3;
    
    typedef itk::Image<PixelType,Dimension> ImageType;
    typedef itk::Image<PixelTypeRGB,Dimension> ImageTypeRGB;
    //typedef itk::ImageSeriesReader<ImageType> ReaderType;
    typedef itk::ImageFileReader<ImageType> ReaderType;
    typedef itk::ImageFileWriter<ImageTypeRGB> WriterType;
    
    ReaderType::Pointer red_reader = ReaderType::New();
    ReaderType::Pointer green_reader = ReaderType::New();
    ReaderType::Pointer blue_reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    
    red_reader->SetFileName(input_red);
    green_reader->SetFileName(input_green);
    blue_reader->SetFileName(input_blue);
    
    typedef itk::ComposeImageFilter< ImageType, ImageTypeRGB > ComposeRGBFilterType;
    ComposeRGBFilterType::Pointer composeRGB = ComposeRGBFilterType::New();
    composeRGB->SetInput1(red_reader->GetOutput());
    composeRGB->SetInput2(green_reader->GetOutput());
    composeRGB->SetInput3(blue_reader->GetOutput());
    
    writer->SetFileName( outputFilename );
    writer->SetInput(composeRGB->GetOutput());
    
    try{
        
        writer->Update();
        
    }catch( itk::ExceptionObject & err ){
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}


