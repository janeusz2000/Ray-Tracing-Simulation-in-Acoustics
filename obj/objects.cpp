#include "objects.h"

namespace objects
{
    int EnergyCollector::population = 0;
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
        setOrigin(or);
        _radius = rad;
    }

    std::unique_ptr<core::RayHitData> Sphere::hitObject(const core::Ray &ray, const double &freq) const
    {
        core::Vec3 rVec3 = ray.getOrigin() - getOrigin();

        // this calculates variables that are neccesary to calculate times at which ray hits the object SphereWall.
        double beta = 2 * rVec3.scalar_product(ray.getDirection());
        double gamma = rVec3.scalar_product(rVec3) - _radius * _radius;
        double discriminant = beta * beta - 4 * gamma;
        // ==================================================

        double time1, time2;

        if (discriminant < 0)
        {
            return std::unique_ptr<core::RayHitData>(nullptr);
        }
        else
        {
            double temp = std::sqrt(discriminant);
            time1 = (-beta - temp) / 2;
            time2 = (-beta + temp) / 2;
        }
        if (time1 < 0 && time2 < 0)
        {
            return std::unique_ptr<core::RayHitData>(nullptr);
        }
        else if (time1 < 0)
        {
            return std::unique_ptr<core::RayHitData>(nullptr);
        }
        else
        {
            double time = std::min({time1, time2});
            core::Vec3 collision = ray.at(time);
            return std::make_unique<core::RayHitData>(time, collision, normal(collision), ray.getDirection(), ray.getEnergy(), ray.phaseAt(freq, time));
        }
    }

    double Sphere::area() const
    {
        return constants::kPi * _radius * _radius;
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

    SphereWall::SphereWall()
    {
        this->setOrigin(core::Vec3(0, 0, 0));
        this->setRadius(constants::kSimulationRadius);
    }

    std::ostream &operator<<(std::ostream &os, const SphereWall &sp)
    {
        return os << "SphereWall origin: " << sp.getOrigin() << ", radius: " << sp.getRadius() << " [m]";
    }

#pragma endregion
#pragma region ENERGY_COLLECTOR

    // CONSTRUCTORS

    EnergyCollector::EnergyCollector() : _id(population), _energy(0)
    {
        setOrigin(core::Vec3(0, 4, 0));
        setRadius(constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        population++;
    };

    EnergyCollector::EnergyCollector(const core::Vec3 &origin) : _id(population), _energy(0)
    {
        setOrigin(origin);
        setRadius(constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        population++;
    };

    EnergyCollector::EnergyCollector(const core::Vec3 &origin, const double &energy) : _id(population), _energy(energy)
    {
        setOrigin(origin);
        setRadius(constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        population++;
    };

    EnergyCollector::EnergyCollector(const core::Vec3 &origin, const double &energy, const int &id)
    {
        setEnergy(energy);
        setID(id);
        setOrigin(origin);
        setRadius(constants::kPi * constants::kSimulationRadius / constants::kPopulation);
    };

    EnergyCollector::EnergyCollector(const EnergyCollector &other)
    {
        setRadius(other.getRadius());
        setOrigin(other.getOrigin());
        setEnergy(other.getEnergy());
        setID(other.getID());
    }

    // OPERATORS
    EnergyCollector &EnergyCollector::operator=(const EnergyCollector &other)
    {
        if (other == *this)
        {
            return *this;
        }

        setOrigin(other.getOrigin());
        setRadius(other.getRadius());
        _energy = other.getEnergy();

        return *this;
    }

    std::ostream &operator<<(std::ostream &os, const EnergyCollector &collector)
    {
        return os << "Energy Collector ID: " << collector.getID() << ", Energy collected: " << collector.getEnergy();
    }

    EnergyCollector operator+(const EnergyCollector &left, const EnergyCollector &right)
    {
        if (left.getOrigin() != right.getOrigin())
        {
            throw exception::differentPositions();
        }
        else
        {
            return EnergyCollector(left.getOrigin(), left.getEnergy() + right.getEnergy());
        }
    }

    bool objects::operator==(const EnergyCollector &left, const EnergyCollector &right)
    {
        return (left.getID() == right.getID() && left.getOrigin() == right.getOrigin() && left.getRadius() == right.getRadius() && left.getEnergy() == right.getEnergy());
    }

    // METHODS

    double EnergyCollector::distanceAt(const core::Vec3 &positionHit) const
    {
        return (getOrigin() - positionHit).magnitude();
    }

    void EnergyCollector::collectEnergy(const std::unique_ptr<core::RayHitData> &hitdata)
    {
        _energy += hitdata->energy; // TODO: Energy distribution between many collectors
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
    void EnergyCollector::setID(const int &id)
    {
        _id = id;
    }
    int EnergyCollector::getID() const
    {
        return _id;
    }

#pragma endregion
#pragma region TRIANGLEOBJ

    // CONSTRUCTORS

    TriangleObj::TriangleObj() : TriangleObj(core::Vec3(1, 0, 0), core::Vec3(0, 1, 0), core::Vec3(0, 0, 1)){};
    TriangleObj::TriangleObj(const core::Vec3 &xCoordinate, const core::Vec3 &yCoordinate, const core::Vec3 &zCoordinate) : _xCoordinate(xCoordinate), _yCoordinate(yCoordinate), _zCoordinate(zCoordinate)
    {
        if (xCoordinate == yCoordinate || xCoordinate == zCoordinate || yCoordinate == zCoordinate)
        {
            throw exception::invalidConstructor();
        }
        this->refreshAttributes();
    }

    TriangleObj::TriangleObj(const std::initializer_list<double> &xCoordinate, const std::initializer_list<double> &yCoordinate, const std::initializer_list<double> &zCoordinate)
    {
        this->setX(core::Vec3(xCoordinate));
        this->setY(core::Vec3(yCoordinate));
        this->setZ(core::Vec3(zCoordinate));

        if (_xCoordinate == _yCoordinate || _xCoordinate == _zCoordinate || _yCoordinate == _zCoordinate)
        {
            throw exception::invalidConstructor();
        }
        this->refreshAttributes();
    }

    TriangleObj::TriangleObj(const TriangleObj &other)
    {
        this->setX(other.getX());
        this->setY(other.getY());
        this->setZ(other.getZ());
        this->refreshAttributes();
    }

    // OPERATORS

    TriangleObj &TriangleObj::operator=(const TriangleObj &other)
    {
        this->setX(other.getX());
        this->setY(other.getY());
        this->setZ(other.getZ());

        this->refreshAttributes();

        return *this;
    }

    bool operator==(const TriangleObj &left, const TriangleObj &right)
    {
        std::vector<core::Vec3> vertexVec({left.getX(), left.getY(), left.getZ()});
        std::vector<core::Vec3> refVec({right.getX(), right.getY(), right.getZ()});

        for (size_t ind = 0; ind < vertexVec.size(); ++ind)
        {
            if (std::find(vertexVec.begin(), vertexVec.end(), refVec.at(ind)) != vertexVec.end())
            {
                continue;
            }
            else
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
        return os << "Triangle Object, vertex: " << object.getX() << ", " << object.getY() << ", " << object.getZ();
    }

    // METHODS

    core::Vec3 TriangleObj::normal(const core::Vec3 &surface_point) const
    {
        return _normal;
    }

    std::unique_ptr<core::RayHitData> TriangleObj::hitObject(const core::Ray &ray, const double &freq) const
    {
        // TODO: add real calculation of the ray hit data
        return std::make_unique<core::RayHitData>(core::RayHitData(0, core::Vec3(0, 0, 0), core::Vec3(0, 0, 1), core::Vec3(0, 0, 1), 20, 20));
    }

    double TriangleObj::area() const
    {
        // TODO: make real calculation of the area
        return 69;
    }

    void TriangleObj::refreshAttributes()
    {
        this->recalculateArea();
        this->recalculateNormal();
    }

    // PRIVATE METHODS

    void TriangleObj::recalculateArea()
    {
        // TODO: makereal recalculation of the Area
        _area = 69;
    }

    void TriangleObj::recalculateNormal()
    {
        core::Vec3 vecA = _xCoordinate - _yCoordinate;
        core::Vec3 vecB = _xCoordinate - _zCoordinate;
        core::Vec3 perpendicular = vecA.cross_product(vecB);
        _normal = perpendicular.normalize();
    }

    // GETTERS AND SETTERS
    core::Vec3 TriangleObj::getX() const
    {
        return _xCoordinate;
    }
    void TriangleObj::setX(const core::Vec3 &point)
    {
        this->_xCoordinate = point;
    }

    core::Vec3 TriangleObj::getY() const
    {
        return _yCoordinate;
    }
    void TriangleObj::setY(const core::Vec3 &point)
    {
        this->_yCoordinate = point;
    }

    core::Vec3 TriangleObj::getZ() const
    {
        return _zCoordinate;
    }
    void TriangleObj::setZ(const core::Vec3 &point)
    {
        this->_zCoordinate = point;
    }

#pragma endregion
} // namespace objects