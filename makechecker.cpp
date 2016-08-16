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
#include "itkCheckerBoardImageFilter.h"

int main( int argc, char ** argv )
{
    
    //typedef unsigned char        InputPixelType;
    //typedef unsigned char        OutputPixelType;
    
    typedef itk::RGBPixel<unsigned char> InputHistoType;
    typedef unsigned char                InputMRIType;
    typedef itk::RGBPixel<unsigned char> OutputPixelType;
    
    typedef itk::Image< InputHistoType,  3 >    InputHistoImageType;
    typedef itk::Image< InputMRIType,  3 >      InputMRIImageType;
    typedef itk::Image< OutputPixelType, 2 >    OutputImageType;
    
    typedef itk::ImageFileReader< InputHistoImageType  >  HistoReaderType;
    typedef itk::ImageFileReader< InputMRIImageType  >    MRIReaderType;
    typedef itk::ImageFileWriter< OutputImageType >       WriterType;
    
    if( argc < 7 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << " num.checkers dim slice histovolume mrivolume outputfile" << std::endl;
        
        return EXIT_FAILURE;
    }
    
    const unsigned int nCheck = atoi( argv[1] );
    const unsigned int dim = atoi( argv[2] );
    const unsigned int sliceNumber = atoi( argv[3] );
    std::string histoFile = argv[4];
    std::string mriFile = argv[5];
    std::string outFile = argv[6];

    
    HistoReaderType::Pointer hreader = HistoReaderType::New();
    MRIReaderType::Pointer mreader = MRIReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    
    hreader->SetFileName(histoFile.c_str());
    mreader->SetFileName(mriFile.c_str());
    writer->SetFileName(outFile.c_str());
    
    typedef itk::ExtractImageFilter< InputHistoImageType, OutputImageType > HistoFilterType;
    typedef itk::ExtractImageFilter< InputMRIImageType, OutputImageType > MRIFilterType;
    HistoFilterType::Pointer hfilter = HistoFilterType::New();
    hfilter->InPlaceOn();
    hfilter->SetDirectionCollapseToSubmatrix();
    
    MRIFilterType::Pointer mfilter = MRIFilterType::New();
    mfilter->InPlaceOn();
    mfilter->SetDirectionCollapseToSubmatrix();
    
    hreader->UpdateOutputInformation();
    mreader->UpdateOutputInformation();
    InputHistoImageType::RegionType inputRegionHisto = hreader->GetOutput()->GetLargestPossibleRegion();
    InputHistoImageType::SizeType hsize = inputRegionHisto.GetSize();
    InputMRIImageType::RegionType inputRegionMri = mreader->GetOutput()->GetLargestPossibleRegion();
    InputMRIImageType::SizeType msize = inputRegionMri.GetSize();
    
    if(hsize[0] != msize[0] || hsize[1] != msize[1] || hsize[2] != msize[2]){
        std::cerr << "Data set dimension must agree. Dimension found: " << hsize[0] << ", " << hsize[1] << ", " << hsize[2] << " (histo)"
         << "  " << msize[0] << ", " <<  msize[1] << ", " << msize[2] << " (MRI)" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::cout << "Histo Size: "  << hsize[0] << " " << hsize[1] << " " << hsize[2] << std::endl;
    std::cout << "MRI Size: " << msize[0] << " " << msize[1] << " " << msize[2] << std::endl;
    
    InputHistoImageType::IndexType hstart = inputRegionHisto.GetIndex();
    InputMRIImageType::IndexType mstart = inputRegionMri.GetIndex();
    //const unsigned int sliceNumber = 1;
    
    //std::cout << "Start: " << std::endl;
    //std::cout << start[0] << std::endl;
    //std::cout << start[1] << std::endl;
    //std::cout << start[2] << std::endl;
    
    hsize[dim] = 0;
    hstart[dim] = sliceNumber;
    msize[dim] = 0;
    mstart[dim] = sliceNumber;
    
    InputHistoImageType::RegionType desiredRegionHisto;
    desiredRegionHisto.SetSize(hsize);
    desiredRegionHisto.SetIndex(hstart);
    
    InputMRIImageType::RegionType desiredRegionMri;
    desiredRegionMri.SetSize(msize);
    desiredRegionMri.SetIndex(mstart);
    
    hfilter->SetExtractionRegion( desiredRegionHisto );
    mfilter->SetExtractionRegion( desiredRegionMri );
    
    hfilter->SetInput( hreader->GetOutput() );
    mfilter->SetInput( mreader->GetOutput() );
    
    typedef itk::CheckerBoardImageFilter<OutputImageType> CheckerBoardFilterType;
    CheckerBoardFilterType::Pointer checker = CheckerBoardFilterType::New();
    checker->SetInput1(hfilter->GetOutput());
    checker->SetInput2(mfilter->GetOutput());
    
    
    typedef CheckerBoardFilterType::PatternArrayType  CheckerPatternType;
    CheckerPatternType pattern;
    pattern[0] = nCheck;
    pattern[1] = nCheck;
    
    try
    {
        checker->SetCheckerPattern(pattern);
        writer->SetInput(checker->GetOutput());
        writer->SetNumberOfStreamDivisions(10);
        
        std::cout << "Writting file..." << std::endl;
        
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

