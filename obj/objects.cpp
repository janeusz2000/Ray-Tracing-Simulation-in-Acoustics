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
        this->setOrigin(or);
        _radius = rad;
    }

    std::unique_ptr<core::RayHitData> Sphere::hitObject(const core::Ray &ray, const double &freq) const
    {
        core::Vec3 rVec3 = ray.getOrigin() - this->getOrigin();

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
        // TODO: Drop else. It will be easier to follow what is going on.
        else
        {
            double temp = std::sqrt(discriminant);
            time1 = (-beta - temp) / 2;
            time2 = (-beta + temp) / 2;
        }
        // TODO: here we would need some comments, what each cases mean.
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
            // TODO: I think min() takes 2 params, no need to pack then in an initializer list.
            double time = std::min({time1, time2});
            core::Vec3 collision = ray.at(time);
            return std::make_unique<core::RayHitData>(time, collision, normal(collision), ray.getDirection(), ray.getEnergy(), ray.phaseAt(freq, time));
        }
    }

    double Sphere::area() const
    {
        // TODO: ಠ_ಠ This is wrong. Do you even use this method? If not, remove.
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
        // TODO: Why not seting default params using these constants? You copied so much code in those 4 constructors.
        this->setOrigin(core::Vec3(0, 4, 0));
        this->setRadius(constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        population++;
    };

    EnergyCollector::EnergyCollector(const core::Vec3 &origin) : _id(population), _energy(0)
    {
        this->setOrigin(origin);
        this->setRadius(constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        population++;
    };

    EnergyCollector::EnergyCollector(const core::Vec3 &origin, const double &energy) : _id(population), _energy(energy)
    {
        this->setOrigin(origin);
        this->setRadius(constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        population++;
    };

    EnergyCollector::EnergyCollector(const core::Vec3 &origin, const double &energy, const int &id)
    {
        this->setEnergy(energy);
        this->setID(id);
        this->setOrigin(origin);
        this->setRadius(constants::kPi * constants::kSimulationRadius / constants::kPopulation);
    };

    EnergyCollector::EnergyCollector(const EnergyCollector &other)
    {
        this->setRadius(other.getRadius());
        this->setOrigin(other.getOrigin());
        this->setEnergy(other.getEnergy());
        this->setID(other.getID());
    }

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
            // TODO: This may cause issues, when you want to keep collectors in a list of unique_ptrs.
            // Since this one creates a new object and it seems that energy collectors are just data objects.
            // My guess would be that you want to mutate collectors?
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
        // TODO: Energy distribution between many collectors
        _energy += hitdata->energy;
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

    TriangleObj::TriangleObj() : TriangleObj(core::Vec3(1, 0, 0), core::Vec3(0, 1, 0), core::Vec3(0, 0, 1))
    {
        this->refreshAttributes();
    }
    TriangleObj::TriangleObj(const core::Vec3 &xCoordinate, const core::Vec3 &yCoordinate, const core::Vec3 &zCoordinate) : _xCoordinate(xCoordinate), _yCoordinate(yCoordinate), _zCoordinate(zCoordinate)
    {
        // TODO: Why don't you hide the first part of the condition inside arePointsInvalid() ?
        if (xCoordinate == yCoordinate || xCoordinate == zCoordinate || yCoordinate == zCoordinate || this->arePointsInvalid())
        {
            throw exception::invalidConstructor();
        }

        this->refreshAttributes();
    }

    TriangleObj::TriangleObj(const std::initializer_list<double> &xCoordinate, const std::initializer_list<double> &yCoordinate, const std::initializer_list<double> &zCoordinate) : TriangleObj(core::Vec3(xCoordinate), core::Vec3(yCoordinate), core::Vec3(zCoordinate)){};

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
        return os << "Triangle Object, vertex: " << object.getX() << ", " << object.getY() << ", " << object.getZ();
    }

    // METHODS

    core::Vec3 TriangleObj::normal(const core::Vec3 &surface_point) const
    {
        return _normal;
    }

    std::unique_ptr<core::RayHitData> TriangleObj::hitObject(const core::Ray &ray, const double &freq) const
    {
        // TODO: "ray.getDirection().scalar_product(_normal)" is calculated 2x. Cache the result.
        // if ray direction is parpedicular to normal, there is no hit. It can be translated into
        // checking if scalar_product of the ray.direction and normal is close or equal to zero.
        // TODO: What if ray points away of the triangle? Do you support hits from behind the triangle?
        if (std::abs(ray.getDirection().scalar_product(_normal)) <= constants::kAccuracy)
        {
            return std::unique_ptr<core::RayHitData>(nullptr);
        }

        // Following code calculates time at which ray is hitting surface where triangle is positioned
        double time = (-1 * (ray.getOrigin() - _zCoordinate)).scalar_product(_normal) / (ray.getDirection().scalar_product(_normal));

        // Following code is making sure that ray doesn't hit the same object.
        if (time < constants::kHitAccuracy)
        {
            return std::unique_ptr<core::RayHitData>(nullptr);
        }

        core::Vec3 surfaceHit = ray.at(time);

        return (this->doesHit(surfaceHit) ? std::make_unique<core::RayHitData>(time, surfaceHit, _normal, ray.getDirection(), ray.getEnergy(), ray.phaseAt(freq, time))
                                          : std::unique_ptr<core::RayHitData>(nullptr));
    }

    bool TriangleObj::doesHit(const core::Vec3 &point) const
    {
        core::Vec3 vecA = _xCoordinate - point;
        core::Vec3 vecB = _yCoordinate - point;
        core::Vec3 vecC = _zCoordinate - point;

        double alpha = vecB.cross_product(vecC).magnitude() / 2; //  Area of the triangle made with point and w triangle points.
        double beta = vecC.cross_product(vecA).magnitude() / 2;
        double gamma = vecA.cross_product(vecB).magnitude() / 2;

        // TODO: Why do you multiply by 0.99? Consider making it a constant and using it here.
        return (((alpha + beta + gamma) > _area + constants::kHitAccuracy * 0.99) ? false : true);
    }

    double TriangleObj::area() const
    {
        return _area;
    }

    void TriangleObj::refreshAttributes()
    {
        this->recalculateArea();
        this->recalculateNormal();
        this->setOrigin((_xCoordinate + _yCoordinate + _zCoordinate) / 3);
    }

    // PRIVATE METHODS

    // TODO: Why do you have all these separate methods recalculating some small parts,
    // while you could just move all the code into refreshAttributes() and cache the results.
    void TriangleObj::recalculateArea()
    {
        core::Vec3 vecA = _xCoordinate - _yCoordinate;
        core::Vec3 vecB = _xCoordinate - _zCoordinate;
        _area = vecA.cross_product(vecB).magnitude() / 2;
    }

    void TriangleObj::recalculateNormal()
    {
        core::Vec3 vecA = _xCoordinate - _yCoordinate;
        core::Vec3 vecB = _xCoordinate - _zCoordinate;
        core::Vec3 perpendicular = vecA.cross_product(vecB);
        _normal = perpendicular.normalize();
    }

    bool TriangleObj::arePointsInvalid()
    {
        core::Vec3 alpha = _xCoordinate - _yCoordinate;
        core::Vec3 beta = _xCoordinate - _zCoordinate;

        return (alpha.cross_product(beta) == core::Vec3(0, 0, 0));
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