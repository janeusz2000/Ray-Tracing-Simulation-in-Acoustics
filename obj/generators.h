#ifndef GENERATORS_H
#define GENERATORS_H

#include "core/vec3.h"
#include "core/ray.h"
#include "constants.h"
#include "core/exceptions.h"

#include <initializer_list>
#include <iostream>

namespace generators
{
    class PointSource // This object is similar to Camera in graphics ray-tracer, that why name convension is simillar to camera
    {
    public:
        PointSource();
        PointSource(const core::Vec3 &dir);
        PointSource(const core::Vec3 &dir, const core::Vec3 &pos);
        PointSource(const std::initializer_list<double> &dir, const std::initializer_list<double> &pos);
        PointSource(const PointSource &) = default;
        ~PointSource() = default;

        // OPERATORS
        PointSource &operator=(const PointSource &) = default;
        bool operator==(const PointSource &other) const;
        friend std::ostream &operator<<(std::ostream &os, const PointSource &pointSource);

    private:
        double _aspectRatio, _focalLength;
        core::Vec3 _origin, _upperLeftCorner;
    };
}; // namespace generators

#endif