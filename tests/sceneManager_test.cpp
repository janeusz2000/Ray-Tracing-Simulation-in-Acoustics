#include "gtest/gtest.h" // https://google.github.io/styleguide/cppguide.html#Namespaces
#include "main/sceneManager.h"
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

// TODO: Change all doubles to float

TEST(SceneManagertest, IsTestSampleRightShape)
{
    Vec3 k1(-0.5, -0.5, 0);
    Vec3 k2(0.5, -0.5, 0);
    Vec3 k3(-0.5, 0.5, 0);
    Vec3 k4(0.5, 0.5, 0);
    Vec3 k5(123, 123, 1);

    SceneManager manager;

    const std::vector<TriangleObj> referenceSample = {TriangleObj(k1, k2, k3), TriangleObj(k2, k3, k4)};

    std::vector<objects::Object *> testSample;
    ASSERT_TRUE(manager.loadTestSample(&testSample)) << "test sample wasn't build";

    // TODO: This needs to be replaced with ASSERT_THAT. I should read more about google test framework
    // ASSERT_THAT(test_sample, UnorderedElementsAre(TriangleObj(k1, k2, k3), TriangleObj(k2, k3, k4)));

    ASSERT_TRUE(std::find(referenceSample.cbegin(), referenceSample.cend(),
                          *testSample[0]) != referenceSample.cend() &&
                std::find(referenceSample.cbegin(), referenceSample.cend(),
                          *testSample[1]) != referenceSample.cend())
        << "Triangle1: " << *testSample[0] << ", Triangle2: " << *testSample[1];

    // TODO: Make class that has function which does it for me. Its good idea to use TEST_F macrofor this.

    const std::vector<TriangleObj> invalidTestSample = {TriangleObj(k1, k2, k3), TriangleObj(k2, k3, k5)};
    ASSERT_FALSE(std::find(invalidTestSample.cbegin(), invalidTestSample.cend(), *testSample[0]) != invalidTestSample.cend() &&
                 std::find(invalidTestSample.cbegin(), invalidTestSample.cend(), *testSample[1]) != referenceSample.cend())
        << "Triangle1: " << *testSample[0] << ", Triangle2: " << *testSample[1];
}

TEST(SceneManagerTest, PointSourceRaysHitsBuildedTestSample)
{
    // Monte Carlo test: https://en.wikipedia.org/wiki/Monte_Carlo_method

    // test sample should return vector of the two triangleObjs creating square that is situated at Vec3(0, 0, 0) in the middle;
    // it also should have SphereWall object at the end.
    // Point Source has origin at Vec3(0, 0, 4)
    //
    // Two triangle objects are making perfect square at:
    //
    // a) Vec3(-0.5, -0.5, 0)
    // b) Vec3(0.5, -0.5, 0)
    // c) Vec3(-0.5, 0.5, 0)
    // d) Vec3(0.5, 0.5, 0)
    //
    // area of the testSample is equal to 1 m^2
    //
    // Square area that generated rays are hiting is equal to the 4/3 * 4/3 = 16 / 9
    // This comes from the triangle laws;
    // Point source origin is at Vec3(0, 0, 4) and it's calculateing its direction at the same
    // square as sample test, but lifted up by Vec3(0, 0, 1)
    // from triangle law we get (3 / 0.5)^2  [ (Vec3(0, 0, 4) - Vec3(0, 0, 1) / ( a / 2))^2 ]
    // which is equal to 4/3 * 4/3 = 16 /9

    const size_t rayNumberPerRow = 10000;
    const double sampleSide = 1;
    PointSource source(kSkipFreq, rayNumberPerRow, sampleSide, &fakeRand);

    SceneManager manager;
    std::vector<objects::Object *> testSample;
    ASSERT_TRUE(manager.loadTestSample(&testSample)) << "Test sample wasn't build";

    RayHitData ignore;

    size_t hits = 0;
    for (size_t xIter = 0; xIter < rayNumberPerRow; ++xIter)
    {
        for (size_t yIter = 0; yIter < rayNumberPerRow; ++yIter)
        {
            for (Object *objPtr : testSample)
            {
                if (objPtr->hitObject(source.generateRay(xIter, yIter), kSkipFreq, &ignore))
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
