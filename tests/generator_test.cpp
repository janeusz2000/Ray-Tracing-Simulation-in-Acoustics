#include "main/model.h"
#include "obj/generators.h"
#include "obj/objects.h"
#include "gtest/gtest.h"

using core::Ray;
using core::RayHitData;
using core::Vec3;
using generators::PointSpeakerRayFactory;
using objects::TriangleObj;

const float kSkipPower = 0;

class FakeModel : public ModelInterface {
public:
  FakeModel() : triangles_({}){};
  const std::vector<TriangleObj> &triangles() const override {
    return triangles_;
  }

  float height() const override { return 1; }
  float sideSize() const override { return 1; }
  bool empty() const override { return false; }

private:
  std::vector<TriangleObj> triangles_;
};

TEST(PointSpeakerRayFactoryTest, RayGenerator) {
  FakeModel model;
  int numOfRaysPerEachAxis = 3;
  PointSpeakerRayFactory rayFactory(numOfRaysPerEachAxis, kSkipPower, &model);

  Ray current(Vec3::kZero, Vec3::kZ);
  ASSERT_TRUE(rayFactory.genRay(&current));
  Vec3 referenceDirection(0, 0, 1 - rayFactory.origin().z());
  Ray referenceLeftLowerCorner(rayFactory.origin(),
                               referenceDirection - (Vec3::kX + Vec3::kY) / 2);
  ASSERT_EQ(referenceLeftLowerCorner, current);

  ASSERT_TRUE(rayFactory.genRay(&current));
  Ray referenceLowerMiddle(rayFactory.origin(),
                           referenceDirection - Vec3::kY / 2);
  ASSERT_EQ(referenceLowerMiddle, current);

  ASSERT_TRUE(rayFactory.genRay(&current));
  Ray referenceRightLowerCorner(
      rayFactory.origin(), referenceDirection + Vec3::kX / 2 - Vec3::kY / 2);
  ASSERT_EQ(referenceRightLowerCorner, current);

  ASSERT_TRUE(rayFactory.genRay(&current));
  Ray referenceLeft(rayFactory.origin(), referenceDirection - Vec3::kX / 2);
  ASSERT_EQ(referenceLeft, current);

  ASSERT_TRUE(rayFactory.genRay(&current));
  Ray referenceMiddle(rayFactory.origin(), referenceDirection);
  ASSERT_EQ(referenceMiddle, current);

  ASSERT_TRUE(rayFactory.genRay(&current));
  Ray referenceRight(rayFactory.origin(), referenceDirection + Vec3::kX / 2);
  ASSERT_EQ(referenceRight, current);

  ASSERT_TRUE(rayFactory.genRay(&current));
  Ray referenceLeftUpperCorner(
      rayFactory.origin(), referenceDirection - Vec3::kX / 2 + Vec3::kY / 2);
  ASSERT_EQ(referenceLeftUpperCorner, current);

  ASSERT_TRUE(rayFactory.genRay(&current));
  Ray referenceUpperMiddle(rayFactory.origin(),
                           referenceDirection + Vec3::kY / 2);
  ASSERT_EQ(referenceUpperMiddle, current);

  ASSERT_TRUE(rayFactory.genRay(&current));
  Ray referenceRightUpperCorner(
      rayFactory.origin(), referenceDirection + Vec3::kX / 2 + Vec3::kY / 2);
  ASSERT_EQ(referenceRightUpperCorner, current);

  ASSERT_FALSE(rayFactory.genRay(&current));
}

TEST(PointSpeakerRayFactoryTest, energyDivisionAcrossRaysTest) {
  // FakeModel model;
  // int numOfRaysPerEachAxis = 1;
  // float power = 900;
  // PointSpeakerRayFactory singleRayFactory(numOfRaysPerEachAxis, power,
  // &model);

  // Ray skipRay(singe)
}