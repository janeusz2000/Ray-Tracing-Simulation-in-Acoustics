#include "ray.h"

// TODO: Change all doubles to float

namespace core
{
#pragma region CONSTRUCTORS

    Ray::Ray(const Vec3 &origin, const Vec3 &direction, const double &energy)
    {
        if (direction == Vec3(0, 0, 0))
        {
            throw std::invalid_argument("Ray object couldn't be constructed. You cannot have direction of the ray equal to Vec3(0, 0, 0)");
        }

        origin_ = origin;
        direction_ = direction / direction.magnitude();
        energy_ = energy;
    }

#pragma endregion
#pragma region METHODS

    Vec3 Ray::at(const double &time) const
    {
        return origin_ + time * direction_;
    }

    double Ray::phaseAt(const double &freq, const double &time) const
    {
        if (freq <= constants::kAccuracy || time <= constants::kAccuracy)
        {
            std::stringstream ss;
            ss << "Could not calculate phase at, because input frequency or time is close or equal to zero. Values are: freq: " << freq << ", time: " << time;
            throw std::invalid_argument(ss.str().c_str());
        }
        double waveLength = constants::kSoundSpeed / freq;
        double displacement = (origin_ - at(time)).magnitude();
        return displacement / waveLength * 2 * constants::kPi;
    }

#pragma endregion
#pragma region OPERATORS

    std::ostream &operator<<(std::ostream &os, const Ray &srcRay)
    {
        return os << "RAY origin: " << srcRay.getOrigin() << ", direction: " << srcRay.getDirection() << ", energy: " << srcRay.getEnergy();
    }

    bool operator==(const Ray &left, const Ray &right)
    {
        return (left.getOrigin() == right.getOrigin() && left.getDirection() == right.getDirection());
    }

#pragma endregion
#pragma region SETTERS_GETTERS

    void Ray::setOrigin(const Vec3 &origin)
    {
        origin_ = origin;
    }

    Vec3 Ray::getOrigin() const
    {
        return origin_;
    }

    void Ray::setDirection(const Vec3 &direction)
    {
        direction_ = direction;
    }

    Vec3 Ray::getDirection() const
    {
        return direction_;
    }

    void Ray::setEnergy(const double &num)
    {
        energy_ = num;
    }

    double Ray::getEnergy() const
    {
        return energy_;
    }

#pragma endregion
#pragma region RAYHITDATA

    bool RayHitData::operator==(const RayHitData &other) const
    {
        return (std::abs(other.time - time) <= constants::kAccuracy && other.collisionPoint - collisionPoint == Vec3(0, 0, 0) && other.direction - direction == Vec3(0, 0, 0) && other.normal - normal == Vec3(0, 0, 0) && std::abs(other.energy - energy) <= constants::kAccuracy && std::abs(other.phase - phase) <= constants::kAccuracy);
    }

    std::ostream &operator<<(std::ostream &os, const RayHitData &rayData)
    {
        return os << "Collision point: " << rayData.collisionPoint << ", incoming ray direction: " << rayData.direction << ", normal: " << rayData.normal << ", time: " << rayData.time << ", energy: " << rayData.energy << ", phase " << rayData.phase << " [radians]";
    }

#pragma endregion
} // namespace core