//
//  regsnap.cpp
//  makevol
//
//  Created by Maryana Alegro on 2/29/16.
//
//

#include <stdio.h>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkExtractImageFilter.h"
#include "itkImage.h"


int main( int argc, char ** argv )
{
    
    //typedef unsigned char        InputPixelType;
    //typedef unsigned char        OutputPixelType;
    
    typedef itk::RGBPixel<unsigned char> InputPixelType;
    typedef itk::RGBPixel<unsigned char> OutputPixelType;
    
    typedef itk::Image< InputPixelType,  3 >    InputImageType;
    typedef itk::Image< OutputPixelType, 2 >    OutputImageType;
    
    typedef itk::ImageFileReader< InputImageType  >  ReaderType;
    typedef itk::ImageFileWriter< OutputImageType >  WriterType;
    
    if( argc < 5 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << " dim slice filein fileout" << std::endl;

        return EXIT_FAILURE;
    }
    
    const unsigned int dim = atoi( argv[1] );
    const unsigned int sliceNumber = atoi( argv[2] );
    std::string fileIn = argv[3];
    std::string fileOut = argv[4];
    const char * inputFilename  = fileIn.c_str();
    const char * outputFilename = fileOut.c_str();
    
    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    
    reader->SetFileName( inputFilename  );
    writer->SetFileName( outputFilename );
    
    typedef itk::ExtractImageFilter< InputImageType, OutputImageType > FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter->InPlaceOn();
    filter->SetDirectionCollapseToSubmatrix();
    
    reader->UpdateOutputInformation();
    InputImageType::RegionType inputRegion = reader->GetOutput()->GetLargestPossibleRegion();
    
    InputImageType::SizeType size = inputRegion.GetSize();
    
    std::cout << "Size: " << std::endl;
    std::cout << size[0] << std::endl;
    std::cout << size[1] << std::endl;
    std::cout << size[2] << std::endl;
    
    
    InputImageType::IndexType start = inputRegion.GetIndex();
    //const unsigned int sliceNumber = 1;
    
    std::cout << "Start: " << std::endl;
    std::cout << start[0] << std::endl;
    std::cout << start[1] << std::endl;
    std::cout << start[2] << std::endl;
    
    size[dim] = 0;
    start[dim] = sliceNumber;
    
    InputImageType::RegionType desiredRegion;
    desiredRegion.SetSize(size);
    desiredRegion.SetIndex(start);
    
    filter->SetExtractionRegion( desiredRegion );
    
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
