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
#include <random>

namespace objects
{

    const double kSkipFreq = 1000;

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
        TriangleObj object1({0, 0, 0}, {0, 1, 0}, {1, 0, 0});
        TriangleObj object2({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
        ASSERT_EQ(object1.normal(core::Vec3()), core::Vec3(0, 0, -1));
        ASSERT_EQ(object2.normal(core::Vec3()), core::Vec3(-1, 0, 0));
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_does_Hit_Vec3_0_0_1)
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
            if (object.hitObject(tempRay, kSkipFreq, hitData))
            {
                ++hits;
            }
        }
        ASSERT_NEAR(referenceRatio, hits / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_does_Hit_Vec3_0_0_minus1)
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
            if (object.hitObject(tempRay, kSkipFreq, hitData))
            {
                ++hits;
            }
        }
        ASSERT_NEAR(referenceRatio, hits / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_does_Hit_Vec3_0_1_0)
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
            if (object.hitObject(tempRay, kSkipFreq, hitData))
            {
                ++hits;
            }
        }
        ASSERT_NEAR(referenceRatio, hits / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_does_Hit_Vec3_0_minus1_0)
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
            if (object.hitObject(tempRay, kSkipFreq, hitData))
            {
                ++hits;
            }
        }
        ASSERT_NEAR(referenceRatio, hits / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_does_Hit_Vec3_1_0_0)
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
            if (object.hitObject(tempRay, kSkipFreq, hitData))
            {
                ++hits;
            }
        }
        ASSERT_NEAR(referenceRatio, hits / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_does_Hit_Vec3_minus1_0_0)
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
            if (object.hitObject(tempRay, kSkipFreq, hitData))
            {
                ++hits;
            }
        }
        ASSERT_NEAR(referenceRatio, hits / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_HitObject) // This is Monte Carlo test: https://en.wikipedia.org/wiki/Monte_Carlo_method
    {
        TriangleObj object1({1, 2, 1}, {1, 1, 1}, {2, 1, 1});

        std::random_device rd;
        std::mt19937_64 e2(rd());
        std::uniform_real_distribution<double> dist(0, 3);

        double hits = 0;
        double missed = 0;
        const double areaRatio = 0.5 / 9;

        for (auto a = 0; a < 10000000; ++a)
        {
            core::Vec3 randomPoint(dist(e2), dist(e2), 1);
            if (object1.doesHit(randomPoint))
            {
                ++hits;
            }
            else
            {
                ++missed;
            }
        }

        double ratio = hits / (hits + missed);
        std::cout << "Missed: " << missed << ", Hits: " << hits << ", RATIO: " << ratio << std::endl;
        ASSERT_NEAR(ratio, areaRatio, constants::kHitAccuracy * 10);
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_Area)
    {
        ASSERT_EQ(TriangleObj({0, 0, 0}, {0, 0, 1}, {0, 1, 1}).area(), 0.5);
        ASSERT_EQ(TriangleObj({0, 0, 0}, {0, 0, 2}, {0, 2, 2}).area(), 2);
        ASSERT_EQ(TriangleObj({0, 0, 0}, {5, 0, 0}, {5, 5, 0}).area(), 12.5);
    }

    TEST(TRIANGLEOBJ_METHOD, Test_Method_Getters_And_Setters)
    {
        TriangleObj object1({0, 1, 1}, {2, 0, 2}, {3, 3, 0});

        ASSERT_EQ(object1.getX(), core::Vec3(0, 1, 1));
        ASSERT_EQ(object1.getY(), core::Vec3(2, 0, 2));
        ASSERT_EQ(object1.getZ(), core::Vec3(3, 3, 0));

        object1.setX(core::Vec3(0, 0, 0));
        object1.setY(core::Vec3(1, 0, 0));
        object1.setZ(core::Vec3(1, 1, 0));

        object1.refreshAttributes();

        ASSERT_EQ(object1.getX(), core::Vec3(0, 0, 0));
        ASSERT_EQ(object1.getY(), core::Vec3(1, 0, 0));
        ASSERT_EQ(object1.getZ(), core::Vec3(1, 1, 0));
        ASSERT_EQ(object1.normal(core::Vec3()), core::Vec3(0, 0, 1));
        ASSERT_EQ(object1.getOrigin(), core::Vec3((2.0 / 3.0), (1.0 / 3.0), 0));
    }

}; // namespace objects

#endif