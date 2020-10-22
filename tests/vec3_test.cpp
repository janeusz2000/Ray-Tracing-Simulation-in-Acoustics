#include "core/vec3.h"

#include "gtest/gtest.h"
#include "core/exceptions.h"
#include <cmath>
#include <sstream>

// TODO: Change all doubles to float

namespace core
{
    TEST(VEC3_BASIC, Test_Operator_ostream)
    {
        std::stringstream testStream1;
        std::stringstream testStream2;
        std::stringstream testStream3;
        std::stringstream testStream4;
        std::stringstream testStream5;
        std::stringstream testStream6;

        testStream1 << Vec3(0.0, 1, 2);
        testStream2 << Vec3(0.1, 0, 3);
        testStream3 << Vec3(3.00001, 2, 1);
        testStream4 << Vec3(std::sqrt(2), 2, 1);
        testStream5 << Vec3(0, 0, 0);
        testStream6 << Vec3({1, 2, 3});

        ASSERT_EQ(testStream1.str(), "Vec3(0, 1, 2)");
        ASSERT_EQ(testStream2.str(), "Vec3(0.1, 0, 3)");
        ASSERT_EQ(testStream3.str(), "Vec3(3.00001, 2, 1)");
        ASSERT_EQ(testStream4.str(), "Vec3(1.41421, 2, 1)");
        ASSERT_EQ(testStream5.str(), "Vec3(0, 0, 0)");
        ASSERT_EQ(testStream6.str(), "Vec3(1, 2, 3)");
    }

    TEST(VEC3_BASIC, Test_constructor_initialier_list)
    {
        ASSERT_EQ(Vec3(0, 0, 0), Vec3({0, 0, 0}));
        ASSERT_EQ(Vec3(0, 0, 1), Vec3({0, 0, 1}));
        ASSERT_EQ(Vec3(std::sqrt(2), 0, 0), Vec3({std::sqrt(2), 0, 0}));
    }

    TEST(VEC3_BASIC, Test_Operator_equal)
    {
        ASSERT_EQ(Vec3(0, 1, 0), Vec3(0, 1, 0));
        ASSERT_EQ(Vec3(1.001, 2.001, 3.001), Vec3(1.001, 2.001, 3.001));
        ASSERT_EQ(Vec3(std::sqrt(2), 0, 1.001), Vec3(1.4142135623730950488016887242097, 0, 1.001));
    }

    TEST(VEC3_BASIC, Test_Operator_equal_equal)
    {
        ASSERT_EQ(Vec3(1.0, 1.0, 1.0), Vec3(1.0, 1.0, 1.0));
        ASSERT_EQ(Vec3(2.0, 1, 3), Vec3(2.0, 1, 3));
        ASSERT_EQ(Vec3(15, 9, 10), Vec3(15, 9, 10));
    }

    TEST(VEC3_BASIC, Test_Operator_Equal)
    {
        Vec3 temp1(0.1, 2, 3);
        Vec3 temp2(0.2, 3, 5);
        Vec3 temp3(0.1, 5, 4);

        Vec3 tempA = temp1;
        Vec3 tempB = temp2;
        Vec3 tempC = temp3;

        ASSERT_EQ(temp1, tempA);
        ASSERT_EQ(temp2, tempB);
        ASSERT_EQ(temp3, tempC);
    }

    TEST(VEC3_OPERATOR_PLUS, Test_Operator_basic_addition)
    {
        ASSERT_EQ(Vec3(0, 0, 1), Vec3(0, 0, 1) + Vec3(0, 0, 0));
        ASSERT_EQ(Vec3(1, 2, 3), Vec3(0, 1, 2) + Vec3(1, 1, 1));
        ASSERT_EQ(Vec3(std::sqrt(2) * 2, 0, 2), Vec3(std::sqrt(2), 0, 1) + Vec3(std::sqrt(2), 0, 1));
        ASSERT_EQ(Vec3(1, 2, 3), Vec3(0.5, 0.5, 0.5) + Vec3(0.5, 0.5, 0.5) + Vec3(0, 0.5, 0.5) + Vec3(0, 0.5, 0.5) + Vec3(0, 0, 0.5) + Vec3(0, 0, 0.5));
    }
    TEST(VEC3_OPERATOR_PLUS, Test_Operator_list_Addition)
    {
        ASSERT_EQ(Vec3(0, 0, 1), Vec3(0, 0, 1) + Vec3({0, 0, 0}));
        ASSERT_EQ(Vec3(0, 0, 1), Vec3({0, 0, 0}) + Vec3(0, 0, 1));
    }

