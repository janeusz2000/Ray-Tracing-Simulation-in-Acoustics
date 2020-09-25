#ifndef RAY_H
#define RAY_H

#include <initializer_list>
#include <iostream>
#include "core/vec3.h"
#include "core/constants.h"

namespace core
{
    class Ray
    {
    public:
        Ray() : Ray(Vec3(0, 0, 0), Vec3(0, 0, 0), 0){};
        Ray(const Vec3 &origin, const Vec3 &direction, const double &energy) : _origin(origin), _direction(direction.normalize()), _energy(energy){};
        Ray(const std::initializer_list<double> &origin, const std::initializer_list<double> &dir, const double &energy);
        ~Ray() = default;

        //METHODS
        Vec3 at(const double &time);
        double phaseAt(const double &freq, const double &time);

        //OPERATORS
        friend std::ostream &operator<<(std::ostream &os, const Ray &srcRay);

        //SETTERS AND GETTERS
        void setOrigin(const Vec3 &origin);
        Vec3 getOrigin() const;
        void setDirection(const Vec3 &direction);
        Vec3 getDirection() const;
        void setEnergy(const double &num);
        double getEnergy() const;

    private:
        Vec3 _origin, _direction;
        double _energy;
    };
} // namespace core

#endif