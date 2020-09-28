#include "objects.h"

namespace objects
{
#pragma region OBJECT

    void Object::setOrigin(const Vec3 & or)
    {
        _origin = or ;
    }
    Vec3 Object::getOrigin() const
    {
        return _origin;
    }

#pragma endregion
#pragma region SPHERE

    virtual core::Vec3 Sphere::normal(const core::Vec3 &surface_point) override
    {
        return (surface_point - origin).normalize();
    }

    virtual std::unique_ptr<core::RayHitData> Sphere::hitObject(const core::Ray &ray, const double &freq) override
    {
        core::Vec3 rVec3 = ray.getOrigin() - origin;

        // this calculates variables that are neccesary to calculate times at which ray hits the object SphereWall.
        double beta = 2 * rVec3.scalar_product(ray.getDirection());
        double gamma = rVec3.scalar_product(rVec3) - _radius * _radius;
        double discriminant = beta * beta - 4 * gamma;
        // ==================================================

        double time1, time2;

        if (discriminant < 0)
        {
            return std::make_unique<core::RayHitData>(nullptr);
        }
        else
        {
            double temp = std::sqrt(discriminant);
            time1 = (-beta - temp) / 2;
            time2 = (-beta + temp) / 2;
        }
        if (time1 < 0 && time2 < 0)
        {
            return std::make_unique<core::RayHitData>(nullptr);
        }
        else if (time1 < 0)
        {
            return std::make_unique<core::RayHitData>(nullptr);
        }
        else
        {
            double time = std::min({time1, time2});
            core::Vec3 collision = ray.at(time);
            return std::make_unique<core::RayHitData>(time, collision, normal(collision), ray.getDirection(), ray.getEnergy(), ray.phaseAt(freq, time));
        }
    }

    // GETTERS AND SETTERS
    void Sphere::setRadius(const double &rad)
    {
        _radius = rad;
    }
    double getRadius() const
    {
        return _radius;
    }

#pragma endregion
#pragma region SPHEREWALL

    static SphereWall &SphereWall::Get()
    {
        return _SphereWallInstance;
    }

#pragma endregion
} // namespace objects