#include "objects.h"

namespace objects
{

#pragma region SPHEREWALL

    core::Vec3 SphereWall::normal(const core::Vec3 &surface_point) override
    {
        return (surface_point - origin).normalize();
    }

    std::unique_ptr<core::RayHitData> SphereWall::hitObject(const core::Ray &ray, const double &freq) override
    {
        core::Vec3 rVec3 = ray.getOrigin() - origin;

        // this calculates variables that are neccesary to calculate times at which ray hits the SphereWall.
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

#pragma endregion
} // namespace objects