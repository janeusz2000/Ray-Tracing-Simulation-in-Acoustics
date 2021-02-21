#ifndef OBJECTS_H
#define OBJECTS_H

#include "constants.h"
#include "core/classUtlilities.h"
#include "core/ray.h"
#include "core/vec3.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <sstream>
#include <vector>

namespace objects {

class Object : public Printable {
public:
  virtual ~Object(){};
  virtual core::Vec3 normal(const core::Vec3 &surfacePoint) const = 0;
  virtual bool hitObject(const core::Ray &ray, float freq,
                         core::RayHitData *hitData) = 0;

  void setOrigin(const core::Vec3 &origin);
  core::Vec3 getOrigin() const;
  void printItself(std::ostream &os) const noexcept override;

protected:
  core::Vec3 origin_;
};

class Sphere : public Object {
public:
  explicit Sphere(const core::Vec3 &origin, float rad = 1);

  core::Vec3 normal(const core::Vec3 &surfacePoint) const override;
  [[nodiscard]] bool hitObject(const core::Ray &ray, float freq,
                               core::RayHitData *hitData) override;
  bool isVecInside(const core::Vec3 &vec) const;
  float getRadius() const;
  void setRadius(float rad);
  void printItself(std::ostream &os) const noexcept override;

protected:
  float radius_;
};

// When this object has two purposes: first - when ray hit this object, there is
// no longer need to continue ray tracing. Second - the collision point is
// passed to energy collector, and after that specific amount of energy from ray
// is collected depends on the distance from collision point to the sphere
// collectors;
class SphereWall : public Sphere {
public:
  explicit SphereWall(float radius)
      : Sphere(core::Vec3(0, 0, 0), /*radius=*/1) {
    setRadius(radius);
  }

protected:
  void printItself(std::ostream &os) const noexcept override;
};

class EnergyCollector : public Sphere {
public:
  explicit EnergyCollector(const core::Vec3 &origin, float radius)
      : Sphere(origin, radius), energy_(0.0f) {
    setRadius(radius);
    setOrigin(origin);
  }

  bool operator==(const EnergyCollector &other) const;
  EnergyCollector &operator=(const EnergyCollector &other);

  float distanceAt(const core::Vec3 &positionHit) const;
  void collectEnergy(const core::RayHitData &hitdata);

  void setEnergy(float en);
  float getEnergy() const;
  void addEnergy(float en);
  void printItself(std::ostream &os) const noexcept override;

private:
  float energy_;
};

class TriangleObj : public Object {
public:
  TriangleObj(const core::Vec3 &point1 = core::Vec3::kX,
              const core::Vec3 &point2 = core::Vec3::kY,
              const core::Vec3 &point3 = core::Vec3::kZ);
  TriangleObj(const TriangleObj &other);

  TriangleObj &operator=(const TriangleObj &other);
  bool operator==(const TriangleObj &other) const;
  friend bool operator!=(const TriangleObj &left, const TriangleObj &right);

  core::Vec3
  normal(const core::Vec3 &surfacePoint = core::Vec3()) const override;
  [[nodiscard]] bool hitObject(const core::Ray &ray, float freq,
                               core::RayHitData *hitData) override;

  float area() const;
  void refreshAttributes();

  core::Vec3 point1() const;
  void setPoint1(const core::Vec3 &point);

  core::Vec3 point2() const;
  void setPoint2(const core::Vec3 &point);

  core::Vec3 point3() const;
  void setPoint3(const core::Vec3 &point);

  std::vector<core::Vec3> getPoints() const;
  void printItself(std::ostream &os) const noexcept override;

private:
  bool doesHit(const core::Vec3 &point) const;

  void recalculateNormal();
  void recalculateArea();
  bool arePointsValid();
  core::Vec3 normal_, point1_, point2_, point3_;
  float area_;
};

} // namespace objects

#endif