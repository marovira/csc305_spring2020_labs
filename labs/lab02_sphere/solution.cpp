#include "lab.hpp"

int main()
{
    // compile time program constants
    constexpr std::size_t image_width{512};
    constexpr std::size_t image_height{512};
    constexpr Colour background{0, 0, 0};
    constexpr Sphere object{{256, 256, 0}, 128, {1, 0, 0}};

    // variable data in computation
    atlas::math::Ray<atlas::math::Vector> ray{{0, 0, 0}, {0, 0, -1}};
    ShadeRec trace_data{};
    std::vector<Colour> image{image_width * image_height};

    for (std::size_t y{0}; y < image_height; ++y)
    {
        for (std::size_t x{0}; x < image_width; ++x)
        {
            // set origin to present pixel
            ray.o = {x + 0.5f, y + 0.5f, 0};

            // check if ray didn't hit the sphere
            if (!object.hit(ray, trace_data))
            {
                trace_data.color = background;
            }

            // set pixel in image
            image[x + y * image_height] = trace_data.color;
        }
    }

    saveToFile("raytrace.bmp", image_width, image_height, image);

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
