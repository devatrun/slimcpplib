////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Simple Long Integer Math for C++
// version 2.0
//
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020-2026 Yury Kalmykov <y_kalmykov@mail.ru>.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "test_type_sets.h"

#include <gtest/gtest.h>

using namespace slim::literals;

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone functions
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_128_mixed_type_operation_tests()
{
    constexpr uint_t uint_hundred = 100;

    // commutativity of addition with native int

    static_assert(uint_hundred + 1 == 1 + uint_hundred);
    static_assert(uint_hundred + 50 == 50 + uint_hundred);
    const uint_t uint_hundred_r = 100;
    ASSERT_EQ(uint_hundred_r + 1, 1 + uint_hundred_r);
    ASSERT_EQ(uint_hundred_r + 50, 50 + uint_hundred_r);

    // sign extension: adding negative native int to unsigned long type
    // uint_t(100) + int(-1) = 99 (int(-1) sign-extends to all-ones, then wraps)

    static_assert(uint_hundred + int(-1) == uint_t(99));
    ASSERT_EQ(uint_hundred_r + int(-1), uint_t(99));
    static_assert(uint_hundred - int(-1) == uint_t(101));
    ASSERT_EQ(uint_hundred_r - int(-1), uint_t(101));

    // comparison: native int on left-hand side

    static_assert(-1 < int_t(0));
    static_assert(0 < int_t(1));
    static_assert(int_t(-1) < 0);
    static_assert(int_t(0) > -1);
    static_assert(0 == int_t(0));
    static_assert(int_t(0) == 0);

    // bitwise with native types (standalone operators)

    static_assert((uint_hundred & 0x0f) == uint_t(4));      // 100 & 15 == 4
    static_assert((0x0f & uint_hundred) == uint_t(4));      // commutativity
    static_assert((uint_hundred | 0x100) == uint_t(0x164)); // 100 | 256 == 356

    // arithmetic: native int on left-hand side

    static_assert(100 / uint_t(10) == uint_t(10));
    ASSERT_EQ(100 / uint_t(10), uint_t(10));
    static_assert(7 / uint_t(3) == uint_t(2));
    ASSERT_EQ(7 / uint_t(3), uint_t(2));
    static_assert(0 / uint_t(99) == uint_t(0));
    ASSERT_EQ(0 / uint_t(99), uint_t(0));
    static_assert(100 % uint_t(7) == uint_t(2));
    ASSERT_EQ(100 % uint_t(7), uint_t(2));

    // signed mixed operations

    static_assert(int_t(10) + 5 == int_t(15));
    ASSERT_EQ(int_t(10) + 5, int_t(15));
    static_assert(5 + int_t(10) == int_t(15));
    ASSERT_EQ(5 + int_t(10), int_t(15));
    static_assert(int_t(10) - 15 == int_t(-5));
    ASSERT_EQ(int_t(10) - 15, int_t(-5));
    static_assert(20 - int_t(15) == int_t(5));
    ASSERT_EQ(20 - int_t(15), int_t(5));

    // multiplication with native type

    static_assert(int_t(-7) * 3 == int_t(-21));
    ASSERT_EQ(int_t(-7) * 3, int_t(-21));
    static_assert(3 * int_t(-7) == int_t(-21));
    ASSERT_EQ(3 * int_t(-7), int_t(-21));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_256_mixed_type_operation_tests()
{
    const uint_t wide_unsigned = (uint_t(0x1234567890abcdefull) << 64) + uint_t(0x0fedcba987654321ull);
    const int_t wide_signed_positive = int_t((uint_t(0x1234567890abcdefull) << 64) + uint_t(0x0fedcba987654321ull));
    const int_t wide_signed_negative = -wide_signed_positive;

    // negative native values with wide unsigned

    ASSERT_EQ(wide_unsigned + int(-1), wide_unsigned - 1);
    ASSERT_EQ(wide_unsigned - int(-1), wide_unsigned + 1);

    // mixed division and modulo on signed wide values

    ASSERT_EQ(wide_signed_positive / 3, wide_signed_positive / int_t(3));
    ASSERT_EQ(wide_signed_negative / 3, wide_signed_negative / int_t(3));
    ASSERT_EQ(int_t(3) * (wide_signed_positive / 3) + (wide_signed_positive % 3), wide_signed_positive);
    ASSERT_EQ(int_t(3) * (wide_signed_negative / 3) + (wide_signed_negative % 3), wide_signed_negative);
    ASSERT_EQ(wide_signed_positive % 3, int_t(0));
    ASSERT_EQ(wide_signed_negative % 3, int_t(0));

    // native integral on the left with wide signed right-hand side

    ASSERT_EQ(5 + wide_signed_negative, wide_signed_negative + 5);
    ASSERT_EQ(5 - wide_signed_negative, int_t(5) - wide_signed_negative);
    ASSERT_EQ(7 * wide_signed_negative, wide_signed_negative * 7);
    ASSERT_EQ(100 % int_t(7), int_t(2));

    // comparisons against native integral values

    ASSERT_LT(wide_signed_negative, 0);
    ASSERT_GT(wide_signed_positive, 0);
    ASSERT_LT(-1, wide_signed_positive);
    ASSERT_GT(1, wide_signed_negative);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t, size_t shift_value>
void run_256_mixed_type_boundary_comparison_tests()
{
    const uint_t uint_zero = 0;
    const uint_t uint_one = 1;
    const uint_t uint_max = uint_t(-1);
    const int_t int_zero = 0;
    const int_t int_one = 1;
    const int_t int_minus_one = -1;
    const int_t int_min = int_t(uint_t(1) << shift_value);
    const int_t int_max = int_t(~(uint_t(1) << shift_value));

    // unsigned compared with native values

    ASSERT_EQ(uint_zero, 0);
    ASSERT_EQ(0, uint_zero);
    ASSERT_EQ(uint_one, 1);
    ASSERT_EQ(1, uint_one);
    ASSERT_LT(uint_zero, 1);
    ASSERT_GT(1, uint_zero);
    ASSERT_LT(0, uint_one);
    ASSERT_GT(uint_one, 0);
    ASSERT_GT(uint_max, 1);
    ASSERT_LT(1, uint_max);

    // signed compared with native values around zero

    ASSERT_EQ(int_zero, 0);
    ASSERT_EQ(0, int_zero);
    ASSERT_EQ(int_one, 1);
    ASSERT_EQ(1, int_one);
    ASSERT_EQ(int_minus_one, -1);
    ASSERT_EQ(-1, int_minus_one);
    ASSERT_LT(int_minus_one, 0);
    ASSERT_GT(0, int_minus_one);
    ASSERT_LT(-1, int_zero);
    ASSERT_GT(int_zero, -1);
    ASSERT_LT(int_zero, 1);
    ASSERT_GT(1, int_zero);

    // signed boundary values against native integers

    ASSERT_LT(int_min, -1);
    ASSERT_LT(int_min, 0);
    ASSERT_LT(int_min, 1);
    ASSERT_GT(int_max, -1);
    ASSERT_GT(int_max, 0);
    ASSERT_GT(int_max, 1);
    ASSERT_GT(0, int_min);
    ASSERT_LT(0, int_max);

    // negative native values are compared after conversion to the unsigned wide domain

    ASSERT_LT(uint_zero, -1);
    ASSERT_LT(uint_one, -1);
    ASSERT_EQ(uint_max, -1);
    ASSERT_GT(-1, uint_zero);
    ASSERT_GT(-1, uint_one);
    ASSERT_EQ(-1, uint_max);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// literal_mixed_tests
////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(literal_mixed_tests, user_defined_literals)
{
    // decimal literals with digit separators

    static_assert(1'000'000_ui128 == uint128_t(1000000));
    static_assert(1'000'000'000'000_ui256 == uint256_t(1000000000000ull));

    // hex literals

    static_assert(0xff_ui128 == uint128_t(255));
    static_assert(0xfe_ui128 == uint128_t(254));
    static_assert(0xdeadbeef_ui128 == uint128_t(0xdeadbeefull));
    static_assert(0xdeadbeef_ui256 == uint256_t(0xdeadbeefull));

    // octal literals

    static_assert(0777_ui128 == uint128_t(511));
    static_assert(010_ui128 == uint128_t(8));
    static_assert(00_ui128 == uint128_t(0));

    // large hex literal: verify correct digit placement in little-endian array
    // 0xaf5705a489525e79'a5120c42daebbc57 -> digits[0]=a5120c42daebbc57, digits[1]=af5705a489525e79

    constexpr auto big = 0xaf5705a4'89525e79'a5120c42'daebbc57_ui128;
    static_assert(big.digits[0] == 0xa5120c42daebbc57ull);
    static_assert(big.digits[1] == 0xaf5705a489525e79ull);

    // full 256-bit hex literal: verify all 4 words

    constexpr auto big256 = 0xaf5705a4'89525e79'a5120c42'daebbc57'd55f0277'53a05970'9fee8a5d'41e2ae79_ui256;
    static_assert(big256.digits[0] == 0x9fee8a5d41e2ae79ull);
    static_assert(big256.digits[1] == 0xd55f027753a05970ull);
    static_assert(big256.digits[2] == 0xa5120c42daebbc57ull);
    static_assert(big256.digits[3] == 0xaf5705a489525e79ull);

    // signed literals: parsed as unsigned, reinterpreted

    static_assert(42_si128 == int128_t(42));
    static_assert(0xff_si128 == int128_t(255));

    // decimal 256-bit literal

    static_assert(1_ui256 == uint256_t(1));
    static_assert(0_ui256 == uint256_t(0));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(literal_mixed_tests, family_128_signed_and_unsigned_mixed_type_operations)
{
    run_128_mixed_type_operation_tests<uint128_t, int128_t>();
    run_128_mixed_type_operation_tests<uint32x4_t, int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(literal_mixed_tests, family_256_signed_and_unsigned_mixed_type_operations)
{
    run_256_mixed_type_operation_tests<uint256_t, int256_t>();
    run_256_mixed_type_operation_tests<uint32x8_t, int32x8_t>();
    run_256_mixed_type_operation_tests<uint64x4_t, int64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(literal_mixed_tests, family_256_signed_and_unsigned_mixed_type_boundary_comparisons)
{
    run_256_mixed_type_boundary_comparison_tests<uint256_t, int256_t, 255>();
    run_256_mixed_type_boundary_comparison_tests<uint32x8_t, int32x8_t, 255>();
    run_256_mixed_type_boundary_comparison_tests<uint64x4_t, int64x4_t, 255>();
}

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// literal_mixed_tests.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////
