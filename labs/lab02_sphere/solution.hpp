#pragma once

#include <atlas/core/Float.hpp>
#include <atlas/math/Math.hpp>
#include <atlas/math/Ray.hpp>

#include <fmt/printf.h>
#include <stb_image.h>
#include <stb_image_write.h>

#include <vector>

using Colour = atlas::math::Vector;

void saveToFile(std::string const& filename,
                std::size_t width,
                std::size_t height,
                std::vector<Colour> const& image);

struct ShadeRec
{
    Colour color;
    float t;
};

class Sphere
{
public:
    constexpr Sphere(atlas::math::Point center, float radius, Colour color) :
        center_{center},
        radius_{radius},
        radius_sqr_{radius * radius},
        color_{color}
    {}

    constexpr Sphere(Sphere const& s) :
        center_{s.center_},
        radius_{s.radius_},
        radius_sqr_{s.radius_sqr_},
        color_{s.color_}
    {}

    // the output data in 'trace_data' is unreliable if 'hit' returns 'false'
    bool hit(atlas::math::Ray<atlas::math::Vector> const& ray,
             ShadeRec& trace_data) const
    {
        // scratch vector used often in calculations
        auto o_c{ray.o - center_};

        // coefficients of the quadratic solution to 't'
        auto a{glm::dot(ray.d, ray.d)};
        auto b{glm::dot(ray.d * 2.0f, o_c)};
        auto c{glm::dot(o_c, o_c) - radius_sqr_};

        // compute 't' determinant
        auto determinant{(b * b) - (4.0f * a * c)};
        auto intersect{false};

        // update trace_data if the ray hit
        if (atlas::core::geq(determinant, 0.0f))
        {
            trace_data.t     = (-b - determinant) / (2.0f * a);
            trace_data.color = color_;
            intersect        = true;
        }

        return intersect;
    }

private:
    atlas::math::Point center_;
    float radius_, radius_sqr_;
    Colour color_;
};
