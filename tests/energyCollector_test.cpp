#include "constants.h"
#include "core/exceptions.h"
#include "core/ray.h"
#include "core/vec3.h"
#include "obj/objects.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <cmath>

// TODO: Change all doubles to float

namespace objects
{
    TEST(ENERGYCOLLECTOR_CONSTRUCTOR, All_Possible_Constructors)
    {
        EnergyCollector object1(core::Vec3(0, 0, 2));

        ASSERT_EQ(object1.getRadius(), constants::kPi * (constants::kSimulationRadius / 2) / constants::kPopulation);
        ASSERT_EQ(object1.getOrigin(), core::Vec3(0, 0, 2));
        ASSERT_EQ(object1.getEnergy(), 0);

        EnergyCollector object2(core::Vec3(0, 0, 2));

        ASSERT_EQ(object2.getRadius(), constants::kPi * (constants::kSimulationRadius / 2) / constants::kPopulation);
        ASSERT_EQ(object2.getOrigin(), core::Vec3(0, 0, 2));
        ASSERT_EQ(object2.getEnergy(), 0);

        EnergyCollector object3(core::Vec3(0, 0, -1));

        ASSERT_EQ(object3.getRadius(), constants::kPi * (constants::kSimulationRadius / 2) / constants::kPopulation);
        ASSERT_EQ(object3.getOrigin(), core::Vec3(0, 0, -1));
        ASSERT_EQ(object3.getEnergy(), 0);

        EnergyCollector object4(core::Vec3(0, 0, -1), 50);

        ASSERT_EQ(object4.getRadius(), constants::kPi * (constants::kSimulationRadius / 2) / constants::kPopulation);
        ASSERT_EQ(object4.getOrigin(), core::Vec3(0, 0, -1));
        ASSERT_EQ(object4.getEnergy(), 50);

        EnergyCollector object5(core::Vec3(0, 0, -1), 50);

        ASSERT_EQ(object5.getRadius(), constants::kPi * (constants::kSimulationRadius / 2) / constants::kPopulation);
        ASSERT_EQ(object5.getOrigin(), core::Vec3(0, 0, -1));
        ASSERT_EQ(object5.getEnergy(), 50);

        EnergyCollector object6(object2);
        ASSERT_EQ(object6.getRadius(), constants::kPi * (constants::kSimulationRadius / 2) / constants::kPopulation);
        ASSERT_EQ(object6.getOrigin(), core::Vec3(0, 0, 2));
        ASSERT_EQ(object6.getEnergy(), 0);

        EnergyCollector object7(core::Vec3(0, 0, 2));
        ASSERT_EQ(object7.getRadius(), constants::kPi * (constants::kSimulationRadius / 2) / constants::kPopulation);
        ASSERT_EQ(object7.getOrigin(), core::Vec3(0, 0, 2));
        ASSERT_EQ(object7.getEnergy(), 0);
    }

    TEST(ENERGYCOLLECTOR_OPERATORS, Operator_ostream)
    {
        EnergyCollector object8(core::Vec3(0, 0, 2));
        EnergyCollector object9(core::Vec3(0, 0, 4));
        EnergyCollector object10(core::Vec3(1, 0, 9));
        EnergyCollector object11(core::Vec3(0, 5, 3));
        EnergyCollector object12(core::Vec3(2, 0, 1));

        std::stringstream ss1;
        std::stringstream ss2;
        std::stringstream ss3;
        std::stringstream ss4;
        std::stringstream ss5;

        ss1 << object8;
        ss2 << object9;
        ss3 << object10;
        ss4 << object11;
        ss5 << object12;

        ASSERT_EQ(ss1.str(), "Energy Collector. Origin: Vec3(0, 0, 2), Energy collected: 0");
        ASSERT_EQ(ss2.str(), "Energy Collector. Origin: Vec3(0, 0, 4), Energy collected: 0");
        ASSERT_EQ(ss3.str(), "Energy Collector. Origin: Vec3(1, 0, 9), Energy collected: 0");
        ASSERT_EQ(ss4.str(), "Energy Collector. Origin: Vec3(0, 5, 3), Energy collected: 0");
        ASSERT_EQ(ss5.str(), "Energy Collector. Origin: Vec3(2, 0, 1), Energy collected: 0");
    }

    TEST(ENERGYCOLLECTOR_OPERATORS, Operator_Equal_Equal)
    {
        EnergyCollector object1(core::Vec3(0, 0, 0), 0);
        EnergyCollector object2(core::Vec3(0, 0, 0), 0);
        ASSERT_EQ(object1, object2);

        EnergyCollector object3(core::Vec3(1, 2, 0), 20);
        EnergyCollector object4(core::Vec3(1, 2, 0), 20);
        ASSERT_EQ(object3, object4);

        EnergyCollector object5(core::Vec3(std::sqrt(2), 1, 2), 20);
        EnergyCollector object6(core::Vec3(std::sqrt(2), 1, 2), 20);
        ASSERT_EQ(object5, object6);

        EnergyCollector object7(core::Vec3(std::sqrt(3) * std::sqrt(3), std::sqrt(2) * std::sqrt(2), 0), 2);
        EnergyCollector object8(core::Vec3(3, 2, 0), 2);
        ASSERT_EQ(object7, object8);
    }

    TEST(ENERGYCOLLECTOR_OPERATORS, Operator_equal)
    {
        EnergyCollector object1(core::Vec3(0, 0, 0), 0);
        EnergyCollector object2 = object1;
        ASSERT_EQ(object1, object2);

        EnergyCollector object3(core::Vec3(1, 2, 0), 20);
        EnergyCollector object4 = object3;
        ASSERT_EQ(object3, object4);

        EnergyCollector object5(core::Vec3(std::sqrt(2), 1, 2), 20);
        EnergyCollector object6 = object5;
        ASSERT_EQ(object5, object6);

        EnergyCollector object7(core::Vec3(std::sqrt(3), 0, std::sqrt(2)), std::sqrt(2));
        EnergyCollector object8 = object7;
        ASSERT_EQ(object7, object8);
    }

    TEST(ENERGYCOLLECTOR_METHODS, Method_DistanceAt)
    {
        EnergyCollector object1(core::Vec3(0, 0, 0), 0);

        ASSERT_EQ(object1.distanceAt(core::Vec3(0, 4, 0)), 4);
        ASSERT_EQ(object1.distanceAt(core::Vec3(3, 0, 4)), 5);
    }

    TEST(ENERGYCOLLECTOR_METHODS, Method_CollectEnergy)
    {
        // TODO: Energy distribution between many collectors
        // TODO: Phase impact on the given energy to the collector

        EnergyCollector object1(core::Vec3(0, 0, 0), 0);
        core::RayHitData temp(core::RayHitData(50, core::Vec3(0, 1, 0).normalize(), core::Ray({0, 1, 0}, {1, 0, 0}, 50), 1000));

        object1.collectEnergy(temp);
        ASSERT_EQ(object1.getEnergy(), 50);
    }

} // namespace objects
