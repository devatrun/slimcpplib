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

#include "type_sets.h"

#include <gtest/gtest.h>

#include <cstddef>
#include <tuple>

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone functions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename uint_t>
void run_128_multiplication_edge_case_tests()
{
    // multiply by 0

    constexpr uint_t large = (uint_t(0xf473e8e5f6e812c3ull) << 64) + uint_t(0xfde4523b51b6d251ull);
    static_assert(large * 0 == 0);
    static_assert(0 * large == 0);
    static_assert(uint_t(0) * uint_t(0) == 0);

    const uint_t large_r = large;
    ASSERT_EQ(large_r * 0, uint_t(0));
    ASSERT_EQ(uint_t(0) * large_r, uint_t(0));

    // multiply by 1

    static_assert(large * 1 == large);
    static_assert(1 * large == large);

    ASSERT_EQ(large_r * 1, large_r);
    ASSERT_EQ(uint_t(1) * large_r, large_r);

    // uint MAX * 2

    constexpr uint_t uint_max = uint_t(-1);
    static_assert((uint_max * 2).digits[0] == native_word_t<uint_t>(native_word_t<uint_t>(-1) - 1));

    const uint_t uint_max_r = uint_t(-1);
    ASSERT_EQ((uint_max_r * 2).digits[0], native_word_t<uint_t>(native_word_t<uint_t>(-1) - 1));

    // multiplication by powers of two

    constexpr uint_t shift_equiv_val = (uint_t(1) << word_bits<uint_t>) + uint_t(1);
    constexpr uint_t factor = uint_t(1) << word_bits<uint_t>;

    static_assert(shift_equiv_val * 2 == (shift_equiv_val << 1));
    static_assert(shift_equiv_val * 256 == (shift_equiv_val << 8));
    static_assert(shift_equiv_val * factor == (shift_equiv_val << word_bits<uint_t>));

    ASSERT_EQ(shift_equiv_val * 2, shift_equiv_val << 1);
    ASSERT_EQ(shift_equiv_val * 256, shift_equiv_val << 8);
    ASSERT_EQ(shift_equiv_val * factor, shift_equiv_val << word_bits<uint_t>);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t>
void run_128_signed_multiplication_edge_case_tests()
{
    // signed: (-x) * (-1) == x

    constexpr int_t neg_val = -int_t(0xdead);
    static_assert(neg_val * int_t(-1) == int_t(0xdead));

    const int_t neg_val_r = -int_t(0xdead);
    ASSERT_EQ(neg_val_r * int_t(-1), int_t(0xdead));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_256_multiplication_edge_case_tests()
{
    // word 0 max * 2, carry into word 1

    constexpr uint_t word0_max = uint_t(native_word_t<uint_t>(-1));
    static_assert((word0_max * 2).digits[0] == native_word_t<uint_t>(native_word_t<uint_t>(-1) - 1));
    static_assert((word0_max * 2).digits[1] == 1);
    static_assert((word0_max * 2).digits[2] == 0);
    static_assert((word0_max * 2).digits[3] == 0);

    const uint_t word0_max_r = word0_max;
    ASSERT_EQ((word0_max_r * 2).digits[0], native_word_t<uint_t>(native_word_t<uint_t>(-1) - 1));
    ASSERT_EQ((word0_max_r * 2).digits[1], native_word_t<uint_t>(1));
    ASSERT_EQ((word0_max_r * 2).digits[2], native_word_t<uint_t>(0));
    ASSERT_EQ((word0_max_r * 2).digits[3], native_word_t<uint_t>(0));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_128_division_edge_case_tests()
{
    // 0 / x == 0, 0 % x == 0

    constexpr uint_t nontrivial = (uint_t(0xf473e8e5f6e812c3ull) << 64) + uint_t(0xfde4523b51b6d251ull);

    static_assert(uint_t(0) / nontrivial == 0);
    static_assert(uint_t(0) % nontrivial == 0);

    ASSERT_EQ(uint_t(0) / nontrivial, uint_t(0));
    ASSERT_EQ(uint_t(0) % nontrivial, uint_t(0));

    // x / x == 1, x % x == 0

    static_assert(nontrivial / nontrivial == 1);
    static_assert(nontrivial % nontrivial == 0);

    ASSERT_EQ(nontrivial / nontrivial, uint_t(1));
    ASSERT_EQ(nontrivial % nontrivial, uint_t(0));

    // x / (x+1) == 0, x % (x+1) == x

    constexpr uint_t small = 5;

    static_assert(small / nontrivial == 0);
    static_assert(small % nontrivial == small);

    ASSERT_EQ(small / nontrivial, uint_t(0));
    ASSERT_EQ(small % nontrivial, small);

    // divisor with both halves set

    constexpr uint_t two_word_divisor = (uint_t(1) << word_bits<uint_t>) + uint_t(1);
    constexpr uint_t two_word_dividend = two_word_divisor * 2;

    static_assert(two_word_dividend / two_word_divisor == 2);
    static_assert(two_word_dividend % two_word_divisor == 0);

    ASSERT_EQ(two_word_dividend / two_word_divisor, uint_t(2));
    ASSERT_EQ(two_word_dividend % two_word_divisor, uint_t(0));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_256_division_edge_case_tests()
{
    // 2^word_bits / 3

    constexpr uint_t divisor_3 = uint_t(3);
    constexpr uint_t two_pow_word = uint_t(1) << word_bits<uint_t>;
    constexpr native_word_t<uint_t> expected_quotient = native_word_t<uint_t>(-1) / 3;

    static_assert(two_pow_word / divisor_3 == uint_t(expected_quotient));
    static_assert(two_pow_word % divisor_3 == uint_t(1));

    ASSERT_EQ(two_pow_word / divisor_3, uint_t(expected_quotient));
    ASSERT_EQ(two_pow_word % divisor_3, uint_t(1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t>
void run_128_signed_division_edge_case_tests()
{
    // truncation toward zero for all sign combinations

    static_assert(int_t(-7) / int_t(3) == int_t(-2));
    static_assert(int_t(7) / int_t(-3) == int_t(-2));
    static_assert(int_t(-7) / int_t(-3) == int_t(2));
    static_assert(int_t(7) / int_t(3) == int_t(2));

    ASSERT_EQ(int_t(-7) / int_t(3), int_t(-2));
    ASSERT_EQ(int_t(7) / int_t(-3), int_t(-2));
    ASSERT_EQ(int_t(-7) / int_t(-3), int_t(2));
    ASSERT_EQ(int_t(7) / int_t(3), int_t(2));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_128_modulo_edge_case_tests()
{
    // basic identities

    constexpr uint_t modulus = (uint_t(0xfedcba9876543211ull) << 0);

    static_assert(uint_t(0) % modulus == 0);
    static_assert(modulus % modulus == 0);
    static_assert((modulus - 1) % modulus == modulus - 1);

    ASSERT_EQ(uint_t(0) % modulus, uint_t(0));
    ASSERT_EQ(modulus % modulus, uint_t(0));
    ASSERT_EQ((modulus - 1) % modulus, modulus - 1);

    // modulo by power of 2

    constexpr uint_t dividend = uint_t(0x123456789abcdef0ull);

    static_assert(dividend % 256 == (dividend & 255));
    static_assert(dividend % 256 == uint_t(0xf0));

    ASSERT_EQ(dividend % 256, dividend & 255);
    ASSERT_EQ(dividend % 256, uint_t(0xf0));

    // modulo by 2^word_bits extracts low native word

    static_assert((dividend % (uint_t(1) << word_bits<uint_t>)) == uint_t(dividend.digits[0]));
    ASSERT_EQ(dividend % (uint_t(1) << word_bits<uint_t>), uint_t(dividend.digits[0]));

    // uint(-1) % 3 == 0

    static_assert(uint_t(-1) % 3 == 0);
    ASSERT_EQ(uint_t(-1) % 3, uint_t(0));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t>
void run_128_signed_modulo_edge_case_tests()
{
    // signed modulo follows sign of dividend

    static_assert(int_t(-7) % int_t(3) == int_t(-1));
    static_assert(int_t(-6) % int_t(3) == int_t(0));
    static_assert(int_t(7) % int_t(-3) == int_t(1));
    static_assert(int_t(-7) % int_t(-3) == int_t(-1));
    static_assert(int_t(10) % int_t(3) == int_t(1));

    ASSERT_EQ(int_t(-7) % int_t(3), int_t(-1));
    ASSERT_EQ(int_t(-6) % int_t(3), int_t(0));
    ASSERT_EQ(int_t(7) % int_t(-3), int_t(1));
    ASSERT_EQ(int_t(-7) % int_t(-3), int_t(-1));
    ASSERT_EQ(int_t(10) % int_t(3), int_t(1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_128_muldiv_extended_tests()
{
    constexpr uint_t uint_val = (uint_t(0xf473e8e5f6e812c3ull) << 64) + uint_t(0xfde4523b51b6d251ull);
    constexpr uint_t uint_divisor = uint_t(0xfedcba9876543210ull);
    constexpr uint_t small = 100;
    constexpr int_t sint_val = -int_t((uint_t(0xf473e8e5f6e812c3ull) << 64) + uint_t(0xfde4523b51b6d251ull));
    constexpr int_t sint_divisor = int_t(0xfedcba9876543210ull);

    // muldiv(a, b, b) == a

    ASSERT_EQ(muldiv(uint_val, uint_divisor, uint_divisor), uint_val);

    // muldiv(a, 1, 1) == a

    ASSERT_EQ(muldiv(uint_val, uint_t(1), uint_t(1)), uint_val);

    // muldiv(a, b, 1) == a * b for small a

    ASSERT_EQ(muldiv(small, uint_t(7), uint_t(7)), small);

    // signed muldiv

    ASSERT_EQ(muldiv(sint_val, sint_divisor, sint_divisor), sint_val);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_128_arithmetic_invariant_tests()
{
    // unsigned additive inverse

    constexpr uint_t ua = (uint_t(0x1234567890abcdefull) << 64) + uint_t(0xfedcba9876543210ull);
    constexpr uint_t ub = (uint_t(0x0fedcba987654321ull) << 64) + uint_t(0x0123456789abcdefull);

    static_assert((ua + ub) - ub == ua);
    static_assert((ua - ub) + ub == ua);

    ASSERT_EQ((ua + ub) - ub, ua);
    ASSERT_EQ((ua - ub) + ub, ua);

    // signed additive inverse

    constexpr int_t sa = -int_t(0x1234567890abcdefull);
    constexpr int_t sb = int_t(0x13579bdf2468ace0ull);

    static_assert((sa + sb) - sb == sa);
    static_assert((sa - sb) + sb == sa);

    ASSERT_EQ((sa + sb) - sb, sa);
    ASSERT_EQ((sa - sb) + sb, sa);

    // division algorithm

    constexpr uint_t dividend = (uint_t(0xf473e8e5f6e812c3ull) << 64) + uint_t(0xfde4523b51b6d251ull);
    constexpr uint_t divisor = uint_t(0xfedcba9876543211ull);
    constexpr uint_t quotient = dividend / divisor;
    constexpr uint_t remainder = dividend % divisor;

    static_assert(quotient * divisor + remainder == dividend);
    static_assert(remainder < divisor);

    ASSERT_EQ(quotient * divisor + remainder, dividend);
    ASSERT_LT(remainder, divisor);

    // signed division algorithm

    constexpr int_t signed_dividend = -int_t(0xf473e8e5f6e812c3ull);
    constexpr int_t signed_divisor = int_t(0x12345ull);
    constexpr int_t signed_quotient = signed_dividend / signed_divisor;
    constexpr int_t signed_remainder = signed_dividend % signed_divisor;

    static_assert(signed_quotient * signed_divisor + signed_remainder == signed_dividend);
    ASSERT_EQ(signed_quotient * signed_divisor + signed_remainder, signed_dividend);

    // multiplication by powers of two matches left shift

    constexpr uint_t shift_value = (uint_t(1) << 96) + (uint_t(1) << 64) + (uint_t(1) << 32) + uint_t(1);

    static_assert(shift_value * 2 == (shift_value << 1));
    static_assert(shift_value * 16 == (shift_value << 4));
    static_assert(shift_value * (uint_t(1) << 64) == (shift_value << 64));

    ASSERT_EQ(shift_value * 2, shift_value << 1);
    ASSERT_EQ(shift_value * 16, shift_value << 4);
    ASSERT_EQ(shift_value * (uint_t(1) << 64), shift_value << 64);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t, size_t shift_value>
void run_special_value_arithmetic_tests()
{
    // unsigned arithmetic around powers of two and wraparound

    constexpr uint_t uint_zero = 0;
    constexpr uint_t uint_one = 1;
    constexpr uint_t uint_max = uint_t(-1);
    constexpr uint_t uint_high_bit = uint_t(1) << shift_value;
    constexpr uint_t uint_before_high_bit = uint_high_bit - 1;
    constexpr uint_t uint_after_high_bit = uint_high_bit + 1;
    constexpr uint_t uint_positive_power = uint_t(1) << (shift_value - 1);

    static_assert(uint_max + uint_one == uint_zero);
    static_assert(uint_before_high_bit + uint_one == uint_high_bit);
    static_assert(uint_after_high_bit - uint_one == uint_high_bit);
    static_assert(uint_high_bit * 2 == uint_zero);
    static_assert((uint_after_high_bit / uint_high_bit) == uint_one);
    static_assert((uint_after_high_bit % uint_high_bit) == uint_one);
    static_assert(uint_positive_power * 2 == uint_high_bit);

    ASSERT_EQ(uint_max + uint_one, uint_zero);
    ASSERT_EQ(uint_before_high_bit + uint_one, uint_high_bit);
    ASSERT_EQ(uint_after_high_bit - uint_one, uint_high_bit);
    ASSERT_EQ(uint_high_bit * 2, uint_zero);
    ASSERT_EQ(uint_after_high_bit / uint_high_bit, uint_one);
    ASSERT_EQ(uint_after_high_bit % uint_high_bit, uint_one);
    ASSERT_EQ(uint_positive_power * 2, uint_high_bit);

    // signed arithmetic around the sign bit

    constexpr int_t int_zero = 0;
    constexpr int_t int_one = 1;
    constexpr int_t int_minus_one = -1;
    constexpr int_t int_min = int_t(uint_high_bit);
    constexpr int_t int_max = int_t(~uint_high_bit);
    constexpr int_t int_positive_power = int_t(uint_positive_power);
    constexpr int_t int_negative_power = -int_positive_power;

    static_assert(int_max + int_one == int_min);
    static_assert(int_min - int_one == int_max);
    static_assert(int_positive_power + int_negative_power == int_zero);
    static_assert(int_negative_power * int_minus_one == int_positive_power);
    static_assert(int_negative_power / int_minus_one == int_positive_power);
    static_assert(int_negative_power % int_positive_power == int_zero);

    ASSERT_EQ(int_max + int_one, int_min);
    ASSERT_EQ(int_min - int_one, int_max);
    ASSERT_EQ(int_positive_power + int_negative_power, int_zero);
    ASSERT_EQ(int_negative_power * int_minus_one, int_positive_power);
    ASSERT_EQ(int_negative_power / int_minus_one, int_positive_power);
    ASSERT_EQ(int_negative_power % int_positive_power, int_zero);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_compound_operator_consistency_tests()
{
    // unsigned compound assignments match the corresponding binary operators

    const uint_t uint_left = (uint_t(0x0123456789abcdefull) << 192)
                           + (uint_t(0xfedcba9876543210ull) << 128)
                           + (uint_t(0x13579bdf2468ace0ull) << 64)
                           + uint_t(0x0badf00dcafebeefull);
    const uint_t uint_right = (uint_t(0x0011223344556677ull) << 64) + uint_t(0x8899aabbccddeeffull);
    const uint_t uint_divisor = (uint_t(0x1234567890abcdefull) << 64) + uint_t(0x0fedcba987654321ull);

    uint_t uint_add_assign = uint_left;
    uint_add_assign += uint_right;
    ASSERT_EQ(uint_add_assign, uint_left + uint_right);

    uint_t uint_sub_assign = uint_left;
    uint_sub_assign -= uint_right;
    ASSERT_EQ(uint_sub_assign, uint_left - uint_right);

    uint_t uint_mul_assign = uint_left;
    uint_mul_assign *= uint_t(3);
    ASSERT_EQ(uint_mul_assign, uint_left * uint_t(3));

    uint_t uint_div_assign = uint_left;
    uint_div_assign /= uint_divisor;
    ASSERT_EQ(uint_div_assign, uint_left / uint_divisor);

    uint_t uint_mod_assign = uint_left;
    uint_mod_assign %= uint_divisor;
    ASSERT_EQ(uint_mod_assign, uint_left % uint_divisor);

    // signed compound assignments preserve the same arithmetic results

    const int_t int_left = -int_t((uint_t(0x1234567890abcdefull) << 64) + uint_t(0x0fedcba987654321ull));
    const int_t int_right = int_t((uint_t(0x0011223344556677ull) << 64) + uint_t(0x8899aabbccddeeffull));
    const int_t int_divisor = int_t(3);

    int_t int_add_assign = int_left;
    int_add_assign += int_right;
    ASSERT_EQ(int_add_assign, int_left + int_right);

    int_t int_sub_assign = int_left;
    int_sub_assign -= int_right;
    ASSERT_EQ(int_sub_assign, int_left - int_right);

    int_t int_mul_assign = int_left;
    int_mul_assign *= int_t(-7);
    ASSERT_EQ(int_mul_assign, int_left * int_t(-7));

    int_t int_div_assign = int_left;
    int_div_assign /= int_divisor;
    ASSERT_EQ(int_div_assign, int_left / int_divisor);

    int_t int_mod_assign = int_left;
    int_mod_assign %= int_divisor;
    ASSERT_EQ(int_mod_assign, int_left % int_divisor);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// arithmetic_tests
////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(arithmetic_tests, family_128_signed_and_unsigned_multiplication_edge_cases)
{
    run_128_multiplication_edge_case_tests<uint128_t>();
    run_128_multiplication_edge_case_tests<uint32x4_t>();
    run_128_signed_multiplication_edge_case_tests<int128_t>();
    run_128_signed_multiplication_edge_case_tests<int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(arithmetic_tests, family_256_unsigned_multiplication_edge_cases)
{
    run_256_multiplication_edge_case_tests<uint256_t>();
    run_256_multiplication_edge_case_tests<uint32x8_t>();
    run_256_multiplication_edge_case_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(arithmetic_tests, family_128_signed_and_unsigned_division_edge_cases)
{
    run_128_division_edge_case_tests<uint128_t>();
    run_128_division_edge_case_tests<uint32x4_t>();
    run_128_signed_division_edge_case_tests<int128_t>();
    run_128_signed_division_edge_case_tests<int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(arithmetic_tests, family_256_unsigned_division_edge_cases)
{
    run_256_division_edge_case_tests<uint256_t>();
    run_256_division_edge_case_tests<uint32x8_t>();
    run_256_division_edge_case_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(arithmetic_tests, family_128_signed_and_unsigned_modulo_edge_cases)
{
    run_128_modulo_edge_case_tests<uint128_t>();
    run_128_modulo_edge_case_tests<uint32x4_t>();
    run_128_signed_modulo_edge_case_tests<int128_t>();
    run_128_signed_modulo_edge_case_tests<int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(arithmetic_tests, family_128_signed_and_unsigned_muldiv_extended)
{
    run_128_muldiv_extended_tests<uint128_t, int128_t>();
    run_128_muldiv_extended_tests<uint32x4_t, int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(arithmetic_tests, family_128_signed_and_unsigned_arithmetic_invariants)
{
    run_128_arithmetic_invariant_tests<uint128_t, int128_t>();
    run_128_arithmetic_invariant_tests<uint32x4_t, int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(arithmetic_tests, family_128_signed_and_unsigned_special_value_arithmetic)
{
    run_special_value_arithmetic_tests<uint128_t, int128_t, 127>();
    run_special_value_arithmetic_tests<uint32x4_t, int32x4_t, 127>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(arithmetic_tests, family_256_signed_and_unsigned_special_value_arithmetic)
{
    run_special_value_arithmetic_tests<uint256_t, int256_t, 255>();
    run_special_value_arithmetic_tests<uint32x8_t, int32x8_t, 255>();
    run_special_value_arithmetic_tests<uint64x4_t, int64x4_t, 255>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(arithmetic_tests, family_256_signed_and_unsigned_compound_operator_consistency)
{
    run_compound_operator_consistency_tests<uint256_t, int256_t>();
    run_compound_operator_consistency_tests<uint32x8_t, int32x8_t>();
    run_compound_operator_consistency_tests<uint64x4_t, int64x4_t>();
}

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// arithmetic_tests.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////
