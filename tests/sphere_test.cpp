#include "obj/objects.h"
#include "gtest/gtest.h"

using core::Ray;
using core::RayHitData;
using core::Vec3;
using objects::Sphere;

const float kSkipFreq = 1000;
const Vec3 kVecZero(0, 0, 0);

TEST(SphereCollisionTest, RayHitFromOutsideSphere) {
  Sphere sphere(kVecZero, 1);
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

  Vec3 randomChosenVec3(12.123, 5.623, 13.235);
  Ray randomChosenOriginRay(randomChosenVec3,
                            sphere.getOrigin() - randomChosenVec3);
  ASSERT_TRUE(sphere.hitObject(randomChosenOriginRay, kSkipFreq, &hitData));
  ASSERT_NEAR(randomChosenOriginRay.origin().magnitude() - sphere.getRadius(),
              hitData.time, constants::kAccuracy);

  Ray randomChosenOriginRayNoHit(randomChosenVec3,
                                 randomChosenVec3 - sphere.getOrigin());
  ASSERT_FALSE(
      sphere.hitObject(randomChosenOriginRayNoHit, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();

  // This comes from proportions of the pitagorus triangle where sides satisfy
  // the equation : 3^2 + 4^2 = 5^5
  Vec3 originFromAngle(0, -3, -4);
  Ray rayToSphereOriginFromAngle(originFromAngle,
                                 sphere.getOrigin() - originFromAngle);
  ASSERT_TRUE(
      sphere.hitObject(rayToSphereOriginFromAngle, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(5 - sphere.getRadius(), hitData.time);

  Ray alongXOneHit(Vec3(-5, 0, 1), Vec3(1, 0, 0));
  ASSERT_TRUE(sphere.hitObject(alongXOneHit, kSkipFreq, &hitData));
  ASSERT_EQ(5, hitData.time);
  ASSERT_EQ(Vec3(0, 0, 1), hitData.collisionPoint());
}

TEST(SphereCollisionTest, RayHitInsideSphere) {
  Sphere sphere(kVecZero, 1);
  RayHitData hitData;

  Ray alongXAxis(kVecZero, Vec3(1, 0, 0));
  ASSERT_TRUE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(sphere.getRadius(), hitData.time);

  Ray alongYAxis(kVecZero, Vec3(0, 1, 0));
  ASSERT_TRUE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(sphere.getRadius(), hitData.time);

  Ray alongZAxis(kVecZero, Vec3(0, 0, 1));
  ASSERT_TRUE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(sphere.getRadius(), hitData.time);

  // TODO: figure out how to test time in this case
  Vec3 randomChosenOriginInsideSphere = Vec3(0.213, 0.523, 0.123);
  Ray randomRayInsideSphere(randomChosenOriginInsideSphere,
                            Vec3(214.2345, 2345, 2235.456));
  ASSERT_TRUE(sphere.hitObject(randomRayInsideSphere, kSkipFreq, &hitData));
}

TEST(SphereCollisionTest, RayAtEdgeOfSphereDontHit) {
  Sphere sphere(kVecZero, 1);
  RayHitData hitData;

  Ray alongXAxis(Vec3(1, 0, 0), Vec3(1, 0, 0));
  ASSERT_FALSE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();

  Ray alongYAxis(Vec3(0, 1, 0), Vec3(0, 1, 0));
  ASSERT_FALSE(sphere.hitObject(alongYAxis, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();

  Ray alongZAxis(Vec3(0, 0, 1), Vec3(0, 0, 1));
  ASSERT_FALSE(sphere.hitObject(alongZAxis, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();
}

TEST(SphereCollisionTest, RayMissSphere) {
  Sphere sphere(Vec3(15, 0, 0), 1);
  RayHitData hitData;

  Ray nextToSphere(Vec3(14 - constants::kAccuracy, 0, -5), Vec3(0, 0, 1));
  ASSERT_FALSE(sphere.hitObject(nextToSphere, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();
}