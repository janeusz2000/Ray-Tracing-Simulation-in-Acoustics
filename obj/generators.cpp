#ifndef GENERATORS_CPP
#define GENERATORS_CPP

#include "generators.h"
#include <cmath>
#include <iostream>

namespace generators
{

#pragma region POINTSOURCE

    // CONSTRUCTORS

    PointSource::PointSource(const double &freq, const size_t &rayNumPerRow, const double &diffusorSize) : _frequency(freq), _rayNumPerRow(rayNumPerRow), _diffusorSize(diffusorSize), _origin(core::Vec3(0, -4, 0)), _leftCorner(core::Vec3(-diffusorSize / 2, 1, -diffusorSize / 2))
    {
        this->updateDiffusorSize();
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
        _leftCorner = core::Vec3(-1 * _diffusorSize / 2, -1, -1 * _diffusorSize / 2);
    }

    core::Ray PointSource::GenerateRay(const size_t &xIter, const size_t &yIter, bool incldueRandom)
    {
        if (xIter >= _rayNumPerRow && yIter >= _rayNumPerRow)
        {
            throw exception::outOfSize(xIter, yIter, _rayNumPerRow);
        }

        double u, v;
        if (incldueRandom)
        {
            v = (static_cast<double>(xIter) + this->getRandom()) / static_cast<double>(_rayNumPerRow - 1) * _diffusorSize;
            u = (static_cast<double>(yIter) + this->getRandom()) / static_cast<double>(_rayNumPerRow - 1) * _diffusorSize;
        }
        else
        {
            v = (static_cast<double>(xIter)) / static_cast<double>(_rayNumPerRow - 1) * _diffusorSize;
            u = (static_cast<double>(yIter)) / static_cast<double>(_rayNumPerRow - 1) * _diffusorSize;
        }
        return core::Ray(_origin, _leftCorner + u * core::Vec3(1, 0, 0) + v * core::Vec3(0, 0, 1) - _origin);
    }

    // PRIVATE METHODS
    double PointSource::getRandom() const
    {
        std::random_device rd;
        std::mt19937_64 engine(rd());
        std::normal_distribution<double> dist{0, 1};
        return dist(rd);
    }

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