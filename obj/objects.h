#ifndef OBJECTS_H
#define OBJECTS_H

#include "core/vec3.h"
#include "core/constants.h"
#include "core/ray.h"
#include <cmath>
#include <memory>

namespace objects
{

    class Object
    {
    public:
        virtual core::Vec3 normal(const core::Vec3 &surface_point) = 0;
        virtual std::unique_ptr<core::RayHitData> hitObject(const core::Ray &ray, const double &freq) = 0;

    protected:
        core::Vec3 origin;
    };

    class SphereWall : public Object
    {
    public:
        core::Vec3 normal(const core::Vec3 &surface_point) override;
        std::unique_ptr<core::RayHitData> hitObject(const core::Ray &ray, const double &freq) override;

    private:
        double _radius;
    }

} // namespace objects

#endif