    TEST(VEC3_OPERATOR_PLUS, Test_Operator_floating_point_error_Addition)
    {
        ASSERT_EQ(Vec3(1, 2, 3), Vec3(0, 1, 2) + Vec3({1, 1, 1}));
        ASSERT_EQ(Vec3(std::sqrt(2) * 2, 0, 2), Vec3(std::sqrt(2), 0, 1) + Vec3({std::sqrt(2), 0, 1}));
        ASSERT_EQ(Vec3(1, 2, 3), Vec3(0.5, 0.5, 0.5) + Vec3({0.5, 0.5, 0.5}) + Vec3({0, 0.5, 0.5}) + Vec3({0, 0.5, 0.5}) + Vec3(0, 0, 0.5) + Vec3({0, 0, 0.5}));
    }

    TEST(VEC3_OPERATOR_PLUS, Test_Operator_Double_Addition)
    {
        ASSERT_EQ(Vec3(1, 1, 2), Vec3(0, 0, 1) + 1);
        ASSERT_EQ(Vec3(1, 1, 2), 1 + Vec3(0, 0, 1));
        ASSERT_EQ(Vec3(-0.1, -1, -2), Vec3(-1.1, -2, -3) + 1);
        ASSERT_EQ(Vec3(1.1, 1.1, 2.1), 0.1 + Vec3(1, 1, 2));
    }

    TEST(VEC3_OPERATOR_PLUS, Test_Operator_Rvalue_Addition)
    {
        ASSERT_EQ(Vec3(1, 2, 3), std::move(Vec3(1, 0, 0)) + Vec3(0, 2, 3));
        ASSERT_EQ(Vec3(0.1, 0.2, 0.3), Vec3(0, 0.2, 0.3) + std::move(Vec3(0.1, 0, 0)));
        ASSERT_EQ(Vec3(0, 1, 0), std::move(Vec3(0, 0, 0) + std::move(Vec3(0, 1, 0))));
        ASSERT_EQ(Vec3(1, 0, -0.1), std::move(Vec3(1, 0, 1)) + std::move(Vec3(0, 0, -1.1)));
    }

    TEST(VEC3_OPERATOR_PLUS, Test_Operator_plus_equal_Vector)
    {
        Vec3 temp1 = Vec3(0, 0, 0);
        temp1 += Vec3(1, 2, 3);
        ASSERT_EQ(Vec3(1, 2, 3), temp1);

        temp1 = Vec3(0.15, 0.1, 0.05);
        temp1 += Vec3(0.15, 0.1, 0.05);
        ASSERT_EQ(Vec3(0.3, 0.2, 0.1), temp1);
    }

    TEST(VEC3_OPERATOR_PLUS, Test_Operator_Plus_equal_Double)
    {
        ASSERT_EQ(Vec3(2, 3, 4), Vec3(1, 2, 3) += 1);
        ASSERT_EQ(Vec3(0.2, 0.3, 0.4), Vec3(0.1, 0.2, 0.3) += 0.1);
    }

    TEST(VEC3_OPERATOR_PLUS, Test_operator_plus_equal_rvalue_double)
    {
        ASSERT_EQ(Vec3(0.2, 0.3, 0.4), Vec3(0.1, 0.2, 0.3) += std::move(0.1));
        ASSERT_EQ(Vec3(0.4, 0.3, 0.2), Vec3(0.3, 0.2, 0.1) += std::move(0.1));
    }

    TEST(VEC3_OPERATOR_PLUS, Test_Operator_plus_Equal_rvalue_Vec3)
    {
        ASSERT_EQ(Vec3(0.2, 0.2, 0.3), Vec3(0.1, 0, 0.2) += std::move(Vec3(0.1, 0.2, 0.1)));
        ASSERT_EQ(Vec3(0.1, 0.5, -0.3), Vec3(-0.1, 0.3, -5.3) += std::move(Vec3(0.2, 0.2, 5)));
    }

    TEST(VEC3_OPERATOR_PLUS, Test_Operator_Plus_Equal_list)
    {
        ASSERT_EQ(Vec3(-0.1, -5, 0.3), Vec3(0, 0, 0) += Vec3({-0.1, -5, 0.3}));
        ASSERT_EQ(Vec3(-4, 10, -5), Vec3(-2, 5, -3) += Vec3({-2, 5, -2}));
    }

