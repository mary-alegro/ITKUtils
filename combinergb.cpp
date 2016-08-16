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
        << " patternRED start end patternGREEN start end patternBLUE start end outputImageFile" << std::endl;
        std::cerr << " i.e: combinergb \"RED_%d.tif\" 1 100 \"GREEN_%d.tif\" 1 100 \"BLUE_%d.tif\" 1 100 histo_rgb.nrrd"
        << std::endl;
        std::cerr << "OR" << std::endl;
        std::cerr << argv[0]
        << " redVolume greenVoluem blueVoluem outputImageFile" << std::endl;
        std::cerr << " i.e: combinergb RED.nii GREEN.nii  BLUE.nii histo_rgb.nrrd"
        << std::endl;
        
        return EXIT_FAILURE;
    }
    
    
    std::string sFormat_red = argv[1];
    const unsigned int f_red = atoi( argv[2] );
    const unsigned int l_red  = atoi( argv[3] );
    std::string sFormat_green = argv[4];
    const unsigned int f_green = atoi( argv[5] );
    const unsigned int l_green  = atoi( argv[6] );
    std::string sFormat_blue = argv[7];
    const unsigned int f_blue = atoi( argv[8] );
    const unsigned int l_blue  = atoi( argv[9] );
    const char * outputFilename = argv[10];
    
    typedef itk::NumericSeriesFileNames NameGeneratorType;
    NameGeneratorType::Pointer nameGenerator_red = NameGeneratorType::New();
    nameGenerator_red->SetSeriesFormat(sFormat_red);
    nameGenerator_red->SetStartIndex(f_red);
    nameGenerator_red->SetEndIndex(l_red);
    nameGenerator_red->SetIncrementIndex(1);
    std::vector<std::string> names_red = nameGenerator_red->GetFileNames();
    
    NameGeneratorType::Pointer nameGenerator_green = NameGeneratorType::New();
    nameGenerator_green->SetSeriesFormat(sFormat_green);
    nameGenerator_green->SetStartIndex(f_green);
    nameGenerator_green->SetEndIndex(l_green);
    nameGenerator_green->SetIncrementIndex(1);
    std::vector<std::string> names_green = nameGenerator_green->GetFileNames();
    
    NameGeneratorType::Pointer nameGenerator_blue = NameGeneratorType::New();
    nameGenerator_blue->SetSeriesFormat(sFormat_blue);
    nameGenerator_blue->SetStartIndex(f_blue);
    nameGenerator_blue->SetEndIndex(l_blue);
    nameGenerator_blue->SetIncrementIndex(1);
    std::vector<std::string> names_blue = nameGenerator_blue->GetFileNames();
    
    std::vector<std::string>::iterator nit;
    int nfiles = names_red.size();
    nit = names_red.begin();
    std::cout << "No. file(e) to stack: " << nfiles << std::endl;
    //list files
    for (nit = names_red.begin(); nit != names_red.end(); nit++){
        std::cout << "File: " << (*nit).c_str() << std::endl;
    }

    typedef unsigned char PixelType;
    typedef itk::RGBPixel<unsigned char> PixelTypeRGB;
    const unsigned int Dimension = 3;
    
    typedef itk::Image<PixelType,Dimension> ImageType;
    typedef itk::Image<PixelTypeRGB,Dimension> ImageTypeRGB;
    typedef itk::ImageSeriesReader<ImageType> ReaderType;
    typedef itk::ImageFileWriter<ImageTypeRGB> WriterType;
    
    ReaderType::Pointer red_reader = ReaderType::New();
    ReaderType::Pointer green_reader = ReaderType::New();
    ReaderType::Pointer blue_reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    
    red_reader->SetFileNames(names_red);
    green_reader->SetFileNames(names_green);
    blue_reader->SetFileNames(names_blue);
    
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


