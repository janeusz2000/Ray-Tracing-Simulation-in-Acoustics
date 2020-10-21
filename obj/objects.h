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

namespace objects
{
    class Object
    {
    public:
        //METHODS
        virtual ~Object(){};
        virtual core::Vec3 normal(const core::Vec3 &surfacePoint) const = 0;
        virtual bool hitObject(const core::Ray &ray, const double &freq, core::RayHitData *hitData) = 0;

        //GETTERS_AND_SETTERS
        void setOrigin(const core::Vec3 & or);
        core::Vec3 getOrigin() const;

    private:
        core::Vec3 origin_;
    };

    class Sphere : public Object
    {
    public:
        Sphere() : radius_(1){};
        Sphere(const core::Vec3 & or, const double &rad = 1);

        //OPERATORS
        friend std::ostream &operator<<(std::ostream &os, const Sphere &sp);

        //METHODS
        core::Vec3 normal(const core::Vec3 &surfacePoint) const override;
        [[nodiscard]] bool hitObject(const core::Ray &ray, const double &freq, core::RayHitData *hitData) override;

        //GETTERS AND SETTERS
        double getRadius() const;
        void setRadius(const double &rad);

    protected:
        double radius_;
    };

    // When this object has two purposes: first - when ray hit this object, there is no longer need
    // to continue ray traceing. Second - the collision point is passed to energy collector, and after that specific
    // amount of energy from ray is collected depends on the distance from collision point to the sphere collectors;
    class SphereWall : public Sphere
    {
    public:
        SphereWall(double radius) { setRadius(radius); }
        SphereWall(const SphereWall &other) = default;

        friend std::ostream &operator<<(std::ostream &os, const SphereWall &sp);
    };

    class EnergyCollector : public Sphere
    {
    public:
        explicit EnergyCollector(const core::Vec3 &origin, double radius = constants::kDefaultEnergyCollectorRadius) : energy_(0) 
        {
            setRadius(radius); 
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
        double energy_;
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
        core::Vec3 normal(const core::Vec3 &surfacePoint = core::Vec3()) const override;
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
        core::Vec3 normal_, point1_, point2_, point3_;
        double area_;
    };

} // namespace objects

#endif