    TEST(VEC3_OPERATOR_MINUS, Test_Operator_minus_basic)
    {
        ASSERT_EQ(Vec3(1, 0, 0), Vec3(0, 0, 0) - Vec3(-1, 0, 0));
        ASSERT_EQ(Vec3(0, 0, 0.1), Vec3(0, 0, 0) - Vec3(0, 0, -0.1));
        ASSERT_EQ(Vec3(-0.1, 5, 3), Vec3(0, -5, 2) - Vec3(0.1, -10, -1));
        ASSERT_EQ(Vec3(0.1, 0.03, 0.002), Vec3(0, 0, 0) - Vec3(-0.1, -0.03, -0.002));
    }

    TEST(VEC3_OPERATOR_MINUS, Test_Operator_minus_basic_vec3_double)
    {
        ASSERT_EQ(Vec3(0, 1, 0), Vec3(1, 2, 1) - 1);
        ASSERT_EQ(Vec3(0, 0, 1), Vec3(1, 1, 2) - 1);
        ASSERT_EQ(Vec3(0, 0, 0), Vec3(0.1, 0.1, 0.1) - 0.1);
    }

    TEST(VEC3_OPERATOR_MINUS, Test_Operator_rvalue_Minus_Vec3_Vec3)
    {
        ASSERT_EQ(Vec3(0, 0, -1), std::move(Vec3(0, 0, 0)) - std::move(Vec3(0, 0, 1)));
        ASSERT_EQ(Vec3(0, 0.1, 0), std::move(Vec3(0, 0.1, 0.1) - std::move(Vec3(0, 0, 0.1))));
        ASSERT_EQ(Vec3(0, 1, 0.03), std::move(Vec3(0, 5, 0.05) - std::move(Vec3(0, 4, 0.02))));
    }

    TEST(VEC3_OPERATOR_MINUS, Test_Operator_Minus_Vec3_Rvalue_Vec3)
    {
        ASSERT_EQ(Vec3(0, 0, 1), Vec3(0, 0, 2) - std::move(Vec3(0, 0, 1)));
        ASSERT_EQ(Vec3(-1, 0.1, 0), Vec3(0, 1, 0) - std::move(Vec3(1, 0.9, 0)));
    }

    TEST(VEC3_OPERATOR_MINUS, Test_Operator_Rvalue_Minus_Vec3)
    {
        ASSERT_EQ(Vec3(0, 0, 0), std::move(Vec3(50, 30, 1)) - Vec3(50, 30, 1));
        ASSERT_EQ(Vec3(-3, 2, 14), std::move(Vec3(0, 0, 3)) - Vec3(3, -2, -11));
        ASSERT_EQ(Vec3(5, 3, 1), std::move(Vec3(50, 0, -4)) - Vec3(45, -3, -5));
    }

    TEST(VEC3_OPERATOR_MINUS, Test_Operator_minus_Vec3_Rvalue_Double)
    {
        ASSERT_EQ(Vec3(0, 0, 0), Vec3(1, 1, 1) - std::move(1));
        ASSERT_EQ(Vec3(0.1, -0.1, 0.2), Vec3(1.1, 0.9, 1.2) - std::move(1));
        ASSERT_EQ(Vec3(0.5, 0.1, 0.1), Vec3(1, 0.6, 0.6) - std::move(0.5));
    }

    TEST(VEC3_OPERATOR_MINUS, Test_Operator_Minus_Rvalue_Vec3_Rvalue_double)
    {
        ASSERT_EQ(Vec3(0.1, 0.2, 0.1), std::move(Vec3(0.2, 0.3, 0.1)) - std::move(Vec3(0.1, 0.1, 0)));
        ASSERT_EQ(Vec3(0.2, 0.3, 0.4), std::move(Vec3(0.1, 0.2, 0.3)) - std::move(Vec3(-0.1, -0.1, -0.1)));
        ASSERT_EQ(Vec3(0.1, 0.1, 0.1), std::move(Vec3(0.8, 0.1, 0.3)) - std::move(Vec3(0.7, 0, 0.2)));
    }

