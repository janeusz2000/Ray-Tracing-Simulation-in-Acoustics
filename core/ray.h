#ifndef RAY_H
#define RAY_H

#include "constants.h"
#include "core/vec3.h"

#include <initializer_list>
#include <iostream>

namespace core {
class Ray {
public:
  Ray(const Vec3 &origin = Vec3(0, 0, 0), const Vec3 &direction = Vec3(0, 0, 1),
      float energy = 0);

  Vec3 at(float time) const;
  float phaseAt(float freq, float time) const;

  friend std::ostream &operator<<(std::ostream &os, const Ray &srcRay);
  friend bool operator==(const Ray &left, const Ray &right);

  void setOrigin(const Vec3 &origin);
  Vec3 getOrigin() const;
  void setDirection(const Vec3 &direction);
  Vec3 getDirection() const;
  void setEnergy(float num);
  float getEnergy() const;

private:
  Vec3 origin_, direction_;
  float energy_;
};

struct RayHitData {
  RayHitData() = default;
  RayHitData(float t, const Vec3 &norm, const Ray &ray, float freq)
      : time(t), ray_(ray), normal_(norm), frequency(freq){};
  ~RayHitData() = default;
  RayHitData(const RayHitData &) = default;

  bool operator==(const RayHitData &other) const;
  friend std::ostream &operator<<(std::ostream &os, const RayHitData &rayData);

  Vec3 normal() const { return normal_; }
  Vec3 collisionPoint() const { return ray_.at(time); }
  Vec3 direction() const { return ray_.getDirection(); }
  Vec3 origin() const { return ray_.getOrigin(); }
  float energy() const { return ray_.getEnergy(); }
  float phase() const { return ray_.phaseAt(frequency, time); }
  float frequency, time;

private:
  Vec3 normal_;

  Ray ray_;
};
} // namespace core

#endif