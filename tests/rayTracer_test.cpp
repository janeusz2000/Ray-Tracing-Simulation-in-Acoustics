#include "core/ray.h"
#include "core/vec3.h"
#include "main/model.h"
#include "main/rayTracer.h"
#include "obj/objects.h"
#include "gtest/gtest.h"

#include <fstream>
#include <string_view>

using core::Ray;
using core::RayHitData;
using core::Vec3;
using objects::TriangleObj;

const float kSkipFrequency = 1000;

class FakeReferenceModel : public ModelInterface {
public:
  FakeReferenceModel() {
    triangles_.push_back(TriangleObj(Vec3::kX, Vec3::kZero, Vec3::kZ));
    triangles_.push_back(TriangleObj(Vec3::kX, Vec3(1, 0, 1), Vec3::kZ));
  }

  const std::vector<TriangleObj> &triangles() const override {
    return triangles_;
  }

  float height() const override { return 0; }
  float sideSize() const override { return 0; }
  bool empty() const { return triangles_.empty(); }

private:
  std::vector<TriangleObj> triangles_;
  void printItself(std::ostream &os) const noexcept override {
    os << "Fake Reference Model";
  }
};

class RayTracerTest : public ::testing::Test {
protected:
  [[nodiscard]] bool saveReachedPositions(const core::RayHitData &hitData,
                                          std::string_view path) {
    std::ofstream outFile(path.data(), std::ios::app);
    if (!outFile.good()) {
      return false;
    }
    outFile << "from: " << hitData.origin()
            << " to: " << hitData.collisionPoint() << "\n";
    outFile.close();
    return true;
  }
};

TEST_F(RayTracerTest, RayReflection) {
  FakeReferenceModel model;
  RayTracer rayTracer(&model);
  RayHitData hitData;

  Vec3 inFrontOfModel = Vec3::kX / 3 + Vec3::kZ / 3 - 5 * Vec3::kY;
  Ray alongX(inFrontOfModel, Vec3::kY);
  ASSERT_EQ(RayTracer::TraceResult::HIT_TRIANGLE,
            rayTracer.rayTrace(alongX, kSkipFrequency, &hitData));
  Ray reflectedRay(inFrontOfModel + 5 * Vec3::kY, -Vec3::kY);
  ASSERT_EQ(reflectedRay, rayTracer.getReflected(&hitData));
}