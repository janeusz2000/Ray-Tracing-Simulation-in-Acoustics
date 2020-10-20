#include "generators.h"
#include <random>

// TODO: Change all doubles to float

namespace generators
{
#pragma region RANDOMGENERATORS

    // double EngineUniformRandom(double min, double max);
    // double EngineGaussianRandom(double mean, double standDev);
    // double EngineZero(double passA, double passB);

#pragma endregion
#pragma region POINTSOURCE

    // OPERATORS

    bool PointSource::operator==(const PointSource &other) const
    {
        return (frequency_ == other.frequency() && numOfRaysPerRow_ == other.numOfRaysPerRow() && origin_ == other.origin() && sampleSize_ == other.sampleSize() && leftCorner_ == other.getLeftCorner());
    }

    std::ostream &operator<<(std::ostream &os, const PointSource &pointSource)
    {
        return os << "Point Source: origin: " << pointSource.origin() << ", number of rays per row: " << pointSource.numOfRaysPerRow() << ", diffusor size: " << pointSource.sampleSize() << ", frequency: " << pointSource.frequency();
    }

    // METHODS

    void PointSource::updateSampleSize()
    {
        leftCorner_ = core::Vec3(-1 * sampleSize_ / 2, -1 * sampleSize_ / 2, 1);
    }

    core::Ray PointSource::generateRay(const size_t &xIter, const size_t &yIter)
    {
        if (xIter >= numOfRaysPerRow_ && yIter >= numOfRaysPerRow_)
        {
            std::stringstream ss;
            ss << "Arguments of x and y are out of range. Arguments are: x: " << xIter << " / " << numOfRaysPerRow_ - 1 << ", y: " << yIter << " / " << numOfRaysPerRow_;
            throw std::out_of_range(ss.str().c_str());
        }
        double v = (static_cast<double>(xIter) + randomGen_->getNext()) / static_cast<double>(numOfRaysPerRow_ - 1) * sampleSize_;
        double u = (static_cast<double>(yIter) + randomGen_->getNext()) / static_cast<double>(numOfRaysPerRow_ - 1) * sampleSize_;
        return core::Ray(origin_, leftCorner_ + u * core::Vec3(1, 0, 0) + v * core::Vec3(0, 1, 0) - origin_);
    }

    // GETTERS AND SETTERS

    double PointSource::frequency() const
    {
        return frequency_;
    }
    void PointSource::setFrequency(const double &freq)
    {
        frequency_ = freq;
    }

    double PointSource::sampleSize() const
    {
        return sampleSize_;
    }
    void PointSource::setDiffusorSize(const double &diffusorSize)
    {
        sampleSize_ = diffusorSize;
        updateSampleSize();
    }

    core::Vec3 PointSource::origin() const
    {
        return origin_;
    }
    void PointSource::setOrigin(const core::Vec3 &point)
    {
        origin_ = point;
    }

    size_t PointSource::numOfRaysPerRow() const
    {
        return numOfRaysPerRow_;
    }
    void PointSource::setNumOfRaysPerRow(const size_t &rayNum)
    {
        numOfRaysPerRow_ = rayNum;
    }

    core::Vec3 PointSource::getLeftCorner() const
    {
        return leftCorner_;
    }

    void PointSource::setLeftCorner(const core::Vec3 &point)
    {
        leftCorner_ = point;
    }
#pragma endregion

} // namespace generators