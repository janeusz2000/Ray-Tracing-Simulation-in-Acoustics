#include "obj/generators.h"
#include "obj/objects.h"
#include "gtest/gtest.h"

#include <cmath>
#include <random>
#include <sstream>

namespace generators {
FakeRandomGen fakeEngine;

TEST(POINTSOURCE_METHOD, Single_rayHit) {
  const float freq = 1000;
  const size_t rayNumPerRow = 1000;

  PointSource source(freq, rayNumPerRow, 1, &fakeEngine,
                     constants::kDefaultSimulationRadius,
                     constants::kDefaultSourcePower);

  std::vector<std::unique_ptr<objects::TriangleObj>> objectsVec;
  objects::TriangleObj object(core::Vec3{-0.6, -0.6, 1},
                              core::Vec3{0.6, -0.6, 1},
                              core::Vec3{-0.6, 0.6, 1});

  core::Ray tempRay = source.generateRay(0, 0);
  core::RayHitData hitData;
  ASSERT_TRUE(object.hitObject(tempRay, freq, &hitData));
  std::cout << hitData << std::endl;
  ASSERT_EQ(
      core::Ray(source.origin(), hitData.direction()),
      core::Ray(core::Vec3(0, 0, 4), core::Vec3(-0.5, -0.5, -3).normalize()));
}

TEST(POINTSOURCE_METHODS, GenerateRay_Test) // Monte Carlo Test
{
  const size_t rayNumPerRow = 1000;
  const float freq = 1000;
  const size_t samples = 3;
  PointSource source(freq, rayNumPerRow, 1, &fakeEngine,
                     constants::kDefaultSimulationRadius,
                     constants::kDefaultSourcePower);

  objects::TriangleObj object(core::Vec3{-0.25, -0.25, 1},
                              core::Vec3{0.25, -0.25, 1},
                              core::Vec3{-0.25, 0.25, 1});
  float hits = 0, missed = 0;
  core::RayHitData hitData;
  for (size_t x = 0; x < rayNumPerRow; ++x) {
    for (size_t y = 0; y < rayNumPerRow; ++y) {
      core::Ray tempRay = source.generateRay(x, y);
      if (object.hitObject(tempRay, freq, &hitData)) {
        ++hits;
      } else {
        ++missed;
      }
    }
  }

  ASSERT_NEAR(hits / (hits + missed), 0.125, constants::kHitAccuracy * 10);
}

} // namespace generators
