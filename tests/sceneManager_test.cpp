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

generators::FakeRandomGen fakeRand;

class SceneManagerTest : public ::testing::Test
{
protected:
    bool performHitAtEnergyCollectors(const Ray &straightUp, RayHitData *hitData)
    {
        for (objects::EnergyCollector *energyCol : manager.getEnergyCollectors())
        {
            const double kSkipFreq = 1000;
            if (energyCol->hitObject(straightUp, kSkipFreq, hitData))
            {
                return true;
            }
        }
        return false;
    }

    SceneManager manager;
};

TEST_F(SceneManagerTest, EnergyCollectorPositionsCheck)
{
    for (const auto &collectorPtr : manager.getEnergyCollectors())
    {
        std::cout << *collectorPtr << std::endl;
    }

    ASSERT_EQ(manager.getEnergyCollectors().size(), constants::kPopulation) << "wrong numebr of energy collectors";

    RayHitData hitData;
    Ray straightUp(core::Vec3(0, 0, 0), core::Vec3(0, 0, 1));

    ASSERT_TRUE(performHitAtEnergyCollectors(straightUp, &hitData)) << "no hit: " << straightUp;

    core::Vec3 referenceCollisionPoint(0, 0, manager.simulatiorRadius() / 2 - manager.collectorRadius());
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint) << "Invalid hit point from: " << straightUp;

    Ray againstYAxis(core::Vec3(0, 0, 0), core::Vec3(-1, 0, 0));
    ASSERT_TRUE(performHitAtEnergyCollectors(againstYAxis, &hitData)) << "no hit: " << againstYAxis;

    Vec3 referenceCollisionPoint2(-(manager.simulatiorRadius() / 2 - manager.collectorRadius()), 0, 0);
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint2) << "Invalid hit point from: " << againstYAxis;

    Ray againstXAxis(core::Vec3(0, 0, 0), core::Vec3(0, 1, 0));
    ASSERT_TRUE(performHitAtEnergyCollectors(againstXAxis, &hitData)) << "no hit: " << againstXAxis;

    Vec3 referenceCollisionPoint3(0, manager.simulatiorRadius() / 2 - manager.collectorRadius(), 0);
    ASSERT_EQ(hitData.collisionPoint, referenceCollisionPoint3) << "Invalid hit point from: " << againstXAxis;
}
