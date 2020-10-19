#include "objects.h"
#include <sstream>

// TODO: Change all doubles to float
// TODO: change underscore to the end

namespace objects
{
#pragma region OBJECT

    void Object::setOrigin(const core::Vec3 & or)
    {
        _origin = or ;
    }
    core::Vec3 Object::getOrigin() const
    {
        return _origin;
    }

#pragma endregion
#pragma region SPHERE

    core::Vec3 Sphere::normal(const core::Vec3 &surface_point) const
    {
        return (surface_point - getOrigin()).normalize();
    }

    Sphere::Sphere(const core::Vec3 & or, const double &rad)
    {
        this->setOrigin(or);
        _radius = rad;
    }

    bool Sphere::hitObject(const core::Ray &ray, const double &freq, core::RayHitData *hitData)
    {
        core::Vec3 rVec3 = ray.getOrigin() - this->getOrigin();

        // this calculates variables that are neccesary to calculate times at which ray hits the object
        double beta = 2 * rVec3.scalarProduct(ray.getDirection());
        double gamma = rVec3.scalarProduct(rVec3) - _radius * _radius;
        double discriminant = beta * beta - 4 * gamma;
        // ==================================================

        if (discriminant < 0) // making sure that ray hits the sphere
        {
            return false;
        }

        double temp = std::sqrt(discriminant);
        double time1 = (-beta - temp) / 2;
        double time2 = (-beta + temp) / 2;

        if (time1 < 0 && time2 > 0) // Ray inside sphere
        {
            double time = std::max(time1, time2);
            core::Vec3 collision = ray.at(time);
            *hitData = core::RayHitData(time, normal(collision), ray, freq);
            return true;
        }
        else if (time1 > 0 && time2 > 0) // ray in front of the sphere
        {
            double time = std::min(time1, time2);
            core::Vec3 collision = ray.at(time);
            *hitData = core::RayHitData(time, normal(collision), ray, freq);
            return true;
        }
        return false;
    }

    std::ostream &operator<<(std::ostream &os, const Sphere &sp)
    {
        return os << "Sphere origin: " << sp.getOrigin() << ", radius: " << sp.getRadius() << " [m]";
    }

    // GETTERS AND SETTERS
    void Sphere::setRadius(const double &rad)
    {
        _radius = rad;
    }
    double Sphere::getRadius() const
    {
        return _radius;
    }

#pragma endregion
#pragma region SPHEREWALL

    std::ostream &operator<<(std::ostream &os, const SphereWall &sp)
    {
        return os << "SphereWall origin: " << sp.getOrigin() << ", radius: " << sp.getRadius() << " [m]";
    }

#pragma endregion
#pragma region ENERGY_COLLECTOR

    // OPERATORS
    EnergyCollector &EnergyCollector::operator=(const EnergyCollector &other)
    {
        if (other == *this)
        {
            return *this;
        }

        this->setOrigin(other.getOrigin());
        this->setRadius(other.getRadius());
        _energy = other.getEnergy();

        return *this;
    }

    std::ostream &operator<<(std::ostream &os, const EnergyCollector &collector)
    {
        return os << "Energy Collector. Origin: " << collector.getOrigin() << ", Energy collected: " << collector.getEnergy();
    }

    bool objects::operator==(const EnergyCollector &left, const EnergyCollector &right)
    {
        return (left.getOrigin() == right.getOrigin() && left.getRadius() == right.getRadius() && left.getEnergy() == right.getEnergy());
    }

    // METHODS

    double EnergyCollector::distanceAt(const core::Vec3 &positionHit) const
    {
        return (getOrigin() - positionHit).magnitude();
    }

    void EnergyCollector::collectEnergy(const core::RayHitData &hitdata)
    {
        // TODO: Energy distribution between many collectors
        _energy += hitdata.energy;
    }

    // GETTERS AND SETTERS
    void EnergyCollector::setEnergy(const double &en)
    {
        _energy = en;
    }
    double EnergyCollector::getEnergy() const
    {
        return _energy;
    }
    void EnergyCollector::addEnergy(const double &en)
    {
        _energy += en;
    }

#pragma endregion
#pragma region TRIANGLEOBJ

    // CONSTRUCTORS

    TriangleObj::TriangleObj(const core::Vec3 &point1,
                             const core::Vec3 &point2,
                             const core::Vec3 &point3) : _point1(point1), _point2(point2), _point3(point3)
    {
        this->arePointsValid();
        this->refreshAttributes();
    }

    TriangleObj::TriangleObj(const TriangleObj &other)
    {
        this->setPoint1(other.point1());
        this->setPoint2(other.point2());
        this->setPoint3(other.point3());
        this->refreshAttributes();
    }

    // OPERATORS

