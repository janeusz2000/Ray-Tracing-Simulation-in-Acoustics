#ifndef OBJECTS_H
#define OBJECTS_H

#include "constants.h"
#include "core/exceptions.h"
#include "core/ray.h"
#include "core/vec3.h"

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <memory>
#include <vector>

// TODO: Change all doubles to float
// TODO: change underscore to the end

namespace objects
{
    class Object
    {
    public:
        //METHODS
        virtual ~Object(){};
        virtual core::Vec3 normal(const core::Vec3 &surface_point) const = 0;
        virtual bool hitObject(const core::Ray &ray, const double &freq, core::RayHitData *hitData) = 0;

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
        Sphere(const core::Vec3 & or, const double &rad = 1);

        //OPERATORS
        friend std::ostream &operator<<(std::ostream &os, const Sphere &sp);

        //METHODS
        core::Vec3 normal(const core::Vec3 &surface_point) const override;
        [[nodiscard]] bool hitObject(const core::Ray &ray, const double &freq, core::RayHitData *hitData) override;

        //GETTERS AND SETTERS
        double getRadius() const;
        void setRadius(const double &rad);

    protected:
        double _radius;
    };

    // When this object has two purposes: first - when ray hit this object, there is no longer need
    // to continue ray traceing. Second - the collision point is passed to energy collector, and after that specific
    // amount of energy from ray is collected depends on the distance from collision point to the sphere collectors;
    class SphereWall : public Sphere
    {
    public:
        SphereWall(const SphereWall &other) = default;
        SphereWall() { this->setRadius(constants::kSimulationRadius); }

        friend std::ostream &operator<<(std::ostream &os, const SphereWall &sp);
    };

    class EnergyCollector : public Sphere
    {
    public:
        EnergyCollector(const core::Vec3 &origin, double energy = 0) : _energy(energy) // TODO: delete argument for energy and assing 0 to energy by default
        {
            setRadius(constants::kPi * (constants::kSimulationRadius) / constants::kPopulation); // ( 2 *  pi * R ) / 2 is for dividing whole sphereWall to qual distances
            setOrigin(origin);
        }

        // OPERATORS
        friend std::ostream &operator<<(std::ostream &os, const EnergyCollector &collector);
        friend bool operator==(const EnergyCollector &left, const EnergyCollector &right);
        EnergyCollector &operator=(const EnergyCollector &other);

        // METHODS
        double distanceAt(const core::Vec3 &positionHit) const;
        void collectEnergy(const core::RayHitData &hitdata);

        // GETTERS AND SETTERS
        void setEnergy(const double &en);
        double getEnergy() const;
        void addEnergy(const double &en);
        void setID(const int &id);
        int getID() const;

    private:
        double _energy;
    };

    class TriangleObj : public Object
    {
    public:
        TriangleObj(const core::Vec3 &point1 = {1, 0, 0},
                    const core::Vec3 &point2 = {0, 1, 0},
                    const core::Vec3 &point3 = {0, 0, 1});
        TriangleObj(const TriangleObj &other);
        ~TriangleObj() = default;

        // OPERATORS
        TriangleObj &operator=(const TriangleObj &other);
        friend bool operator==(const TriangleObj &left, const TriangleObj &right);
        friend bool operator!=(const TriangleObj &left, const TriangleObj &right);
        friend std::ostream &operator<<(std::ostream &os, const TriangleObj &object);

        // METHODS
        core::Vec3 normal(const core::Vec3 &surface_point = core::Vec3()) const override;
        // https://en.cppreference.com/w/cpp/language/attributes/nodiscard
        [[nodiscard]] bool hitObject(const core::Ray &ray, const double &freq, core::RayHitData *hitData) override;

        double area() const;
        void refreshAttributes();

        // GETTERS AND SETTERS
        core::Vec3 point1() const;
        void setPoint1(const core::Vec3 &point);

        core::Vec3 point2() const;
        void setPoint2(const core::Vec3 &point);

        core::Vec3 point3() const;
        void setPoint3(const core::Vec3 &point);

    private:
        bool doesHit(const core::Vec3 &point) const;

        void recalculateNormal();
        void recalculateArea();
        bool arePointsValid();
        core::Vec3 _normal, _point1, _point2, _point3;
        double _area;
    };

} // namespace objects

#endif