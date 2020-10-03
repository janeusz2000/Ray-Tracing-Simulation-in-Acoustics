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

namespace objects
{

    TEST(TRIANGLEOBJ_OPERATORS, Test_Operator_iostream)
    {
        TriangleObj object1(core::Vec3(0, 0, 0), core::Vec3(0, 0, 1), core::Vec3(0, 1, 0));
        TriangleObj object2(core::Vec3(1, 0, 2), core::Vec3(1, 0, 1), core::Vec3(1, 1, 0));
        TriangleObj object3(core::Vec3(0, 2, 0), core::Vec3(8, 0, 1), core::Vec3(0, 1, 3));
        TriangleObj object4(core::Vec3(0, 0, 3), core::Vec3(0, 0, -5), core::Vec3(0, 1, 5));
        TriangleObj object5(core::Vec3(0, 0, 2), core::Vec3(0, 0, 3), core::Vec3(0, 4, 0));

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

        ASSERT_EQ(ss1.str(), "Triangle Object, vertex: Vec3(0, 0, 0), Vec3(0, 0, 1), Vec3(0, 1, 0)");
        ASSERT_EQ(ss2.str(), "Triangle Object, vertex: Vec3(1, 0, 2), Vec3(1, 0, 1), Vec3(1, 1, 0)");
        ASSERT_EQ(ss3.str(), "Triangle Object, vertex: Vec3(0, 2, 0), Vec3(8, 0, 1), Vec3(0, 1, 3)");
        ASSERT_EQ(ss4.str(), "Triangle Object, vertex: Vec3(0, 0, 3), Vec3(0, 0, -5), Vec3(0, 1, 5)");
        ASSERT_EQ(ss5.str(), "Triangle Object, vertex: Vec3(0, 0, 2), Vec3(0, 0, 3), Vec3(0, 4, 0)");
    }

    TEST(TRIANGLEOBJ_OPERATORS, Test_Operator_equal_Equal)
    {
        ASSERT_EQ(TriangleObj(core::Vec3(0, 0, 0), core::Vec3(0, 0, 1), core::Vec3(0, 1, 0)), TriangleObj(core::Vec3(0, 0, 0), core::Vec3(0, 0, 1), core::Vec3(0, 1, 0)));
        ASSERT_EQ(TriangleObj(core::Vec3(1, 1, 0), core::Vec3(1, 0, 1), core::Vec3(0, 1, 1)), TriangleObj(core::Vec3(1, 1, 0), core::Vec3(1, 0, 1), core::Vec3(0, 1, 1)));
        ASSERT_TRUE(TriangleObj(core::Vec3(0, 1, 0), core::Vec3(1, 0, 0), core::Vec3(0, 1, 1)) != TriangleObj(core::Vec3(0, 0, 2), core::Vec3(0, 2, 0), core::Vec3(2, 0, 0)));

        ASSERT_EQ(TriangleObj(core::Vec3(0, 0, 1), core::Vec3(0, 1, 0), core::Vec3(1, 0, 0)), TriangleObj(core::Vec3(1, 0, 0), core::Vec3(0, 1, 0), core::Vec3(0, 0, 1)));
    }

    TEST(TRIANGLEOBJ_CONSTRUCTORS, Test_All_Possible_Constructors)
    {
        TriangleObj object1({1, 0, 0}, {0, 1, 0}, {0, 0, 1});
        TriangleObj object2(core::Vec3(1, 0, 0), core::Vec3(0, 1, 0), core::Vec3(0, 0, 1));
        TriangleObj object3(object1);
        TriangleObj object4;

        ASSERT_EQ(object1, object2);
        ASSERT_EQ(object2, object3);
        ASSERT_EQ(object3, object1);
        ASSERT_EQ(object4, object1);
        ASSERT_EQ(object4, object2);
        ASSERT_EQ(object4, object3);
    }

    TEST(TRIANGLEOBJ_CONSTRUCTORS, Test_invalid_COnstructors)
    {
        ASSERT_THROW(TriangleObj({0, 0, 0}, {0, 0, 1}, {0, 0, 0, 1}), exception::invalidConstructor);
        ASSERT_THROW(TriangleObj({0, 0, 0, 0}, {0, 0, 1}, {0, 0, 1}), exception::invalidConstructor);
        ASSERT_THROW(TriangleObj({0, 0, 0}, {0, 0, 1}, {0, 0, 0}), exception::invalidConstructor);
        ASSERT_THROW(TriangleObj({0, 0, 0}, {0, 0, 0}, {0, 0, 1}), exception::invalidConstructor);
        ASSERT_THROW(TriangleObj({0, 0, 1}, {0, 0, 0}, {0, 0, 1}), exception::invalidConstructor);
        ASSERT_THROW(TriangleObj({0, 0, 0}, {0, 0, 1}, {0, 0, 1}), exception::invalidConstructor);
    }

    TEST(TRIANGLEOBJ_OPERATORS, Test_Operator_equal)
    {
        TriangleObj object1;
        TriangleObj object2 = object1;
        ASSERT_EQ(object1, object2);

        TriangleObj object3(core::Vec3(0, 0, 1), core::Vec3(0, 1, 0), core::Vec3(1, 0, 0));
        TriangleObj object4 = object3;
        ASSERT_EQ(object3, object4);

        TriangleObj object5 = TriangleObj({0, 1, 1}, {1, 0, 0}, {5, 0, 0});
        TriangleObj object6 = object5;
        ASSERT_EQ(object5, object6);
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_Normal)
    {
        // TODO: This should return stored Vec3 calculated in constructor and override method normal from object,
        // TODO: after setting new cooordinate, normal should look different
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_does_Hit)
    {
        // ! VERY IMPORTANT TO WORK
        // TODO: Monte Carlo test of if ray hits object properly
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_HitObject)
    {
        // TODO: THis method uses mainly method doesHit. This method is for making interface od the object easy to use
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_Area)
    {
        // TODO: Test Method Area - it should return are calculated in the constructor. IT SHOULD NOT CALCULATE ARE AON THE GO
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_Getters_And_Setters)
    {
        // TODO: Test method getX(), getY(), getZ(), setX(), setY(), setZ()
        //  ! AFTER Changing one of te x, y, or z area and normal should be updated!
    }

}; // namespace objects

#endif