    TriangleObj &TriangleObj::operator=(const TriangleObj &other)
    {
        this->setPoint1(other.point1());
        this->setPoint2(other.point2());
        this->setPoint3(other.point3());

        this->refreshAttributes();

        return *this;
    }

    bool operator==(const TriangleObj &left, const TriangleObj &right)
    {
        std::vector<core::Vec3> vertexVec({left.point1(), left.point2(), left.point3()});
        std::vector<core::Vec3> refVec({right.point1(), right.point2(), right.point3()});

        for (size_t ind = 0; ind < vertexVec.size(); ++ind)
        {
            if (std::find(vertexVec.begin(), vertexVec.end(), refVec.at(ind)) == vertexVec.end())
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const TriangleObj &left, const TriangleObj &right)
    {
        return (!(left == right));
    }

    std::ostream &operator<<(std::ostream &os, const TriangleObj &object)
    {
        return os << "Triangle Object, vertex: " << object.point1() << ", " << object.point2() << ", " << object.point3();
    }

    // METHODS

    core::Vec3 TriangleObj::normal(const core::Vec3 &surface_point) const
    {
        return _normal;
    }

    bool TriangleObj::hitObject(const core::Ray &ray, const double &freq, core::RayHitData *hitData)
    {
        // if ray direction is parpedicular to normal, there is no hit. It can be translated into
        // checking if scalarProduct of the ray.direction and normal is close or equal to zero.
        double parpCoeff = ray.getDirection().scalarProduct(_normal);
        if (std::abs(parpCoeff) <= constants::kAccuracy)
        {
            return false;
        }

        // Following code calculates time at which ray is hitting surface where triangle is positioned
        double time = (-1 * (ray.getOrigin() - _point3)).scalarProduct(_normal) / parpCoeff;

        // Following code is making sure that ray doesn't hit the same object.
        if (time < constants::kHitAccuracy)
        {
            return false;
        }

        core::Vec3 surfaceHit = ray.at(time);

        if (doesHit(surfaceHit))
        {
            *hitData = core::RayHitData(time, _normal, ray, freq);
            return true;
        }
        return false;
    }

    bool TriangleObj::doesHit(const core::Vec3 &point) const
    {
        core::Vec3 vecA = _point1 - point;
        core::Vec3 vecB = _point2 - point;
        core::Vec3 vecC = _point3 - point;

        double alpha = vecB.crossProduct(vecC).magnitude() / 2; //  Area of the triangle made with point and w triangle points.
        double beta = vecC.crossProduct(vecA).magnitude() / 2;
        double gamma = vecA.crossProduct(vecB).magnitude() / 2;

        return (((alpha + beta + gamma) > _area + constants::kHitAccuracy) ? false : true);
    }

    double TriangleObj::area() const
    {
        return _area;
    }

    void TriangleObj::refreshAttributes()
    {
        this->recalculateArea();
        this->recalculateNormal();
        this->setOrigin((_point1 + _point2 + _point3) / 3);
    }

    // PRIVATE METHODS
    void TriangleObj::recalculateArea()
    {
        core::Vec3 vecA = _point1 - _point2;
        core::Vec3 vecB = _point1 - _point3;
        _area = vecA.crossProduct(vecB).magnitude() / 2;
    }

    void TriangleObj::recalculateNormal()
    {
        core::Vec3 vecA = _point1 - _point2;
        core::Vec3 vecB = _point1 - _point3;
        core::Vec3 perpendicular = vecA.crossProduct(vecB);
        _normal = perpendicular.normalize();
    }

    bool TriangleObj::arePointsValid()
    {
        if (_point1 == _point2 || _point1 == _point3 || _point2 == _point3)
        {
            std::stringstream ss;
            ss << "TriangleObj arguments error. You cannot have duplications of the same point in constructor 1: " << _point1 << ", 2: " << _point2 << ", 3: " << _point3;
            throw std::invalid_argument(ss.str());
        }

        core::Vec3 alpha = _point1 - _point2;
        core::Vec3 beta = _point1 - _point3;

        if (alpha.crossProduct(beta) == core::Vec3(0, 0, 0))
        {
            throw std::invalid_argument("Triangle object couldn't be constructed. You cannot have all points of the object linedup straight");
        }
        return true;
    }

    // GETTERS AND SETTERS
    core::Vec3 TriangleObj::point1() const
    {
        return _point1;
    }
    void TriangleObj::setPoint1(const core::Vec3 &point)
    {
        this->_point1 = point;
    }

    core::Vec3 TriangleObj::point2() const
    {
        return _point2;
    }
    void TriangleObj::setPoint2(const core::Vec3 &point)
    {
        this->_point2 = point;
    }

    core::Vec3 TriangleObj::point3() const
    {
        return _point3;
    }
    void TriangleObj::setPoint3(const core::Vec3 &point)
    {
        this->_point3 = point;
    }

#pragma endregion
} // namespace objects