#include "obj/objects.h"
#include "gtest/gtest.h"

using core::Ray;
using core::RayHitData;
using core::Vec3;
using objects::Sphere;

const float kSkipFreq = 1000;

TEST(SphereCollisionTest, RayHitFromOutsideSphere) {

  Sphere sphere(Vec3(0, 0, 0), 1);
  RayHitData hitData;
  Ray alongYAxis(Vec3(0, -4, 0), Vec3(0, 1, 0));
  ASSERT_TRUE(sphere.hitObject(alongYAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(alongYAxis.origin().magnitude() - sphere.getRadius(),
                  hitData.time);

  Ray alongYAxisNoHit(Vec3(0, 4, 0), Vec3(0, 1, 0));
  ASSERT_FALSE(sphere.hitObject(alongYAxisNoHit, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();

  Ray alongXAxis(Vec3(-4, 0, 0), Vec3(1, 0, 0));
  ASSERT_TRUE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(alongXAxis.origin().magnitude() - sphere.getRadius(),
                  hitData.time);

  Ray alongXAxisNoHit(Vec3(4, 0, 0), Vec3(1, 0, 0));
  ASSERT_FALSE(sphere.hitObject(alongYAxisNoHit, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();

  Ray alongZAxis(Vec3(0, 0, -4), Vec3(0, 0, 1));
  ASSERT_TRUE(sphere.hitObject(alongZAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(alongZAxis.origin().magnitude() - sphere.getRadius(),
                  hitData.time);

  Ray alongZAxisNoHit(Vec3(0, 0, 4), Vec3(0, 0, 1));
  ASSERT_FALSE(sphere.hitObject(alongZAxisNoHit, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();

  Vec3 randomChosenVec3(42.123, 345.623, 234.235);
  Ray randomChosenOriginRay(randomChosenVec3,
                            sphere.getOrigin() - randomChosenVec3);
  // ASSERT_TRUE(sphere.hitObject(randomChosenOriginRay, kSkipFreq, &hitData));
  // ASSERT_FLOAT_EQ(randomChosenOriginRay.origin().magnitude() -
  // sphere.getRadius(), hitData.time);

  Ray randomChosenOriginRayNoHit(randomChosenVec3,
                                 randomChosenVec3 - sphere.getOrigin());
  ASSERT_FALSE(
      sphere.hitObject(randomChosenOriginRayNoHit, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();
}