#ifndef OBJECT_TEST_H
#define OBJECT_TEST_H

#include "gtest/gtest.h"
#include "constants.h"
#include "core/exceptions.h"
#include "obj/objects.h"

#include <cmath>
#include <sstream>
#include <random>

namespace objects
{

    TEST(SPHERE_OPERATORS, Test_Operator_ostream)
    {
        std::stringstream ss1;
        std::stringstream ss2;
        std::stringstream ss3;
        std::stringstream ss4;
        std::stringstream ss5;
        std::stringstream ss6;

        Sphere temp1(core::Vec3(0, 0, 0), 4);
        Sphere temp2(core::Vec3(1, 0, 1), 2);
        Sphere temp3(core::Vec3(6, 5, 4), 3);
        Sphere temp4(core::Vec3(7, 8, 9), 10);
        Sphere temp5(core::Vec3(14, 13, 12), 11);
        Sphere temp6(core::Vec3(15, 16, 17), 18);

        ss1 << temp1;
        ss2 << temp2;
        ss3 << temp3;
        ss4 << temp4;
        ss5 << temp5;
        ss6 << temp6;

        ASSERT_EQ(ss1.str(), "Sphere origin: Vec3(0, 0, 0), radius: 4 [m]");
        ASSERT_EQ(ss2.str(), "Sphere origin: Vec3(1, 0, 1), radius: 2 [m]");
        ASSERT_EQ(ss3.str(), "Sphere origin: Vec3(6, 5, 4), radius: 3 [m]");
        ASSERT_EQ(ss4.str(), "Sphere origin: Vec3(7, 8, 9), radius: 10 [m]");
        ASSERT_EQ(ss5.str(), "Sphere origin: Vec3(14, 13, 12), radius: 11 [m]");
        ASSERT_EQ(ss6.str(), "Sphere origin: Vec3(15, 16, 17), radius: 18 [m]");
    }

    TEST(SPHERE_METHOD, Test_Normal)
    {
        Sphere tempSphere(core::Vec3(0, 0, 0), 1);

        core::Vec3 normal1 = tempSphere.normal(core::Vec3(0, 0, -1));
        core::Vec3 normal2 = tempSphere.normal(core::Vec3(0, 0, 1));
        core::Vec3 normal3 = tempSphere.normal(core::Vec3(0, 1, 0));
        core::Vec3 normal4 = tempSphere.normal(core::Vec3(0, -1, 0));

        ASSERT_EQ(normal1, core::Vec3(0, 0, -1));
        ASSERT_EQ(normal2, core::Vec3(0, 0, 1));
        ASSERT_EQ(normal3, core::Vec3(0, 1, 0));
        ASSERT_EQ(normal4, core::Vec3(0, -1, 0));
    }

    TEST(SPHERE_METHOD, Test_Getters_and_Setters)
    {
        Sphere object1(core::Vec3(0, 0, 0), 0.5);
        Sphere object2(core::Vec3(1, 2, 1), 5);
        Sphere object3(core::Vec3(1, 3, 2), 123);
        Sphere object4(core::Vec3(6, -1, 2), 8);
        Sphere object5(core::Vec3(1, 43, 3), 4);
        Sphere object6(core::Vec3(12, -12, 1), 0.3);

        ASSERT_EQ(core::Vec3(0, 0, 0), object1.getOrigin());
        ASSERT_EQ(core::Vec3(1, 2, 1), object2.getOrigin());
        ASSERT_EQ(core::Vec3(1, 3, 2), object3.getOrigin());
        ASSERT_EQ(core::Vec3(6, -1, 2), object4.getOrigin());
        ASSERT_EQ(core::Vec3(1, 43, 3), object5.getOrigin());
        ASSERT_EQ(core::Vec3(12, -12, 1), object6.getOrigin());

        ASSERT_EQ(0.5, object1.getRadius());
        ASSERT_EQ(5, object2.getRadius());
        ASSERT_EQ(123, object3.getRadius());
        ASSERT_EQ(8, object4.getRadius());
        ASSERT_EQ(4, object5.getRadius());
        ASSERT_EQ(0.3, object6.getRadius());
    }

    TEST(SPHERE_METHOD, Test_HitObject) // MonteCarlo test: https://en.wikipedia.org/wiki/Monte_Carlo_method
    {
        std::random_device rd;
        std::mt19937_64 e2(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double radius1 = 0.2;
        Sphere object1(core::Vec3(1, 1, 20), radius1);

        double radius2 = 0.3;
        Sphere object2(core::Vec3(1, 1, 20), radius2);

        double radius3 = 0.4;
        Sphere object3(core::Vec3(1, 1, 20), radius3);

        double rayNum = 10000000;

        double hits1 = 0;
        double hits2 = 0;
        double hits3 = 0;

        const double reference1 = constants::kPi * radius1 * radius1 / 4;
        const double reference2 = constants::kPi * radius2 * radius2 / 4;
        const double reference3 = constants::kPi * radius3 * radius3 / 4;
        const double tempFreq = 1000;

        for (size_t n = 0; n < rayNum; n++)
        {
            core::Vec3 rayPosition(dist(e2), dist(e2), -50);
            core::Ray tempRay(rayPosition, core::Vec3(0, 0, 1));
            if (object1.hitObject(tempRay, tempFreq)) // if std::unique_ptr is not nullptr
            {
                hits1++;
            }
            if (object2.hitObject(tempRay, tempFreq)) // if std::unique_ptr is not nullptr
            {
                hits2++;
            }
            if (object3.hitObject(tempRay, tempFreq)) // if std::unique_ptr is not nullptr
            {
                hits3++;
            }
        }

        ASSERT_NEAR(reference1, hits1 / rayNum, 0.001);
        ASSERT_NEAR(reference2, hits2 / rayNum, 0.001);
        ASSERT_NEAR(reference3, hits3 / rayNum, 0.001);
    }

    TEST(SPHEREWALL_CONSTRUCTOR, Test_All_Possible_Constructors)
    {
        //TODO: Test all possible constructors on Sphere Wall
    }

    TEST(SPHEREWALL_METHOD, Test_HItObject)
    {
        //TODO: Test Hit on Sphere wall with monte Carlo method
    }

    TEST(SPHEREWALL_METHOD, Test_Normal)
    {
        //TODO: TEst normal method on Sphere wall Object
    }

    TEST(SPHEREWALL_METHOD, Test_Getters_and_Setters)
    {
        //TODO: Test all getters and setters in spherewall
    }
} // namespace objects
#endif