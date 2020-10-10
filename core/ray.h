#ifndef RAY_H
#define RAY_H

#include "constants.h"
#include "core/exceptions.h"
#include "core/vec3.h"

#include <initializer_list>
#include <iostream>

namespace core
{
    class Ray
    {
    public:
        Ray(const Vec3 &origin= Vec3(0, 0, 0), const Vec3 &direction = Vec3(0, 0, 1), const double &energy = 0);

        // TODO: do not pass simple types using const type&, as it does not make sense.
        // They are quick to copy no need to use const references with floats, as it make
        // the code harder to read.
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
        RayHitData(const double &t, const Vec3 &norm, const Ray &ray, const double &freq) : time(t), normal(norm), frequency(freq)
        {
            collisionPoint = ray.at(time);
            direction = ray.getDirection();
            energy = ray.getEnergy();
            phase = ray.phaseAt(frequency, time);
        }
        ~RayHitData() = default;
        RayHitData(const RayHitData &) = default;

        // OPERATORS
        bool operator==(const RayHitData &other) const;
        friend std::ostream &operator<<(std::ostream &os, const RayHitData &rayData);

        // VARIABLES
        Vec3 collisionPoint, direction, normal;
        double time, energy, phase, frequency;
    };
} // namespace core

#endif