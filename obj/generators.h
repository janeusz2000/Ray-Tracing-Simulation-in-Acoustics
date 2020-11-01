#ifndef GENERATORS_H
#define GENERATORS_H

#include "constants.h"
#include "core/ray.h"
#include "core/vec3.h"

#include <cmath>
#include <initializer_list>
#include <iostream>
#include <random>

namespace generators {
struct RandomGen {
  virtual ~RandomGen(){};
  virtual float getNext() = 0;
};

struct UniformRandomGen : public RandomGen {
  UniformRandomGen(float min, float max)
      : engine_(std::random_device()()), dist_(min, max){};
  float getNext() override { return dist_(engine_); }

  std::mt19937_64 engine_;
  std::uniform_real_distribution<float> dist_;
};

struct GaussianRandomGen : public RandomGen {
  GaussianRandomGen(float mean, float standardDeviation)
      : engine_(std::random_device()()), dist_(mean, standardDeviation){};
  float getNext() override { return dist_(engine_); }

  std::mt19937_64 engine_;
  std::normal_distribution<float> dist_;
};

struct FakeRandomGen : public RandomGen {
  float getNext() override { return 0; }
};

// This object is generating rays with the origin
// of the point source position and direction calculated
// from leftCorner position + x * Vec3(1, 0, 0) + y * Vec3(0, 1, 0)
// and origin of the source position
class PointSource final {
public:
  PointSource() = default;
  explicit PointSource(
      float freq, int numOfRaysPerRow, float SampleSize, RandomGen *randomGen,
      core::Vec3 origin = core::Vec3(0, 0, constants::kDefaultSimulationRadius))
      : frequency_(freq), numOfRaysPerRow_(numOfRaysPerRow),
        sampleSize_(SampleSize), origin_(origin), randomGen_(randomGen) {
    updateSampleSize();
  }

  bool operator==(const PointSource &other) const;
  friend std::ostream &operator<<(std::ostream &os,
                                  const PointSource &pointSource);

  void updateSampleSize();
  core::Ray generateRay(int xIter, int yIter);

  float frequency() const;
  void setFrequency(float freq);

  float sampleSize() const;
  void setDiffusorSize(float diffusorSize);

  size_t numOfRaysPerRow() const;
  void setNumOfRaysPerRow(int raynum);

  core::Vec3 origin() const;
  void setOrigin(const core::Vec3 &point);

  core::Vec3 getLeftCorner() const;
  void setLeftCorner(const core::Vec3 &point);

private:
  core::Vec3 origin_, leftCorner_;
  float frequency_, sampleSize_;
  size_t numOfRaysPerRow_;

  RandomGen *randomGen_;
};

} // namespace generators

#endif
