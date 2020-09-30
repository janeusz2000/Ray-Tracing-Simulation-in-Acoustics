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
} // namespace objects

#endif