#include "vec3.h"

#include <exception>
#include <sstream>

namespace core
{
#pragma region CONSTRUCTOR

    Vec3::Vec3(std::initializer_list<float> initList)
    {
        if (initList.size() != 3)
        {
            std::stringstream ss1;
            ss1 << "Vec3 could not ve constructed: initializer_list size: " << initList.size() << "When only allowed is size 3";
            throw std::invalid_argument(ss1.str());
        }
        auto iterator = initList.begin();
        x_ = *iterator;
        iterator++;
        y_ = *iterator;
        iterator++;
        z_ = *iterator;
    }

#pragma endregion
#pragma region OPERATORS_PLUS

    Vec3 operator+(const Vec3 &left, const Vec3 &right)
    {
        return Vec3(left.x() + right.x(), left.y() + right.y(), left.z() + right.z());
    }

    Vec3 operator+(const Vec3 &left, float right)
    {
        return Vec3(left.x() + right, left.y() + right, left.z() + right);
    }

    Vec3 operator+(float left, const Vec3 &right)
    {
        return right + left;
    }

#pragma endregion
#pragma region OPERATORS_MINUS

    Vec3 operator-(const Vec3 &left, const Vec3 &right)
    {
        return Vec3(left.x() - right.x(), left.y() - right.y(), left.z() - right.z());
    }
    Vec3 operator-(const Vec3 &left, float num)
    {
        return Vec3(left.x() - num, left.y() - num, left.z() - num);
    }

#pragma endregion
#pragma region OPERATORS_PLUS_EQUAL

    Vec3 &Vec3::operator+=(const Vec3 &other)
    {
        x_ += other.x();
        y_ += other.y();
        z_ += other.z();
        return *this;
    }

    Vec3 &Vec3::operator+=(float num)
    {
        x_ += num;
        y_ += num;
        z_ += num;
        return *this;
    }

#pragma endregion
#pragma region OPERATORS_MINUS_EQUAL

    Vec3 &Vec3::operator-=(const Vec3 &other)
    {
        x_ -= other.x();
        y_ -= other.y();
        z_ -= other.z();
        return *this;
    }

    Vec3 &Vec3::operator-=(float num)
    {
        x_ -= num;
        y_ -= num;
        z_ -= num;
        return *this;
    }

#pragma endregion
#pragma region OPERATOR_MULTIPLY

    Vec3 operator*(float num, const Vec3 &vec)
    {
        return Vec3(vec.x() * num, vec.y() * num, vec.z() * num);
    }

    Vec3 operator*(const Vec3 &vec, float num)
    {
        return Vec3(vec.x() * num, vec.y() * num, vec.z() * num);
    }

#pragma endregion
#pragma region OPERATOR_MULTIPLY_EQUAL

    Vec3 &Vec3::operator*=(float num)
    {
        x_ *= num;
        y_ *= num;
        z_ *= num;
        return *this;
    }

#pragma endregion
#pragma region OPERATOR_DIVISION

    Vec3 operator/(const Vec3 &vec, float num)
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

    float Vec3::scalarProduct(const Vec3 &other) const
    {
        return x_ * other.x() + y_ * other.y() + z_ * other.z();
    }
    Vec3 Vec3::crossProduct(const Vec3 &other) const
    {
        return Vec3(y_ * other.z() - z_ * other.y(), z_ * other.x() - x_ * other.z(), x_ * other.y() - y_ * other.x());
    }
    float Vec3::magnitudeSquared() const
    {
        return x_ * x_ + y_ * y_ + z_ * z_;
    }
    float Vec3::magnitude() const
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
        x_ = other.x();
        y_ = other.y();
        z_ = other.z();
        return *this;
    }

#pragma endregion
#pragma region GETTERS_SETTERS

    void Vec3::setX(float num)
    {
        x_ = num;
    }
    float Vec3::x() const
    {
        return x_;
    }
    void Vec3::setY(float num)
    {
        y_ = num;
    }
    float Vec3::y() const
    {
        return y_;
    }
    void Vec3::setZ(float num)
    {
        z_ = num;
    }
    float Vec3::z() const
    {
        return z_;
    }

#pragma endregion

} // namespace core