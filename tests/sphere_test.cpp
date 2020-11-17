#include "obj/objects.h"
#include "gtest/gtest.h"

using core::Ray;
using core::RayHitData;
using core::Vec3;
using objects::Sphere;

const float kSkipFreq = 1000;
const Vec3 kVecZero(0, 0, 0);

class SphereCollisionTest : public ::testing::Test {
protected:
  [[nodiscard]] bool isVecInsideSphere(const Vec3 &vec, const Sphere &sphere) {
    return (vec - sphere.getOrigin()).magnitude() < sphere.getRadius();
  }
};

TEST_F(SphereCollisionTest, RayHitFromOutsideSphere) {
  Sphere sphere(kVecZero, 1);
  RayHitData hitData;

  Ray alongYAxis(Vec3(0, -4, 0), Vec3(0, 1, 0));
  ASSERT_TRUE(sphere.hitObject(alongYAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(alongYAxis.origin().magnitude() - sphere.getRadius(),
                  hitData.time);

  Ray alongYAxisOppposite(Vec3(0, 4, 0), Vec3(0, 1, 0));
  ASSERT_FALSE(sphere.hitObject(alongYAxisOppposite, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();

  Vec3 arbitraryVec3(12.123, 5.623, 13.235);
  Ray arbitraryOriginRay(arbitraryVec3, sphere.getOrigin() - arbitraryVec3);
  ASSERT_TRUE(sphere.hitObject(arbitraryOriginRay, kSkipFreq, &hitData));
  ASSERT_NEAR((arbitraryVec3 - sphere.getOrigin()).magnitude() -
                  sphere.getRadius(),
              hitData.time, constants::kAccuracy);

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

TEST_F(SphereCollisionTest, RayHitInsideSphere) {
  Sphere sphere(kVecZero, 1);
  RayHitData hitData;

  Ray alongXAxis(sphere.getOrigin(), Vec3(1, 0, 0));
  ASSERT_TRUE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(sphere.getRadius(), hitData.time);

  // TODO: figure out how to test time in this case
  Vec3 arbitraryChosenOriginInsideSphere(0.213, 0.523, 0.123);
  ASSERT_TRUE(isVecInsideSphere(arbitraryChosenOriginInsideSphere, sphere));
  Ray arbitraryRayInsideSphere(arbitraryChosenOriginInsideSphere,
                               Vec3(214.2345, 2345, 2235.456));
  ASSERT_TRUE(sphere.hitObject(arbitraryRayInsideSphere, kSkipFreq, &hitData));

  Vec3 closeToEdgeOrigin(0, 0, sphere.getRadius() - constants::kAccuracy);
  ASSERT_TRUE(isVecInsideSphere(closeToEdgeOrigin, sphere));
  Ray closeToEdge(closeToEdgeOrigin, Vec3(0, 0, 1));
  ASSERT_TRUE(sphere.hitObject(closeToEdge, kSkipFreq, &hitData));
  ASSERT_NEAR(constants::kAccuracy, hitData.time, constants::kAccuracy);
}

TEST_F(SphereCollisionTest, RayAtEdgeOfSphereDontHit) {
  Sphere sphere(kVecZero, 1);
  RayHitData hitData;

  Ray alongXAxis(Vec3(1, 0, 0), Vec3(1, 0, 0));
  ASSERT_FALSE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();
}

TEST_F(SphereCollisionTest, RayMissSphere) {
  Sphere sphere(Vec3(15, 0, 0), 1);
  RayHitData hitData;

  Ray nextToSphere(Vec3(14 - constants::kAccuracy, 0, -5), Vec3(0, 0, 1));
  ASSERT_FALSE(sphere.hitObject(nextToSphere, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();

  Ray nextToSphereHit(Vec3(14, 0, -5), Vec3(0, 0, 1));
  ASSERT_TRUE(sphere.hitObject(nextToSphereHit, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(5, hitData.time);
}