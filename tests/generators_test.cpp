#include "gtest/gtest.h"
#include "obj/generators.h"
#include "obj/objects.h"

#include <random>
#include <cmath>
#include <sstream>

namespace generators
{
    FakeRandomGen fakeEngine;

    TEST(POINTSOURCE_CONSTRUCTOR, All_Possible_Constructors)
    {
        ASSERT_EQ(PointSource(100, 1000, 1, &fakeEngine), PointSource(100, 1000, 1, &fakeEngine));
        ASSERT_EQ(PointSource(100, 1000, 1, &fakeEngine), PointSource(100, 1000, 1, &fakeEngine));
    }

    TEST(POINTSOURCE_OPERATORS, Operator_equal)
    {
        PointSource source = PointSource(100, 1000, 1, &fakeEngine);
        PointSource source2 = source;

        ASSERT_EQ(source2, source);
    }

    TEST(POINTSOURCE_OPERATORS, Operator_Equal_Equal)
    {
        PointSource object1(100, 100, 100, &fakeEngine);
        PointSource object2(100, 100, 100, &fakeEngine);
        PointSource object3(1000, 200, 100, &fakeEngine);
        PointSource object4(1000, 200, 100, &fakeEngine);
        PointSource object5(std::sqrt(2), 100, std::sqrt(2), &fakeEngine);
        PointSource object6(std::sqrt(2), 100, std::sqrt(2), &fakeEngine);

        ASSERT_EQ(object1, object2);
        ASSERT_EQ(object3, object4);
        ASSERT_EQ(object5, object6);
    }

    TEST(POINTSOURCE_OPERATORS, Operators_ostream)
    {
        PointSource object1(100, 100, 100, &fakeEngine);
        PointSource object2(1000, 200, 100, &fakeEngine);
        PointSource object3(std::sqrt(2), 100, std::sqrt(2), &fakeEngine);

        std::stringstream ss1;
        std::stringstream ss2;
        std::stringstream ss3;

        ss1 << object1;
        ss2 << object2;
        ss3 << object3;

        ASSERT_EQ(ss1.str(), "Point Source: origin: Vec3(0, 0, 4), number of rays per row: 100, diffusor size: 100, frequency: 100");
        ASSERT_EQ(ss2.str(), "Point Source: origin: Vec3(0, 0, 4), number of rays per row: 200, diffusor size: 100, frequency: 1000");
        ASSERT_EQ(ss3.str(), "Point Source: origin: Vec3(0, 0, 4), number of rays per row: 100, diffusor size: 1.41421, frequency: 1.41421");
    }

    TEST(POINTSOURCE_METHODS, Getters)
    {
        PointSource object1(100, 100, 100, &fakeEngine);
        PointSource object2(1000, 200, 100, &fakeEngine);
        PointSource object3(std::sqrt(2), 100, std::sqrt(2), &fakeEngine);

        ASSERT_EQ(object1.frequency(), 100);
        ASSERT_EQ(object2.frequency(), 1000);
        ASSERT_EQ(object3.frequency(), std::sqrt(2));

        ASSERT_EQ(object1.numOfRaysPerRow(), 100);
        ASSERT_EQ(object2.numOfRaysPerRow(), 200);
        ASSERT_EQ(object3.numOfRaysPerRow(), 100);

        ASSERT_EQ(object1.sampleSize(), 100);
        ASSERT_EQ(object2.sampleSize(), 100);
        ASSERT_EQ(object3.sampleSize(), std::sqrt(2));

        ASSERT_EQ(object1.origin(), core::Vec3(0, 0, 4));
        ASSERT_EQ(object2.origin(), core::Vec3(0, 0, 4));
        ASSERT_EQ(object3.origin(), core::Vec3(0, 0, 4));
    }

