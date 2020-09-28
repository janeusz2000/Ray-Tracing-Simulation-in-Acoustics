#include "vec3.h"
#include "exceptions.h"

namespace core
{
#pragma region CONSTRUCTOR

  
    Vec3::Vec3(std::initializer_list<double> initList)
    {
        if (initList.size() != 3)
        {
            throw exception::invalidConstructor();
        }
        auto iterator = initList.begin();
        _x = *iterator;
        iterator++;
        _y = *iterator;
        iterator++;
        _z = *iterator;
    }

    Vec3::Vec3(const Vec3 &other)
    {
        _x = other.getX();
        _y = other.getY();
        _z = other.getZ();
    }

#pragma endregion
#pragma region OPERATORS_PLUS

    bool operator==(const Vec3 &left, const Vec3 &right)
    {
        return (std::abs(left.getX() - right.getX()) < kAccuracy && std::abs(left.getY() - right.getY()) < kAccuracy && std::abs(left.getZ() - right.getZ()) < kAccuracy);
    }

    Vec3 operator+(const Vec3 &left, const Vec3 &right)
    {
        return Vec3(left.getX() + right.getX(), left.getY() + right.getY(), left.getZ() + right.getZ());
    }

    Vec3 operator+(const Vec3 &left, const double &right)
    {
        return Vec3(left.getX() + right, left.getY() + right, left.getZ() + right);
    }

    Vec3 operator+(const double &left, const Vec3 &right)
    {
        return Vec3(right.getX() + left, right.getY() + left, right.getZ() + left);
    }

#pragma endregion
#pragma region OPERATORS_MINUS

    Vec3 operator-(const Vec3 &left, const Vec3 &right)
    {
        return Vec3(left.getX() - right.getX(), left.getY() - right.getY(), left.getZ() - right.getZ());
    }
    Vec3 operator-(const Vec3 &left, const double &num)
    {
        return Vec3(left.getX() - num, left.getY() - num, left.getZ() - num);
    }

#pragma endregion
#pragma region OPERATORS_PLUS_EQUAL

    Vec3 &Vec3::operator+=(const Vec3 &other)
    {
        _x += other.getX();
        _y += other.getY();
        _z += other.getZ();
        return *this;
    }

    Vec3 &Vec3::operator+=(const double &num)
    {
        _x += num;
        _y += num;
        _z += num;
        return *this;
    }

#pragma endregion
#pragma region OPERATORS_MINUS_EQUAL

    Vec3 &Vec3::operator-=(const Vec3 &other)
    {
        _x -= other.getX();
        _y -= other.getY();
        _z -= other.getZ();
        return *this;
    }

    Vec3 &Vec3::operator-=(const double &num)
    {
        _x -= num;
        _y -= num;
        _z -= num;
        return *this;
    }

#pragma endregion
#pragma region OPERATOR_MULTIPLY

    Vec3 operator*(const double &num, const Vec3 &vec)
    {
        return Vec3(vec.getX() * num, vec.getY() * num, vec.getZ() * num);
    }

    Vec3 operator*(const Vec3 &vec, const double &num)
    {
        return Vec3(vec.getX() * num, vec.getY() * num, vec.getZ() * num);
    }

#pragma endregion
#pragma region OPERATOR_MULTIPLY_EQUAL

    Vec3 &Vec3::operator*=(const double &num)
    {
        _x *= num;
        _y *= num;
        _z *= num;
        return *this;
    }

#pragma endregion
#pragma region OPERATOR_DIVISION

    Vec3 operator/(const Vec3 &vec, const double &num)
    {
        if (num <= kAccuracy)
            throw exception::divisionByZero();
        else
        {
            return Vec3(vec.getX() / num, vec.getY() / num, vec.getZ() / num);
        }
    }

#pragma endregion
#pragma region METHODS

    double Vec3::scalar_product(const Vec3 &other) const
    {
        return _x * other.getX() + _y * other.getY() + _z * other.getZ();
    }

    Vec3 Vec3::cross_product(const Vec3 &other) const
    {
        return Vec3(_y * other.getZ() - _z * other.getY(), _z * other.getX() - _x * other.getZ(), _x * other.getY() - _y * other.getX());
    }

    double Vec3::magnitude() const
    {
        return std::sqrt(getX() * getX() + getY() * getY() + getZ() * getZ());
    }

    double Vec3::magnitudeSquared() const
    {
        return _x * _x + _y * _y + _z * _z;
    }
    Vec3 Vec3::normalize() const
    {
        return Vec3(getX(), getY(), getZ()) / magnitude();
    }

#pragma endregion
#pragma region SINGLE_OPERATORS

    std::ostream &operator<<(std::ostream &os, const Vec3 &srcVec3)
    {
        return os << "Vec3(" << srcVec3.getX() << ", " << srcVec3.getY() << ", " << srcVec3.getZ() << ")";
    }

    Vec3 &Vec3::operator=(const Vec3 &other)
    {
        _x = other.getX();
        _y = other.getY();
        _z = other.getZ();
        return *this;
    }

#pragma endregion
#pragma region GETTERS_SETTERS

    void Vec3::setX(const double &num)
    {
        _x = num;
    }
    double Vec3::getX() const
    {
        return _x;
    }
    void Vec3::setY(const double &num)
    {
        _y = num;
    }
    double Vec3::getY() const
    {
        return _y;
    }
    void Vec3::setZ(const double &num)
    {
        _z = num;
    }
    double Vec3::getZ() const
    {
        return _z;
    }

#pragma endregion

} // namespace core