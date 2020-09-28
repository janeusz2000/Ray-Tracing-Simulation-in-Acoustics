#ifndef OBJECTS_H
#define OBJECTS_H

#include "core/vec3.h"
#include "constants.h"
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

        //GETTERS_AND_SETTERS
        void setOrigin(const Vec3 & or);
        Vec3 getOrigin() const;

    protected:
        core::Vec3 _origin;
    };

    //===========================

    class Sphere : public Object
    {
    public:
        Sphere() = delete;
        Sphere::Sphere(const core::Vec3 & or, const double &rad) : _origin(or), _radius(rad){};
        core::Vec3 normal(const core::Vec3 &surface_point) override;
        std::unique_ptr<core::RayHitData> hitObject(const core::Ray &ray, const double &freq) override;

        //GETTERS AND SETTERS
        void setRadius(const double &rad);
        double getRadius() const;

    protected:
        double _radius;
    };

    //===========================

    class SphereWall : public Sphere
    {
    public:
        SphereWall(const SphereWall &other) = default;
        SphereWall() : Sphere(Vec3(0, 0, 0), kSimulationRadius){};
    }

} // namespace objects

#endif