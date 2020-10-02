#ifndef TRIANGLEOBJ_TEST_CPP
#define TRIANGLEOBJ_TEST_CPP

#include "gtest/gtest.h"
#include "obj/objects.h"
#include "core/vec3.h"
#include "core/ray.h"
#include "core/exceptions.h"
#include "constants.h"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace object
{

    TEST(TRIANGLEOBJ_OPERATORS, Test_Operator_iostream)
    {
        TriangleObj object1(core::Vec3(0, 0, 0), core::Vec3(0, 0, 1), core::Vec3(0, 1, 0));
        TriangleObj object1(core::Vec3(1, 0, 2), core::Vec3(1, 0, 1), core::Vec3(1, 1, 0));
        TriangleObj object1(core::Vec3(0, 2, 0), core::Vec3(8, 0, 1), core::Vec3(0, 1, 3));
        TriangleObj object1(core::Vec3(0, 0, 3), core::Vec3(0, 0, -5), core::Vec3(0, 1, 5));
        TriangleObj object1(core::Vec3(0, 0, 2), core::Vec3(0, 0, 3), core::Vec3(0, 4, 0));

        std::stringstream ss1;
        std::stringstream ss2;
        std::stringstream ss3;
        std::stringstream ss4;
        std::stringstream ss5;

        ss1 << object1;
        ss2 << object2;
        ss3 << object3;
        ss4 << object4;
        ss5 << object5;

        ASSERT_EQ(ss1.str(), "Triangle object, vertex: Vec3(0, 0, 0), Vec3(0, 0, 1), Vec3(0, 1, 0)");
        ASSERT_EQ(ss2.str(), "Triangle object, vertex: Vec3(1, 0, 2), Vec3(1, 0, 1), Vec3(1, 1, 0)");
        ASSERT_EQ(ss3.str(), "Triangle object, vertex: Vec3(0, 2, 0), Vec3(8, 0, 1), Vec3(0, 1, 3)");
        ASSERT_EQ(ss4.str(), "Triangle object, vertex: Vec3(0, 0, 3), Vec3(0, 0, -5), Vec3(0, 1, 5)");
        ASSERT_EQ(ss5.str(), "Triangle object, vertex: Vec3(0, 0, 2), Vec3(0, 0, 3), Vec3(0, 4, 0)");
    }

    TEST(TRIANGLEOBJ_CONSTRUCTORS, Test_All_Possible_Constructors)
    {
        // TODO: Test all possible constructors of the triangle obj
    }

    TEST(TRIANGLEOBJ_CONSTRUCTORS, Test_invalid_COnstructors)
    {
        // TODO: constructor should throw exception::invalidConstructor() when one of the Vec3 is the same as other ones
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_Normal)
    {
        // TODO: This should return stored Vec3 calculated in constructor and override method normal from object,
    }

    TEST(TRANGLEOBJ_METHOD, Test_Method_hitObject)
    {
        // TODO: Monte Carlo test of if ray hits object
    }

}; // namespace object

#ifndef