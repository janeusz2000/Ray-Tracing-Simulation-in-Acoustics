#include "main/sceneManager.h"
#include "obj/generators.h"
#include "obj/objects.h"
#include "gtest/gtest.h" // https://google.github.io/styleguide/cppguide.html#Namespaces

// TODO: change underscore to the end

using core::Ray;
using core::RayHitData;
using core::Vec3;
using generators::PointSource;
using objects::Object;
using objects::TriangleObj;

generators::FakeRandomGen fakeRand;

class SceneManagerTest : public ::testing::Test
{
protected:
    SceneManager manager;
    double energyCollectorRadius = objects::EnergyCollector(Vec3()).getRadius(); // TODO: I dont like this. Vec3 should be skip ;-)
                                                                                 // TODO: delete hard dependency
    bool performHitAtEnergyCollectors(const Ray &tempRay, RayHitData *hitData)
    {
        bool hit = false;
        for (objects::EnergyCollector *energyCol : manager.getEnergyCollectors())
        {
            const double kSkipFreq = 1000;
            if (energyCol->hitObject(tempRay, kSkipFreq, hitData))
                hit = true;
        }
        return hit;
    }
};

TEST_F(SceneManagerTest, PrintAllEnergyCollectorpositions) // TODO: Remove when second test is passed
{
    for (const auto &collectorPtr : manager.getEnergyCollectors())
    {
        std::cout << *collectorPtr << std::endl;
    }
}

TEST_F(SceneManagerTest, EnergyCollectorPositionsCheck)
{
    ASSERT_EQ(manager.getEnergyCollectors().size(), constants::kPopulation) << "wrong numebr of energy collectors";

    RayHitData hitData;
    Ray tempRay(core::Vec3(0, 0, 0), core::Vec3(0, 0, 1));

    ASSERT_TRUE(performHitAtEnergyCollectors(tempRay, &hitData)) << "no hit: " << tempRay;

    core::Vec3 referenceCollisionPoint(0, 0, constants::kSimulationRadius / 2 - energyCollectorRadius);
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint) << "Invalid hit point from: " << tempRay;

    Ray tempRay2(core::Vec3(0, 0, 0), core::Vec3(-1, 0, 0));
    ASSERT_TRUE(performHitAtEnergyCollectors(tempRay2, &hitData)) << "no hit: " << tempRay;

    Vec3 referenceCollisionPoint2(-constants::kSimulationRadius / 2 - energyCollectorRadius, 0, 0);
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint2) << "Invalid hit point from: " << tempRay;

    Ray tempRay3(core::Vec3(0, 0, 0), core::Vec3(0, 1, 0));
    ASSERT_TRUE(performHitAtEnergyCollectors(tempRay3, &hitData)) << "no hit: " << tempRay;

    Vec3 referenceCollisionPoint3(0, constants::kSimulationRadius / 2 - energyCollectorRadius, 0);
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint3) << "Invalid hit point from: " << tempRay;
}
