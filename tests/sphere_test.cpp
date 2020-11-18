#include "obj/objects.h"
#include "gtest/gtest.h"

#include <random>

using namespace core;
using objects::Sphere;

const float kSkipFreq = 1000;

class RandomFloatGenerator {
public:
  RandomFloatGenerator(float min, float max)
      : engine_(std::random_device()()), dist_(min, max){};
  virtual ~RandomFloatGenerator(){};
  virtual float getFloat() { return dist_(engine_); }

protected:
  std::mt19937_64 engine_;
  std::normal_distribution<float> dist_;
};

class SphereCollisionTest : public ::testing::Test {
public:
  SphereCollisionTest() : gen_(-1, 1){};

protected:
  [[nodiscard]] bool isVecInsideSphere(const Vec3 &vec, const Sphere &sphere) {
    return (vec - sphere.getOrigin()).magnitude() < sphere.getRadius();
  }
  Vec3 getRandomVec() {
    return Vec3(gen_.getFloat(), gen_.getFloat(), gen_.getFloat());
  }
  RandomFloatGenerator gen_;
};

TEST_F(SphereCollisionTest, RayHitFromOutsideSphere) {
  Sphere sphere(kVecZero, 1);
  RayHitData hitData;

  Ray alongYAxis(Vec3(0, -4, 0), kVecY);
  ASSERT_TRUE(sphere.hitObject(alongYAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(alongYAxis.origin().magnitude() - sphere.getRadius(),
                  hitData.time);

  Ray alongYAxisOppposite(Vec3(0, 4, 0), kVecY);
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

  Ray alongXOneHit(Vec3(-5, 0, 1), kVecX);
  ASSERT_TRUE(sphere.hitObject(alongXOneHit, kSkipFreq, &hitData));
  ASSERT_EQ(5, hitData.time);
  ASSERT_EQ(Vec3(0, 0, 1), hitData.collisionPoint());
}

TEST_F(SphereCollisionTest, RayHitInsideSphere) {
  Sphere sphere(kVecZero, 1);
  RayHitData hitData;

  Ray alongXAxis(sphere.getOrigin(), kVecX);
  ASSERT_TRUE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(sphere.getRadius(), hitData.time);

  Vec3 arbitraryChosenOriginInsideSphere(0.213, 0.523, 0.123);
  ASSERT_TRUE(isVecInsideSphere(arbitraryChosenOriginInsideSphere, sphere));
  Ray arbitraryRayInsideSphere(arbitraryChosenOriginInsideSphere,
                               Vec3(214.2345, 2345, 2235.456));
  ASSERT_TRUE(sphere.hitObject(arbitraryRayInsideSphere, kSkipFreq, &hitData));

  Vec3 closeToEdgeOrigin(0, 0, sphere.getRadius() - constants::kAccuracy);
  ASSERT_TRUE(isVecInsideSphere(closeToEdgeOrigin, sphere));
  Ray closeToEdge(closeToEdgeOrigin, kVecUp);
  ASSERT_TRUE(sphere.hitObject(closeToEdge, kSkipFreq, &hitData));
  ASSERT_NEAR(constants::kAccuracy, hitData.time, constants::kAccuracy);

  Vec3 randomInsideSphere = getRandomVec().normalize() * 0.99;
  ASSERT_TRUE(isVecInsideSphere(randomInsideSphere, sphere));
  Ray randomInside(randomInsideSphere, getRandomVec());
  ASSERT_TRUE(sphere.hitObject(randomInside, kSkipFreq, &hitData));
}

TEST_F(SphereCollisionTest, RayAtEdgeOfSphereDontHit) {
  Sphere sphere(kVecZero, 1);
  RayHitData hitData;

  Ray alongXAxis(Vec3(1, 0, 0), kVecX);
  ASSERT_FALSE(sphere.hitObject(alongXAxis, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();

  Ray alongXAxisHit(Vec3(1 - constants::kAccuracy, 0, 0), kVecX);
  ASSERT_TRUE(sphere.hitObject(alongXAxisHit, kSkipFreq, &hitData));
  ASSERT_NEAR(constants::kAccuracy, hitData.time, constants::kAccuracy);
}

TEST_F(SphereCollisionTest, RayMissSphere) {
  Sphere sphere(Vec3(15, 0, 0), 1);
  RayHitData hitData;

  Ray nextToSphere(Vec3(14 - constants::kAccuracy, 0, -5), kVecUp);
  ASSERT_FALSE(sphere.hitObject(nextToSphere, kSkipFreq, &hitData))
      << "hit at: " << hitData.collisionPoint();

  Ray nextToSphereHit(Vec3(14, 0, -5), kVecUp);
  ASSERT_TRUE(sphere.hitObject(nextToSphereHit, kSkipFreq, &hitData));
  ASSERT_FLOAT_EQ(5, hitData.time);
}