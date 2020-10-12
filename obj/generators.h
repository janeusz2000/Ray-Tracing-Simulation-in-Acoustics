#ifndef GENERATORS_H
#define GENERATORS_H

#include "constants.h"
#include "core/exceptions.h"
#include "core/ray.h"
#include "core/vec3.h"

#include <initializer_list>
#include <iostream>
#include <random>

namespace generators
{

    double getRandom();
    class PointSource final // This object is similar to Camera in graphics ray-tracer, that why name convension is simillar to camera
    {
    public:
        PointSource() = delete;
        PointSource(const double &freq, const size_t &rayNumPerRow, const double &diffusorSize);
        PointSource(const PointSource &) = default;
        ~PointSource() = default;

        // OPERATORS
        PointSource &operator=(const PointSource &) = default;
        bool operator==(const PointSource &other) const;
        friend std::ostream &operator<<(std::ostream &os, const PointSource &pointSource);

        // METHODS
        void updateDiffusorSize();
        // TODO: Parameter names are not self explanatory. Please update.
        core::Ray GenerateRay(const size_t &xIter, const size_t &yIter, bool incldueRandom = false);

        // GETTERS AND SETTERS
        double getFrequency() const;
        void setFrequency(const double &freq);

        double getDiffusorSize() const;
        void setDiffusorSize(const double &diffusorSize);

        size_t getRayNumPerRow() const;
        void setRayNumPerRow(const size_t &raynum);

        core::Vec3 getOrigin() const;
        void setOrigin(const core::Vec3 &point);

        core::Vec3 PointSource::getLeftCorner() const;
        void PointSource::setLeftCorner(const core::Vec3 &point);

    private:
        core::Vec3 _origin, _leftCorner;
        double _frequency, _diffusorSize;
        size_t _rayNumPerRow;
        
    };

} // namespace generators

#endif
