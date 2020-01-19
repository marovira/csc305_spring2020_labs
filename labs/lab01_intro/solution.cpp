#include <atlas/math/Math.hpp>

#include <fmt/printf.h>
#include <stb_image.h>
#include <stb_image_write.h>

#include <vector>

// C++ equivalent of typedef. Creates an alias for the
// given type so we can make our code easier to read.
using Colour = atlas::math::Vector;

// This function will save the given array of pixels to a file.
// The arguments are:
// filename: the name of the file to save to.
// width & height: the dimensions of the image.
// image: the array of pixels in the range [0, 1].
void saveToFile(std::string const& filename,
                std::size_t width,
                std::size_t height,
                std::vector<Colour> const& image);

int main()
{
    const std::size_t imageWidth{600};
    const std::size_t imageHeight{600};
    const std::size_t squareDims{20};

    std::vector<Colour> image(imageWidth * imageHeight);

    bool isShaded{true};
    bool flip{false};
    for (std::size_t y{0}; y < imageHeight; ++y)
    {
        flip     = (y % squareDims == 0 && y != 0) ? !flip : flip;
        isShaded = (flip) ? true : false;
        for (std::size_t x{0}; x < imageWidth; ++x)
        {
            isShaded = (x % squareDims == 0 && x != 0) ? !isShaded : isShaded;
            Colour c = (isShaded) ? Colour{1, 1, 1} : Colour{0, 0, 0};
            image[x + y * imageHeight] = c;
        }
    }

    saveToFile("checkerboard.bmp", imageWidth, imageHeight, image);

    return 0;
}

void saveToFile(std::string const& filename,
                std::size_t width,
                std::size_t height,
                std::vector<Colour> const& image)
{
    std::vector<unsigned char> data(image.size() * 3);

    for (std::size_t i{0}, k{0}; i < image.size(); ++i, k += 3)
    {
        Colour pixel = image[i];
        data[k + 0]  = static_cast<unsigned char>(pixel.r * 255);
        data[k + 1]  = static_cast<unsigned char>(pixel.g * 255);
        data[k + 2]  = static_cast<unsigned char>(pixel.b * 255);
    }

    stbi_write_bmp(filename.c_str(),
                   static_cast<int>(width),
                   static_cast<int>(height),
                   3,
                   data.data());
}
