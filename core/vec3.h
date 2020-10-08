#ifndef VEC3_H
#define VEC3_H

#include "constants.h"
#include <initializer_list>
#include <iostream>

namespace core
{

    class Vec3
    {
        // X cord: represents left to right coordinates
        // Y cord: represents up to down coordinates
        // Z cord: represents forward to backwards coordinates
    public:
        Vec3(double x, double y, double z) : _x(x), _y(y), _z(z){};
        // TODO:
        //  better: reuse the constructor you already implemented, like Vec3(0, 0, 0)
        //  best: just define the default values in the constructor above.
        Vec3() : _x(0), _y(0), _z(0){};
        // TODO: add explicit keyword. You do not want random initlizer_list<double>
        // to be convertiable to Vec3 whenever compiler thinks it can do that. Not using
        // this leads to less readable code and bugs.
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
        friend bool operator!=(const Vec3 &left, const Vec3 &right);
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
        // TODO: use just x() and drop the get prefix; this x() is self explanatory
        // and makes code more readable.
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