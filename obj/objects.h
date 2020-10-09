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
#include <vector>

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
        // TODO: Why surface point is zero by default?
        virtual core::Vec3 normal(const core::Vec3 &surface_point = core::Vec3()) const override;
        // TODO: Do not use unique_ptr with RayHitData. This is a data transfer object, that

        // can be easily copied and no need to construct it on the heap. It will be also faster
        // as you will not need to constantly allocate memory.
        //
        // You can change this to sth like:
        //  virtual bool hitObject(const core::Ray &ray, const double &freq, RayHitData* out) const = 0;
        //
        // Then in the code you will have nicely readable pieces like:
        //
        //  RayHitData data:
        //  if (object->hitObject(ray, freq, &data)) {
        //  object was hit!
        //  }
        //
        virtual std::unique_ptr<core::RayHitData> hitObject(const core::Ray &ray, const double &freq) const override;
        virtual double area() const override;

        //GETTERS AND SETTERS
        // TODO: make & formatting consistent.
        void setRadius(const double &rad);
        double getRadius() const;

    protected:
        double _radius;
    };

    // TODO: Add a comment what this object is doing.
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
        // TODO: Having static variables is a sign of bad design. If you want to keep track of this,
        // you should create some kind of factory that makes those collectors, or maybe just keep
        // them all in a vector somewhere.
        static int population;

        // These constructors changes static int population number
        EnergyCollector();
        EnergyCollector(const core::Vec3 &origin);
        EnergyCollector(const core::Vec3 &origin, const double &energy);

        // This constructor is only used for passing copy of the object.
        // It doesn't change any of the id, poplation or any paramterers.
        EnergyCollector(const core::Vec3 &origin, const double &energy, const int &id);

        // Copy constructor copies all data except id of the Energy collector.
        // it doesn't change static int population number.
        // TODO: in which circumstances you want to copy an EnergyCollector?
        EnergyCollector(const EnergyCollector &other);

        // OPERATORS
        friend std::ostream &operator<<(std::ostream &os, const EnergyCollector &collector);
        friend EnergyCollector operator+(const EnergyCollector &left, const EnergyCollector &right);
        friend bool operator==(const EnergyCollector &left, const EnergyCollector &right);

        // This operator copies all data except Id. It doesn't change population number.
        EnergyCollector &operator=(const EnergyCollector &other);

        // METHODS
        // TODO: Do not pass unique_ptr to the method, unless you want to transfer ownership.
        // You either want to pass RayHitData* or const RayHitData& if hitData cannot be ever null.
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
        // TODO: I am confused by the parameter names, why vectors are called coordinates?
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
        core::Vec3 normal(const core::Vec3 &surface_point = core::Vec3()) const override;
        std::unique_ptr<core::RayHitData> hitObject(const core::Ray &ray, const double &freq) const override;
        bool doesHit(const core::Vec3 &point) const;
        double area() const override;
        void refreshAttributes();

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
        bool arePointsValid();
        core::Vec3 _normal, _xCoordinate, _yCoordinate, _zCoordinate;
        double _area;
    };

} // namespace objects

#endif