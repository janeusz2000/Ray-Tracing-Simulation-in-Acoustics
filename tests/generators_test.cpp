#ifndef GENERATORS_TEST_CPP
#define GENERATORS_TEST_CPP

#include "gtest/gtest.h"
#include "obj/generators.h"
#include "obj/objects.h"

#include <random>
#include <cmath>
#include <sstream>

namespace generators
{

    TEST(POINTSOURCE_CONSTRUCTOR, Test_All_Possible_Constructors)
    {
        ASSERT_EQ(PointSource(100, 1000, 1), PointSource(100, 1000, 1));
        ASSERT_EQ(PointSource(PointSource(100, 1000, 1)), PointSource(100, 1000, 1));
    }

    TEST(POINTSOURCE_OPERATORS, Test_Operator_equal)
    {
        PointSource source = PointSource(100, 1000, 1);
        PointSource source2 = source;

        ASSERT_EQ(source2, source);
    }

    TEST(POINTSOURCE_OPERATORS, Test_Operator_Equal_Equal)
    {
        PointSource object1(100, 100, 100);
        PointSource object2(100, 100, 100);
        PointSource object3(1000, 200, 100);
        PointSource object4(1000, 200, 100);
        PointSource object5(std::sqrt(2), 100, std::sqrt(2));
        PointSource object6(std::sqrt(2), 100, std::sqrt(2));

        ASSERT_EQ(object1, object2);
        ASSERT_EQ(object3, object4);
        ASSERT_EQ(object5, object6);
    }

    TEST(POINTSOURCE_OPERATORS, Test_Operators_ostream)
    {
        PointSource object1(100, 100, 100);
        PointSource object2(1000, 200, 100);
        PointSource object3(std::sqrt(2), 100, std::sqrt(2));

        std::stringstream ss1;
        std::stringstream ss2;
        std::stringstream ss3;

        ss1 << object1;
        ss2 << object2;
        ss3 << object3;

        ASSERT_EQ(ss1.str(), "Point Source: origin: Vec3(0, -4, 0), number of rays per row: 100, diffusor size: 100, frequency: 100");
        ASSERT_EQ(ss2.str(), "Point Source: origin: Vec3(0, -4, 0), number of rays per row: 200, diffusor size: 100, frequency: 1000");
        ASSERT_EQ(ss3.str(), "Point Source: origin: Vec3(0, -4, 0), number of rays per row: 100, diffusor size: 1.41421, frequency: 1.41421");
    }

    TEST(POINTSOURCE_METHODS, Test_Getters)
    {
        PointSource object1(100, 100, 100);
        PointSource object2(1000, 200, 100);
        PointSource object3(std::sqrt(2), 100, std::sqrt(2));

        ASSERT_EQ(object1.getFrequency(), 100);
        ASSERT_EQ(object2.getFrequency(), 1000);
        ASSERT_EQ(object3.getFrequency(), std::sqrt(2));

        ASSERT_EQ(object1.getRayNumPerRow(), 100);
        ASSERT_EQ(object2.getRayNumPerRow(), 200);
        ASSERT_EQ(object3.getRayNumPerRow(), 100);

        ASSERT_EQ(object1.getDiffusorSize(), 100);
        ASSERT_EQ(object2.getDiffusorSize(), 100);
        ASSERT_EQ(object3.getDiffusorSize(), std::sqrt(2));

        ASSERT_EQ(object1.getOrigin(), core::Vec3(0, -4, 0));
        ASSERT_EQ(object2.getOrigin(), core::Vec3(0, -4, 0));
        ASSERT_EQ(object3.getOrigin(), core::Vec3(0, -4, 0));
    }

    TEST(POINTSOURCE_METHODS, Test_Setters)
    {
        PointSource object1(100, 100, 100);
        PointSource object2(1000, 200, 100);
        PointSource object3(std::sqrt(2), 100, std::sqrt(2));

        object1.setFrequency(1000);
        object2.setFrequency(std::sqrt(3));
        object3.setFrequency(std::sqrt(5));

        ASSERT_EQ(object1.getFrequency(), 1000);
        ASSERT_EQ(object2.getFrequency(), std::sqrt(3));
        ASSERT_EQ(object3.getFrequency(), std::sqrt(5));

        object1.setRayNumPerRow(200);
        object2.setRayNumPerRow(800);
        object3.setRayNumPerRow(2);

        ASSERT_EQ(object1.getRayNumPerRow(), 200);
        ASSERT_EQ(object2.getRayNumPerRow(), 800);
        ASSERT_EQ(object3.getRayNumPerRow(), 2);

        object1.setDiffusorSize(std::sqrt(2));
        object2.setDiffusorSize(std::sqrt(13));
        object3.setDiffusorSize(std::sqrt(21));

        ASSERT_EQ(object1.getDiffusorSize(), std::sqrt(2));
        ASSERT_EQ(object2.getDiffusorSize(), std::sqrt(13));
        ASSERT_EQ(object3.getDiffusorSize(), std::sqrt(21));

        object1.setOrigin(core::Vec3(0, 0, 1));
        object2.setOrigin(core::Vec3(1, 0, 2));
        object3.setOrigin(core::Vec3(0, 12, 2));

        ASSERT_EQ(object1.getOrigin(), core::Vec3(0, 0, 1));
        ASSERT_EQ(object2.getOrigin(), core::Vec3(1, 0, 2));
        ASSERT_EQ(object3.getOrigin(), core::Vec3(0, 12, 2));
    }

    TEST(POINTSOURCE_METHODS, Test_Method_updateDiffusorSize)
    {
        PointSource object1(100, 1000, 100);
        PointSource object2(200, 100, 1);

        ASSERT_EQ(object1.getLeftCorner(), core::Vec3(-50, 1, -50));
        ASSERT_EQ(object2.getLeftCorner(), core::Vec3(-0.5, 1, -0.5));
    }

    TEST(POINTSOURCE_METHOD, Test_Single_rayHit)
    {
        const double freq = 1000;
        const size_t rayNumPerRow = 1000;

        PointSource source(freq, rayNumPerRow, 1);

        objects::TriangleObj object({-0.6, 1, -0.6}, {0.6, 1, -0.6}, {-0.6, 1, 0.6});

        core::Ray tempRay = source.GenerateRay(0, 0);
        auto hitData = object.hitObject(tempRay, freq);

        ASSERT_TRUE(hitData);
        // TODO: WHY THIS DOESN"T WORK?
        // ASSERT_EQ(core::Ray(source.getOrigin(), hitData->direction), core::Ray(core::Vec3(0, -4, 0), core::Vec3(-0.5, 3, -0.5).normalize()));
    }

    TEST(POINTSOURCE_METHODS, Test_GenerateRay_Test) // Monte Carlo Test
    {
        const size_t rayNumPerRow = 1000;
        const double freq = 1000;
        const size_t samples = 3;
        PointSource source(freq, rayNumPerRow, 1);

        objects::TriangleObj object({-0.25, 1, -0.25}, {0.25, 1, -0.25}, {-0.25, 1, 0.25});
        double hits = 0, missed = 0;

        for (size_t x = 0; x < rayNumPerRow; ++x)
        {
            for (size_t y = 0; y < rayNumPerRow; ++y)
            {
                core::Ray tempRay = source.GenerateRay(x, y, true);
                if (object.hitObject(tempRay, freq))
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
#endif
