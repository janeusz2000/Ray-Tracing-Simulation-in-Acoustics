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

    TEST(RAY_METHOD, test_Method_PhaseAt)
    {
        //TODO: This test needs to check at which phase is wave within direction point
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
        //TODO: TEST For getters
    }

    TEST(RAY_CONSTRUCTOR, Test_All_Possible_Constructors)
    {
        //TODO: TEST ALL POSSIBLE CONSTRUCTORS
    }

} // namespace core
#endif