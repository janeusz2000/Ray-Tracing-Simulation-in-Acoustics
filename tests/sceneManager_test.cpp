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
    double energyCollectorRadius = objects::EnergyCollector(kSkipVec3).getRadius();
    void SetUp() override
    {
        energyCollectors = manager.getEnergyCollectors();
    }

    bool performHitAtEnergyCollectors(const Ray &tempRay, RayHitData *hitData)
    {
        bool hit = false;
        for (const auto &energyColPtr : energyCollectors)
        {
            if (energyColPtr->hitObject(tempRay, kSkipFreq, hitData))
                hit = true;
        }
        return hit;
    }

public:
    SceneManagerTest(){};
};

TEST_F(SceneManagerTest, PrintAllEnergyCollectorpositions)
{
    SetUp();
    for (const auto &collectorPtr : energyCollectors)
    {
        std::cout << *collectorPtr << std::endl;
    }
}

TEST_F(SceneManagerTest, EnergyCollectorPositionsCheck)
{
    SetUp();
    ASSERT_EQ(energyCollectors.size(), constants::kCollectors) << "wrong numebr of energy collectors";

    RayHitData hitData;
    Ray tempRay(core::Vec3(0, 0, 0), core::Vec3(0, 0, 1));

    ASSERT_TRUE(performHitAtEnergyCollectors(tempRay, &hitData)) << "no hit: " << tempRay;

    core::Vec3 referenceCollisionPoint(0, 0, constants::kSimulationRadius / 2 - energyCollectorRadius);
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint) << "Invalid hit point from: " << tempRay;

    tempRay.setDirection(Vec3(-1, 0, 0));

    ASSERT_TRUE(performHitAtEnergyCollectors(tempRay, &hitData)) << "no hit: " << tempRay;

    referenceCollisionPoint = Vec3(-constants::kSimulationRadius / 2 - energyCollectorRadius, 0, 0);
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint) << "Invalid hit point from: " << tempRay;

    tempRay.setDirection(Vec3(0, 1, 0));

    ASSERT_TRUE(performHitAtEnergyCollectors(tempRay, &hitData)) << "no hit: " << tempRay;

    referenceCollisionPoint = Vec3(0, constants::kSimulationRadius / 2 - energyCollectorRadius, 0);
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint) << "Invalid hit point from: " << tempRay;
}
