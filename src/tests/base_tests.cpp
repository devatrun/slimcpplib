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
// Copyright (c) 2021-2026 Yury Kalmykov <y_kalmykov@mail.ru>.
//
// Permission is hereby  granted, free of charge, to any  person obtaining a copy
// of this software and associated  documentation files (the "Software"), to deal
// in the Software  without restriction, including without  limitation the rights
// to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
// copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "type_sets.h"

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone functions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename uint_t>
void run_unsigned_128_initialization_and_conversion_tests()
{
    // construct long integer initialized by zero

    constexpr auto zero_init = uint_t();
    static_assert(zero_init == 0);

    // construct long integer from unsigned integer value

    constexpr uint_t from_uint = 1u;
    constexpr uint_t from_intmax = ~std::intmax_t(0);

    static_assert(from_uint == 1);
    static_assert(from_intmax == -1);

    // construct long integer from signed integer value

    constexpr uint_t from_neg = -1;
    static_assert(from_neg == -1);

    // construct long integer from shorter long integer value

    constexpr auto low_word_value = uint_t(native_word_t<uint_t>(0x11223344u));

    constexpr auto narrowed_to_uint32 = static_cast<uint32_t>(low_word_value);
    static_assert(narrowed_to_uint32 == 0x11223344u);

    // construct long integer from boolean value

    constexpr uint_t from_true = true;
    static_assert(from_true == 1);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t, typename uint_t>
void run_signed_128_initialization_and_conversion_tests()
{
    // construct long integer from signed integer value

    constexpr int_t from_neg = -1;
    static_assert(from_neg == -1);

    // construct long integer from long integer with inverse sign

    constexpr uint_t from_int_neg = int_t(-1);
    constexpr int_t from_uint_neg = uint_t(-1);
    
    static_assert(from_uint_neg == -1);
    static_assert(from_int_neg == -1);

    // construct long integer from boolean value

    constexpr int_t from_false = false;
    static_assert(from_false == 0);

}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_unsigned_256_initialization_and_conversion_tests()
{
    // construct long integer from unsigned integer value

    constexpr uint_t from_uint = 1u;
    static_assert(from_uint == 1);

    // construct long integer from longer long integer value

    constexpr auto narrowed_to_int = static_cast<int>(uint_t(1));
    static_assert(narrowed_to_int == 1);

    // construct long integer from long integer constant

    constexpr auto large_literal = uint_t(-1);
    static_assert(large_literal == uint_t(-1));

}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t, typename narrow_int_t>
void run_signed_256_initialization_and_conversion_tests()
{
    // construct long integer from shorter long integer value

    constexpr int_t wide_from_narrow_neg = narrow_int_t(-1);
    static_assert(wide_from_narrow_neg == -1);

    // construct long integer from boolean value

    constexpr int_t from_false = false;
    static_assert(from_false == 0);

}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_unsigned_128_swap_and_comparison_tests()
{
    // swap unsigned values

    const uint_t left_expected = (uint_t(0xfedcba9876543210ull) << 64) + uint_t(0xffeeddccbbaa9988ull);
    const uint_t right_expected = (uint_t(0x0123456789abcdefull) << 64) + uint_t(0x0011223344556677ull);

    uint_t left_runtime = right_expected;
    uint_t right_runtime = left_expected;
    
    left_runtime.swap(right_runtime);

    ASSERT_EQ(left_runtime, left_expected);
    ASSERT_EQ(right_runtime, right_expected);

    // compare long unsigned integers

    constexpr auto zero = uint_t(0);
    static_assert(zero == 0);
    static_assert(zero != 1);
    static_assert(zero < -1);
    static_assert(zero <= 0);
    static_assert(-1 > zero);
    static_assert(-1 >= zero);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t, typename uint_t>
void run_signed_128_swap_and_comparison_tests()
{
    // swap signed values

    const int_t left_expected = int_t(0x13579bdf2468ace0ull);
    const int_t right_expected = -int_t(0x1234567890abcdefull);

    int_t left_runtime = right_expected;
    int_t right_runtime = left_expected;

    left_runtime.swap(right_runtime);

    ASSERT_EQ(left_runtime, left_expected);
    ASSERT_EQ(right_runtime, right_expected);

    // compare long signed integers

    constexpr auto zero = int_t(0);
    static_assert(zero == 0);
    static_assert(zero != 1);
    static_assert(-1 < zero);
    static_assert(-1 <= zero);
    static_assert(zero > -1);
    static_assert(zero >= 0);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_unsigned_256_bitwise_tests()
{
    constexpr uint_t zero = 0;
    constexpr uint_t ones = -1;

    // inverse all bits

    static_assert(~zero == -1);
    static_assert(~ones == 0);

    // perform AND, OR and XOR operations

    static_assert((zero & ones) == 0);
    static_assert((zero | ones) == -1);
    static_assert((zero ^ ones) == -1);

    ASSERT_EQ(~uint_t(0), uint_t(-1));
    ASSERT_EQ(uint_t(0) | uint_t(-1), uint_t(-1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_unsigned_128_bit_shift_tests()
{
    // shift argument has native integer type

    constexpr uint_t bits = -1;
    constexpr uint_t shift = 127;

    static_assert((bits >> static_cast<int>(shift)) == 1);
    static_assert((bits << 127) == (uint_t(0x8000000000000000ull) << 64));

    ASSERT_EQ(bits >> static_cast<int>(shift), uint_t(1));
    ASSERT_EQ(bits << 127, uint_t(0x8000000000000000ull) << 64);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_128_arithmetic_operation_tests()
{
    constexpr auto uvalue = (uint_t(0xf473e8e5f6e812c3ull) << 64) + uint_t(0xfde4523b51b6d251ull);
    constexpr auto svalue = -int_t((uint_t(0xf473e8e5f6e812c3ull) << 64) + uint_t(0xfde4523b51b6d251ull));

    // unary plus

    static_assert(+uvalue == uvalue);
    static_assert(+svalue == svalue);

    // increment and decrement

    static_assert(++uint_t(uvalue) == uvalue + 1);
    static_assert(uint_t(uvalue)++ == uvalue);
    static_assert(--uint_t(uvalue) == uvalue - 1);
    static_assert(uint_t(uvalue)-- == uvalue);
    static_assert(++int_t(svalue) == svalue + 1);
    static_assert(int_t(svalue)++ == svalue);
    static_assert(--int_t(svalue) == svalue - 1);
    static_assert(int_t(svalue)-- == svalue);

    // multiplication, division and modulo

    static_assert(uvalue * 1 == uvalue);
    static_assert(svalue * 1 == svalue);
    static_assert(uvalue / 1 == uvalue);
    static_assert(svalue / 1 == svalue);
    static_assert(uvalue % 1 == 0);
    static_assert(svalue % 1 == 0);

    ASSERT_EQ(++uint_t(uvalue), uvalue + 1);
    ASSERT_EQ(uint_t(uvalue)++, uvalue);
    ASSERT_EQ(--uint_t(uvalue), uvalue - 1);
    ASSERT_EQ(uint_t(uvalue)--, uvalue);
    ASSERT_EQ(++int_t(svalue), svalue + 1);
    ASSERT_EQ(int_t(svalue)++, svalue);
    ASSERT_EQ(--int_t(svalue), svalue - 1);
    ASSERT_EQ(int_t(svalue)--, svalue);
    ASSERT_EQ(uvalue * 1, uvalue);
    ASSERT_EQ(svalue * 1, svalue);
    ASSERT_EQ(uvalue / 1, uvalue);
    ASSERT_EQ(svalue / 1, svalue);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_128_muldiv_tests()
{
    // multiplication then division operation

    constexpr auto uvalue = (uint_t(0xf473e8e5f6e812c3ull) << 64) + uint_t(0xfde4523b51b6d251ull);
    constexpr auto svalue = -int_t((uint_t(0xf473e8e5f6e812c3ull) << 64) + uint_t(0xfde4523b51b6d251ull));

    ASSERT_EQ(muldiv(uvalue, uvalue, uvalue), uvalue);
    ASSERT_EQ(muldiv(svalue, svalue, svalue), svalue);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename wide_uint_t, typename wide_int_t, typename narrow_uint_t, typename narrow_int_t>
void run_wide_cross_width_conversion_tests()
{
    const wide_uint_t wide_unsigned_value = (wide_uint_t(0x0123456789abcdefull) << 192)
                                          + (wide_uint_t(0xfedcba9876543210ull) << 128)
                                          + (wide_uint_t(0x13579bdf2468ace0ull) << 64)
                                          + wide_uint_t(0x0badf00dcafebeefull);
    const wide_int_t wide_signed_negative = -wide_int_t((wide_uint_t(0x1234567890abcdefull) << 64) + wide_uint_t(0x0fedcba987654321ull));
    const wide_int_t wide_signed_positive = wide_int_t((wide_uint_t(0x1234567890abcdefull) << 64) + wide_uint_t(0x0fedcba987654321ull));

    // narrowing keeps the low bits

    const narrow_uint_t narrowed_unsigned = static_cast<narrow_uint_t>(wide_unsigned_value);
    ASSERT_EQ(narrowed_unsigned, narrow_uint_t((narrow_uint_t(0x13579bdf2468ace0ull) << 64) + narrow_uint_t(0x0badf00dcafebeefull)));

    // widening restores the narrowed unsigned value in the low half

    const wide_uint_t widened_unsigned = wide_uint_t(narrowed_unsigned);
    ASSERT_EQ(widened_unsigned, wide_uint_t((narrow_uint_t(0x13579bdf2468ace0ull) << 64) + narrow_uint_t(0x0badf00dcafebeefull)));

    // signed widening preserves sign extension

    const wide_int_t widened_negative = wide_int_t(narrow_int_t(-1));
    ASSERT_EQ(widened_negative, wide_int_t(-1));

    // narrowing signed values keeps the low half representation

    const narrow_int_t narrowed_negative = static_cast<narrow_int_t>(wide_signed_negative);
    ASSERT_EQ(wide_int_t(narrowed_negative), wide_signed_negative);

    const narrow_int_t narrowed_positive = static_cast<narrow_int_t>(wide_signed_positive);
    ASSERT_EQ(wide_int_t(narrowed_positive), wide_signed_positive);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_pre_post_increment_decrement_tests()
{
    constexpr uint_t uint_value = (uint_t(0x1234567890abcdefull) << 64) + uint_t(0x0fedcba987654321ull);
    constexpr int_t int_value = -int_t((uint_t(0x1234567890abcdefull) << 64) + uint_t(0x0fedcba987654321ull));

    // prefix/postfix on representative values

    static_assert(++uint_t(uint_value) == uint_value + 1);
    static_assert(uint_t(uint_value)++ == uint_value);
    static_assert(--uint_t(uint_value) == uint_value - 1);
    static_assert(uint_t(uint_value)-- == uint_value);
    static_assert(++int_t(int_value) == int_value + 1);
    static_assert(int_t(int_value)++ == int_value);
    static_assert(--int_t(int_value) == int_value - 1);
    static_assert(int_t(int_value)-- == int_value);

    const uint_t uint_value_r = uint_value;
    const int_t int_value_r = int_value;
    ASSERT_EQ(++uint_t(uint_value_r), uint_value_r + 1);
    ASSERT_EQ(uint_t(uint_value_r)++, uint_value_r);
    ASSERT_EQ(--uint_t(uint_value_r), uint_value_r - 1);
    ASSERT_EQ(uint_t(uint_value_r)--, uint_value_r);
    ASSERT_EQ(++int_t(int_value_r), int_value_r + 1);
    ASSERT_EQ(int_t(int_value_r)++, int_value_r);
    ASSERT_EQ(--int_t(int_value_r), int_value_r - 1);
    ASSERT_EQ(int_t(int_value_r)--, int_value_r);

    // postfix returns the original value while still mutating the operand

    uint_t uint_postfix_value = uint_value_r;
    const uint_t uint_postfix_old = uint_postfix_value++;
    ASSERT_EQ(uint_postfix_old, uint_value_r);
    ASSERT_EQ(uint_postfix_value, uint_value_r + 1);

    int_t int_postfix_value = int_value_r;
    const int_t int_postfix_old = int_postfix_value--;
    ASSERT_EQ(int_postfix_old, int_value_r);
    ASSERT_EQ(int_postfix_value, int_value_r - 1);

}



////////////////////////////////////////////////////////////////////////////////////////////////////
// base_tests
////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(base_tests, family_128_unsigned_initialization_and_conversion)
{
    run_unsigned_128_initialization_and_conversion_tests<uint128_t>();
    run_unsigned_128_initialization_and_conversion_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_tests, family_128_signed_initialization_and_conversion)
{
    run_signed_128_initialization_and_conversion_tests<int128_t, uint128_t>();
    run_signed_128_initialization_and_conversion_tests<int32x4_t, uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_tests, family_256_unsigned_initialization_and_conversion)
{
    run_unsigned_256_initialization_and_conversion_tests<uint256_t>();
    run_unsigned_256_initialization_and_conversion_tests<uint32x8_t>();
    run_unsigned_256_initialization_and_conversion_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_tests, family_256_signed_initialization_and_conversion)
{
    run_signed_256_initialization_and_conversion_tests<int256_t, int128_t>();
    run_signed_256_initialization_and_conversion_tests<int32x8_t, int32x4_t>();
    run_signed_256_initialization_and_conversion_tests<int64x4_t, int128_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_tests, family_128_unsigned_swap_and_comparison)
{
    run_unsigned_128_swap_and_comparison_tests<uint128_t>();
    run_unsigned_128_swap_and_comparison_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_tests, family_128_signed_swap_and_comparison)
{
    run_signed_128_swap_and_comparison_tests<int128_t, uint128_t>();
    run_signed_128_swap_and_comparison_tests<int32x4_t, uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_tests, family_256_unsigned_bitwise_operations)
{
    run_unsigned_256_bitwise_tests<uint256_t>();
    run_unsigned_256_bitwise_tests<uint32x8_t>();
    run_unsigned_256_bitwise_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_tests, family_128_unsigned_bit_shift_operations)
{
    run_unsigned_128_bit_shift_tests<uint128_t>();
    run_unsigned_128_bit_shift_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_tests, family_128_signed_and_unsigned_arithmetic_operations)
{
    run_128_arithmetic_operation_tests<uint128_t, int128_t>();
    run_128_arithmetic_operation_tests<uint32x4_t, int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_tests, family_128_signed_and_unsigned_muldiv_operation)
{
    run_128_muldiv_tests<uint128_t, int128_t>();
    run_128_muldiv_tests<uint32x4_t, int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_tests, family_256_signed_and_unsigned_cross_width_conversions)
{
    run_wide_cross_width_conversion_tests<uint256_t, int256_t, uint128_t, int128_t>();
    run_wide_cross_width_conversion_tests<uint32x8_t, int32x8_t, uint32x4_t, int32x4_t>();
    run_wide_cross_width_conversion_tests<uint64x4_t, int64x4_t, uint128_t, int128_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_tests, family_256_signed_and_unsigned_pre_post_increment_decrement)
{
    run_pre_post_increment_decrement_tests<uint256_t, int256_t>();
    run_pre_post_increment_decrement_tests<uint32x8_t, int32x8_t>();
    run_pre_post_increment_decrement_tests<uint64x4_t, int64x4_t>();
}

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// base_tests.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////
