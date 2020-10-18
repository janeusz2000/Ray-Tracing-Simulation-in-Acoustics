#include "generators.h"
#include <random>
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
        return (_frequency == other.frequency() && _numOfRaysPerRow == other.numOfRaysPerRow() && _origin == other.origin() && _sampleSize == other.sampleSize() && _leftCorner == other.getLeftCorner());
    }

    std::ostream &operator<<(std::ostream &os, const PointSource &pointSource)
    {
        return os << "Point Source: origin: " << pointSource.origin() << ", number of rays per row: " << pointSource.numOfRaysPerRow() << ", diffusor size: " << pointSource.sampleSize() << ", frequency: " << pointSource.frequency();
    }

    // METHODS

    void PointSource::updateSampleSize()
    {
        _leftCorner = core::Vec3(-1 * _sampleSize / 2, -1 * _sampleSize / 2, 1);
    }

    core::Ray PointSource::generateRay(const size_t &xIter, const size_t &yIter)
    {
        if (xIter >= _numOfRaysPerRow && yIter >= _numOfRaysPerRow)
        {
            std::stringstream ss;
            ss << "Arguments of x and y are out of range. Arguments are: x: " << xIter << " / " << _numOfRaysPerRow - 1 << ", y: " << yIter << " / " << _numOfRaysPerRow;
            throw std::out_of_range(ss.str().c_str());
        }
        double v = (static_cast<double>(xIter) + _randomGen->getNext()) / static_cast<double>(_numOfRaysPerRow - 1) * _sampleSize;
        double u = (static_cast<double>(yIter) + _randomGen->getNext()) / static_cast<double>(_numOfRaysPerRow - 1) * _sampleSize;
        return core::Ray(_origin, _leftCorner + u * core::Vec3(1, 0, 0) + v * core::Vec3(0, 1, 0) - _origin);
    }

    // GETTERS AND SETTERS

    double PointSource::frequency() const
    {
        return _frequency;
    }
    void PointSource::setFrequency(const float freq)
    {
        _frequency = freq;
    }

    double PointSource::sampleSize() const
    {
        return _sampleSize;
    }
    void PointSource::setSampleSize(const float diffusorSize)
    {
        _sampleSize = diffusorSize;
        updateSampleSize();
    }

    core::Vec3 PointSource::origin() const
    {
        return _origin;
    }
    void PointSource::setOrigin(const core::Vec3 &point)
    {
        _origin = point;
    }

    size_t PointSource::numOfRaysPerRow() const
    {
        return _numOfRaysPerRow;
    }
    void PointSource::setNumOfRaysPerRow(const size_t rayNum)
    {
        _numOfRaysPerRow = rayNum;
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