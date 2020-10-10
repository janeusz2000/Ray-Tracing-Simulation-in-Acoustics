#ifndef VEC3_H
#define VEC3_H

#include "constants.h"
#include <initializer_list>
#include <iostream>

namespace core
{

    class Vec3
    {
        // X cord: represents left (-) to right (+) coordinates
        // Y cord: represents backwards (-) to forward (+) coordinates
        // Z cord: represents down (-) to up (+) coordinates

    public:
        explicit Vec3(double x = 0, double y = 0, double z = 0) : _x(x), _y(y), _z(z){};
        Vec3(std::initializer_list<double> initList);
        Vec3(const Vec3 &other) = default;
        ~Vec3() = default;

        Vec3 &operator=(const Vec3 &other);
        Vec3 &operator+=(const Vec3 &other);
        Vec3 &operator+=(const double &num);
        Vec3 &operator-=(const Vec3 &other);
        Vec3 &operator-=(const double &num);
        Vec3 &operator*=(const double &num);

        friend std::ostream &operator<<(std::ostream &os, const Vec3 &srcVec3);
        friend bool operator==(const Vec3 &left, const Vec3 &right);
        friend bool operator!=(const Vec3 &left, const Vec3 &right);
        friend Vec3 operator+(const Vec3 &left, const Vec3 &right);
        friend Vec3 operator+(const Vec3 &left, const double &right);
        friend Vec3 operator+(const double &left, const Vec3 &right);
        friend Vec3 operator-(const Vec3 &left, const Vec3 &right);
        friend Vec3 operator-(const Vec3 &left, const double &right);
        friend Vec3 operator*(const double &num, const Vec3 &vec);
        friend Vec3 operator*(const Vec3 &vec, const double &num);
        friend Vec3 operator/(const Vec3 &vec, const double &num);

        double scalarProduct(const Vec3 &other) const;
        Vec3 crossProduct(const Vec3 &other) const;
        double magnitude() const;
        double magnitudeSquared() const;
        Vec3 normalize() const;

        //GETTERS & SETTERS
        double x() const;
        void setX(const double &num);
        double y() const;
        void setY(const double &num);
        double z() const;
        void setZ(const double &num);

    private:
        double _x, _y, _z;
    };

} // namespace core
#endif