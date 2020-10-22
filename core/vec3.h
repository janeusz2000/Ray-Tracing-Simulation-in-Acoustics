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
        explicit Vec3(float x = 0, float y = 0, float z = 0) : x_(x), y_(y), z_(z){};
        Vec3(std::initializer_list<float> initList);
        Vec3(const Vec3 &other) = default;
        ~Vec3() = default;

        Vec3 &operator=(const Vec3 &other);
        Vec3 &operator+=(const Vec3 &other);
        Vec3 &operator+=(float num);
        Vec3 &operator-=(const Vec3 &other);
        Vec3 &operator-=(float num);
        Vec3 &operator*=(float num);
        Vec3 operator-()
        {
            return *this * -1;
        }

        friend std::ostream &operator<<(std::ostream &os, const Vec3 &srcVec3);
        friend bool operator==(const Vec3 &left, const Vec3 &right);
        friend bool operator!=(const Vec3 &left, const Vec3 &right);
        friend Vec3 operator+(const Vec3 &left, const Vec3 &right);
        friend Vec3 operator+(const Vec3 &left, float right);
        friend Vec3 operator+(float left, const Vec3 &right);
        friend Vec3 operator-(const Vec3 &left, const Vec3 &right);
        friend Vec3 operator-(const Vec3 &left, float right);
        friend Vec3 operator*(float num, const Vec3 &vec);
        friend Vec3 operator*(const Vec3 &vec, float num);
        friend Vec3 operator/(const Vec3 &vec, float num);

        float scalarProduct(const Vec3 &other) const;
        Vec3 crossProduct(const Vec3 &other) const;
        float magnitude() const;
        float magnitudeSquared() const;
        Vec3 normalize() const;

        //GETTERS & SETTERS
        float x() const;
        void setX(float num);
        float y() const;
        void setY(float num);
        float z() const;
        void setZ(float num);

    private:
        float x_, y_, z_;
    };

} // namespace core
#endif