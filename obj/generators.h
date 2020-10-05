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

        // GETTERS AND SETTERS
        double getAspectRatio() const;
        void setAspecRatio(const double &num);

        double setFocalLength() const;
        void setFocalLength(double const &num);

        core::Vec3 getOrigin() const;
        void setOrigin(const core::Vec3 &point);

        core::Vec3 getUpperLeftCorner() const;
        void setUpperLeftCorner(const core::Vec3 &point);

    private:
        double _aspectRatio, _focalLength;
        core::Vec3 _origin, _upperLeftCorner;
    };
}; // namespace generators

#endif