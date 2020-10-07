#ifndef GENERATORS_CPP
#define GENERATORS_CPP

#include "generators.h"

namespace generators
{

#pragma region POINTSOURCE

    // CONSTRUCTORS

    PointSource::PointSource(const double &freq, const size_t &rayNum, const double &sampleSize) : _frequency(freq), _sampleSize(sampleSize) _origin(core::Vec3(0, 4, 0))
    {
        this->updateSampleSize();
    };

    // OPERATORS

    bool PointSource::operator==(const PointSource &other) const
    {
        return (_aspectRatio == other.getAspectRatio() && _focalLength == other.getFocalLength() && _origin == other.getOrigin() && _dir == other.getDirection() && _frequency == other.getFrequency());
    }

    std::ostream &operator<<(std::ostream &os, const PointSource &pointSource)
    {
        return os << "Point Source: origin: " << pointSource.getOrigin() << ", aspect ratio: " << pointSource.getAspectRatio() << ", direction: " << pointSource.getDirection() << ", frequency: " << pointSource.getFrequency();
    }

    // METHODS

    void PointSource::updateSampleSize()
    {
        _dirSquareReference = {core::Vec3(-1 * _sampleSize / 2, -1 * _sampleSize / 2, 1), core::Vec3(_sampleSize / 2, -1 * _sampleSize / 2, 1), core::Vec3(-1 * _sampleSize / 2, _sampleSize / 2, 1), core::Vec3(_sampleSize / 2, _sampleSize / 2, 1)};
    }

    // GETTERS AND SETTERS

    double PointSource::getFrequency() const
    {
        return _frequency;
    }
    void PointSource::setFrequency(const double &freq)
    {
        _freq = freq;
    }

    double PointSource::getSampleSize() const
    {
        return _sampleSize;
    }
    void PointSource::setSampleSize(const double &sampleSize)
    {
        _sampleSize = sampleSize;
    }

    core::Vec3 PointSource::getOrigin() const
    {
        return _origin;
    }
    void PointSource::setOrigin(const core::Vec3 &point)
    {
        _origin = point;
    }

    size_t PointSource::getRayNum() const
    {
        return _rayNum;
    }
    void PointSource::setRayNum(const size_t &rayNum)
    {
        _rayNum = rayNum;
    }

#pragma endregion

} // namespace generators
#endif