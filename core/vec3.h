#ifndef VEC3_H
#define VEC3_H

#include "constants.h"

#include <ctime>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <random>
#include <sstream>

namespace core {

class Vec3 {
public:
  static const Vec3 kVecZero;
  static const Vec3 kVecZ;
  static const Vec3 kVecX;
  static const Vec3 kVecY;

  // creates Vec3 object, which every coordinate is drawn from Normal
  // Distribution.
  static Vec3 getRandomVec();

  // X cord: represents left (-) to right (+) coordinates
  // Y cord: represents backwards (-) to forward (+) coordinates
  // Z cord: represents down (-) to up (+) coordinates
  explicit Vec3(float x = 0, float y = 0, float z = 0) : x_(x), y_(y), z_(z){};
  Vec3(std::initializer_list<float> initList);

  Vec3 &operator=(const Vec3 &other);
  Vec3 &operator+=(const Vec3 &other);
  Vec3 &operator+=(float num);
  Vec3 &operator-=(const Vec3 &other);
  Vec3 &operator-=(float num);
  Vec3 &operator*=(float num);
  Vec3 operator-() const { return *this * -1; }
  bool operator==(const Vec3 &other) const;
  bool operator!=(const Vec3 &other) const;

  friend Vec3 operator+(const Vec3 &left, const Vec3 &right);
  friend Vec3 operator+(const Vec3 &left, float right);
  friend Vec3 operator+(float left, const Vec3 &right);
  friend Vec3 operator-(const Vec3 &left, const Vec3 &right);
  friend Vec3 operator-(const Vec3 &left, float right);
  friend Vec3 operator*(float num, const Vec3 &vec);
  friend Vec3 operator*(const Vec3 &vec, float num);
  friend Vec3 operator/(const Vec3 &vec, float num);
  friend std::ostream &operator<<(std::ostream &os, const Vec3 &srcVec3);

  float scalarProduct(const Vec3 &other) const;
  Vec3 crossProduct(const Vec3 &other) const;
  float magnitude() const;
  float magnitudeSquared() const;
  Vec3 normalize() const;

  float x() const;
  void setX(float num);
  float y() const;
  void setY(float num);
  float z() const;
  void setZ(float num);

private:
  float x_, y_, z_;
  static std::mt19937_64 engine_;
  static std::normal_distribution<float> dist_;
};

} // namespace core
#endif