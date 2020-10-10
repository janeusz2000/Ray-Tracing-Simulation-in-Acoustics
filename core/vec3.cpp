#include "exceptions.h"
#include "vec3.h"

#include <exception>

namespace core
{
#pragma region CONSTRUCTOR

    Vec3::Vec3(std::initializer_list<double> initList)
    {
        if (initList.size() != 3)
        {
            std::stringstream ss1;
            ss1 << "Vec3 could not ve constructed: initializer_list size: " << initList.size() << "When only allowed is size 3";
            throw std::invalid_argument(ss1.str());
        }
        auto iterator = initList.begin();
        _x = *iterator;
        iterator++;
        _y = *iterator;
        iterator++;
        _z = *iterator;
    }

#pragma endregion
#pragma region OPERATORS_PLUS

    Vec3 operator+(const Vec3 &left, const Vec3 &right)
    {
        return Vec3(left.x() + right.x(), left.y() + right.y(), left.z() + right.z());
    }

    Vec3 operator+(const Vec3 &left, const double &right)
    {
        return Vec3(left.x() + right, left.y() + right, left.z() + right);
    }

    Vec3 operator+(const double &left, const Vec3 &right)
    {
        return right + left;
    }

#pragma endregion
#pragma region OPERATORS_MINUS

    Vec3 operator-(const Vec3 &left, const Vec3 &right)
    {
        return Vec3(left.x() - right.x(), left.y() - right.y(), left.z() - right.z());
    }
    Vec3 operator-(const Vec3 &left, const double &num)
    {
        return Vec3(left.x() - num, left.y() - num, left.z() - num);
    }

#pragma endregion
#pragma region OPERATORS_PLUS_EQUAL

    Vec3 &Vec3::operator+=(const Vec3 &other)
    {
        _x += other.x();
        _y += other.y();
        _z += other.z();
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
        _x -= other.x();
        _y -= other.y();
        _z -= other.z();
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
        return Vec3(vec.x() * num, vec.y() * num, vec.z() * num);
    }

    Vec3 operator*(const Vec3 &vec, const double &num)
    {
        return Vec3(vec.x() * num, vec.y() * num, vec.z() * num);
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
        if (num <= constants::kAccuracy)
        {
            std::stringstream ss;
            ss << "Divider of the Vec3 object can't be close or equal to 0. Object: " << vec << ", divider: " << num;
            throw std::logic_error(ss.str().c_str());
        }
        else
        {
            return Vec3(vec.x() / num, vec.y() / num, vec.z() / num);
        }
    }

#pragma endregion
#pragma region METHODS

    double Vec3::scalarProduct(const Vec3 &other) const
    {
        return _x * other.x() + _y * other.y() + _z * other.z();
    }
    Vec3 Vec3::crossProduct(const Vec3 &other) const
    {
        return Vec3(_y * other.z() - _z * other.y(), _z * other.x() - _x * other.z(), _x * other.y() - _y * other.x());
    }
    double Vec3::magnitudeSquared() const
    {
        return _x * _x + _y * _y + _z * _z;
    }
    double Vec3::magnitude() const
    {
        return std::sqrt(this->magnitudeSquared());
    }
    Vec3 Vec3::normalize() const
    {
        return (*this) / magnitude();
    }

#pragma endregion
#pragma region SINGLE_OPERATORS

    bool operator==(const Vec3 &left, const Vec3 &right)
    {
        return (std::abs(left.x() - right.x()) < constants::kAccuracy &&
                std::abs(left.y() - right.y()) < constants::kAccuracy &&
                std::abs(left.z() - right.z()) < constants::kAccuracy);
    }

    bool operator!=(const Vec3 &left, const Vec3 &right)
    {
        return !(left == right);
    }
    std::ostream &operator<<(std::ostream &os, const Vec3 &srcVec3)
    {
        return os << "Vec3(" << srcVec3.x() << ", " << srcVec3.y() << ", " << srcVec3.z() << ")";
    }

    Vec3 &Vec3::operator=(const Vec3 &other)
    {
        _x = other.x();
        _y = other.y();
        _z = other.z();
        return *this;
    }

#pragma endregion
#pragma region GETTERS_SETTERS

    void Vec3::setX(const double &num)
    {
        _x = num;
    }
    double Vec3::x() const
    {
        return _x;
    }
    void Vec3::setY(const double &num)
    {
        _y = num;
    }
    double Vec3::y() const
    {
        return _y;
    }
    void Vec3::setZ(const double &num)
    {
        _z = num;
    }
    double Vec3::z() const
    {
        return _z;
    }

#pragma endregion

} // namespace core