    TEST(VEC3_OPERATOR_MINUS, Test_Operator_Minus_Rvalue_Vec3_Rvalue_Double)
    {
        ASSERT_EQ(Vec3(0.1, 0.2, 0.3), std::move(Vec3(0.2, 0.3, 0.4)) - std::move(0.1));
        ASSERT_EQ(Vec3(0.2, 0.3, 0.1), std::move(Vec3(0.1, 0.2, 0)) - std::move(-0.1));
        ASSERT_EQ(Vec3(0.3, 0.1, 0.1), std::move(Vec3(0.5, 0.3, 0.3)) - std::move(0.2));
    }

    TEST(VEC3_OPERATOR_MINUS, Test_Operator_minus_equal_Double)
    {
        ASSERT_EQ(Vec3(0.2, 0.1, 0.1), Vec3(0.3, 0.5, -0.4) -= Vec3(0.1, 0.4, -0.5));
        ASSERT_EQ(Vec3(0.5, 0.1, -8), Vec3(0.4, 0.1, 0.5) -= Vec3(-0.1, 0, 8.5));
        ASSERT_EQ(Vec3(0.4, 0.1, 0.2), Vec3(0.5, 0.1, 0.2) -= Vec3(0.1, 0, 0));
    }

    TEST(VEC3_OPERATOR_MINUS, Test_Operator_minus_equal_rvalue_double)
    {
        ASSERT_EQ(Vec3(0.1, 0.3, 0.1), Vec3(0.2, 0.4, 0.2) -= std::move(0.1));
        ASSERT_EQ(Vec3(0.4, 0.1, 0.2), Vec3(5.4, 5.1, 5.2) -= std::move(5));
        ASSERT_EQ(Vec3(-7, -10, 78), Vec3(-6, -9, 79) -= std::move(1));
    }

    TEST(VEC3_OPERATOR_MINUS, Test_Operator_minus_equal_Vec3)
    {
        ASSERT_EQ(Vec3(0.4, 0.1, 0.2), Vec3(0.5, 0.8, 0.3) -= Vec3(0.1, 0.7, 0.1));
        ASSERT_EQ(Vec3(0.5, 0.5, 0.8), Vec3(0.8, 0.7, 80) -= Vec3(0.3, 0.2, 79.2));
        ASSERT_EQ(Vec3(0.4, 1, -40.1), Vec3(0.9, 0.3, 40) -= Vec3(0.5, -0.7, 80.1));
    }

    TEST(VEC3_OPERATOR_MINUS, Test_Operator_minus_equal_Rvalue_Vec3)
    {
        ASSERT_EQ(Vec3(0.4, 0.1, 0.2), Vec3(0.5, 0.8, 0.3) -= std::move(Vec3(0.1, 0.7, 0.1)));
        ASSERT_EQ(Vec3(0.5, 0.5, 0.8), Vec3(0.8, 0.7, 80) -= std::move(Vec3(0.3, 0.2, 79.2)));
        ASSERT_EQ(Vec3(0.4, 1, -40.1), Vec3(0.9, 0.3, 40) -= std::move(Vec3(0.5, -0.7, 80.1)));
    }

    TEST(VEC3_OPERATOR_MULTIPLY, Test_Operator_multiply_double_vec3)
    {
        ASSERT_EQ(Vec3(0.1, 0.3, 0.2), 0.1 * Vec3(1, 3, 2));
        ASSERT_EQ(Vec3(40, 50, 20), 10 * Vec3(4, 5, 2));
        ASSERT_EQ(Vec3(0.3, 0.2, 0.1), 0.001 * Vec3(300, 200, 100));
    }

    TEST(VEC3_OPERATOR_MULTIPLY, Test_Operator_multiply_vec3_double)
    {
        ASSERT_EQ(Vec3(0.1, 0.3, 0.2), Vec3(1, 3, 2) * 0.1);
        ASSERT_EQ(Vec3(40, 50, 20), Vec3(4, 5, 2) * 10);
        ASSERT_EQ(Vec3(0.3, 0.2, 0.1), Vec3(300, 200, 100) * 0.001);
    }

    TEST(VEC3_OPERATOR_MULTIPLY, Test_Operator_multiply_rvalue_double_vec3)
    {
        ASSERT_EQ(Vec3(0.1, 0.3, 0.2), std::move(0.1) * Vec3(1, 3, 2));
        ASSERT_EQ(Vec3(40, 50, 20), std::move(10) * Vec3(4, 5, 2));
        ASSERT_EQ(Vec3(0.3, 0.2, 0.1), std::move(0.001) * Vec3(300, 200, 100));
    }

