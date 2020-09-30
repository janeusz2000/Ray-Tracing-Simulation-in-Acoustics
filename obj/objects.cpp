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
        _energy += hitdata->energy; // TODO: How energy is splitted between energycollectors
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
} // namespace objects