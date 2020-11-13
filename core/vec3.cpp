#include "vec3.h"

#include <exception>
#include <sstream>

namespace core {

Vec3::Vec3(std::initializer_list<double> initList) {
  if (initList.size() != 3) {
    std::stringstream ss1;
    ss1 << "Vec3 could not ve constructed: initializer_list size: "
        << initList.size() << "When only allowed is size 3";
    throw std::invalid_argument(ss1.str());
  }
  auto iterator = initList.begin();
  x_ = *iterator;
  iterator++;
  y_ = *iterator;
  iterator++;
  z_ = *iterator;
}

Vec3 operator+(const Vec3 &left, const Vec3 &right) {
  return Vec3(left.x() + right.x(), left.y() + right.y(), left.z() + right.z());
}

Vec3 operator+(const Vec3 &left, double right) {
  return Vec3(left.x() + right, left.y() + right, left.z() + right);
}

Vec3 operator+(double left, const Vec3 &right) { return right + left; }

Vec3 operator-(const Vec3 &left, const Vec3 &right) {
  return Vec3(left.x() - right.x(), left.y() - right.y(), left.z() - right.z());
}
Vec3 operator-(const Vec3 &left, double num) {
  return Vec3(left.x() - num, left.y() - num, left.z() - num);
}

Vec3 &Vec3::operator+=(const Vec3 &other) {
  x_ += other.x();
  y_ += other.y();
  z_ += other.z();
  return *this;
}

Vec3 &Vec3::operator+=(double num) {
  x_ += num;
  y_ += num;
  z_ += num;
  return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &other) {
  x_ -= other.x();
  y_ -= other.y();
  z_ -= other.z();
  return *this;
}

Vec3 &Vec3::operator-=(double num) {
  x_ -= num;
  y_ -= num;
  z_ -= num;
  return *this;
}

Vec3 operator*(double num, const Vec3 &vec) {
  return Vec3(vec.x() * num, vec.y() * num, vec.z() * num);
}

Vec3 operator*(const Vec3 &vec, double num) {
  return Vec3(vec.x() * num, vec.y() * num, vec.z() * num);
}

Vec3 &Vec3::operator*=(double num) {
  x_ *= num;
  y_ *= num;
  z_ *= num;
  return *this;
}

Vec3 operator/(const Vec3 &vec, double num) {
  if (num <= constants::kAccuracy) {
    std::stringstream ss;
    ss << "Divider of the Vec3 object can't be close or equal to 0. Object: "
       << vec << ", divider: " << num;
    throw std::logic_error(ss.str().c_str());
  } else {
    return Vec3(vec.x() / num, vec.y() / num, vec.z() / num);
  }
}

double Vec3::scalarProduct(const Vec3 &other) const {
  return x_ * other.x() + y_ * other.y() + z_ * other.z();
}
Vec3 Vec3::crossProduct(const Vec3 &other) const {
  return Vec3(y_ * other.z() - z_ * other.y(), z_ * other.x() - x_ * other.z(),
              x_ * other.y() - y_ * other.x());
}
double Vec3::magnitudeSquared() const { return x_ * x_ + y_ * y_ + z_ * z_; }
double Vec3::magnitude() const { return std::sqrt(this->magnitudeSquared()); }
Vec3 Vec3::normalize() const { return (*this) / magnitude(); }

bool Vec3::operator==(const Vec3 &other) const {
  return (std::abs(x() - other.x()) < constants::kAccuracy &&
          std::abs(y() - other.y()) < constants::kAccuracy &&
          std::abs(z() - other.z()) < constants::kAccuracy);
}

bool Vec3::operator!=(const Vec3 &other) const { return !(*this == other); }
std::ostream &operator<<(std::ostream &os, const Vec3 &srcVec3) {
  return os << "Vec3(" << srcVec3.x() << ", " << srcVec3.y() << ", "
            << srcVec3.z() << ")";
}

Vec3 &Vec3::operator=(const Vec3 &other) {
  x_ = other.x();
  y_ = other.y();
  z_ = other.z();
  return *this;
}

void Vec3::setX(double num) { x_ = num; }
double Vec3::x() const { return x_; }
void Vec3::setY(double num) { y_ = num; }
double Vec3::y() const { return y_; }
void Vec3::setZ(double num) { z_ = num; }
double Vec3::z() const { return z_; }

} // namespace core