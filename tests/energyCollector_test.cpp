#ifndef ENERGYCOLLECTOR_TEST_CPP
#define ENERGYCOLLECTOR_TEST_CPP

#include "gtest/gtest.h"
#include "obj/objects.h"
#include "core/exceptions.h"
#include "core/vec3.h"
#include "core/ray.h"
#include "constants.h"

#include <cmath>
#include <algorithm>

namespace objects
{
    TEST(ENERGYCOLLECTOR_CONSTRUCTOR, Test_All_Possible_Constructors)
    {
        EnergyCollector object1;

        ASSERT_EQ(object1.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object1.getOrigin(), core::Vec3(0, 4, 0));
        ASSERT_EQ(object1.getID(), 0);
        ASSERT_EQ(object1.getEnergy(), 0);

        EnergyCollector object2;

        ASSERT_EQ(object2.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object2.getOrigin(), core::Vec3(0, 4, 0));
        ASSERT_EQ(object2.getID(), 1);
        ASSERT_EQ(object2.getEnergy(), 0);

        EnergyCollector object3(core::Vec3(0, -1, 0));

        ASSERT_EQ(object3.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object3.getOrigin(), core::Vec3(0, -1, 0));
        ASSERT_EQ(object3.getID(), 2);
        ASSERT_EQ(object3.getEnergy(), 0);

        EnergyCollector object4(core::Vec3(0, -1, 0), 50);

        ASSERT_EQ(object4.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object4.getOrigin(), core::Vec3(0, -1, 0));
        ASSERT_EQ(object4.getID(), 3);
        ASSERT_EQ(object4.getEnergy(), 50);

        EnergyCollector object5(core::Vec3(0, -1, 0), 50, 80);

        ASSERT_EQ(object5.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object5.getOrigin(), core::Vec3(0, -1, 0));
        ASSERT_EQ(object5.getID(), 80);
        ASSERT_EQ(object5.getEnergy(), 50);

        EnergyCollector object6(object2);
        ASSERT_EQ(object6.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object6.getOrigin(), core::Vec3(0, 4, 0));
        ASSERT_EQ(object6.getID(), 1);
        ASSERT_EQ(object6.getEnergy(), 0);

        EnergyCollector object7;
        ASSERT_EQ(object7.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
        ASSERT_EQ(object7.getOrigin(), core::Vec3(0, 4, 0));
        ASSERT_EQ(object7.getID(), 4);
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

        ASSERT_EQ(ss1.str(), "Energy Collector ID: 5, Energy collected: 0");
        ASSERT_EQ(ss2.str(), "Energy Collector ID: 6, Energy collected: 0");
        ASSERT_EQ(ss3.str(), "Energy Collector ID: 7, Energy collected: 0");
        ASSERT_EQ(ss4.str(), "Energy Collector ID: 8, Energy collected: 0");
        ASSERT_EQ(ss5.str(), "Energy Collector ID: 9, Energy collected: 0");
    }

    TEST(ENERGYCOLLECTOR_OPERATORS, Test_Operator_equal)
    {
        //TODO:Test Operator=
    }

    TEST(ENERGYCOLLECTOR_OPERATORS, Test_Operator_plus)
    {
        EnergyCollector object13(core::Vec3(0, 0, 0), 50, 4);
        EnergyCollector object14(core::Vec3(0, 0, 0), 50, 5);

        EnergyCollector object15 = object13 + object14;

        ASSERT_EQ(object15.getEnergy(), object13.getEnergy() + object14.getEnergy());
        ASSERT_EQ(object15.getID(), 10);
        ASSERT_EQ(object15.getOrigin(), core::Vec3(0, 0, 0));
        ASSERT_EQ(object15.getRadius(), constants::kPi * constants::kSimulationRadius / constants::kPopulation);
    }

    TEST(ENERGYCOLLECTOR_OPERATORS, Test_Operator_Equal_Equal)
    {
        // TODO: Test Operator==
    }

    TEST(ENERGYCOLLECTOR_METHODS, Test_Method_DistanceAt)
    {
        // TODO: Test Method DistanceAt
    }

    TEST(ENERGYCOLLECTOR_METHODS, Test_Method_CollectEnergy)
    {
        // TODO: Test MEthod CollectEnergy
    }

    TEST(ENERGYCOLLECTOR_METHODS, Test_Method_Getters_and_Setters)
    {
        // TODO: Test MEthod CollectEnergy
    }

} // namespace objects

#endif