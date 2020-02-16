#pragma once

#include <atlas/core/Float.hpp>
#include <atlas/math/Math.hpp>
#include <atlas/math/Random.hpp>
#include <atlas/math/Ray.hpp>

#include <fmt/printf.h>
#include <stb_image.h>
#include <stb_image_write.h>

#include <limits>
#include <vector>

using atlas::core::areEqual;

using Colour = atlas::math::Vector;

void saveToFile(std::string const& filename,
                std::size_t width,
                std::size_t height,
                std::vector<Colour> const& image);

// Declarations
class Camera;
class Shape;
class Sampler;

struct ShadeRec
{
    Colour color;
    float t;
};

struct World
{
    std::size_t width, height;
    Colour background;
    std::shared_ptr<Sampler> sampler;
    std::vector<std::shared_ptr<Shape>> scene;
    std::vector<Colour> image;
};

// Abstract classes defining the interfaces for concrete entities

class Camera
{
public:
    Camera();

    virtual ~Camera() = default;

    virtual void renderScene(World& world) const = 0;

    void setEye(atlas::math::Point const& eye);

    void setLookAt(atlas::math::Point const& lookAt);

    void setUpVector(atlas::math::Vector const& up);

    void computeUVW();

protected:
    atlas::math::Point mEye;
    atlas::math::Point mLookAt;
    atlas::math::Point mUp;
    atlas::math::Vector mU, mV, mW;
};

class Sampler
{
public:
    Sampler(int numSamples, int numSets);
    virtual ~Sampler() = default;

    int getNumSamples() const;

    void setupShuffledIndeces();

    virtual void generateSamples() = 0;

    atlas::math::Point sampleUnitSquare();

protected:
    std::vector<atlas::math::Point> mSamples;
    std::vector<int> mShuffledIndeces;

    int mNumSamples;
    int mNumSets;
    unsigned long mCount;
    int mJump;
};

class Shape
{
public:
    Shape();
    virtual ~Shape() = default;

    // if t computed is less than the t in sr, it and the color should be updated in sr
    virtual bool hit(atlas::math::Ray<atlas::math::Vector> const& ray,
                     ShadeRec& sr) const = 0;

    void setColour(Colour const& col);

    Colour getColour() const;

protected:
    virtual bool intersectRay(atlas::math::Ray<atlas::math::Vector> const& ray,
                              float& tMin) const = 0;

    Colour mColour;
};

// Concrete classes which we can construct and use in our ray tracer

class Sphere : public Shape
{
public:
    Sphere(atlas::math::Point center, float radius);

    bool hit(atlas::math::Ray<atlas::math::Vector> const& ray,
             ShadeRec& sr) const;

private:
    bool intersectRay(atlas::math::Ray<atlas::math::Vector> const& ray,
                      float& tMin) const;

    atlas::math::Point mCentre;
    float mRadius;
    float mRadiusSqr;
};

class Pinhole : public Camera
{
public:
    Pinhole();

    void setDistance(float distance);
    void setZoom(float zoom);

    atlas::math::Vector rayDirection(atlas::math::Point const& p) const;
    void renderScene(World& world) const;

private:
    float mDistance;
    float mZoom;
};

class Regular : public Sampler
{
public:
    Regular(int numSamples, int numSets);

    void generateSamples();
};

class Random : public Sampler
{
public:
    Random(int numSamples, int numSets);

    void generateSamples();
};
