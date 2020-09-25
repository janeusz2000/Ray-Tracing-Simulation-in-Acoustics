#ifndef VEC3_H
#define VEC3_H

#include <initializer_list>
#include <iostream>
#include "core/constants.h"

namespace core
{

    class Vec3
    {
    public:
        Vec3(double x, double y, double z) : _x(x), _y(y), _z(z){};
        Vec3() : _x(0), _y(0), _z(0){};
        Vec3(std::initializer_list<double> initList);
        Vec3(const Vec3 &other);
        ~Vec3() = default;

        Vec3 &operator=(const Vec3 &other);
        Vec3 &operator+=(const Vec3 &other);
        Vec3 &operator+=(const double &num);
        Vec3 &operator-=(const Vec3 &other);
        Vec3 &operator-=(const double &num);
        Vec3 &operator*=(const double &num);

        friend std::ostream &operator<<(std::ostream &os, const Vec3 &srcVec3);
        friend bool operator==(const Vec3 &left, const Vec3 &right);
        friend Vec3 operator+(const Vec3 &left, const Vec3 &right);
        friend Vec3 operator+(const Vec3 &left, const double &right);
        friend Vec3 operator+(const double &left, const Vec3 &right);
        friend Vec3 operator-(const Vec3 &left, const Vec3 &right);
        friend Vec3 operator-(const Vec3 &left, const double &right);
        friend Vec3 operator*(const double &num, const Vec3 &vec);
        friend Vec3 operator*(const Vec3 &vec, const double &num);
        friend Vec3 operator/(const Vec3 &vec, const double &num);

        double scalar_product(const Vec3 &other) const;
        Vec3 cross_product(const Vec3 &other) const;
        double magnitude() const;
        double magnitudeSquared() const;
        Vec3 normalize() const;

        //GETTERS & SETTERS
        double getX() const;
        void setX(const double &num);
        double getY() const;
        void setY(const double &num);
        double getZ() const;
        void setZ(const double &num);

    private:
        double _x, _y, _z;
    };

} // namespace core
#endif