#include "itkTileImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"

int main(int argc, char *argv[] )
{

    if( argc < 2 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[0]
        << " imagefile.ext"
        << std::endl;
        return EXIT_FAILURE;
    }
    
    std::string inputFilename = argv[1];
    
    typedef itk::ImageIOBase::IOComponentType ScalarPixelType;
    
    itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(inputFilename.c_str(), itk::ImageIOFactory::ReadMode);
    if( !imageIO ){
        std::cerr << "Could not CreateImageIO for: " << inputFilename << std::endl;
        return EXIT_FAILURE;
    }
    imageIO->SetFileName(inputFilename);
    imageIO->ReadImageInformation();
    const ScalarPixelType pixelType = imageIO->GetComponentType();
    std::cout << "Pixel Type is: " << imageIO->GetComponentTypeAsString(pixelType)
    << std::endl;
    
    const double specX = imageIO->GetSpacing(0);
    const double specY = imageIO->GetSpacing(1);
    const double specZ = imageIO->GetSpacing(2);
    std::cout << "Spacing: " << specX << " " << specY << " " << specZ << std::endl;
    
    const size_t numDimensions =  imageIO->GetNumberOfDimensions();
    std::cout << "NumDimensions: " << numDimensions << std::endl;
    
    const double dimX = imageIO->GetDimensions(0);
    const double dimY = imageIO->GetDimensions(1);
    const double dimZ = imageIO->GetDimensions(2);
    std::cout << "Size: " << dimX << " " << dimY << " " << dimZ << std::endl;
    
    std::cout << "Total num. pixels: " << imageIO->GetImageSizeInPixels() << std::endl;
    std::cout << "Pixel component size: " << imageIO->GetComponentSize() << std::endl;
    std::cout << "Pixel type (string): " << imageIO->GetPixelTypeAsString(imageIO->GetPixelType()) << std::endl;

    
    return EXIT_SUCCESS;
}