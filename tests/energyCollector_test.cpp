#ifndef ENERGYCOLLECTOR_TEST_CPP
#define ENERGYCOLLECTOR_TEST_CPP

#include "constants.h"
#include "core/exceptions.h"
#include "core/ray.h"
#include "core/vec3.h"
#include "obj/objects.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <cmath>

namespace objects
{
    TEST(ENERGYCOLLECTOR_CONSTRUCTOR, Test_All_Possible_Constructors)
    {
        EnergyCollector object1;

        ASSERT_EQ(object1.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object1.getOrigin(), core::Vec3(0, 0, 2));
        ASSERT_EQ(object1.getID(), 0);
        ASSERT_EQ(object1.getEnergy(), 0);

        EnergyCollector object2;

        ASSERT_EQ(object2.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object2.getOrigin(), core::Vec3(0, 0, 2));
        ASSERT_EQ(object2.getID(), 0);
        ASSERT_EQ(object2.getEnergy(), 0);

        EnergyCollector object3(core::Vec3(0, 0, -1));

        ASSERT_EQ(object3.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object3.getOrigin(), core::Vec3(0, 0, -1));
        ASSERT_EQ(object3.getID(), 0);
        ASSERT_EQ(object3.getEnergy(), 0);

        EnergyCollector object4(core::Vec3(0, 0, -1), 50);

        ASSERT_EQ(object4.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object4.getOrigin(), core::Vec3(0, 0, -1));
        ASSERT_EQ(object4.getID(), 0);
        ASSERT_EQ(object4.getEnergy(), 50);

        EnergyCollector object5(core::Vec3(0, 0, -1), 50, 80);

        ASSERT_EQ(object5.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object5.getOrigin(), core::Vec3(0, 0, -1));
        ASSERT_EQ(object5.getID(), 80);
        ASSERT_EQ(object5.getEnergy(), 50);

        EnergyCollector object6(object2);
        ASSERT_EQ(object6.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object6.getOrigin(), core::Vec3(0, 0, 2));
        ASSERT_EQ(object6.getID(), 0);
        ASSERT_EQ(object6.getEnergy(), 0);

        EnergyCollector object7;
        ASSERT_EQ(object7.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object7.getOrigin(), core::Vec3(0, 0, 2));
        ASSERT_EQ(object7.getID(), 0);
        ASSERT_EQ(object7.getEnergy(), 0);
    }

    TEST(ENERGYCOLLECTOR_OPERATORS, Test_Operator_ostream)
    {
        EnergyCollector object8;
        EnergyCollector object9;
        EnergyCollector object10;
        EnergyCollector object11;
        EnergyCollector object12;

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

        ASSERT_EQ(ss1.str(), "Energy Collector ID: 0, Energy collected: 0");
        ASSERT_EQ(ss2.str(), "Energy Collector ID: 0, Energy collected: 0");
        ASSERT_EQ(ss3.str(), "Energy Collector ID: 0, Energy collected: 0");
        ASSERT_EQ(ss4.str(), "Energy Collector ID: 0, Energy collected: 0");
        ASSERT_EQ(ss5.str(), "Energy Collector ID: 0, Energy collected: 0");
    }

    TEST(ENERGYCOLLECTOR_OPERATORS, Test_Operator_Equal_Equal)
    {
        EnergyCollector object1(core::Vec3(0, 0, 0), 0, 1);
        EnergyCollector object2(core::Vec3(0, 0, 0), 0, 1);
        ASSERT_EQ(object1, object2);

        EnergyCollector object3(core::Vec3(1, 2, 0), 20, 1);
        EnergyCollector object4(core::Vec3(1, 2, 0), 20, 1);
        ASSERT_EQ(object3, object4);

        EnergyCollector object5(core::Vec3(std::sqrt(2), 1, 2), 20, 1);
        EnergyCollector object6(core::Vec3(std::sqrt(2), 1, 2), 20, 1);
        ASSERT_EQ(object5, object6);

        // TODO: Make tests more complicated. From the fact that you used sqrt(2) does not mean
        // that you are testing against floating point issues. sqrt(2) will always return the same
        // value. Issue arises if you do some operations, and the order of operations matters.
        EnergyCollector object7(core::Vec3(std::sqrt(3), std::sqrt(2), 0), std::sqrt(2), 1);
        EnergyCollector object8(core::Vec3(std::sqrt(3), std::sqrt(2), 0), std::sqrt(2), 1);
        ASSERT_EQ(object7, object8);
    }

    TEST(ENERGYCOLLECTOR_OPERATORS, Test_Operator_equal)
    {
        EnergyCollector object1(core::Vec3(0, 0, 0), 0, 1);
        EnergyCollector object2 = object1;
        ASSERT_EQ(object1, object2);

        EnergyCollector object3(core::Vec3(1, 2, 0), 20, 1);
        EnergyCollector object4 = object3;
        ASSERT_EQ(object3, object4);

        EnergyCollector object5(core::Vec3(std::sqrt(2), 1, 2), 20, 1);
        EnergyCollector object6 = object5;
        ASSERT_EQ(object5, object6);

        EnergyCollector object7(core::Vec3(std::sqrt(3), 0, std::sqrt(2)), std::sqrt(2), 1);
        EnergyCollector object8 = object7;
        ASSERT_EQ(object7, object8);
    }

    TEST(ENERGYCOLLECTOR_OPERATORS, Test_Operator_plus)
    {
        EnergyCollector object13(core::Vec3(0, 0, 0), 50, 4);
        EnergyCollector object14(core::Vec3(0, 0, 0), 50, 5);

        EnergyCollector object15 = object13 + object14;

        ASSERT_EQ(object15.getEnergy(), object13.getEnergy() + object14.getEnergy());
        ASSERT_EQ(object15.getID(), 0);
        ASSERT_EQ(object15.getOrigin(), core::Vec3(0, 0, 0));
        ASSERT_EQ(object15.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
    }

    TEST(ENERGYCOLLECTOR_METHODS, Test_Method_DistanceAt)
    {
        EnergyCollector object1(core::Vec3(0, 0, 0), 0, 1);

        ASSERT_EQ(object1.distanceAt(core::Vec3(0, 4, 0)), 4);
        ASSERT_EQ(object1.distanceAt(core::Vec3(3, 0, 4)), 5);
    }

    TEST(ENERGYCOLLECTOR_METHODS, Test_Method_CollectEnergy)
    {
        // TODO: Energy distribution between many collectors
        // TODO: Phase impact on the given energy to the collector

        EnergyCollector object1(core::Vec3(0, 0, 0), 0, 1);
        core::RayHitData temp(core::RayHitData(50, core::Vec3(0, 1, 0).normalize(), core::Ray({0, 1, 0}, {1, 0, 0}, 50), 1000));

        object1.collectEnergy(temp);
        ASSERT_EQ(object1.getEnergy(), 50);
    }

} // namespace objects

#endif