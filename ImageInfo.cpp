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
    std::cout << "Pixel Type is: " << imageIO->GetComponentTypeAsString(pixelType) << std::endl;
    
    const double specX = imageIO->GetSpacing(0);
    const double specY = imageIO->GetSpacing(1);
    const double specZ = imageIO->GetSpacing(2);
    std::cout << "Spacing: " << specX << " " << specY << " " << specZ << std::endl;
    std::cout << "Origin: " << imageIO->GetOrigin(0) << " " << imageIO->GetOrigin(1) << " " << imageIO->GetOrigin(2) << std::endl;
    
    std::vector<double> direcX = imageIO->GetDirection(0);
    std::vector<double> direcY = imageIO->GetDirection(1);
    std::vector<double> direcZ = imageIO->GetDirection(2);
    std::cout << "Direction: " << std::endl;
    std::cout << "     X : " << direcX[0] << " " << direcX[1] << " " << direcX[2] << std::endl;
    std::cout << "     Y : " << direcY[0] << " " << direcY[1] << " " << direcY[2] << std::endl;
    std::cout << "     Z : " << direcZ[0] << " " << direcZ[1] << " " << direcZ[2] << std::endl;

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