    TEST(VEC3_OPERATOR_MULTIPLY, Test_Operator_multiply_vec3_rvalue_double)
    {
        ASSERT_EQ(Vec3(0.1, 0.3, 0.2), Vec3(1, 3, 2) * std::move(0.1));
        ASSERT_EQ(Vec3(40, 50, 20), Vec3(4, 5, 2) * std::move(10));
        ASSERT_EQ(Vec3(0.3, 0.2, 0.1), Vec3(300, 200, 100) * std::move(0.001));
    }

    TEST(VEC3_OPERATOR_MULTIPLY, Test_Operator_multiply_double_rvalue_vec3)
    {
        ASSERT_EQ(Vec3(0.1, 0.3, 0.2), 0.1 * std::move(Vec3(1, 3, 2)));
        ASSERT_EQ(Vec3(40, 50, 20), 10 * std::move(Vec3(4, 5, 2)));
        ASSERT_EQ(Vec3(0.3, 0.2, 0.1), 0.001 * std::move(Vec3(300, 200, 100)));
    }

    TEST(VEC3_OPERATOR_MULTIPLY, Test_Operator_multiply_rvalue_vec3_double)
    {
        ASSERT_EQ(Vec3(0.1, 0.3, 0.2), std::move(Vec3(1, 3, 2)) * 0.1);
        ASSERT_EQ(Vec3(40, 50, 20), std::move(Vec3(4, 5, 2)) * 10);
        ASSERT_EQ(Vec3(0.3, 0.2, 0.1), std::move(Vec3(300, 200, 100)) * 0.001);
    }

    TEST(VEC3_OPERATOR_MULTIPLY, Test_Operator_multiply_rvalue_double_rvalue_vec3)
    {
        ASSERT_EQ(Vec3(0.1, 0.3, 0.2), std::move(0.1) * std::move(Vec3(1, 3, 2)));
        ASSERT_EQ(Vec3(40, 50, 20), std::move(10) * std::move(Vec3(4, 5, 2)));
        ASSERT_EQ(Vec3(0.3, 0.2, 0.1), std::move(0.001) * std::move(Vec3(300, 200, 100)));
    }

    TEST(VEC3_OPERATOR_MULTIPLY, Test_Operator_multiply_rvalue_vec3_rvalue_double)
    {
        ASSERT_EQ(Vec3(0.1, 0.3, 0.2), std::move(Vec3(1, 3, 2)) * std::move(0.1));
        ASSERT_EQ(Vec3(40, 50, 20), std::move(Vec3(4, 5, 2)) * std::move(10));
        ASSERT_EQ(Vec3(0.3, 0.2, 0.1), std::move(Vec3(300, 200, 100)) * std::move(0.001));
    }

    TEST(VEC3_OPERATOR_MULTIPLY, Test_Operator_multiply_equal_double)
    {
        ASSERT_EQ(Vec3(0.1, 0.3, 0.2), Vec3(1, 3, 2) *= 0.1);
        ASSERT_EQ(Vec3(40, 50, 20), Vec3(4, 5, 2) *= 10);
        ASSERT_EQ(Vec3(0.3, 0.2, 0.1), Vec3(300, 200, 100) *= 0.001);
    }

    TEST(VEC3_OPERATOR_MULTIPLY, Test_Operator_multiply_equal_rvalue_double)
    {
        ASSERT_EQ(Vec3(0.1, 0.3, 0.2), Vec3(1, 3, 2) *= std::move(0.1));
        ASSERT_EQ(Vec3(40, 50, 20), Vec3(4, 5, 2) *= std::move(10));
        ASSERT_EQ(Vec3(0.3, 0.2, 0.1), Vec3(300, 200, 100) *= std::move(0.001));
    }

    TEST(VEC3_OPERATOR_DIVISION, Test_Operator_Division_Throw_exception_division_by_zero)
    {
        ASSERT_THROW(Vec3(0, 0, 1) / 0, std::logic_error);
        ASSERT_THROW(Vec3(0, 0, 0) / (constants::kAccuracy * 0.1), std::logic_error);
        ASSERT_THROW(Vec3(0, 0, 0) / (constants::kAccuracy * 0.5), std::logic_error);
        ASSERT_THROW(Vec3(0, 0, 0) / (constants::kAccuracy * 0.9), std::logic_error);
        ASSERT_THROW(Vec3(0, 0, 0) / constants::kAccuracy, std::logic_error);
    }

