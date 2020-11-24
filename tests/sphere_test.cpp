#include "constants.h"
#include "obj/objects.h"
#include "gtest/gtest.h"
#include <random>

using core::Ray;
using core::RayHitData;
using core::Vec3;
using objects::Sphere;

const float kSkipFreq = 1000;

TEST(SphereCollisionTest, RayHitFromOutsideSphere) {
  Sphere sphere(Vec3::kZero, 1);
  RayHitData hitData;

  ASSERT_FALSE(sphere.isVecInside(-4 * Vec3::kY));
  Ray alongYAxis(Vec3(0, -4, 0), Vec3::kY);
  ASSERT_TRUE(sphere.hitObject(alongYAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(alongYAxis.origin().magnitude() - sphere.getRadius(),
                  hitData.time);

  Vec3 behindSphereYAxis = 4 * Vec3::kY;
  ASSERT_FALSE(sphere.isVecInside(behindSphereYAxis));
  Ray alongYAxisOppposite(behindSphereYAxis, Vec3::kY);
  ASSERT_FALSE(sphere.hitObject(alongYAxisOppposite, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();

  Vec3 arbitraryVec3(12.123, 5.623, 13.235);
  ASSERT_FALSE(sphere.isVecInside(arbitraryVec3));
  Ray arbitraryOriginRay(arbitraryVec3, sphere.getOrigin() - arbitraryVec3);
  ASSERT_TRUE(sphere.hitObject(arbitraryOriginRay, kSkipFreq, &hitData));
  ASSERT_NEAR((arbitraryVec3 - sphere.getOrigin()).magnitude() -
                  sphere.getRadius(),
              hitData.time, constants::kAccuracy);

  Vec3 alongXOneHitOrigin = Vec3(-5, 0, 1);
  ASSERT_FALSE(sphere.isVecInside(alongXOneHitOrigin));
  Ray alongXOneHit(alongXOneHitOrigin, Vec3::kX);
  ASSERT_TRUE(sphere.hitObject(alongXOneHit, kSkipFreq, &hitData));
  ASSERT_EQ(5, hitData.time);
  ASSERT_EQ(Vec3(0, 0, 1), hitData.collisionPoint());

  Vec3 alongXOneHitOriginMiss = Vec3(-5, 0, 1 + constants::kAccuracy);
  ASSERT_FALSE(sphere.isVecInside(alongXOneHitOriginMiss));
  Ray alongXOneHitMiss(alongXOneHitOriginMiss, Vec3::kX);
  ASSERT_FALSE(sphere.hitObject(alongXOneHitMiss, kSkipFreq, &hitData));
  ASSERT_EQ(std::abs(alongXOneHitOriginMiss.x()), hitData.time);
  ASSERT_EQ(Vec3(0, 0, 1), hitData.collisionPoint());
}

TEST(SphereCollisionTest, RayHitInsideSphere) {
  Sphere sphere(Vec3::kZero, 1);
  RayHitData hitData;

  Ray alongXAxis(Vec3::kZero, Vec3::kX);
  ASSERT_TRUE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(sphere.getRadius(), hitData.time);

  Vec3 arbitraryChosenOriginInsideSphere(0.213, 0.523, 0.123);
  ASSERT_TRUE(sphere.isVecInside(arbitraryChosenOriginInsideSphere));
  Ray arbitraryRayInsideSphere(arbitraryChosenOriginInsideSphere,
                               Vec3(214.2345, 2345, 2235.456));
  ASSERT_TRUE(sphere.hitObject(arbitraryRayInsideSphere, kSkipFreq, &hitData));

  Vec3 arbitraryInsideSphere = Vec3(12, 0.345, 3.457).normalize() * 0.99;
  ASSERT_TRUE(sphere.isVecInside(arbitraryInsideSphere));
  Ray randomInside(arbitraryInsideSphere, Vec3::kY);
  ASSERT_TRUE(sphere.hitObject(randomInside, kSkipFreq, &hitData));
}

TEST(SphereCollisionTest, RayAtEdgeOfSphere) {
  Sphere sphere(Vec3::kZero, 1);
  RayHitData hitData;

  Ray alongXAxis(Vec3::kX, Vec3::kX);
  ASSERT_FALSE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();
  Ray alongXAxisHit(Vec3(1 - constants::kAccuracy, 0, 0), Vec3::kX);
  ASSERT_TRUE(sphere.hitObject(alongXAxisHit, kSkipFreq, &hitData));
  ASSERT_NEAR(constants::kAccuracy, hitData.time, constants::kAccuracy / 2);

  // Prevents of getting ray inside to the sphere by
  // floating point number error
  Ray toOriginOfSphere(Vec3::kX, -Vec3::kX);
  ASSERT_FALSE(sphere.hitObject(toOriginOfSphere, kSkipFreq, &hitData));

  Ray parpendicularToRadius(Vec3::kY, Vec3::kX);
  ASSERT_FALSE(sphere.hitObject(parpendicularToRadius, kSkipFreq, &hitData));
}