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

    // TODO faceing directiin of the point source
    PointSource::PointSource() : _freq(1000), _aspectRatio(1), _focalLength(1), _origin(core::Vec3(0, 4, 0)), _upperLeftCorner(core::Vec3(0, 0, 0)){};

#pragma endregion

} // namespace generators
#endif