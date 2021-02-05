#include "ray.h"



namespace core {

Ray Ray::fromSphericalCoords(const Vec3 &origin, float zRotation,
                             float xyInclination) {
  return Ray(origin, Vec3(std::cos(xyInclination) * std::cos(zRotation),
                          std::cos(xyInclination) * std::sin(zRotation),
                          std::sin(xyInclination)));
}

Ray::Ray(const Vec3 &origin, const Vec3 &direction, float energy)
    : origin_(origin), energy_(energy) {
  if (direction == Vec3(0, 0, 0)) {
    throw std::invalid_argument("Direction  equal to Vec3(0, 0, 0)");
  }

  direction_ = direction.normalize();
}

Vec3 Ray::at(float time) const { return origin_ + time * direction_; }

float Ray::phaseAt(float freq, float time) const {
  if (freq <= constants::kAccuracy || time <= constants::kAccuracy) {
    std::stringstream ss;
    ss << "Could not calculate phase at, because input "
       << "frequency or time is close or equal to zero. Values are: freq: "
       << freq << ", time: " << time;
    throw std::invalid_argument(ss.str());
  }
  float waveLength = constants::kSoundSpeed / freq;
  return time / waveLength * 2 * constants::kPi;
}

std::ostream &operator<<(std::ostream &os, const Ray &srcRay) {
  return os << "RAY origin: " << srcRay.origin()
            << ", direction: " << srcRay.direction()
            << ", energy: " << srcRay.energy();
}

bool Ray::operator==(const Ray &other) const {
  return (origin() == other.origin() && direction() == other.direction());
}

void Ray::setOrigin(const Vec3 &origin) { origin_ = origin; }

Vec3 Ray::origin() const { return origin_; }

void Ray::setDirection(const Vec3 &direction) {
  direction_ = direction.normalize();
}

Vec3 Ray::direction() const { return direction_; }

void Ray::setEnergy(float num) { energy_ = num; }

float Ray::energy() const { return energy_; }

bool RayHitData::operator==(const RayHitData &other) const {
  return (std::abs(other.time - time) <= constants::kAccuracy &&
          other.collisionPoint() - collisionPoint() == Vec3(0, 0, 0) &&
          other.direction() - direction() == Vec3(0, 0, 0) &&
          other.normal() - normal() == Vec3(0, 0, 0) &&
          std::abs(other.energy() - energy()) <= constants::kAccuracy &&
          std::abs(other.phase() - phase()) <= constants::kAccuracy);
}

std::ostream &operator<<(std::ostream &os, const RayHitData &rayData) {
  return os << "Collision point: " << rayData.collisionPoint()
            << ", incoming ray direction: " << rayData.direction()
            << ", normal: " << rayData.normal() << ", time: " << rayData.time
            << ", energy: " << rayData.energy() << ", phase " << rayData.phase()
            << " [radians]";
}

} // namespace core