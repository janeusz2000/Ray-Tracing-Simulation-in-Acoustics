#ifndef GENERATORS_CPP
#define GENERATORS_CPP

#include "generators.h"

namespace generators
{
#pragma region RANDOMGENERATORS

    double EngineUniformRandom()
    {
        std::random_device rd;
        std::mt19937_64 engine(rd);
        std::uniform_real_distribution<double> dist{0, 1};
        return dist(engine);
    }
    double EngineGaussianRandom()
    {
        std::random_device rd;
        std::mt19937_64 engine(rd);
        std::normal_distribution<double> dist{0, 1};
        return dist(engine);
    }
    double EngineZero()
    {
        return 0;
    }

#pragma endregion
#pragma region POINTSOURCE

    // CONSTRUCTORS

    PointSource::PointSource(const double &freq, const size_t &rayNumPerRow, const double &diffusorSize, double (*gen)() = EngineGaussianRandom) : _frequency(freq), _rayNumPerRow(rayNumPerRow), _diffusorSize(diffusorSize), _origin(core::Vec3(0, 0, 4))
    {
        _gen = gen;
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

    core::Ray PointSource::GenerateRay(const size_t &xIter, const size_t &yIter)
    {
        if (xIter >= _rayNumPerRow && yIter >= _rayNumPerRow)
        {
            std::stringstream ss;
            ss << "Arguments of x and y are out of range. Arguments are: x: " << xIter << " / " << _rayNumPerRow - 1 << ", y: " << yIter << " / " << _rayNumPerRow;
            throw std::out_of_range(ss.str().c_str());
        }
        double v = (static_cast<double>(xIter) + _gen()) / static_cast<double>(_rayNumPerRow - 1) * _diffusorSize;
        double u = (static_cast<double>(yIter) + _gen()) / static_cast<double>(_rayNumPerRow - 1) * _diffusorSize;
        return core::Ray(_origin, _leftCorner + u * core::Vec3(1, 0, 0) + v * core::Vec3(0, 1, 0) - _origin);
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