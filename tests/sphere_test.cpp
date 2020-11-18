#include "obj/objects.h"
#include "gtest/gtest.h"

#include <random>

using namespace core;
using objects::Sphere;

const float kSkipFreq = 1000;

class RandomFloatGenerator {
public:
  RandomFloatGenerator(float mean, float standardDeviation)
      : engine_(static_cast<std::mt19937_64::result_type>(std::time(nullptr))),
        dist_(mean, standardDeviation){};
  virtual ~RandomFloatGenerator(){};
  virtual float getFloat() { return dist_(engine_); }

  Vec3 getRandomVec() { return Vec3(getFloat(), getFloat(), getFloat()); }

protected:
  std::mt19937_64 engine_;
  std::normal_distribution<float> dist_;
};

class SphereCollisionTest : public ::testing::Test {
public:
  SphereCollisionTest() : generator(0, 1){};

protected:
  RandomFloatGenerator generator;
};

TEST_F(SphereCollisionTest, RayHitFromOutsideSphere) {
  Sphere sphere(kVecZero, 1);
  RayHitData hitData;

  Vec3 inFrontOfSphereYAxis = -4 * kVecY;
  ASSERT_FALSE(sphere.isVecInside(inFrontOfSphereYAxis));
  Ray alongYAxis(Vec3(0, -4, 0), kVecY);
  ASSERT_TRUE(sphere.hitObject(alongYAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(alongYAxis.origin().magnitude() - sphere.getRadius(),
                  hitData.time);

  Vec3 behindSphereYAxis = 4 * kVecY;
  ASSERT_FALSE(sphere.isVecInside(behindSphereYAxis));
  Ray alongYAxisOppposite(behindSphereYAxis, kVecY);
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
  Ray alongXOneHit(alongXOneHitOrigin, kVecX);
  ASSERT_TRUE(sphere.hitObject(alongXOneHit, kSkipFreq, &hitData));
  ASSERT_EQ(5, hitData.time);
  ASSERT_EQ(Vec3(0, 0, 1), hitData.collisionPoint());

  Vec3 alongXOneHitOriginMiss = Vec3(-5, 0, 1 + constants::kAccuracy);
  ASSERT_FALSE(sphere.isVecInside(alongXOneHitOriginMiss));
  Ray alongXOneHitMiss(alongXOneHitOriginMiss, kVecX);
  ASSERT_FALSE(sphere.hitObject(alongXOneHitMiss, kSkipFreq, &hitData));
  ASSERT_EQ(5, hitData.time);
  ASSERT_EQ(Vec3(0, 0, 1), hitData.collisionPoint());
}

TEST_F(SphereCollisionTest, RayHitInsideSphere) {
  Sphere sphere(kVecZero, 1);
  RayHitData hitData;

  Ray alongXAxis(kVecZero, kVecX);
  ASSERT_TRUE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(sphere.getRadius(), hitData.time);

  Vec3 arbitraryChosenOriginInsideSphere(0.213, 0.523, 0.123);
  ASSERT_TRUE(sphere.isVecInside(arbitraryChosenOriginInsideSphere));
  Ray arbitraryRayInsideSphere(arbitraryChosenOriginInsideSphere,
                               Vec3(214.2345, 2345, 2235.456));
  ASSERT_TRUE(sphere.hitObject(arbitraryRayInsideSphere, kSkipFreq, &hitData));

  Vec3 closeToEdgeOrigin(0, 0, sphere.getRadius() - constants::kAccuracy);
  ASSERT_TRUE(sphere.isVecInside(closeToEdgeOrigin));
  Ray closeToEdge(closeToEdgeOrigin, kVecUp);
  ASSERT_TRUE(sphere.hitObject(closeToEdge, kSkipFreq, &hitData));
  ASSERT_NEAR(constants::kAccuracy, hitData.time, constants::kAccuracy);

  Vec3 randomInsideSphere = generator.getRandomVec().normalize() * 0.99;
  ASSERT_TRUE(sphere.isVecInside(randomInsideSphere));
  Ray randomInside(randomInsideSphere, generator.getRandomVec());
  ASSERT_TRUE(sphere.hitObject(randomInside, kSkipFreq, &hitData));
}

TEST_F(SphereCollisionTest, RayAtEdgeOfSphereDontHit) {
  Sphere sphere(kVecZero, 1);
  RayHitData hitData;

  Ray alongXAxis(kVecX, kVecX);
  ASSERT_FALSE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();

  Ray alongXAxisHit(Vec3(1 - constants::kAccuracy, 0, 0), kVecX);
  ASSERT_TRUE(sphere.hitObject(alongXAxisHit, kSkipFreq, &hitData));
  ASSERT_NEAR(constants::kAccuracy, hitData.time, constants::kAccuracy);
}