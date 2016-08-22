
#include <stdio.h>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkExtractImageFilter.h"
#include "itkImage.h"
#include "itkNumericSeriesFileNames.h"

int main( int argc, char ** argv )
{
    
    typedef unsigned char        InputPixelType;
    typedef unsigned char        OutputPixelType;
    
    //typedef itk::RGBPixel<unsigned char> InputPixelType;
    //typedef itk::RGBPixel<unsigned char> OutputPixelType;
    
    typedef itk::Image< InputPixelType,  3 >    InputImageType;
    typedef itk::Image< OutputPixelType, 2 >    OutputImageType;
    
    typedef itk::ImageFileReader< InputImageType  >  ReaderType;
    typedef itk::ImageFileWriter< OutputImageType >  WriterType;
    
    if( argc < 4 )
    {
        std::cerr << "Saves all slices in dimension DIM as tif 2D images." << std::endl;
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << " <dim> <filein> <pattern_out>" << std::endl;
        
        return EXIT_FAILURE;
    }
    
    const unsigned int dim = atoi( argv[1] );
    //const unsigned int sliceNumber = atoi( argv[2] );
    std::string fileIn = argv[2];
    std::string fileOut = argv[3];
    const char * inputFilename  = fileIn.c_str();
    const char * outputFilename = fileOut.c_str();
    std::string format =  std::string( outputFilename ) + std::string( "_%d.tif" );
    
    std::cout << "- Arguments -" << std::endl;
    std::cout << "Dim: " << dim << std::endl;
    std::cout << "Input: " << inputFilename << std::endl;
    std::cout << "Output: " << format << std::endl;
    
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
    
    std::cout << "Size: " << size[0] << " " << size[1] << " " << size[2] << std::endl;
    
    InputImageType::IndexType start = inputRegion.GetIndex();
    //const unsigned int sliceNumber = 1;
    
    std::cout << "Largest region start: " << start[0] << " " << start[1] << " " << start[2] << std::endl;
    
    itk::NumericSeriesFileNames::Pointer fnames = itk::NumericSeriesFileNames::New();
    fnames->SetStartIndex(0);
    fnames->SetEndIndex(size[dim] - 1);
    fnames->SetIncrementIndex(1);
    fnames->SetSeriesFormat(format.c_str());
    
    size[dim] = 0;

    
    std::vector<std::string> names = fnames->GetFileNames();
    std::vector<std::string>::iterator nit;
    int nfiles = names.size();
    nit = names.begin();
    std::string outFilename = (*nit).c_str();
    std::cout << "No. slices to extract: " << nfiles << std::endl;
    //list files
    unsigned int nSlice = 0;
    filter->SetInput( reader->GetOutput() );
    for (nit = names.begin(); nit != names.end(); nit++){
        std::cout << "File: " << (*nit).c_str() << std::endl;
        
        start[dim] = nSlice++;
        InputImageType::RegionType desiredRegion;
        desiredRegion.SetSize(size);
        desiredRegion.SetIndex(start);
        
        filter->SetExtractionRegion( desiredRegion );
        writer->SetFileName((*nit).c_str());
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
    }
    
    
    return EXIT_SUCCESS;
}
