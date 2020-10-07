#ifndef GENERATORS_CPP
#define GENERATORS_CPP

#include "generators.h"

namespace generators
{

#pragma region POINTSOURCE

    // CONSTRUCTORS

    PointSource::PointSource(const double &freq, const size_t &rayNumPerRow, const double &diffusorSize, const size_t &samplePerPixel) : _frequency(freq), _diffusorSize(diffusorSize) _origin(core::Vec3(0, 4, 0)), _rayNumPerRow(rayNumPerRow), _samplePerPixel(samplePerPixel);
    {
        this->updateDiffusorSize();
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

    void PointSource::updatediffusorSize()
    {
        _leftCorner = core::Vec3(-1 * _diffusorSize / 2, -1 * _diffusorSize / 2, 1);
    }

    core::Ray PointSource::GenerateRay(const size_t &xIter, const size_t &yIter)
    {
        if (xIter >= _rayNumPerRow && yIter >= _rayNumPerRow)
        {
            throw exception::outOfSize(xIter, yIter, _rayNumPerRow);
        }

        double u = (static_cast<double>(xIter) + _dist(_engine)) / static_cast<double>(_rayNumPerRow - 1);
        double v = (static_cast<double>(uIter) + _dist(_engine)) / static_cast<double>(_rayNumPerRow - 1);

        return Ray(_origin, _leftCorner + u * core::Vec3(1, 0, 0) + v * core::Vec3(0, 1, 0) - _origin);
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

    double PointSource::getdiffusorSize() const
    {
        return _diffusorSize;
    }
    void PointSource::setdiffusorSize(const double &diffusorSize)
    {
        _diffusorSize = diffusorSize;
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