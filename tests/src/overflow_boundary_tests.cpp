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

#include <cstddef>
#include <tuple>

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone functions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename uint_t>
void run_unsigned_128_overflow_wraparound_tests()
{
    // adding 1 to the maximum wraps to zero

    constexpr uint_t uint_max = uint_t(-1);
    static_assert(uint_max + 1 == 0);
    static_assert(uint_max + uint_t(1) == 0);

    const uint_t uint_max_r = uint_t(-1);
    ASSERT_EQ(uint_max_r + 1, uint_t(0));
    ASSERT_EQ(uint_max_r + uint_t(1), uint_t(0));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_unsigned_256_overflow_wraparound_tests()
{
    // subtracting 1 from zero wraps to the maximum

    constexpr uint_t uint_max = uint_t(-1);
    static_assert(uint_t(0) - 1 == uint_max);

    const uint_t uint_max_r = uint_t(-1);
    ASSERT_EQ(uint_t(0) - 1, uint_max_r);

    // carry ripples through all 4 words

    constexpr uint_t all_ones = uint_t(-1);
    static_assert(all_ones + 1 == 0);

    ASSERT_EQ(uint_max_r + 1, uint_t(0));

    // partial carry: only words 0 and 1 are at maximum

    constexpr uint_t partial = (uint_t(1) << (2 * word_bits<uint_t>)) - 1;
    constexpr uint_t after_carry = partial + 1;
    static_assert(after_carry.digits[0] == 0);
    static_assert(after_carry.digits[1] == 0);
    static_assert(after_carry.digits[2] == 1);
    static_assert(after_carry.digits[3] == 0);

    const uint_t partial_r = partial;
    ASSERT_EQ((partial_r + 1).digits[0], 0ull);
    ASSERT_EQ((partial_r + 1).digits[1], 0ull);
    ASSERT_EQ((partial_r + 1).digits[2], 1ull);
    ASSERT_EQ((partial_r + 1).digits[3], 0ull);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t, typename uint_t>
void run_signed_128_overflow_wraparound_tests()
{
    // maximum + 1 wraps to minimum in two's complement

    constexpr int_t int_max = int_t(~(uint_t(1) << 127));
    constexpr int_t int_min = int_t(uint_t(1) << 127);
    static_assert(int_max + 1 == int_min);
    static_assert(int_min - 1 == int_max);

    const int_t int_max_r = int_t(~(uint_t(1) << 127));
    const int_t int_min_r = int_t(uint_t(1) << 127);
    ASSERT_EQ(int_max_r + 1, int_min_r);
    ASSERT_EQ(int_min_r - 1, int_max_r);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_128_boundary_value_tests()
{
    constexpr uint_t uint_max = uint_t(-1);
    constexpr int_t int_min = int_t(uint_t(1) << 127);
    constexpr int_t int_max = int_t(~(uint_t(1) << 127));

    // uint maximum: all bits set

    static_assert(uint_max.digits[0] == native_word_t<uint_t>(-1));
    static_assert(uint_max.digits[word_count<uint_t> - 1] == native_word_t<uint_t>(-1));

    const uint_t uint_max_r = uint_t(-1);
    ASSERT_EQ(uint_max_r.digits[0], native_word_t<uint_t>(-1));
    ASSERT_EQ(uint_max_r.digits[word_count<uint_t> - 1], native_word_t<uint_t>(-1));

    // int minimum: only the sign bit set

    static_assert(int_min.digits[0] == 0);
    static_assert(int_min.digits[word_count<uint_t> - 1] == (native_word_t<uint_t>(1) << (word_bits<uint_t> - 1)));

    const int_t int_min_r = int_t(uint_t(1) << 127);
    ASSERT_EQ(int_min_r.digits[0], 0ull);
    ASSERT_EQ(int_min_r.digits[word_count<uint_t> - 1], (native_word_t<uint_t>(1) << (word_bits<uint_t> - 1)));

    // int maximum: all bits set except the sign bit

    static_assert(int_max.digits[0] == native_word_t<uint_t>(-1));
    static_assert(int_max.digits[word_count<uint_t> - 1] == ~(native_word_t<uint_t>(1) << (word_bits<uint_t> - 1)));

    const int_t int_max_r = int_t(~(uint_t(1) << 127));
    ASSERT_EQ(int_max_r.digits[0], native_word_t<uint_t>(-1));
    ASSERT_EQ(int_max_r.digits[word_count<uint_t> - 1], ~(native_word_t<uint_t>(1) << (word_bits<uint_t> - 1)));

    // ordering invariants

    static_assert(uint_t(0) < uint_max);
    static_assert(int_min < int_t(0));
    static_assert(int_t(0) < int_max);
    static_assert(int_min < int_max);

    ASSERT_LT(uint_t(0), uint_max_r);
    ASSERT_LT(int_min_r, int_t(0));
    ASSERT_LT(int_t(0), int_max_r);
    ASSERT_LT(int_min_r, int_max_r);

    // incrementing and decrementing around boundaries

    static_assert(++uint_t(uint_max) == 0);
    static_assert(--int_t(int_min) == int_max);

    ASSERT_EQ(++uint_t(uint_max_r), uint_t(0));
    ASSERT_EQ(--int_t(int_min_r), int_max_r);

    // maximum value as arithmetic operand

    static_assert(uint_max * 1 == uint_max);
    static_assert(uint_max / uint_max == 1);
    static_assert(uint_max % uint_max == 0);
    static_assert(uint_max - uint_max == 0);

    ASSERT_EQ(uint_max_r * 1, uint_max_r);
    ASSERT_EQ(uint_max_r / uint_max_r, uint_t(1));
    ASSERT_EQ(uint_max_r % uint_max_r, uint_t(0));
    ASSERT_EQ(uint_max_r - uint_max_r, uint_t(0));

    // uint(-1) % 3 == 0

    static_assert(uint_max % 3 == 0);
    ASSERT_EQ(uint_max_r % 3, uint_t(0));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t, typename uint_t>
void run_signed_256_boundary_value_tests()
{
    constexpr int_t int_min = int_t(uint_t(1) << 255);
    constexpr int_t int_max = int_t(~(uint_t(1) << 255));

    static_assert(int_min < int_t(0));
    static_assert(int_t(0) < int_max);

    const int_t int_min_r = int_t(uint_t(1) << 255);
    const int_t int_max_r = int_t(~(uint_t(1) << 255));
    ASSERT_LT(int_min_r, int_t(0));
    ASSERT_LT(int_t(0), int_max_r);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_256_boundary_transition_tests()
{
    constexpr uint_t uint_max = uint_t(-1);
    constexpr int_t int_min = int_t(uint_t(1) << 255);
    constexpr int_t int_max = int_t(~(uint_t(1) << 255));
    constexpr uint_t uint_high_bit = uint_t(1) << 255;
    constexpr uint_t uint_high_bit_minus_one = uint_high_bit - 1;

    // unsigned increment/decrement and compound operators at boundaries

    static_assert(++uint_t(uint_max) == 0);
    static_assert(--uint_t(0) == uint_max);
    static_assert((uint_high_bit_minus_one + 1) == uint_high_bit);
    static_assert((uint_high_bit - 1) == uint_high_bit_minus_one);

    const uint_t uint_max_r = uint_t(-1);
    const uint_t uint_high_bit_r = uint_t(1) << 255;
    const uint_t uint_high_bit_minus_one_r = uint_high_bit_r - 1;
    ASSERT_EQ(++uint_t(uint_max_r), uint_t(0));
    ASSERT_EQ(--uint_t(0), uint_max_r);
    ASSERT_EQ(uint_high_bit_minus_one_r + 1, uint_high_bit_r);
    ASSERT_EQ(uint_high_bit_r - 1, uint_high_bit_minus_one_r);

    uint_t uint_add_assign_r = uint_high_bit_minus_one_r;
    uint_add_assign_r += 1;
    ASSERT_EQ(uint_add_assign_r, uint_high_bit_r);

    uint_t uint_sub_assign_r = uint_high_bit_r;
    uint_sub_assign_r -= 1;
    ASSERT_EQ(uint_sub_assign_r, uint_high_bit_minus_one_r);

    // signed increment/decrement and compound operators at boundaries

    static_assert(++int_t(int_max) == int_min);
    static_assert(--int_t(int_min) == int_max);
    static_assert((int_max - 1) < int_max);
    static_assert((int_min + 1) > int_min);

    const int_t int_min_r = int_t(uint_t(1) << 255);
    const int_t int_max_r = int_t(~(uint_t(1) << 255));
    ASSERT_EQ(++int_t(int_max_r), int_min_r);
    ASSERT_EQ(--int_t(int_min_r), int_max_r);
    ASSERT_LT(int_max_r - 1, int_max_r);
    ASSERT_GT(int_min_r + 1, int_min_r);

    int_t int_add_assign_r = int_max_r;
    int_add_assign_r += 1;
    ASSERT_EQ(int_add_assign_r, int_min_r);

    int_t int_sub_assign_r = int_min_r;
    int_sub_assign_r -= 1;
    ASSERT_EQ(int_sub_assign_r, int_max_r);

    // special values preserve ordering around zero

    static_assert(int_t(-1) < int_t(0));
    static_assert(int_t(1) > int_t(0));
    static_assert(uint_t(0) < uint_high_bit);
    ASSERT_LT(int_t(-1), int_t(0));
    ASSERT_GT(int_t(1), int_t(0));
    ASSERT_LT(uint_t(0), uint_high_bit_r);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// overflow_boundary_tests
////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(overflow_boundary_tests, family_128_unsigned_overflow_wraparound)
{
    run_unsigned_128_overflow_wraparound_tests<uint128_t>();
    run_unsigned_128_overflow_wraparound_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(overflow_boundary_tests, family_256_unsigned_overflow_wraparound)
{
    run_unsigned_256_overflow_wraparound_tests<uint256_t>();
    run_unsigned_256_overflow_wraparound_tests<uint32x8_t>();
    run_unsigned_256_overflow_wraparound_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(overflow_boundary_tests, family_128_signed_overflow_wraparound)
{
    run_signed_128_overflow_wraparound_tests<int128_t, uint128_t>();
    run_signed_128_overflow_wraparound_tests<int32x4_t, uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(overflow_boundary_tests, family_128_signed_and_unsigned_boundary_values)
{
    run_128_boundary_value_tests<uint128_t, int128_t>();
    run_128_boundary_value_tests<uint32x4_t, int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(overflow_boundary_tests, family_256_signed_boundary_values)
{
    run_signed_256_boundary_value_tests<int256_t, uint256_t>();
    run_signed_256_boundary_value_tests<int32x8_t, uint32x8_t>();
    run_signed_256_boundary_value_tests<int64x4_t, uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(overflow_boundary_tests, family_256_signed_and_unsigned_boundary_transitions)
{
    run_256_boundary_transition_tests<uint256_t, int256_t>();
    run_256_boundary_transition_tests<uint32x8_t, int32x8_t>();
    run_256_boundary_transition_tests<uint64x4_t, int64x4_t>();
}

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// overflow_boundary_tests.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////
