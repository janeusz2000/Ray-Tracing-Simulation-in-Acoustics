#ifndef OBJECTS_H
#define OBJECTS_H

#include "constants.h"
#include "core/ray.h"
#include "core/vec3.h"

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <memory>
#include <vector>

namespace objects {

class Object {
public:
  virtual ~Object(){};
  virtual core::Vec3 normal(const core::Vec3 &surfacePoint) const = 0;
  virtual bool hitObject(const core::Ray &ray, double freq,
                         core::RayHitData *hitData) = 0;

  void setOrigin(const core::Vec3 &origin);
  core::Vec3 getOrigin() const;

protected:
  core::Vec3 origin_;
};

class Sphere : public Object {
public:
  explicit Sphere(const core::Vec3 &origin, double rad = 1);

  friend std::ostream &operator<<(std::ostream &os, const Sphere &sp);

  core::Vec3 normal(const core::Vec3 &surfacePoint) const override;
  [[nodiscard]] bool hitObject(const core::Ray &ray, double freq,
                               core::RayHitData *hitData) override;

  double getRadius() const;
  void setRadius(double rad);

protected:
  double radius_;
};

// When this object has two purposes: first - when ray hit this object, there is
// no longer need
// to continue ray traceing. Second - the collision point is passed to energy
// collector, and after that specific
// amount of energy from ray is collected depends on the distance from collision
// point to the sphere collectors;
class SphereWall : public Sphere {
public:
  explicit SphereWall(double radius)
      : Sphere(core::Vec3(0, 0, 0), /*radius=*/1) {
    setRadius(radius);
  }
  SphereWall(const SphereWall &other) = default;

  friend std::ostream &operator<<(std::ostream &os, const SphereWall &sp);
};

class EnergyCollector : public Sphere {
public:
  explicit EnergyCollector(const core::Vec3 &origin, double radius)
      : Sphere(origin, radius), energy_(0.0f) {
    setRadius(radius);
    setOrigin(origin);
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const EnergyCollector &collector);
  bool operator==(const EnergyCollector &other) const;
  EnergyCollector &operator=(const EnergyCollector &other);

  double distanceAt(const core::Vec3 &positionHit) const;
  void collectEnergy(const core::RayHitData &hitdata);

  void setEnergy(double en);
  double getEnergy() const;
  void addEnergy(double en);
  void setID(int id);
  int getID() const;

private:
  double energy_;
};

class TriangleObj : public Object {
public:
  TriangleObj(const core::Vec3 &point1 = core::Vec3{1, 0, 0},
              const core::Vec3 &point2 = core::Vec3{0, 1, 0},
              const core::Vec3 &point3 = core::Vec3{0, 0, 1});
  TriangleObj(const TriangleObj &other);

  TriangleObj &operator=(const TriangleObj &other);
  bool operator==(const TriangleObj &other) const;
  friend bool operator!=(const TriangleObj &left, const TriangleObj &right);
  friend std::ostream &operator<<(std::ostream &os, const TriangleObj &object);

  core::Vec3
  normal(const core::Vec3 &surfacePoint = core::Vec3()) const override;
  [[nodiscard]] bool hitObject(const core::Ray &ray, double freq,
                               core::RayHitData *hitData) override;

  double area() const;
  void refreshAttributes();

  core::Vec3 point1() const;
  void setPoint1(const core::Vec3 &point);

  core::Vec3 point2() const;
  void setPoint2(const core::Vec3 &point);

  core::Vec3 point3() const;
  void setPoint3(const core::Vec3 &point);

  std::vector<core::Vec3> getPoints() const;

private:
  bool doesHit(const core::Vec3 &point) const;

  void recalculateNormal();
  void recalculateArea();
  bool arePointsValid();
  core::Vec3 normal_, point1_, point2_, point3_;
  double area_;
};

} // namespace objects

#endif