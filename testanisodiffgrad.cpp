
#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkRGBPixel.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVectorCastImageFilter.h"


int main( int argc, char * argv[] )
{
    if( argc != 6 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0] << "  inputRGBImageFile  outputRGBImageFile ";
        std::cerr << "numberOfIterations conductance  timeStep  " << std::endl;
        return EXIT_FAILURE;
    }
    
    typedef itk::RGBPixel< float >          InputPixelType;
    typedef itk::Image< InputPixelType, 2 > InputImageType;
    
    typedef itk::VectorGradientAnisotropicDiffusionImageFilter<InputImageType, InputImageType >  FilterType;
    FilterType::Pointer filter = FilterType::New();
    
    typedef itk::ImageFileReader< InputImageType >  ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( argv[1] );
    filter->SetInput( reader->GetOutput() );
    // Software Guide : EndCodeSnippet
    const unsigned int numberOfIterations = atoi( argv[3] );
    const double       conducPar = atof( argv[4] );
    const double       timeStep = atof( argv[5] );
    
    filter->SetNumberOfIterations( numberOfIterations );
    filter->SetTimeStep( timeStep );
    filter->SetConductanceParameter(conducPar);
    filter->Update();
    
    typedef itk::RGBPixel< unsigned char >   WritePixelType;
    typedef itk::Image< WritePixelType, 2 >  WriteImageType;
    typedef itk::VectorCastImageFilter<InputImageType, WriteImageType>  CasterType;
    CasterType::Pointer caster = CasterType::New();
    
    typedef itk::ImageFileWriter< WriteImageType >  WriterType;
    WriterType::Pointer writer = WriterType::New();
    caster->SetInput( filter->GetOutput() );
    writer->SetInput( caster->GetOutput() );
    writer->SetFileName( argv[2] );
    writer->Update();
    
    return EXIT_SUCCESS;
}
