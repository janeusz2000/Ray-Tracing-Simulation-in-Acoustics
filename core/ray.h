#ifndef RAY_H
#define RAY_H

#include "constants.h"
#include "core/vec3.h"

#include <initializer_list>
#include <iostream>
#include <limits>

namespace core {
class Ray {
public:
  // Returns Ray from |origin| and direction calculated from given spherical
  // coordinates, where |psiRotation| represents rotation on z-axis and |phi|
  // represents rotation on y-axis. Both |psi| and |phi| must be in radians.
  static Ray makeRayFromSphericalCoords(const Vec3 &origin, const float psi,
                                        const float phi);

  Ray(const Vec3 &origin = Vec3::kVecZero, const Vec3 &direction = Vec3::kVecZ,
      float energy = 0);

  Vec3 at(float time) const;
  float phaseAt(float freq, float time) const;

  friend std::ostream &operator<<(std::ostream &os, const Ray &srcRay);
  bool operator==(const Ray &other) const;

  void setOrigin(const Vec3 &origin);
  Vec3 origin() const;
  void setDirection(const Vec3 &direction);
  Vec3 direction() const;
  void setEnergy(float num);
  float energy() const;

private:
  Vec3 origin_, direction_;
  float energy_;
};

struct RayHitData {
  RayHitData()
      : RayHitData(std::numeric_limits<float>::max(), Vec3::kVecZ,
                   Ray(Vec3::kVecZero, Vec3::kVecZ), 1000){};
  RayHitData(float t, const Vec3 &norm, const Ray &ray, float freq)
      : time(t), ray_(ray), normal_(norm), frequency(freq){};
  ~RayHitData() = default;
  RayHitData(const RayHitData &) = default;

  bool operator==(const RayHitData &other) const;
  friend std::ostream &operator<<(std::ostream &os, const RayHitData &rayData);

  Vec3 normal() const { return normal_; }
  Vec3 collisionPoint() const { return ray_.at(time); }
  Vec3 direction() const { return ray_.direction(); }
  Vec3 origin() const { return ray_.origin(); }
  float energy() const { return ray_.energy(); }
  float phase() const { return ray_.phaseAt(frequency, time); }
  float frequency, time;

private:
  Vec3 normal_;

  Ray ray_;
};
} // namespace core

#endif