#ifndef GENERATORS_CPP
#define GENERATORS_CPP

#include "generators.h"
#include <cmath>
#include <iostream>

namespace generators
{
#pragma region GLOBALMETHODS

    double getRandom()
    {
        std::random_device rd;
        std::mt19937_64 engine(rd());
        std::normal_distribution<double> dist{0, 1};
        return dist(rd);
    }

#pragma endregion
#pragma region POINTSOURCE

    // CONSTRUCTORS

    PointSource::PointSource(const double &freq, const size_t &rayNumPerRow, const double &diffusorSize) : _frequency(freq), _rayNumPerRow(rayNumPerRow), _diffusorSize(diffusorSize), _origin(core::Vec3(0, 0, 4))
    {
        updateDiffusorSize();
    }

    // OPERATORS

    bool PointSource::operator==(const PointSource &other) const
    {
        return (_frequency == other.getFrequency() && _rayNumPerRow == other.getRayNumPerRow() && _origin == other.getOrigin() && _diffusorSize == other.getDiffusorSize() && _leftCorner == other.getLeftCorner());
    }

    std::ostream &operator<<(std::ostream &os, const PointSource &pointSource)
    {
        return os << "Point Source: origin: " << pointSource.getOrigin() << ", number of rays per row: " << pointSource.getRayNumPerRow() << ", diffusor size: " << pointSource.getDiffusorSize() << ", frequency: " << pointSource.getFrequency();
    }

    // METHODS

    void PointSource::updateDiffusorSize()
    {
        _leftCorner = core::Vec3(-1 * _diffusorSize / 2, -1 * _diffusorSize / 2, 1);
    }

    core::Ray PointSource::GenerateRay(const size_t &xIter, const size_t &yIter, bool incldueRandom)
    {
        if (xIter >= _rayNumPerRow && yIter >= _rayNumPerRow)
        {
            std::stringstream ss;
            ss << "Arguments of x and y are out of range. Arguments are: x: " << xIter << " / " << _rayNumPerRow - 1 << ", y: " << yIter << " / " << _rayNumPerRow;
            throw std::out_of_range(ss.str().c_str());
        }

        double u, v;
        // TODO: Using randomness in tests will make it very hard to debug them. Using two different paths,
        // one in test one in real code means that there can be bugs that you will never see.
        //
        // The way it should be done is:
        // 1. This class should take random number generator in constructor
        // 2. in real code you create a real generator
        // 3. in tests you pass a fake generator that you can precisely configure

        if (incldueRandom)
        {
            v = (static_cast<double>(xIter) + getRandom()) / static_cast<double>(_rayNumPerRow - 1) * _diffusorSize;
            u = (static_cast<double>(yIter) + getRandom()) / static_cast<double>(_rayNumPerRow - 1) * _diffusorSize;
        }
        else
        {
            v = (static_cast<double>(xIter)) / static_cast<double>(_rayNumPerRow - 1) * _diffusorSize;
            u = (static_cast<double>(yIter)) / static_cast<double>(_rayNumPerRow - 1) * _diffusorSize;
        }
        return core::Ray(_origin, _leftCorner + u * core::Vec3(1, 0, 0) + v * core::Vec3(0, 1, 0) - _origin);
    }

    // PRIVATE METHODS
    // TODO: This function does not look like it belongs to this class. Move it outside to
    // an unnamed namespace and move to the top of this file.

    // GETTERS AND SETTERS

    double PointSource::getFrequency() const
    {
        return _frequency;
    }
    void PointSource::setFrequency(const double &freq)
    {
        _frequency = freq;
    }

    double PointSource::getDiffusorSize() const
    {
        return _diffusorSize;
    }
    void PointSource::setDiffusorSize(const double &diffusorSize)
    {
        _diffusorSize = diffusorSize;
        // TODO: Why update diffusor size is not called here? This is what setters and getters are for.
        // If the only thing your getters and setters are doing is exporing variables, then I would not
        // add them, but make variables public instead.
    }

    core::Vec3 PointSource::getOrigin() const
    {
        return _origin;
    }
    void PointSource::setOrigin(const core::Vec3 &point)
    {
        _origin = point;
    }

    size_t PointSource::getRayNumPerRow() const
    {
        return _rayNumPerRow;
    }
    void PointSource::setRayNumPerRow(const size_t &rayNum)
    {
        _rayNumPerRow = rayNum;
    }

    core::Vec3 PointSource::getLeftCorner() const
    {
        return _leftCorner;
    }

    void PointSource::setLeftCorner(const core::Vec3 &point)
    {
        _leftCorner = point;
    }
#pragma endregion

} // namespace generators
#endif