#ifndef RAY_H
#define RAY_H



#include "constants.h"
#include "core/vec3.h"

#include <initializer_list>
#include <iostream>

namespace core
{
    class Ray
    {
    public:
        Ray(const Vec3 &origin = Vec3(0, 0, 0), const Vec3 &direction = Vec3(0, 0, 1), float energy = 0);

        //METHODS
        Vec3 at(float time) const;
        float phaseAt(float freq, float time) const;

        //OPERATORS
        friend std::ostream &operator<<(std::ostream &os, const Ray &srcRay);
        friend bool operator==(const Ray &left, const Ray &right);

        //SETTERS AND GETTERS
        void setOrigin(const Vec3 &origin);
        Vec3 getOrigin() const;
        void setDirection(const Vec3 &direction);
        Vec3 getDirection() const;
        void setEnergy(float num);
        float getEnergy() const;

    private:
        Vec3 origin_, direction_;
        float energy_;
    };

    struct RayHitData
    {
        RayHitData() = default;
        RayHitData(float t, const Vec3 &norm, const Ray &ray, float freq)
         : time(t), normal(norm), frequency(freq), collisionPoint(ray.at(t)),
          direction(ray.getDirection()), energy(ray.getEnergy()),
           phase(ray.phaseAt(freq, t)), origin(ray.getOrigin()) {};
        ~RayHitData() = default;
        RayHitData(const RayHitData &) = default;

        // Operators? No kidding!
        // These comments are just visual clutter. I would remove them. It is more important to e.g
        // describe what this does if the impl. is somehow not really self explanatory (e.g skips some params).
        // OPERATORS
        bool operator==(const RayHitData &other) const;
        friend std::ostream &operator<<(std::ostream &os, const RayHitData &rayData);

        // VARIABLES
        // Why do you store the members of the Ray separately?
        Vec3 collisionPoint, direction, normal, origin;
        float time, energy, phase, frequency;
    };
} // namespace core

#endif