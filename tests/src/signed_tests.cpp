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

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone functions
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t, typename uint_t>
void run_signed_128_sign_and_negate_tests()
{
    // sign() for key values

    static_assert(int_t(0).sign() == false);
    static_assert(int_t(1).sign() == false);
    static_assert(int_t(-1).sign() == true);

    // sign() for minimum and maximum values

    constexpr int_t int_min = int_t(uint_t(1) << 127);
    constexpr int_t int_max = int_t(~(uint_t(1) << 127));
    static_assert(int_min.sign() == true);
    static_assert(int_max.sign() == false);

    // negate on temporaries

    static_assert(int_t(42).negate() == int_t(-42));
    static_assert(int_t(-42).negate() == int_t(42));
    static_assert(int_t(0).negate() == int_t(0));

    // double negate is identity

    static_assert(int_t(0xdead).negate().negate() == int_t(0xdead));

    // minimum value negated wraps back to itself in two's complement

    static_assert(int_t(int_min).negate() == int_min);

    // unary minus == negate for signed

    constexpr int_t sign_test_val = int_t(0x12345678u);
    static_assert(-sign_test_val == int_t(-0x12345678));

    // runtime negate result

    int_t mutable_val = 12345;
    mutable_val = mutable_val.negate();
    ASSERT_EQ(mutable_val, int_t(-12345));

    mutable_val = mutable_val.negate();
    ASSERT_EQ(mutable_val, int_t(12345));

    // negating the minimum value at runtime

    int_t min_val_mutable = int_min;
    min_val_mutable = min_val_mutable.negate();
    ASSERT_EQ(min_val_mutable, int_min);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t, typename uint_t>
void run_signed_128_comparison_tests()
{
    constexpr int_t int_min = int_t(uint_t(1) << 127);
    constexpr int_t int_max = int_t(~(uint_t(1) << 127));

    // three-way ordering: minimum < 0 < maximum

    static_assert(int_min < int_t(0));
    static_assert(int_t(0) < int_max);
    static_assert(int_min < int_max);
    static_assert(!(int_t(0) < int_min));
    static_assert(!(int_max < int_t(0)));

    // minimum and maximum values relative to +/-1

    static_assert(int_min < int_t(-1));
    static_assert(int_t(-1) < int_t(0));
    static_assert(int_t(0) < int_t(1));
    static_assert(int_t(1) < int_max);

    // symmetry of standalone comparison operators

    static_assert(-1 < int_t(0));
    static_assert(0 < int_t(1));
    static_assert(int_t(-1) < 0);
    static_assert(int_t(0) > -1);

    // equality

    static_assert(int_t(0) == 0);
    static_assert(0 == int_t(0));
    static_assert(int_t(-1) == -1);
    static_assert(-1 == int_t(-1));

    // signed comparison with large positive: both words nonzero

    constexpr int_t big_pos = int_t((uint_t(0x7fffffffffffffffull) << 64) + uint_t(0xffffffffffffffffull));
    constexpr int_t big_neg = -big_pos;
    static_assert(big_neg < int_t(0));
    static_assert(big_pos > int_t(0));
    static_assert(big_neg < big_pos);

}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t, typename uint_t>
void run_signed_256_comparison_tests()
{
    constexpr int_t int_min = int_t(uint_t(1) << 255);
    constexpr int_t int_max = int_t(~(uint_t(1) << 255));

    static_assert(int_min < int_t(0));
    static_assert(int_t(0) < int_max);
    static_assert(int_min < int_max);

}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t, typename uint_t>
void run_signed_128_extreme_value_operation_tests()
{
    constexpr int_t int_min = int_t(uint_t(1) << 127);
    constexpr int_t int_max = int_t(~(uint_t(1) << 127));

    // adding and subtracting around the signed boundaries

    static_assert(int_min + 1 == -int_max);
    static_assert(int_max - 1 == int_t((uint_t(0x7fffffffffffffffull) << 64) + uint_t(0xfffffffffffffffeull)));
    static_assert(int_max + int_t(-1) == int_max - 1);
    static_assert(int_min - int_t(-1) == int_min + 1);

    const int_t int_min_r = int_t(uint_t(1) << 127);
    const int_t int_max_r = int_t(~(uint_t(1) << 127));
    ASSERT_EQ(int_min_r + 1, -int_max_r);
    ASSERT_EQ(int_max_r - 1, int_t((uint_t(0x7fffffffffffffffull) << 64) + uint_t(0xfffffffffffffffeull)));
    ASSERT_EQ(int_max_r + int_t(-1), int_max_r - 1);
    ASSERT_EQ(int_min_r - int_t(-1), int_min_r + 1);

    // division by 1 and -1 on boundary values

    static_assert(int_max / int_t(1) == int_max);
    static_assert(int_min / int_t(1) == int_min);
    static_assert(int_max / int_t(-1) == -int_max);
    static_assert(int_min / int_t(-1) == int_min);

    ASSERT_EQ(int_max_r / int_t(1), int_max_r);
    ASSERT_EQ(int_min_r / int_t(1), int_min_r);
    ASSERT_EQ(int_max_r / int_t(-1), -int_max_r);
    ASSERT_EQ(int_min_r / int_t(-1), int_min_r);

    // modulo by 1 and -1 is always zero

    static_assert(int_max % int_t(1) == int_t(0));
    static_assert(int_min % int_t(1) == int_t(0));
    static_assert(int_max % int_t(-1) == int_t(0));
    static_assert(int_min % int_t(-1) == int_t(0));

    ASSERT_EQ(int_max_r % int_t(1), int_t(0));
    ASSERT_EQ(int_min_r % int_t(1), int_t(0));
    ASSERT_EQ(int_max_r % int_t(-1), int_t(0));
    ASSERT_EQ(int_min_r % int_t(-1), int_t(0));

    // multiplication by zero and one at the extremes

    static_assert(int_min * int_t(0) == int_t(0));
    static_assert(int_max * int_t(0) == int_t(0));
    static_assert(int_min * int_t(1) == int_min);
    static_assert(int_max * int_t(1) == int_max);

    ASSERT_EQ(int_min_r * int_t(0), int_t(0));
    ASSERT_EQ(int_max_r * int_t(0), int_t(0));
    ASSERT_EQ(int_min_r * int_t(1), int_min_r);
    ASSERT_EQ(int_max_r * int_t(1), int_max_r);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t, typename uint_t, size_t shift_value>
void run_signed_special_value_arithmetic_tests()
{
    constexpr int_t int_zero = 0;
    constexpr int_t int_one = 1;
    constexpr int_t int_two = 2;
    constexpr int_t int_minus_one = -1;
    constexpr uint_t uint_high_bit = uint_t(1) << shift_value;
    constexpr uint_t uint_next_high_bit = uint_t(1) << (shift_value - 1);
    constexpr uint_t uint_mid_bit = uint_t(1) << (shift_value / 2);
    constexpr int_t int_min = int_t(uint_high_bit);
    constexpr int_t int_max = int_t(~uint_high_bit);
    constexpr int_t big_pos = int_t(uint_next_high_bit + uint_mid_bit + uint_t(0x12345));
    constexpr int_t big_neg = -big_pos;
    constexpr int_t half_min = -int_t(uint_next_high_bit);

    // multiword positive and negative values

    static_assert(big_neg < int_minus_one);
    static_assert(big_pos > int_one);
    static_assert(big_neg < big_pos);
    static_assert(big_pos + big_neg == int_zero);

    ASSERT_LT(big_neg, int_minus_one);
    ASSERT_GT(big_pos, int_one);
    ASSERT_LT(big_neg, big_pos);
    ASSERT_EQ(big_pos + big_neg, int_zero);

    // arithmetic around the boundary values

    static_assert(int_min + int_two == -(int_max - int_one));
    static_assert(int_max + int_max == int_t(-2));
    static_assert(int_min + int_min == int_zero);
    static_assert(int_min / int_two == half_min);
    static_assert(int_min % int_two == int_zero);

    ASSERT_EQ(int_min + int_two, -(int_max - int_one));
    ASSERT_EQ(int_max + int_max, int_t(-2));
    ASSERT_EQ(int_min + int_min, int_zero);
    ASSERT_EQ(int_min / int_two, half_min);
    ASSERT_EQ(int_min % int_two, int_zero);

    // division and modulo on multiword negative values

    static_assert(big_neg / int_two == -(big_pos / int_two));
    static_assert(big_neg % int_two == int_minus_one);
    static_assert(big_pos % int_two == int_one);

    ASSERT_EQ(big_neg / int_two, -(big_pos / int_two));
    ASSERT_EQ(big_neg % int_two, int_minus_one);
    ASSERT_EQ(big_pos % int_two, int_one);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// signed_tests
////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(signed_tests, family_128_signed_sign_and_negate)
{
    run_signed_128_sign_and_negate_tests<int128_t, uint128_t>();
    run_signed_128_sign_and_negate_tests<int32x4_t, uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(signed_tests, family_128_signed_comparison_edge_cases)
{
    run_signed_128_comparison_tests<int128_t, uint128_t>();
    run_signed_128_comparison_tests<int32x4_t, uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(signed_tests, family_256_signed_comparison_edge_cases)
{
    run_signed_256_comparison_tests<int256_t, uint256_t>();
    run_signed_256_comparison_tests<int32x8_t, uint32x8_t>();
    run_signed_256_comparison_tests<int64x4_t, uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(signed_tests, family_128_signed_extreme_value_operations)
{
    run_signed_128_extreme_value_operation_tests<int128_t, uint128_t>();
    run_signed_128_extreme_value_operation_tests<int32x4_t, uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(signed_tests, family_256_signed_special_value_arithmetic)
{
    run_signed_special_value_arithmetic_tests<int256_t, uint256_t, 255>();
    run_signed_special_value_arithmetic_tests<int32x8_t, uint32x8_t, 255>();
    run_signed_special_value_arithmetic_tests<int64x4_t, uint64x4_t, 255>();
}

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// signed_tests.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////
