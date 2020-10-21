#include "constants.h"
#include "core/exceptions.h"
#include "core/ray.h"
#include "core/vec3.h"
#include "obj/objects.h"
#include "gtest/gtest.h" // https://google.github.io/styleguide/cppguide.html#Namespaces

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

using core::Ray;
using core::RayHitData;
using core::Vec3;

// TODO: Change all doubles to float

namespace objects
{

    const double kSkipFreq = 1000;

    TEST(TRIANGLEOBJ_OPERATORS, Operator_iostream)
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

    TEST(TRIANGLEOBJ_OPERATORS, Operator_equal_Equal)
    {
        ASSERT_EQ(TriangleObj(core::Vec3(0, 0, 0), core::Vec3(0, 0, 1), core::Vec3(0, 1, 0)), TriangleObj(core::Vec3(0, 0, 0), core::Vec3(0, 0, 1), core::Vec3(0, 1, 0)));
        ASSERT_EQ(TriangleObj(core::Vec3(1, 1, 0), core::Vec3(1, 0, 1), core::Vec3(0, 1, 1)), TriangleObj(core::Vec3(1, 1, 0), core::Vec3(1, 0, 1), core::Vec3(0, 1, 1)));
        ASSERT_TRUE(TriangleObj(core::Vec3(0, 1, 0), core::Vec3(1, 0, 0), core::Vec3(0, 1, 1)) != TriangleObj(core::Vec3(0, 0, 2), core::Vec3(0, 2, 0), core::Vec3(2, 0, 0)));

        ASSERT_EQ(TriangleObj(core::Vec3(0, 0, 1), core::Vec3(0, 1, 0), core::Vec3(1, 0, 0)), TriangleObj(core::Vec3(1, 0, 0), core::Vec3(0, 1, 0), core::Vec3(0, 0, 1)));
    }

    TEST(TRIANGLEOBJ_CONSTRUCTORS, All_Possible_Constructors)
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

    TEST(TRIANGLEOBJ_CONSTRUCTORS, invalid_COnstructors)
    {
        ASSERT_THROW(TriangleObj({0, 0, 0}, {0, 0, 1}, {0, 0, 0, 1}), std::invalid_argument);
        ASSERT_THROW(TriangleObj({0, 0, 0, 0}, {0, 0, 1}, {0, 0, 1}), std::invalid_argument);
        ASSERT_THROW(TriangleObj({0, 0, 0}, {0, 0, 1}, {0, 0, 0}), std::invalid_argument);
        ASSERT_THROW(TriangleObj({0, 0, 0}, {0, 0, 0}, {0, 0, 1}), std::invalid_argument);
        ASSERT_THROW(TriangleObj({0, 0, 1}, {0, 0, 0}, {0, 0, 1}), std::invalid_argument);
        ASSERT_THROW(TriangleObj({0, 0, 0}, {0, 0, 1}, {0, 0, 1}), std::invalid_argument);
        ASSERT_THROW(TriangleObj({1, 1, 1}, {2, 2, 2}, {3, 3, 3}), std::invalid_argument);
        ASSERT_THROW(TriangleObj({1, 1, 1}, {2, 2, 2}, {4, 4, 4}), std::invalid_argument);
        ASSERT_THROW(TriangleObj({3, 3, 3}, {2, 2, 2}, {4, 4, 4}), std::invalid_argument);
    }

    TEST(TRIANGLEOBJ_OPERATORS, Operator_equal)
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

