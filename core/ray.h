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
      double energy = 0);

  Vec3 at(double time) const;
  double phaseAt(double freq, double time) const;

  friend std::ostream &operator<<(std::ostream &os, const Ray &srcRay);
  bool operator==(const Ray &other) const;

  void setOrigin(const Vec3 &origin);
  Vec3 origin() const;
  void setDirection(const Vec3 &direction);
  Vec3 direction() const;
  void setEnergy(double num);
  double energy() const;

private:
  Vec3 origin_, direction_;
  double energy_;
};

struct RayHitData {
  RayHitData() = default;
  RayHitData(double t, const Vec3 &norm, const Ray &ray, double freq)
      : time(t), ray_(ray), normal_(norm), frequency(freq){};
  ~RayHitData() = default;
  RayHitData(const RayHitData &) = default;

  bool operator==(const RayHitData &other) const;
  friend std::ostream &operator<<(std::ostream &os, const RayHitData &rayData);

  Vec3 normal() const { return normal_; }
  Vec3 collisionPoint() const { return ray_.at(time); }
  Vec3 direction() const { return ray_.direction(); }
  Vec3 origin() const { return ray_.origin(); }
  double energy() const { return ray_.energy(); }
  double phase() const { return ray_.phaseAt(frequency, time); }
  double frequency, time;

private:
  Vec3 normal_;

  Ray ray_;
};
} // namespace core

#endif