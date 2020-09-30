#ifndef RAY_H
#define RAY_H

#include <initializer_list>
#include <iostream>
#include "core/vec3.h"
#include "constants.h"
#include "core/exceptions.h"

namespace core
{
    class Ray
    {
    public:
        Ray() : Ray(Vec3(0, 0, 0), Vec3(0, 0, 1), 0){};
        Ray(const Vec3 &origin, const Vec3 &direction, const double &energy);
        Ray(const Vec3 &origin, const Vec3 &direction) : _origin(origin), _direction(direction.normalize()), _energy(0){};
        Ray(const std::initializer_list<double> &origin, const std::initializer_list<double> &dir, const double &energy);
        Ray(const std::initializer_list<double> &origin, const std::initializer_list<double> &dir) : Ray(origin, dir, 0){};
        Ray(const Ray &other);
        ~Ray() = default;

        //METHODS
        Vec3 at(const double &time) const;
        double phaseAt(const double &freq, const double &time) const;

        //OPERATORS
        friend std::ostream &operator<<(std::ostream &os, const Ray &srcRay);
        friend bool operator==(const Ray &left, const Ray &right);

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

    struct RayHitData
    {
        RayHitData() = delete;
        RayHitData(const double &t, const Vec3 &collision,
                   const Vec3 &norm, const Vec3 &dir,
                   const double &en, const double &ph) : time(t), collisionPoint(collision), direction(dir), normal(norm), energy(en), phase(ph){};
        ~RayHitData() = default;
        RayHitData(const RayHitData &) = default;

        // OPERATORS
        RayHitData &operator=(const RayHitData &) = default;
        bool operator==(const RayHitData &other) const;
        friend std::ostream &operator<<(std::ostream &os, const RayHitData &rayData);

        // VARIABLES
        Vec3 collisionPoint, direction, normal;
        double time, energy, phase;
    };
} // namespace core

#endif