#ifndef GENERATORS_CPP
#define GENERATORS_CPP

#include "generators.h"

namespace generators
{

#pragma region POINTSOURCE

    // PointSource(const double &freq);
    //     PointSource(const double &freq, const core::Vec3 &dir);
    //     PointSource(const double &freq, const core::Vec3 &dir, const core::Vec3 &pos);
    //     PointSource(const double &freq, const std::initializer_list<double> &dir, const std::initializer_list<double> &pos);

    // private:
    //     double _aspectRatio, _focalLength, _frequency;
    //     core::Vec3 _origin, _upperLeftCorner;

    // TODO: faceing directiin of the point source
    // PointSource::PointSource() : _freq(1000), _aspectRatio(1), _focalLength(1), _origin(core::Vec3(0, 4, 0)), _upperLeftCorner(core::Vec3(0, 0, 0)){};

    // CONSTRUCTORS

    PointSource::PointSource(const double &freq) : _frequency(freq), _aspectRatio(1), _focalLength(1), _origin(core::Vec3(0, 4, 0)), _dir(core::Vec3(0, 0, -1)){};
    PointSource::PointSource(const double &freq, const core::Vec3 &dir) : _frequency(freq), _aspectRatio(1), _focalLength(1), _origin(core::Vec3(0, 4, 0)), _dir(dir){};
    PointSource::PointSource(const double &freq, const core::Vec3 &dir, const core::Vec3 &origin) : _frequency(freq), _aspectRatio(1), _focalLength(1), _origin(origin), _dir(dir){};
    PointSource::PointSource(const double &freq, const std::initializer_list<double> &dir, const std::initializer_list<double> &pos) _frequency(freq), _aspectRatio(1), _focalLength(1), _origin(core::Vec3(origin)), _dir(core::Vec3(dir)){};

    // OPERATORS

    bool PointSource::operator==(const PointSource &other) const
    {
        return (_aspectRatio == other.getAspectRatio() && _focalLength == other.getFocalLength() && _origin == other.getOrigin() && _dir == other.getDirection() && _frequency == other.getFrequency());
    }

    std::ostream &operator<<(std::ostream &os, const PointSource &pointSource)
    {
        return os << "Point Source: origin: " << pointSource.getOrigin() << ", aspect ratio: " << pointSource.getAspectRatio() << ", direction: " << pointSource.getDirection() << ", frequency: " << pointSource.getFrequency();
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