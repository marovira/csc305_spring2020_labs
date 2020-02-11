#include "lab.hpp"

// ******* Function Member Implementation *******

// ***** Shape function members *****
Shape::Shape() : mColour{0, 0, 0}
{}

void Shape::setColour(Colour const& col)
{
    mColour = col;
}

Colour Shape::getColour() const
{
    return mColour;
}

void Shape::setMaterial(std::shared_ptr<Material> const& material)
{
    mMaterial = material;
}

std::shared_ptr<Material> Shape::getMaterial() const
{
    return mMaterial;
}

// ***** Sampler function members *****
Sampler::Sampler(int numSamples, int numSets) :
    mNumSamples{numSamples}, mNumSets{numSets}, mCount{0}, mJump{0}
{
    mSamples.reserve(mNumSets * mNumSamples);
    setupShuffledIndeces();
}

int Sampler::getNumSamples() const
{
    return mNumSamples;
}

void Sampler::setupShuffledIndeces()
{
    mShuffledIndeces.reserve(mNumSamples * mNumSets);
    std::vector<int> indices;

    std::random_device d;
    std::mt19937 generator(d());

    for (int j = 0; j < mNumSamples; ++j)
    {
        indices.push_back(j);
    }

    for (int p = 0; p < mNumSets; ++p)
    {
        std::shuffle(indices.begin(), indices.end(), generator);

        for (int j = 0; j < mNumSamples; ++j)
        {
            mShuffledIndeces.push_back(indices[j]);
        }
    }
}

atlas::math::Point Sampler::sampleUnitSquare()
{
    if (mCount % mNumSamples == 0)
    {
        atlas::math::Random<int> engine;
        mJump = (engine.getRandomMax() % mNumSets) * mNumSamples;
    }

    return mSamples[mJump + mShuffledIndeces[mJump + mCount++ % mNumSamples]];
}

// ***** Light function members *****
Colour Light::L([[maybe_unused]] ShadeRec& sr)
{
    return Colour{0.0f};
}

void Light::scaleRadiance([[maybe_unused]] float b)
{}

void Light::setColour([[maybe_unused]] Colour const& c)
{}

// ***** Sphere function members *****
Sphere::Sphere(atlas::math::Point center, float radius) :
    mCentre{center}, mRadius{radius}, mRadiusSqr{radius * radius}
{}

bool Sphere::hit(atlas::math::Ray<atlas::math::Vector> const& ray,
                 ShadeRec& sr) const
{
    atlas::math::Vector tmp = ray.o - mCentre;
    float t{std::numeric_limits<float>::max()};
    bool intersect{intersectRay(ray, t)};

    // update ShadeRec info about new closest hit
    if (intersect && t < sr.t)
    {
        sr.normal   = (tmp + t * ray.d) / mRadius;
        sr.ray      = ray;
        sr.color    = mColour;
        sr.t        = t;
        sr.material = mMaterial;
    }

    return intersect;
}

bool Sphere::intersectRay(atlas::math::Ray<atlas::math::Vector> const& ray,
                          float& tMin) const
{
    const auto tmp{ray.o - mCentre};
    const auto a{glm::dot(ray.d, ray.d)};
    const auto b{2.0f * glm::dot(ray.d, tmp)};
    const auto c{glm::dot(tmp, tmp) - mRadiusSqr};
    const auto disc{(b * b) - (4.0f * a * c)};

    if (atlas::core::geq(disc, 0.0f))
    {
        const float kEpsilon{0.01f};
        const float e{std::sqrt(disc)};
        const float denom{2.0f * a};

        // Look at the negative root first
        float t = (-b - e) / denom;
        if (atlas::core::geq(t, kEpsilon))
        {
            tMin = t;
            return true;
        }

        // Now the positive root
        t = (-b + e);
        if (atlas::core::geq(t, kEpsilon))
        {
            tMin = t;
            return true;
        }
    }

    return false;
}

// ***** Regular function members *****
Regular::Regular(int numSamples, int numSets) : Sampler{numSamples, numSets}
{
    generateSamples();
}

void Regular::generateSamples()
{
    int n = static_cast<int>(glm::sqrt(static_cast<float>(mNumSamples)));

    for (int j = 0; j < mNumSets; ++j)
    {
        for (int p = 0; p < n; ++p)
        {
            for (int q = 0; q < n; ++q)
            {
                mSamples.push_back(
                    atlas::math::Point{(q + 0.5f) / n, (p + 0.5f) / n, 0.0f});
            }
        }
    }
}

// ***** Regular function members *****
Random::Random(int numSamples, int numSets) : Sampler{numSamples, numSets}
{
    generateSamples();
}

void Random::generateSamples()
{
    atlas::math::Random<float> engine;
    for (int p = 0; p < mNumSets; ++p)
    {
        for (int q = 0; q < mNumSamples; ++q)
        {
            mSamples.push_back(atlas::math::Point{
                engine.getRandomOne(), engine.getRandomOne(), 0.0f});
        }
    }
}

// ******* Driver Code *******

int main()
{
    using atlas::math::Point;
    using atlas::math::Ray;
    using atlas::math::Vector;

    std::shared_ptr<World> world{std::make_shared<World>()};

    world->width      = 600;
    world->height     = 600;
    world->background = {0, 0, 0};
    world->sampler    = std::make_shared<Random>(4, 83);

    world->scene.push_back(
        std::make_shared<Sphere>(atlas::math::Point{0, 0, -600}, 128.0f));
    world->scene[0]->setColour({1, 0, 0});

    world->scene.push_back(
        std::make_shared<Sphere>(atlas::math::Point{128, 32, -700}, 64.0f));
    world->scene[1]->setColour({0, 0, 1});

    world->scene.push_back(
        std::make_shared<Sphere>(atlas::math::Point{-128, 32, -700}, 64.0f));
    world->scene[2]->setColour({0, 1, 0});

    Point samplePoint{}, pixelPoint{};
    Ray<atlas::math::Vector> ray{{0, 0, 0}, {0, 0, -1}};

    float avg{1.0f / world->sampler->getNumSamples()};

    for (int r{0}; r < world->height; ++r)
    {
        for (int c{0}; c < world->width; ++c)
        {
            Colour pixelAverage{0, 0, 0};

            for (int j = 0; j < world->sampler->getNumSamples(); ++j)
            {
                ShadeRec trace_data{};
                trace_data.world = world;
                trace_data.t     = std::numeric_limits<float>::max();
                samplePoint      = world->sampler->sampleUnitSquare();
                pixelPoint.x     = c - 0.5f * world->width + samplePoint.x;
                pixelPoint.y     = r - 0.5f * world->height + samplePoint.y;
                ray.o = atlas::math::Vector{pixelPoint.x, pixelPoint.y, 0};
                bool hit{};

                for (auto obj : world->scene)
                {
                    hit |= obj->hit(ray, trace_data);
                }

                if (hit)
                {
                    pixelAverage += Colour{0.0f};
                }
            }

            world->image.push_back({pixelAverage.r * avg,
                                    pixelAverage.g * avg,
                                    pixelAverage.b * avg});
        }
    }

    saveToFile("raytrace.bmp", world->width, world->height, world->image);

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
