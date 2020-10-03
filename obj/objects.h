#ifndef OBJECTS_H
#define OBJECTS_H

#include "core/vec3.h"
#include "constants.h"
#include "core/ray.h"
#include "core/exceptions.h"

#include <cmath>
#include <memory>
#include <algorithm>
#include <initializer_list>

namespace objects
{
    class Object
    {
    public:
        //METHODS
        virtual core::Vec3 normal(const core::Vec3 &surface_point) const = 0;
        virtual std::unique_ptr<core::RayHitData> hitObject(const core::Ray &ray, const double &freq) const = 0;
        virtual double area() const = 0;

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
        Sphere::Sphere(const core::Vec3 & or, const double &rad);

        //OPERATORS
        friend std::ostream &operator<<(std::ostream &os, const Sphere &sp);

        //METHODS
        virtual core::Vec3 normal(const core::Vec3 &surface_point) const override;
        virtual std::unique_ptr<core::RayHitData> hitObject(const core::Ray &ray, const double &freq) const override;
        virtual double area() const override;

        //GETTERS AND SETTERS
        void setRadius(const double &rad);
        double getRadius() const;

    protected:
        double _radius;
    };

    class SphereWall : public Sphere
    {
    public:
        SphereWall(const SphereWall &other) = default;
        SphereWall();

        friend std::ostream &operator<<(std::ostream &os, const SphereWall &sp);
    };

    class EnergyCollector : public Sphere
    {
    public:
        static int population;

        // Change population number
        EnergyCollector();

        // change population number
        EnergyCollector(const core::Vec3 &origin);

        // Change population number
        EnergyCollector(const core::Vec3 &origin, const double &energy);

        // This constructor is only used for passing copy of the object.
        // It doesn't change any of the id, poplation or any paramterers.
        EnergyCollector(const core::Vec3 &origin, const double &energy, const int &id);

        // Copy constructor copies all data except id of the Energy collector.
        // it doesn't change population number.
        EnergyCollector(const EnergyCollector &other);

        // OPERATORS
        friend std::ostream &operator<<(std::ostream &os, const EnergyCollector &collector);
        friend EnergyCollector operator+(const EnergyCollector &left, const EnergyCollector &right);
        friend bool operator==(const EnergyCollector &left, const EnergyCollector &right);

        // This operator copies all data except Id. It doesn't change population number.
        EnergyCollector &operator=(const EnergyCollector &other);

        // METHODS
        double distanceAt(const core::Vec3 &positionHit) const;
        void collectEnergy(const std::unique_ptr<core::RayHitData> &hitdata);

        // GETTERS AND SETTERS
        void setEnergy(const double &en);
        double getEnergy() const;
        void addEnergy(const double &en);
        void setID(const int &id);
        int getID() const;

    private:
        int _id;
        double _energy;
    };

    class TriangleObj : public Object
    {
    public:
        TriangleObj();
        TriangleObj(const core::Vec3 &xCoordinate, const core::Vec3 &yCoordinate, const core::Vec3 &zCoordinate);
        TriangleObj(const std::initializer_list<double> &xCoordinate, const std::initializer_list<double> &yCoordinate, const std::initializer_list<double> &zCoordinate);
        TriangleObj(const TriangleObj &other);
        ~TriangleObj() = default;

        // OPERATORS
        TriangleObj &operator=(const TriangleObj &other);
        friend bool operator==(const TriangleObj &left, const TriangleObj &right);
        friend bool operator!=(const TriangleObj &left, const TriangleObj &right);
        friend std::ostream &operator<<(std::ostream &os, const TriangleObj &object);

        // METHODS
        core::Vec3 normal(const core::Vec3 &surface_point) const override;
        std::unique_ptr<core::RayHitData> hitObject(const core::Ray &ray, const double &freq) const override;
        bool doesHit(const core::Vec3 &point);
        double area() const override;

        // GETTERS AND SETTERS
        core::Vec3 getX() const;
        void setX(const core::Vec3 &point);

        core::Vec3 getY() const;
        void setY(const core::Vec3 &point);

        core::Vec3 getZ() const;
        void setZ(const core::Vec3 &point);

    private:
        void recalculateNormal();
        void recalculateArea();
        core::Vec3 _normal, _xCoordinate, _yCoordinate, _zCoordinate;
        double _area;
    };

} // namespace objects

#endif