    TEST(VEC3_OPERATOR_DIVISION, Test_Operator_Division_Throw_exception_rvalue_division_by_zero_)
    {
        ASSERT_THROW(std::move(Vec3(0, 0, 1)) / 0, std::logic_error);
        ASSERT_THROW(std::move(Vec3(0, 0, 0)) / (constants::kAccuracy * 0.1), std::logic_error);
        ASSERT_THROW(std::move(Vec3(0, 0, 0)) / (constants::kAccuracy * 0.5), std::logic_error);
        ASSERT_THROW(std::move(Vec3(0, 0, 0)) / (constants::kAccuracy * 0.9), std::logic_error);
        ASSERT_THROW(std::move(Vec3(0, 0, 0)) / constants::kAccuracy, std::logic_error);
    }

    TEST(VEC3_OPERATOR_DIVISION, Test_Operator_Division_Throw_exception_division_by_rvalue_zero)
    {
        ASSERT_THROW(Vec3(0, 0, 1) / std::move(0), std::logic_error);
        ASSERT_THROW(Vec3(0, 0, 0) / std::move(constants::kAccuracy * 0.1), std::logic_error);
        ASSERT_THROW(Vec3(0, 0, 0) / std::move(constants::kAccuracy * 0.5), std::logic_error);
        ASSERT_THROW(Vec3(0, 0, 0) / std::move(constants::kAccuracy * 0.9), std::logic_error);
        ASSERT_THROW(Vec3(0, 0, 0) / std::move(constants::kAccuracy), std::logic_error);
    }

    TEST(VEC3_OPERATOR_DIVISION, Test_Operator_Division_Throw_exception_rvalue_division_by_rvalue_zero)
    {
        ASSERT_THROW(std::move(Vec3(0, 0, 1)) / std::move(0), std::logic_error);
        ASSERT_THROW(std::move(Vec3(0, 0, 0)) / std::move(constants::kAccuracy * 0.1), std::logic_error);
        ASSERT_THROW(std::move(Vec3(0, 0, 0)) / std::move(constants::kAccuracy * 0.5), std::logic_error);
        ASSERT_THROW(std::move(Vec3(0, 0, 0)) / std::move(constants::kAccuracy * 0.9), std::logic_error);
        ASSERT_THROW(std::move(Vec3(0, 0, 0)) / std::move(constants::kAccuracy), std::logic_error);
    }

    TEST(VEC3_OPERATOR_DIVISION, Test_Operator_division_Vec3_double)
    {
        ASSERT_EQ(Vec3(2, 1, 1), Vec3(4, 2, 2) / 2);
        ASSERT_EQ(Vec3(0.1, 0.2, 0.3), Vec3(10, 20, 30) / 100);
        ASSERT_EQ(Vec3(50, 40, 20), Vec3(25, 20, 10) / 0.5);
        ASSERT_EQ(Vec3(3, 5, 4), Vec3(15, 25, 20) / 5);
    }

    TEST(VEC3_OPERATOR_DIVISION, Test_Operator_Division_vec3_rvalue_double)
    {
        ASSERT_EQ(Vec3(2, 1, 1), Vec3(4, 2, 2) / std::move(2));
        ASSERT_EQ(Vec3(0.1, 0.2, 0.3), Vec3(10, 20, 30) / std::move(100));
        ASSERT_EQ(Vec3(50, 40, 20), Vec3(25, 20, 10) / std::move(0.5));
        ASSERT_EQ(Vec3(3, 5, 4), Vec3(15, 25, 20) / std::move(5));
    }

    TEST(VEC3_OPERATOR_DIVISION, Test_Operator_Division_rvalue_vec3_double)
    {
        ASSERT_EQ(Vec3(2, 1, 1), std::move(Vec3(4, 2, 2)) / 2);
        ASSERT_EQ(Vec3(0.1, 0.2, 0.3), std::move(Vec3(10, 20, 30)) / 100);
        ASSERT_EQ(Vec3(50, 40, 20), std::move(Vec3(25, 20, 10)) / 0.5);
        ASSERT_EQ(Vec3(3, 5, 4), std::move(Vec3(15, 25, 20)) / 5);
    }

