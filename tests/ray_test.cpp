#ifndef RAY_TEST_H
#define RAY_TEST_H

#include "gtest/gtest.h"
#include "core/exceptions.h"
#include "core/vec3.h"
#include "core/ray.h"

#include <sstream>
#include <cmath>

namespace core
{
    TEST(RAY_OPERATOR, Test_operator_ostream)
    {
        std::stringstream s1;
        std::stringstream s2;
        std::stringstream s3;
        std::stringstream s4;
        std::stringstream s5;

        Ray temp1(Vec3(0, 0, 0), Vec3(0, 0, 1), 0);
        Ray temp2(Vec3(0, 1, 0), Vec3(0, 0, 1), 0);
        Ray temp3(Vec3(1, 1, 1), Vec3(1, 0, 1), 0);
        Ray temp4(Vec3(1, 2, 3), Vec3(1, 2, 3), 0);
        Ray temp5(Vec3(std::sqrt(2), 1, 1), Vec3(std::sqrt(2), 0, 0), 0);

        s1 << temp1;
        s2 << temp2;
        s3 << temp3;
        s4 << temp4;
        s5 << temp5;

        ASSERT_EQ(s1.str(), "RAY origin: Vec3(0, 0, 0), direction: Vec3(0, 0, 1), energy: 0");
        ASSERT_EQ(s2.str(), "RAY origin: Vec3(0, 1, 0), direction: Vec3(0, 0, 1), energy: 0");
        ASSERT_EQ(s3.str(), "RAY origin: Vec3(1, 1, 1), direction: Vec3(0.707107, 0, 0.707107), energy: 0");
        ASSERT_EQ(s4.str(), "RAY origin: Vec3(1, 2, 3), direction: Vec3(0.267261, 0.534522, 0.801784), energy: 0");
        ASSERT_EQ(s5.str(), "RAY origin: Vec3(1.41421, 1, 1), direction: Vec3(1, 0, 0), energy: 0");
    }

    TEST(RAY_OPERATOR, Test_Operator_Equal_Equal)
    {
        ASSERT_EQ(Ray(Vec3(0, 0, 0), Vec3(1, 0, -1)), Ray(Vec3(0, 0, 0), Vec3(1, 0, -1)));
        ASSERT_EQ(Ray(Vec3(0, 0, 1), Vec3(1, 0, 1)), Ray(Vec3(0, 0, 1), Vec3(1, 0, 1)));
    }

    TEST(RAY_METHOD, test_Method_PhaseAt)
    {
        double freq1 = 1000;
        double freq2 = 300;
        double freq3 = 500;

        Ray temp1(Vec3(0, 0, 0), Vec3(0, 0, 1));
        Ray temp2(Vec3(0, 0, 0), Vec3(0, 1, 0));
        Ray temp3(Vec3(0, 0, 0), Vec3(1, 0, 0));

        ASSERT_NEAR(2 * kPi, temp1.phaseAt(freq1, 0.343216), kAccuracy);
        ASSERT_NEAR(2 * kPi, temp2.phaseAt(freq2, 1.144053333), kAccuracy);
        ASSERT_NEAR(2 * kPi, temp3.phaseAt(freq3, 0.686432), kAccuracy);
    }

    TEST(RAY_METHOD, Test_Method_At)
    {
        Ray temp1(Vec3(0, 0, 0), Vec3(0, 0, 1), 0);
        Ray temp2(Vec3(0, 1, 0), Vec3(0, 0, 1), 0);
        Ray temp3(Vec3(1, 1, 1), Vec3(1, 0, 1), 0);

        ASSERT_EQ(Vec3(0, 0, 8), temp1.at(8));
        ASSERT_EQ(Vec3(0, 1, -10), temp2.at(-10));
    }

    TEST(RAY_CONSTRUCTOR, Test_constructor_invalid)
    {
        ASSERT_THROW(Ray({0, 0, 0}, {0, 0, 0}), exception::invalidConstructor);
        ASSERT_THROW(Ray({0, 0, 0}, {0, 0, kAccuracy * 0.999}), exception::invalidConstructor);
    }
    TEST(RAY_CONSTRUCTOR, Test_All_Possible_Constructors)
    {

        Ray temp1;
        Ray temp2(Vec3(0, 0, 0), Vec3(0, 0, 1), 0);
        Ray temp3(Vec3(0, 0, 0), Vec3(0, 0, 1));
        Ray temp4({0, 0, 0}, {0, 0, 1});
        Ray temp5({0, 0, 0}, {0, 0, 1}, 0);
        Ray temp6(temp1);

        ASSERT_EQ(temp1, Ray(Vec3(0, 0, 0), Vec3(0, 0, 1)));
        ASSERT_EQ(temp1, temp2);
        ASSERT_EQ(temp2, temp3);
        ASSERT_EQ(temp4, temp3);
        ASSERT_EQ(temp4, temp5);
        ASSERT_EQ(temp1, temp4);
        ASSERT_EQ(temp1, temp6);
        ASSERT_EQ(temp3, temp6);
    }

    TEST(RAY_METHOD, Test_Method_Setters_Getters)
    {
        Ray temp1;
        Ray temp2(Vec3(0, 0, 0), Vec3(0, 0, 1), 0);
        Ray temp3(Vec3(0, 0, 0), Vec3(0, 0, 1));
        Ray temp4({0, 0, 0}, {0, 0, 1});
        Ray temp5({0, 0, 0}, {0, 0, 1}, 0);
        Ray temp6(temp1);

        ASSERT_EQ(temp1.getDirection(), temp2.getDirection());
        ASSERT_EQ(temp1.getOrigin(), temp2.getOrigin());
        ASSERT_EQ(temp1.getEnergy(), temp4.getEnergy());
        ASSERT_EQ(temp3.getDirection(), temp5.getDirection());
    }

} // namespace core
#endif