#ifndef OBJECT_TEST_H
#define OBJECT_TEST_H

#include "gtest/gtest.h"
#include "constants.h"
#include "core/exceptions.h"
#include "obj/objects.h"

#include <cmath>
#include <sstream>

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

} // namespace objects
#endif