    TEST(POINTSOURCE_METHODS, Setters)
    {
        PointSource object1(100, 100, 100, &fakeEngine);
        PointSource object2(1000, 200, 100, &fakeEngine);
        PointSource object3(std::sqrt(2), 100, std::sqrt(2), &fakeEngine);

        object1.setFrequency(1000);
        object2.setFrequency(std::sqrt(3));
        object3.setFrequency(std::sqrt(5));

        ASSERT_EQ(object1.frequency(), 1000);
        ASSERT_EQ(object2.frequency(), std::sqrt(3));
        ASSERT_EQ(object3.frequency(), std::sqrt(5));

        object1.setNumOfRaysPerRow(200);
        object2.setNumOfRaysPerRow(800);
        object3.setNumOfRaysPerRow(2);

        ASSERT_EQ(object1.numOfRaysPerRow(), 200);
        ASSERT_EQ(object2.numOfRaysPerRow(), 800);
        ASSERT_EQ(object3.numOfRaysPerRow(), 2);

        object1.setDiffusorSize(std::sqrt(2));
        object2.setDiffusorSize(std::sqrt(13));
        object3.setDiffusorSize(std::sqrt(21));

        ASSERT_EQ(object1.sampleSize(), std::sqrt(2));
        ASSERT_EQ(object2.sampleSize(), std::sqrt(13));
        ASSERT_EQ(object3.sampleSize(), std::sqrt(21));

        object1.setOrigin(core::Vec3(0, 0, 1));
        object2.setOrigin(core::Vec3(1, 0, 2));
        object3.setOrigin(core::Vec3(0, 12, 2));

        ASSERT_EQ(object1.origin(), core::Vec3(0, 0, 1));
        ASSERT_EQ(object2.origin(), core::Vec3(1, 0, 2));
        ASSERT_EQ(object3.origin(), core::Vec3(0, 12, 2));
    }

    TEST(POINTSOURCE_METHODS, Method_updateDiffusorSize)
    {
        PointSource object1(100, 1000, 100, &fakeEngine);
        PointSource object2(200, 100, 1, &fakeEngine);

        ASSERT_EQ(object1.getLeftCorner(), core::Vec3(-50, -50, 1));
        ASSERT_EQ(object2.getLeftCorner(), core::Vec3(-0.5, -0.5, 1));
    }

    TEST(POINTSOURCE_METHOD, Single_rayHit)
    {
        const double freq = 1000;
        const size_t rayNumPerRow = 1000;

        PointSource source(freq, rayNumPerRow, 1, &fakeEngine);

        std::vector<std::unique_ptr<objects::TriangleObj>> objectsVec;
        objects::TriangleObj object({-0.6, -0.6, 1}, {0.6, -0.6, 1}, {-0.6, 0.6, 1});

        core::Ray tempRay = source.GenerateRay(0, 0);
        core::RayHitData hitData;
        ASSERT_TRUE(object.hitObject(tempRay, freq, &hitData));
        std::cout << hitData << std::endl;
        ASSERT_EQ(core::Ray(source.origin(), hitData.direction), core::Ray(core::Vec3(0, 0, 4), core::Vec3(-0.5, -0.5, -3).normalize()));
    }

    TEST(POINTSOURCE_METHODS, GenerateRay_Test) // Monte Carlo Test
    {
        const size_t rayNumPerRow = 1000;
        const double freq = 1000;
        const size_t samples = 3;
        PointSource source(freq, rayNumPerRow, 1, &fakeEngine);

        objects::TriangleObj object({-0.25, -0.25, 1}, {0.25, -0.25, 1}, {-0.25, 0.25, 1});
        double hits = 0, missed = 0;
        core::RayHitData hitData;
        for (size_t x = 0; x < rayNumPerRow; ++x)
        {
            for (size_t y = 0; y < rayNumPerRow; ++y)
            {
                core::Ray tempRay = source.GenerateRay(x, y);
                if (object.hitObject(tempRay, freq, &hitData))
                {
                    ++hits;
                }
                else
                {
                    ++missed;
                }
            }
        }

        ASSERT_NEAR(hits / (hits + missed), 0.125, constants::kHitAccuracy * 10);
    }

} // namespace generators
