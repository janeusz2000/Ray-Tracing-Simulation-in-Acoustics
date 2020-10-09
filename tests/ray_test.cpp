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

    TEST(RAY_CONSTRUCTOR, Test_constructor_invalid)
    {
        ASSERT_THROW(Ray({0, 0, 0}, {0, 0, 0}), std::invalid_argument);
        ASSERT_THROW(Ray({0, 0, 0}, {0, 0, constants::kAccuracy * 0.999}), std::invalid_argument);
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

        ASSERT_NEAR(2 * constants::kPi, temp1.phaseAt(freq1, 0.343216), constants::kAccuracy);
        ASSERT_NEAR(2 * constants::kPi, temp2.phaseAt(freq2, 1.144053333), constants::kAccuracy);
        ASSERT_NEAR(2 * constants::kPi, temp3.phaseAt(freq3, 0.686432), constants::kAccuracy);
    }

    TEST(RAY_METHOD, Test_PhaseAt_catch_exception)
    {
        ASSERT_THROW(Ray({0, 0, 1}, {1, 0, 0}).phaseAt(0, 1), std::invalid_argument);
        ASSERT_THROW(Ray({0, 0, 1}, {1, 0, 0}).phaseAt(1, 0), std::invalid_argument);
        ASSERT_THROW(Ray({0, 0, 1}, {1, 0, 0}).phaseAt(constants::kAccuracy, 1), std::invalid_argument);
        ASSERT_THROW(Ray({0, 0, 1}, {1, 0, 0}).phaseAt(1, constants::kAccuracy), std::invalid_argument);
        ASSERT_THROW(Ray({0, 0, 1}, {1, 0, 0}).phaseAt(constants::kAccuracy, constants::kAccuracy), std::invalid_argument);
    }

    TEST(RAY_METHOD, Test_Method_At)
    {
        Ray temp1(Vec3(0, 0, 0), Vec3(0, 0, 1), 0);
        Ray temp2(Vec3(0, 1, 0), Vec3(0, 0, 1), 0);
        Ray temp3(Vec3(1, 1, 1), Vec3(1, 0, 1), 0);

        ASSERT_EQ(Vec3(0, 0, 8), temp1.at(8));
        ASSERT_EQ(Vec3(0, 1, -10), temp2.at(-10));
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

    TEST(RAYHITDATA_OPERATORS, Test_Operator_ostream)
    {
        // std::stringstream ss1;
        // std::stringstream ss2;
        // std::stringstream ss3;

        // RayHitData temp1(50, Vec3(0, 1, 2), Vec3(0, 0, 1).normalize(), Vec3(1, 0, 0).normalize(), 50, 20);
        // RayHitData temp2(3, Vec3(4, 0, 1), Vec3(1, 0, 0).normalize(), Vec3(0, 1, 0).normalize(), 20, 10);
        // RayHitData temp3(-3, Vec3(0, 1, 2), Vec3(0, 1, 0).normalize(), Vec3(0, 0, 1).normalize(), 3, 5);

        // ss1 << temp1;
        // ss2 << temp2;
        // ss3 << temp3;

        // ASSERT_EQ(ss1.str(), "Collision point: Vec3(0, 1, 2), incoming ray direction: Vec3(1, 0, 0), normal: Vec3(0, 0, 1), time: 50, energy: 50, phase 20 [radians]");
        // ASSERT_EQ(ss2.str(), "Collision point: Vec3(4, 0, 1), incoming ray direction: Vec3(0, 1, 0), normal: Vec3(1, 0, 0), time: 3, energy: 20, phase 10 [radians]");
        // ASSERT_EQ(ss3.str(), "Collision point: Vec3(0, 1, 2), incoming ray direction: Vec3(0, 0, 1), normal: Vec3(0, 1, 0), time: -3, energy: 3, phase 5 [radians]");
    }

    TEST(RAYHITDATA_OPERATOR, Test_Operator_Equal_Equal)
    {
        RayHitData temp1(3, Vec3(1, 4, 0).normalize(), Ray({0, 0, 1}, {2, 9, 1}), 1000);
        RayHitData temp2(8, Vec3(1, 0, 2).normalize(), Ray({5, 0, 1}, {2, 3, 1}), 20);
        RayHitData temp3(15, Vec3(1, 0, 0).normalize(), Ray({0, 2, 1}, {13, 3, 1}), 60);

        RayHitData temp4(3, Vec3(1, 4, 0).normalize(), Ray({0, 0, 1}, {2, 9, 1}), 1000);
        RayHitData temp5(8, Vec3(1, 0, 2).normalize(), Ray({5, 0, 1}, {2, 3, 1}), 20);
        RayHitData temp6(15, Vec3(1, 0, 0).normalize(), Ray({0, 2, 1}, {13, 3, 1}), 60);

        ASSERT_EQ(temp1, temp4);
        ASSERT_EQ(temp2, temp5);
        ASSERT_EQ(temp6, temp3);
    }

    TEST(RAYHITDATA_OPERATORS, Test_Operator_equal)
    {
        RayHitData temp1(3, Vec3(1, 2, 0).normalize(), Ray({2, 0, 1}, {1, 3, 1}), 1000);
        RayHitData temp2(40, Vec3(1, 0, 8).normalize(), Ray({0, 4, 1}, {2, 8, 1}), 50);
        RayHitData temp3(4, Vec3(3, 0, 0).normalize(), Ray({5, 0, 1}, {2, 3, 2}), 20);

        RayHitData temp4 = temp1;
        RayHitData temp5 = temp2;
        RayHitData temp6 = temp3;

        ASSERT_EQ(temp1, temp4);
        ASSERT_EQ(temp2, temp5);
        ASSERT_EQ(temp6, temp3);
    }

} // namespace core
#endif