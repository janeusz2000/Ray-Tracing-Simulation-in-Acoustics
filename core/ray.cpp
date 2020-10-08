#include "ray.h"

namespace core
{
#pragma region CONSTRUCTORS

    Ray::Ray(const Vec3 &origin, const Vec3 &direction, const double &energy)
    {
        if (direction == Vec3(0, 0, 0))
        {
            throw exception::invalidConstructor();
        }

        _origin = origin;
        _direction = direction / direction.magnitude();
        _energy = energy;
    }

    Ray::Ray(const std::initializer_list<double> &origin, const std::initializer_list<double> &dir, const double &energy)
    {
        if (origin.size() != 3 || dir.size() != 3 || dir == Vec3(0, 0, 0))
            throw exception::invalidConstructor();

        _origin = Vec3(origin);
        _direction = Vec3(dir) / Vec3(dir).magnitude();
        _energy = energy;
    }

    Ray::Ray(const Ray &other)
    {
        _origin = other.getOrigin();
        _direction = other.getDirection();
        _energy = other.getEnergy();
    }

#pragma endregion
#pragma region METHODS

    Vec3 Ray::at(const double &time) const
    {
        return _origin + time * _direction;
    }

    double Ray::phaseAt(const double &freq, const double &time) const
    {
        // TODO: check for negative or zero frequency?
        double waveLength = constants::kSoundSpeed / freq;
        // TODO: at() performs useless operation of adding origin while you need to remove it again
        // Please optimize.
        double displacement = (_origin - at(time)).magnitude();
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
        // TODO: this does not compare energy?
        return (left.getOrigin() == right.getOrigin() && left.getDirection() == right.getDirection());
    }

#pragma endregion
#pragma region SETTERS_GETTERS

    void Ray::setOrigin(const Vec3 &origin)
    {
        _origin = origin;
    }

    Vec3 Ray::getOrigin() const
    {
        return _origin;
    }

    void Ray::setDirection(const Vec3 &direction)
    {
        _direction = direction;
    }

    Vec3 Ray::getDirection() const
    {
        return _direction;
    }

    void Ray::setEnergy(const double &num)
    {
        _energy = num;
    }

    double Ray::getEnergy() const
    {
        return _energy;
    }

#pragma endregion
#pragma region RAYHITDATA

    bool RayHitData::operator==(const RayHitData &other) const
    {
        // TODO: this will likely be useless, because you check equality of floating point numbers.
        return (other.time == time, other.collisionPoint == collisionPoint && other.direction == direction && other.normal == normal && other.energy == energy && other.phase == phase);
    }

    std::ostream &operator<<(std::ostream &os, const RayHitData &rayData)
    {
        return os << "Collision point: " << rayData.collisionPoint << ", incoming ray direction: " << rayData.direction << ", normal: " << rayData.normal << ", time: " << rayData.time << ", energy: " << rayData.energy << ", phase " << rayData.phase << " [radians]";
    }

#pragma endregion
} // namespace core