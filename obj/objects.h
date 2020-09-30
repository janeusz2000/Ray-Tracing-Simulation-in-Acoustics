#ifndef OBJECTS_H
#define OBJECTS_H

#include "core/vec3.h"
#include "constants.h"
#include "core/ray.h"

#include <cmath>
#include <memory>
#include <algorithm>

namespace objects
{
    class Object
    {
    public:
        //METHODS
        virtual core::Vec3 normal(const core::Vec3 &surface_point) const = 0;
        virtual std::unique_ptr<core::RayHitData> hitObject(const core::Ray &ray, const double &freq) const = 0;
        virtual double area() const = 0;

        //GETTERS_AND_SETTERS
        void setOrigin(const core::Vec3 & or);
        core::Vec3 getOrigin() const;

    private:
        core::Vec3 _origin;
    };

    class Sphere : public Object
    {
    public:
        Sphere() : _radius(1){};
        Sphere::Sphere(const core::Vec3 & or, const double &rad);

        //OPERATORS
        friend std::ostream &operator<<(std::ostream &os, const Sphere &sp);

        //METHODS
        virtual core::Vec3 normal(const core::Vec3 &surface_point) const override;
        virtual std::unique_ptr<core::RayHitData> hitObject(const core::Ray &ray, const double &freq) const override;
        virtual double area() const override;

        //GETTERS AND SETTERS
        void setRadius(const double &rad);
        double getRadius() const;

    protected:
        double _radius;
    };

    class SphereWall : public Sphere
    {
    public:
        SphereWall(const SphereWall &other) = default;
        SphereWall();

        friend std::ostream &operator<<(std::ostream &os, const SphereWall &sp);
    };

    class EnergyCollector : public Sphere
    {
    public:
        EnergyCollector();
        EnergyCollector(size_t num)
    };

} // namespace objects

#endif