    TEST(VEC3_OPERATOR_DIVISION, Test_Operator_Division_rvalue_Vec3_rvalue_double)
    {
        ASSERT_EQ(Vec3(2, 1, 1), std::move(Vec3(4, 2, 2)) / std::move(2));
        ASSERT_EQ(Vec3(0.1, 0.2, 0.3), std::move(Vec3(10, 20, 30)) / std::move(100));
        ASSERT_EQ(Vec3(50, 40, 20), std::move(Vec3(25, 20, 10)) / std::move(0.5));
        ASSERT_EQ(Vec3(3, 5, 4), std::move(Vec3(15, 25, 20)) / std::move(5));
    }

    TEST(VEC3_METHODS, Test_method_scalarProduct)
    {
        ASSERT_EQ(Vec3(1, 1, 1).scalarProduct(Vec3(1, 1, 1)), 3);
        ASSERT_EQ(Vec3(1, 2, 3).scalarProduct(Vec3(1, 5, 7)), 32);
        ASSERT_EQ(Vec3(2, -1, 43).scalarProduct(Vec3(0, 8, 0.3)), 4.9);
    }

    TEST(VEC3_METHODS, Test_method_crossProduct)
    {
        ASSERT_EQ(Vec3(5, 0.3, 0.1).crossProduct(Vec3(6, 3, 2)), Vec3(0.3, -9.4, 13.2));
        ASSERT_EQ(Vec3(4, 8, 0.1).crossProduct(Vec3(26, -5, 2)), Vec3(16.5, -5.4, -228));
        ASSERT_EQ(Vec3(3, 0.18, 50).crossProduct(Vec3(13, -8, 3)), Vec3(400.54, 641, -26.34));
    }

    TEST(VEC3_METHODS, Test_method_magnitude)
    {
        ASSERT_EQ(Vec3(2, 4, -2).magnitude(), 2 * std::sqrt(6));
        ASSERT_EQ(Vec3(1, 0, -3).magnitude(), std::sqrt(10));
        ASSERT_EQ(Vec3(3, -2, -1).magnitude(), std::sqrt(14));
        ASSERT_EQ(Vec3(3, 4, 5).magnitude(), 5 * std::sqrt(2));
        ASSERT_EQ(Vec3(-2, 2, 1).magnitude(), 3);
    }

    TEST(VEC3_METHODS, Test_method_magnitude_squared)
    {
        ASSERT_EQ(Vec3(2, 4, -2).magnitudeSquared(), 24);
        ASSERT_EQ(Vec3(1, 0, -3).magnitudeSquared(), 10);
        ASSERT_EQ(Vec3(3, -2, -1).magnitudeSquared(), 14);
        ASSERT_EQ(Vec3(3, 4, 5).magnitudeSquared(), 50);
        ASSERT_EQ(Vec3(-2, 2, 1).magnitudeSquared(), 9);
    }

    TEST(VEC3_METHODS, Test_method_normalize)
    {
        ASSERT_EQ(Vec3(4, 0, 0).normalize(), Vec3(1, 0, 0));
        ASSERT_EQ(Vec3(0, 4, 0).normalize(), Vec3(0, 1, 0));
        ASSERT_EQ(Vec3(0, 0, 4).normalize(), Vec3(0, 0, 1));
        ASSERT_EQ(Vec3(4, 0, 2).normalize(), Vec3(2 / std::sqrt(5), 0, 1 / sqrt(5)));
    }

    TEST(VEC3_METHODS, Test_Getters_Setters)
    {
        ASSERT_EQ(Vec3(0, 1, 0).x(), 0);
        ASSERT_EQ(Vec3(0.543, std::sqrt(2), std::sqrt(3)).y(), std::sqrt(2));
        ASSERT_EQ(Vec3(0.2, 0.1, 0.12312453214).z(), 0.12312453214);

        Vec3 temp1 = Vec3(0, 12, 0);
        temp1.setY(0);
        Vec3 temp2 = Vec3(123, 1245, 123);
        temp2.setX(0);
        Vec3 temp3 = Vec3(0, std::sqrt(2), 0);
        temp3.setX(5);

        ASSERT_EQ(temp1, Vec3(0, 0, 0));
        ASSERT_EQ(temp2, Vec3(0, 1245, 123));
        ASSERT_EQ(temp3, Vec3(5, std::sqrt(2), 0));
    }

} // namespace core