    TEST(TRIANGLEOBJ_METHOD, Normal)
    {
        TriangleObj object1({0, 0, 0}, {0, 1, 0}, {1, 0, 0});
        TriangleObj object2({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
        ASSERT_EQ(object1.normal(core::Vec3()), core::Vec3(0, 0, -1));
        ASSERT_EQ(object2.normal(core::Vec3()), core::Vec3(-1, 0, 0));
    }

    TEST(TRIANGLEOBJ_METHOD, does_Hit_Vec3_0_0_1)
    {
        TriangleObj object({0, 0, 0}, {1, 0, 0}, {1, 1, 0});

        std::random_device rd;
        std::mt19937_64 engine(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double hits = 0;
        const double referenceRatio = 0.5 / 4;
        const double rayNum = 1000000;

        for (auto num = 0; num < rayNum; ++num)
        {
            core::Ray tempRay({dist(engine), dist(engine), -20}, {0, 0, 1});
            core::RayHitData hitData;
            if (object.hitObject(tempRay, kSkipFreq, &hitData))
            {
                ++hits;
            }
        }
        ASSERT_NEAR(referenceRatio, hits / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, does_Hit_Vec3_0_0_minus1)
    {
        TriangleObj object({0, 0, 0}, {1, 0, 0}, {1, 1, 0});

        std::random_device rd;
        std::mt19937_64 engine(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double hits = 0;
        const double referenceRatio = 0.5 / 4;
        const double rayNum = 1000000;

        for (auto num = 0; num < rayNum; ++num)
        {
            core::Ray tempRay({dist(engine), dist(engine), 20}, {0, 0, -1});
            core::RayHitData hitData;
            if (object.hitObject(tempRay, kSkipFreq, &hitData))
            {
                ++hits;
            }
        }
        ASSERT_NEAR(referenceRatio, hits / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, does_Hit_Vec3_0_1_0)
    {
        TriangleObj object({0, 0, 0}, {1, 0, 0}, {1, 0, 1});

        std::random_device rd;
        std::mt19937_64 engine(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double hits = 0;
        const double referenceRatio = 0.5 / 4;
        const double rayNum = 1000000;

        for (auto num = 0; num < rayNum; ++num)
        {
            core::Ray tempRay({dist(engine), -20, dist(engine)}, {0, 1, 0});
            core::RayHitData hitData;
            if (object.hitObject(tempRay, kSkipFreq, &hitData))
            {
                ++hits;
            }
        }
        ASSERT_NEAR(referenceRatio, hits / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, does_Hit_Vec3_0_minus1_0)
    {
        TriangleObj object({0, 0, 0}, {1, 0, 0}, {1, 0, 1});

        std::random_device rd;
        std::mt19937_64 engine(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double hits = 0;
        const double referenceRatio = 0.5 / 4;
        const double rayNum = 1000000;

        for (auto num = 0; num < rayNum; ++num)
        {
            core::Ray tempRay({dist(engine), 20, dist(engine)}, {0, -1, 0});
            core::RayHitData hitData;
            if (object.hitObject(tempRay, kSkipFreq, &hitData))
            {
                ++hits;
            }
        }
        ASSERT_NEAR(referenceRatio, hits / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, does_Hit_Vec3_1_0_0)
    {
        TriangleObj object({0, 0, 0}, {0, 1, 0}, {0, 0, 1});

        std::random_device rd;
        std::mt19937_64 engine(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double hits = 0;
        const double referenceRatio = 0.5 / 4;
        const double rayNum = 1000000;

        for (auto num = 0; num < rayNum; ++num)
        {
            core::Ray tempRay({20, dist(engine), dist(engine)}, {-1, 0, 0});
            core::RayHitData hitData;
            if (object.hitObject(tempRay, kSkipFreq, &hitData))
            {
                ++hits;
            }
        }
        ASSERT_NEAR(referenceRatio, hits / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, does_Hit_Vec3_minus1_0_0)
    {
        TriangleObj object({0, 0, 0}, {0, 1, 0}, {0, 0, 1});

        std::random_device rd;
        std::mt19937_64 engine(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double hits = 0;
        const double referenceRatio = 0.5 / 4;
        const double rayNum = 1000000;

        for (auto num = 0; num < rayNum; ++num)
        {
            core::Ray tempRay({-20, dist(engine), dist(engine)}, {1, 0, 0});
            core::RayHitData hitData;
            if (object.hitObject(tempRay, kSkipFreq, &hitData))
            {
                ++hits;
            }
        }
        ASSERT_NEAR(referenceRatio, hits / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, Area)
    {
        ASSERT_EQ(TriangleObj({0, 0, 0}, {0, 0, 1}, {0, 1, 1}).area(), 0.5);
        ASSERT_EQ(TriangleObj({0, 0, 0}, {0, 0, 2}, {0, 2, 2}).area(), 2);
        ASSERT_EQ(TriangleObj({0, 0, 0}, {5, 0, 0}, {5, 5, 0}).area(), 12.5);
    }

    TEST(TRIANGLEOBJ_METHOD, RayInsideTriangleDoesNotHit)
    {
        std::vector<TriangleObj> objectList = {TriangleObj(Vec3(0, 0, 1), Vec3(0, 0, 0), Vec3(1, 0, 0)), TriangleObj(Vec3(0, 4, 1), Vec3(0, 4, 0), Vec3(1, 4, 0))};

        size_t iteration = 0;
        RayHitData hitData;
        Ray tempRay(Vec3(0.25, 0, 0.25), Vec3(0, 1, 0)); // Ray has origin inside object1
        std::vector<bool> expectedResults = {false, true};

        for (TriangleObj &obj : objectList)
        {
            ASSERT_TRUE((obj.hitObject(tempRay, kSkipFreq, &hitData) == expectedResults[iteration])); // First object doesn't hit. Second object does hit.
            ++iteration;
        }

        ASSERT_EQ(hitData.collisionPoint, Vec3(0.25, 4, 0.25)); // Making sure that second object is hit properly.
        ASSERT_EQ(hitData.direction, tempRay.getDirection());
        ASSERT_EQ(hitData.origin, tempRay.getOrigin());
        ASSERT_EQ(hitData.time, 4);
    }

    TEST(TRIANGLEOBJ_METHOD, Hit_Object_Ray_Back_Object_Vec_0_1_0)
    {
        using namespace core;

        RayHitData hitData;
        Ray tempRay(Vec3(0.25, 5, 0.25), Vec3(0, 1, 0));

        std::vector<TriangleObj> objectList = {TriangleObj(Vec3(0, 0, 1), Vec3(0, 0, 0), Vec3(1, 0, 0)), TriangleObj(Vec3(0, 4, 1), Vec3(0, 4, 0), Vec3(1, 4, 0))};

        for (auto &obj : objectList)
        {
            ASSERT_TRUE(!obj.hitObject(tempRay, kSkipFreq, &hitData));
        }
    }

    TEST(TRIANGLEOBJ_METHOD, Hit_Object_Ray_front_Object_Vec_0_1_0)
    {
        using namespace core;

        RayHitData hitData;
        Ray tempRay(Vec3(0.25, -4, 0.25), Vec3(0, 1, 0)); // Ray is in front of object2 with direction outside of object2;

        std::vector<TriangleObj> objectList = {TriangleObj(Vec3(0, 0, 1), Vec3(0, 0, 0), Vec3(1, 0, 0)), TriangleObj(Vec3(0, 4, 1), Vec3(0, 4, 0), Vec3(1, 4, 0))};

        for (auto &obj : objectList)
        {
            ASSERT_TRUE(obj.hitObject(tempRay, kSkipFreq, &hitData));
            ASSERT_EQ(hitData.collisionPoint, Vec3(0.25, obj.point1().y(), 0.25));
            ASSERT_EQ(hitData.time, std::abs(tempRay.getOrigin().y()) + obj.point1().y());
        }
    }

    TEST(TRIANGLEOBJ_METHOD, Hit_Object_Ray_Inside_Object_Vec_0_0_1)
    {

        RayHitData hitData;
        Ray tempRay(Vec3(0.25, 0.25, 0), Vec3(0, 0, 1)); // Ray has origin inside object1

        std::vector<TriangleObj> objectList = {TriangleObj(Vec3(1, 0, 0), Vec3(0, 0, 0), Vec3(0, 1, 0)), TriangleObj(Vec3(1, 0, 4), Vec3(0, 0, 4), Vec3(0, 1, 4))};

        std::vector<bool> expectedResults = {false, true};
        size_t iteration = 0;

        for (auto &obj : objectList)
        {
            ASSERT_TRUE((obj.hitObject(tempRay, kSkipFreq, &hitData)) == expectedResults[iteration]);
            ++iteration;
        }

        ASSERT_EQ(hitData.collisionPoint, Vec3(0.25, 0.25, 4));
        ASSERT_EQ(hitData.direction, tempRay.getDirection());
        ASSERT_EQ(hitData.origin, tempRay.getOrigin());
        ASSERT_EQ(hitData.time, 4);
    }

    TEST(TRIANGLEOBJ_METHOD, Hit_Object_Ray_Back_Object_Vec_0_0_1)
    {
        using namespace core;

        RayHitData hitData;
        Ray tempRay(Vec3(0.25, 0.25, 5), Vec3(0, 0, 1)); // Ray is in front of object2 with direction outside of object2;

        std::vector<TriangleObj> objectList = {TriangleObj(Vec3(1, 0, 0), Vec3(0, 0, 0), Vec3(0, 1, 0)), TriangleObj(Vec3(1, 0, 4), Vec3(0, 0, 4), Vec3(0, 1, 4))};

        for (auto &obj : objectList)
        {
            // Assert false? RTFM
            ASSERT_TRUE(!obj.hitObject(tempRay, kSkipFreq, &hitData));
        }
    }

    TEST(TRIANGLEOBJ_METHOD, Hit_Object_Ray_front_Object_Vec_0_0_1)
    {
        using namespace core;

        RayHitData hitData;
        Ray tempRay(Vec3(0.25, 0.25, -4), Vec3(0, 0, 1)); // Ray is in front of object2 with direction outside of object2;

        std::vector<TriangleObj> objectList = {TriangleObj(Vec3(1, 0, 0), Vec3(0, 0, 0), Vec3(0, 1, 0)), TriangleObj(Vec3(1, 0, 4), Vec3(0, 0, 4), Vec3(0, 1, 4))};

        for (auto &obj : objectList)
        {
            ASSERT_TRUE(obj.hitObject(tempRay, kSkipFreq, &hitData));
            ASSERT_EQ(hitData.collisionPoint, Vec3(0.25, 0.25, obj.point1().z()));
            ASSERT_EQ(hitData.time, std::abs(tempRay.getOrigin().z()) + obj.point1().z());
        }
    }

    TEST(TRIANGLEOBJ_METHOD, Hit_Object_Ray_Inside_Object_Vec_1_0_0)
    {
        using namespace core;

        RayHitData hitData;
        Ray tempRay(Vec3(0, 0.25, 0.25), Vec3(1, 0, 0)); // Ray has origin inside object1

        std::vector<TriangleObj> objectList = {TriangleObj(Vec3(0, 0, 1), Vec3(0, 0, 0), Vec3(0, 1, 0)), TriangleObj(Vec3(4, 0, 1), Vec3(4, 0, 0), Vec3(4, 1, 0))};
        std::vector<bool> ExpectedResults = {false, true};
        size_t iteration = 0;
        for (auto &obj : objectList)
        {
            ASSERT_TRUE((obj.hitObject(tempRay, kSkipFreq, &hitData) == ExpectedResults[iteration]));
            ++iteration;
        }

        ASSERT_EQ(hitData.collisionPoint, Vec3(4, 0.25, 0.25));
        ASSERT_EQ(hitData.direction, tempRay.getDirection());
        ASSERT_EQ(hitData.origin, tempRay.getOrigin());
        ASSERT_EQ(hitData.time, 4);
    }

    TEST(TRIANGLEOBJ_METHOD, Hit_Object_Ray_Back_Object_Vec_1_0_0)
    {
        using namespace core;

        RayHitData hitData;
        Ray tempRay(Vec3(5, 0.25, 0.25), Vec3(1, 0, 0)); // Ray is in front of object2 with direction outside of object2;

        std::vector<TriangleObj> objectList = {TriangleObj(Vec3(0, 0, 1), Vec3(0, 0, 0), Vec3(0, 1, 0)), TriangleObj(Vec3(4, 0, 1), Vec3(4, 0, 0), Vec3(4, 1, 0))};

        for (auto &obj : objectList)
        {
            ASSERT_TRUE(!obj.hitObject(tempRay, kSkipFreq, &hitData));
        }
    }

    TEST(TRIANGLEOBJ_METHOD, Hit_Object_Ray_front_Object_Vec_1_0_0)
    {
        using namespace core;

        RayHitData hitData;
        Ray tempRay(Vec3(-4, 0.25, 0.25), Vec3(1, 0, 0)); // Ray is in front of object2 with direction outside of object2;

        std::vector<TriangleObj> objectList = {TriangleObj(Vec3(0, 0, 1), Vec3(0, 0, 0), Vec3(0, 1, 0)), TriangleObj(Vec3(4, 0, 1), Vec3(4, 0, 0), Vec3(4, 1, 0))};

        for (auto &obj : objectList)
        {
            ASSERT_TRUE(obj.hitObject(tempRay, kSkipFreq, &hitData));
            ASSERT_EQ(hitData.collisionPoint, Vec3(obj.point1().x(), 0.25, 0.25));
            ASSERT_EQ(hitData.time, std::abs(tempRay.getOrigin().x()) + obj.point1().x());
        }
    }

    TEST(TRIANGLEOBJ_METHOD, Getters_And_Setters)
    {
        TriangleObj object1({0, 1, 1}, {2, 0, 2}, {3, 3, 0});

        ASSERT_EQ(object1.point1(), core::Vec3(0, 1, 1));
        ASSERT_EQ(object1.point2(), core::Vec3(2, 0, 2));
        ASSERT_EQ(object1.point3(), core::Vec3(3, 3, 0));

        object1.setPoint1(core::Vec3(0, 0, 0));
        object1.setPoint2(core::Vec3(1, 0, 0));
        object1.setPoint3(core::Vec3(1, 1, 0));

        object1.refreshAttributes();

        ASSERT_EQ(object1.point1(), core::Vec3(0, 0, 0));
        ASSERT_EQ(object1.point2(), core::Vec3(1, 0, 0));
        ASSERT_EQ(object1.point3(), core::Vec3(1, 1, 0));
        ASSERT_EQ(object1.normal(core::Vec3()), core::Vec3(0, 0, 1));
        ASSERT_EQ(object1.getOrigin(), core::Vec3((2.0 / 3.0), (1.0 / 3.0), 0));
    }

    // This test is kind of useless anyway, but hey...
    TEST(TriangleObjTest, Printing)
    {
        {
            std::stringstream ss;
            ss << TriangleObj(core::Vec3(std::sqrt(2), 0, 0), core::Vec3(1, 2, 3), core::Vec3(-4, -5, -6));
            ASSERT_EQ(ss.str(), "Triangle Object, vertex: Vec3(1.41421, 0, 0), Vec3(1, 2, 3), Vec3(-4, -5, -6)");
        }

        // This test is useless, it is here just for demonstration purposes that with braces I could reuse "ss".
        {
            std::stringstream ss;
            ss << TriangleObj();
            ASSERT_EQ(ss.str(), "Triangle Object, vertex: Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1)");
        }
    }

    // If ray has an origin inside the triangle then it never hits, as we try to
    // avoid hitting the same object twice.
    TEST(TriangleObjectTest, RayWithOriginInsideDoNotHit)
    {
        // 1. Define some constant data points and name them nicely, so that you can just
        // reuse them but not rewrite them every time. What if you make a mistake in the
        // numbers and then you spend hours trying to figure out where is the error?
        //
        // 2. Try to define variables just before you use them.
        // https://stackoverflow.com/questions/17386326/what-downsides-is-declaring-variables-just-before-first-time-use-it
        // https://wiki.c2.com/?DeclareVariablesAtFirstUse
        Vec3 kA(1, 2, 3);
        Vec3 kB(4, 5, 2);
        Vec3 kC(-2, -2, 1);
        TriangleObj triangle(kA, kB, kC);

        // Naming is key, this way the asserts below read nicely.
        // Alternatively, allow to pass nullptr in hitObject.
        RayHitData ignore;

        // Group the code so that it has:
        // 1. Data preparation
        // 2. Call method under test
        // 3. Asserts (all you need for this specific test).
        //
        // This way you can focus on this one specific test case, as opposed to first read
        // all the data you prepare for all other tests, and then see the methods under test and then
        // asserts you, and then you need to jump back and fourth between data and checks to make sure it works...
        // Nightmare.
        //
        // The worst example of this is the first test with ostream opeartor.
        //
        // References:
        // https://blogs.agilefaqs.com/2010/11/14/single-assert-per-unit-test-myth/
        // https://www.agilealliance.org/glossary/gwt/
        //
        // Side note: Thanks to my clever initialization, I am sure this is inside the triangle
        // regardless of what is A, B, C. I am not limited to simple cases!
        //
        // This defines just the center of the triangle, but more general you have Barycentric coordinates [1],
        // so you could just generate A, B, C, and generate random point on a triangle and always check if this
        // is true.
        //
        // [1] https://en.wikipedia.org/wiki/Barycentric_coordinate_system#Barycentric_coordinates_on_triangles
        Ray rayOppositeNormal((kA + kB + kC) / 3.0, -triangle.normal());
        ASSERT_FALSE(triangle.hitObject(rayOppositeNormal, kSkipFreq, &ignore));

        // Here I am constructing this object in a non trivial way so I need to also check if
        // my assumption that this ray has a perperpendicular normal checks out.
        // Even if the constrution is trivial, just putting numbers in place still check if you assumptions hold.
        // You made this mistake in the past, multiple times, e.g when triangle was really a line,
        // so assumptions were wrong, and you spend coutless hours debugging.
        {
            Ray rayPerpendicularNormal((kA + kB + kC) / 3.0,
                                       triangle.normal().crossProduct(kA - kB));
            ASSERT_NEAR(0, rayPerpendicularNormal.getDirection().scalarProduct(triangle.normal()), 0.001f)
                << "Ray is not perpendicular to the triangle normal, but it should be";
            ASSERT_FALSE(triangle.hitObject(rayPerpendicularNormal, kSkipFreq, &ignore));
        }

        // Sometimes it is a good idea to wrap test cases with braces like this. This way:
        // 1. you have some visual separation of the test cases. You can also add comments explaining each case instead,
        // saying, why you want this to happen.
        // 2. if you want to reuse the same variables you used earlier (see Printing test).
        {
            Ray raySamDirectionNormal((kA + kB + kC) / 3.0, triangle.normal());
            ASSERT_FALSE(triangle.hitObject(raySamDirectionNormal, kSkipFreq, &ignore));
        }
    }

    // This is how it looks without comments.
    TEST(TriangleObjectTest, RayWithOriginInsideDoNotHit2)
    {
        Vec3 kA(1, 2, 3);
        Vec3 kB(4, 5, 2);
        Vec3 kC(-2, -2, 1);
        TriangleObj triangle(kA, kB, kC);

        RayHitData ignore;

        Ray rayOppositeNormal((kA + kB + kC) / 3.0, -triangle.normal());
        ASSERT_FALSE(triangle.hitObject(rayOppositeNormal, kSkipFreq, &ignore));

        Ray raySamDirectionNormal((kA + kB + kC) / 3.0, triangle.normal());
        ASSERT_FALSE(triangle.hitObject(raySamDirectionNormal, kSkipFreq, &ignore));

        Ray rayPerpendicularNormal((kA + kB + kC) / 3.0,
                                   triangle.normal().crossProduct(kA - kB));
        ASSERT_NEAR(0, rayPerpendicularNormal.getDirection().scalarProduct(triangle.normal()), 0.001f)
            << "Ray is not perpendicular to the triangle normal, but it should be";
        ASSERT_FALSE(triangle.hitObject(rayPerpendicularNormal, kSkipFreq, &ignore));
    }

}; // namespace objects
