#include "gtest/gtest.h" // https://google.github.io/styleguide/cppguide.html#Namespaces
#include "main/sceneBuilder.h"
#include "obj/generators.h"
#include "obj/objects.h"

using core::Ray;
using core::RayHitData;
using core::Vec3;
using generators::PointSource;
using objects::Object;
using objects::TriangleObj;

const double kSkipFreq = 1000;
generators::FakeRandomGen fakeRand;

TEST(SceneBuildertest, IsTestSampleRightShape)
{
    Vec3 k1(-0.5, -0.5, 0);
    Vec3 k2(0.5, -0.5, 0);
    Vec3 k3(-0.5, 0.5, 0);
    Vec3 k4(0.5, 0.5, 0);
    Vec3 k5(123, 123, 1);

    SceneBuilder builder;

    const std::vector<TriangleObj> referenceSample = {TriangleObj(k1, k2, k3), TriangleObj(k2, k3, k4)};

    // because how operator== in triangleObj is implemented, I can do that:

    std::vector<std::unique_ptr<Object>> testSample = builder.BuildTestSample();
    ASSERT_TRUE(std::find(referenceSample.cbegin(), referenceSample.cend(), *testSample[0]) != referenceSample.cend() &&
                std::find(referenceSample.cbegin(), referenceSample.cend(), *testSample[1]) != referenceSample.cend())
        << "Triangle1: " << *testSample[0] << ", Triangle2: " << *testSample[1];

    const std::vector<TriangleObj> invalidTestSample = {TriangleObj(k1, k2, k3), TriangleObj(k2, k3, k5)};
    ASSERT_FALSE(std::find(invalidTestSample.cbegin(), invalidTestSample.cend(), *testSample[0]) != invalidTestSample.cend() &&
                 std::find(invalidTestSample.cbegin(), invalidTestSample.cend(), *testSample[1]) != referenceSample.cend())
        << "Triangle1: " << *testSample[0] << ", Triangle2: " << *testSample[1];
}

TEST(SceneBuilderTest, PointSourceRaysHitsBuildedTestSample)
{
    // ! IM NOT SURE THIS IS USEFULL BUT HEEEEEY ;-)
    // Monte Carlo test: https://en.wikipedia.org/wiki/Monte_Carlo_method

    const size_t rayNumberPerRow = 10000;
    const double sampleSide = 1;
    PointSource source(kSkipFreq, rayNumberPerRow, sampleSide, fakeRand);

    SceneBuilder builder;
    std::vector<std::unique_ptr<Object>> testSample = builder.BuildTestSample();

    RayHitData ignore;

    size_t hits = 0;
    for (size_t xIter = 0; xIter < rayNumberPerRow; ++xIter)
    {
        for (size_t yIter = 0; yIter < rayNumberPerRow; ++yIter)
        {
            for (const Object *objPtr : testSample)
            {
                if (objPtr->hitObject(source.generateRay(xIter, yIter), kSkipFreq, &ignore)
                {
                    ++hits;
                    break;
                }
            }
        }
    }

    const double referenceRatio = 9 / 16;
    const double ratio = static_cast<double>(hits) / static_cast<double>(rayNumberPerRow * rayNumberPerRow);
    ASSERT_NEAR(referenceRatio, ratio, constants::kHitAccuracy) << "Successful hits: " << hits << ", missed: "
                                                                << rayNumberPerRow * rayNumberPerRow - hits;
}
