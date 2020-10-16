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

TEST(SceneBuilderTest, PointSourceRaysHitsBuildedTestSample) // Monte Carlo test: https://en.wikipedia.org/wiki/Monte_Carlo_method
{
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
