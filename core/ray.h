#ifndef RAY_H
#define RAY_H

#include "constants.h"
#include "core/vec3.h"

#include "classUtlilities.h"
#include <iostream>
#include <limits>
#include <sstream>

namespace core {
class Ray : public Printable {
public:
  // Returns Ray from |origin| and direction, which is calculated from given
  // spherical coordinates, where |zRotation| represents rotation about z-axis
  // and |xyInclination| represents inclination from the surface, that crosses x
  // and y axis. Both |zRotation| and |xyInclination| value must be in radians.
  static Ray fromSphericalCoords(const Vec3 &origin, float zRotation,
                                 float xyInclination);

  explicit Ray(const Vec3 &origin = Vec3::kZero,
               const Vec3 &direction = Vec3::kZ, float energy = 0,
               float accumulatedTime = 0);

  Vec3 at(float time) const;
  static float phaseAt(float freq, float time);

  bool operator==(const Ray &other) const;

  void setOrigin(const Vec3 &origin);
  Vec3 origin() const;
  void setDirection(const Vec3 &direction);
  Vec3 direction() const;
  void setEnergy(float num);
  float energy() const;
  float accumulatedTime() const { return accumulatedTime_; }
  void printItself(std::ostream &os) const noexcept override;

private:
  Vec3 origin_, direction_;
  float energy_, accumulatedTime_;
};

struct RayHitData : public Printable {
  explicit RayHitData(float t = std::numeric_limits<float>::max(),
                      const Vec3 &norm = Vec3::kZ,
                      const Ray &ray = Ray(Vec3::kZero, Vec3::kZ),
                      float freq = 1000, float accumulatedTime = 0)
      : time(t), frequency(freq), accumulatedTime(accumulatedTime), ray_(ray),
        normal_(norm){};

  bool operator==(const RayHitData &other) const;
  void printItself(std::ostream &os) const noexcept override;

  Vec3 normal() const { return normal_; }
  Vec3 collisionPoint() const { return ray_.at(time); }
  Vec3 direction() const { return ray_.direction(); }
  Vec3 origin() const { return ray_.origin(); }
  float energy() const { return ray_.energy(); }
  float phase() const { return Ray::phaseAt(frequency, accumulatedTime); }
  float time, frequency, accumulatedTime;

private:
  Ray ray_;
  Vec3 normal_;
};
} // namespace core

#endif
