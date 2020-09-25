#include "ray.h"

namespace core
{
#pragma region METHODS

    Vec3 Ray::at(const double &time)
    {
        return _origin + time * _direction;
    }

    double Ray::phaseAt(const double &freq, const double &time)
    {
        double waveLength = kSoundSpeed / freq;
        double displacement = (_origin - at(time)).magnitude();
        return displacement / waveLength * 2 * kPi;
    }

#pragma endregion
#pragma region OPERATORS

    std::ostream &operator<<(std::ostream &os, const Ray &srcRay)
    {
        return os << "RAY origin: " << srcRay.getOrigin() << ", direction: " << srcRay.getDirection() << ", energy: " << srcRay.getEnergy();
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
} // namespace core