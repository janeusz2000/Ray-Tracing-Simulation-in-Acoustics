#include "main/sceneManager.h"
#include "obj/generators.h"
#include "obj/objects.h"
#include "gtest/gtest.h" // https://google.github.io/styleguide/cppguide.html#Namespaces

using core::Ray;
using core::RayHitData;
using core::Vec3;
using generators::PointSource;
using objects::Object;
using objects::TriangleObj;

const double kSkipFreq = 1000;
const core::Vec3 kSkipVec3(0, 0, 0);
generators::FakeRandomGen fakeRand;

class SceneManagerTest : public ::testing::Test
{
protected:
    SceneManager manager;
    std::vector<objects::EnergyCollector *> energyCollectors;
    void SetUp() override
    {
        energyCollectors = manager.getEnergyCollectors();
    }

public:
    SceneManagerTest(){};
};

TEST_F(SceneManagerTest, EnergyCollectorPositionCheck)
{
    SetUp();
    ASSERT_EQ(energyCollectors.size(), constants::kCollectors) << "wrong numebr ogf energy collectors";

    RayHitData hitData;
    Ray tempRay(core::Vec3(0, 0, 0), core::Vec3(0, 0, 1));
    bool hit = false;

    for (const auto &energyColPtr : energyCollectors)
    {
        if (energyColPtr->hitObject(tempRay, kSkipFreq, &hitData))
            hit = true;
    }
    ASSERT_TRUE(hit) << "Ray didn't hit any of the collectors;";

    core::Vec3 referenceCollisionPoint(0, 0, constants::kSimulationRadius / 2 - objects::EnergyCollector(kSkipVec3).getRadius());
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint) << "Invalid hit point from ray(core::Vec3(0, 0, 0), core::Vec3(0, 0, 1))";
}
