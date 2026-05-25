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

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
using native_word_t = typename uint_t::native_array_t::value_type;



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
constexpr size_t word_bits = sizeof(native_word_t<uint_t>) * 8;



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
constexpr size_t word_count = std::tuple_size<typename uint_t::native_array_t>::value;



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
constexpr size_t total_bits = word_bits<uint_t> * word_count<uint_t>;



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_128_compound_assignment_tests()
{
    // += and -= on 128-bit unsigned family

    uint_t add_target = 0xdeadbeef00000000ull;
    add_target += 0x0000000100000000ull;
    ASSERT_EQ(add_target, uint_t(0xdeadbef000000000ull));

    uint_t sub_target = 0xdeadbef000000000ull;
    sub_target -= 0x0000000100000000ull;
    ASSERT_EQ(sub_target, uint_t(0xdeadbeef00000000ull));

    // += with carry into high word

    uint_t carry_add_target = uint_t(native_word_t<uint_t>(-1));
    carry_add_target += 1;
    ASSERT_EQ(carry_add_target.digits[0], 0ull);
    ASSERT_EQ(carry_add_target.digits[1], 1ull);

    // *= and /= on 128-bit signed family

    int_t mul_target = 100;
    mul_target *= -7;
    ASSERT_EQ(mul_target, int_t(-700));

    mul_target /= 7;
    ASSERT_EQ(mul_target, int_t(-100));

    // %= on 128-bit signed family

    int_t rem_target = -7;
    rem_target %= 3;
    ASSERT_EQ(rem_target, int_t(-1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_256_compound_assignment_tests()
{
    // &=, |=, ^= on 256-bit unsigned family

    constexpr uint_t pattern = uint_t(0xaaaaaaaaaaaaaaaaull);
    uint_t bitand_target = uint_t(-1);
    bitand_target &= pattern;
    ASSERT_EQ(bitand_target, pattern);

    uint_t bitor_target = uint_t(0);
    bitor_target |= pattern;
    ASSERT_EQ(bitor_target, pattern);

    uint_t bitxor_target = pattern;
    bitxor_target ^= pattern;
    ASSERT_EQ(bitxor_target, uint_t(0));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_128_bit_shift_edge_case_tests()
{
    // shift by 0 is identity

    constexpr uint_t shift_identity_val = (uint_t(native_word_t<uint_t>(0x12345678u)) << word_bits<uint_t>) + uint_t(native_word_t<uint_t>(0xdeadbeefu));
    static_assert((shift_identity_val << 0) == shift_identity_val);
    static_assert((shift_identity_val >> 0) == shift_identity_val);
    const uint_t shift_identity_val_r = shift_identity_val;
    ASSERT_EQ(shift_identity_val_r << 0, shift_identity_val_r);
    ASSERT_EQ(shift_identity_val_r >> 0, shift_identity_val_r);

    // shift by exactly one native word

    constexpr uint_t one = 1;
    static_assert((one << word_bits<uint_t>).digits[0] == 0);
    static_assert((one << word_bits<uint_t>).digits[1] == 1);
    static_assert(((one << word_bits<uint_t>) >> word_bits<uint_t>) == 1);
    ASSERT_EQ((uint_t(1) << word_bits<uint_t>).digits[0], native_word_t<uint_t>(0));
    ASSERT_EQ((uint_t(1) << word_bits<uint_t>).digits[1], native_word_t<uint_t>(1));
    ASSERT_EQ((uint_t(1) << word_bits<uint_t>) >> word_bits<uint_t>, uint_t(1));

    // shift by the highest bit position

    static_assert((one << (total_bits<uint_t> - 1)).digits[0] == 0);
    static_assert((one << (total_bits<uint_t> - 1)).digits[word_count<uint_t> - 1] == (native_word_t<uint_t>(1) << (word_bits<uint_t> - 1)));
    static_assert(((one << (total_bits<uint_t> - 1)) >> (total_bits<uint_t> - 1)) == 1);
    ASSERT_EQ((uint_t(1) << (total_bits<uint_t> - 1)).digits[0], native_word_t<uint_t>(0));
    ASSERT_EQ((uint_t(1) << (total_bits<uint_t> - 1)).digits[word_count<uint_t> - 1], (native_word_t<uint_t>(1) << (word_bits<uint_t> - 1)));
    ASSERT_EQ((uint_t(1) << (total_bits<uint_t> - 1)) >> (total_bits<uint_t> - 1), uint_t(1));

    // logical right shift of all ones

    constexpr uint_t all_ones = uint_t(-1);
    static_assert((all_ones >> (total_bits<uint_t> - 1)) == 1);
    ASSERT_EQ(uint_t(-1) >> (total_bits<uint_t> - 1), uint_t(1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t>
void run_128_signed_shift_tests()
{
    // signed type uses the same logical shift

    constexpr int_t neg_one = int_t(-1);
    static_assert((neg_one >> (total_bits<int_t> - 1)) == 1);
    ASSERT_EQ(int_t(-1) >> (total_bits<int_t> - 1), int_t(1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t>
void run_256_signed_shift_tests()
{
    // signed type uses the same logical shift

    constexpr int_t neg_one = int_t(-1);
    static_assert((neg_one >> (total_bits<int_t> - 1)) == 1);
    ASSERT_EQ(int_t(-1) >> (total_bits<int_t> - 1), int_t(1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_256_bit_shift_edge_case_tests()
{
    // shift of zero value stays zero

    static_assert((uint_t(0) << 200) == 0);
    static_assert((uint_t(0) >> 200) == 0);
    ASSERT_EQ(uint_t(0) << 200, uint_t(0));
    ASSERT_EQ(uint_t(0) >> 200, uint_t(0));

    // shifts at word boundaries

    constexpr uint_t one = 1;
    static_assert((one << word_bits<uint_t>).digits[0] == 0 && (one << word_bits<uint_t>).digits[1] == 1 &&
                  (one << word_bits<uint_t>).digits[2] == 0 && (one << word_bits<uint_t>).digits[3] == 0);
    static_assert((one << (2 * word_bits<uint_t>)).digits[2] == 1 && (one << (2 * word_bits<uint_t>)).digits[3] == 0);
    static_assert((one << (3 * word_bits<uint_t>)).digits[3] == 1);
    const uint_t one_r = 1;
    ASSERT_EQ((one_r << word_bits<uint_t>).digits[0], native_word_t<uint_t>(0));
    ASSERT_EQ((one_r << word_bits<uint_t>).digits[1], native_word_t<uint_t>(1));
    ASSERT_EQ((one_r << word_bits<uint_t>).digits[2], native_word_t<uint_t>(0));
    ASSERT_EQ((one_r << word_bits<uint_t>).digits[3], native_word_t<uint_t>(0));
    ASSERT_EQ((one_r << (2 * word_bits<uint_t>)).digits[2], native_word_t<uint_t>(1));
    ASSERT_EQ((one_r << (2 * word_bits<uint_t>)).digits[3], native_word_t<uint_t>(0));
    ASSERT_EQ((one_r << (3 * word_bits<uint_t>)).digits[3], native_word_t<uint_t>(1));

    // round-trips at word boundaries

    static_assert(((one << word_bits<uint_t>) >> word_bits<uint_t>) == 1);
    static_assert(((one << (2 * word_bits<uint_t>)) >> (2 * word_bits<uint_t>)) == 1);
    static_assert(((one << (3 * word_bits<uint_t>)) >> (3 * word_bits<uint_t>)) == 1);
    ASSERT_EQ((one_r << word_bits<uint_t>) >> word_bits<uint_t>, uint_t(1));
    ASSERT_EQ((one_r << (2 * word_bits<uint_t>)) >> (2 * word_bits<uint_t>), uint_t(1));
    ASSERT_EQ((one_r << (3 * word_bits<uint_t>)) >> (3 * word_bits<uint_t>), uint_t(1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_256_multiword_carry_borrow_tests()
{
    // {MAX, 0, 0, 0} + 1 -> carry into word 1 only

    constexpr uint_t word0_max = uint_t(native_word_t<uint_t>(-1));
    static_assert((word0_max + 1).digits[0] == 0);
    static_assert((word0_max + 1).digits[1] == 1);
    static_assert((word0_max + 1).digits[2] == 0);
    static_assert((word0_max + 1).digits[3] == 0);
    const uint_t word0_max_r = word0_max;
    ASSERT_EQ((word0_max_r + 1).digits[0], 0ull);
    ASSERT_EQ((word0_max_r + 1).digits[1], 1ull);
    ASSERT_EQ((word0_max_r + 1).digits[2], 0ull);
    ASSERT_EQ((word0_max_r + 1).digits[3], 0ull);

    // {MAX, MAX, 0, 0} + 1 -> carry ripples through words 0 and 1

    constexpr uint_t words01_max = (uint_t(1) << (2 * word_bits<uint_t>)) - 1;
    static_assert((words01_max + 1).digits[0] == 0);
    static_assert((words01_max + 1).digits[1] == 0);
    static_assert((words01_max + 1).digits[2] == 1);
    static_assert((words01_max + 1).digits[3] == 0);
    const uint_t words01_max_r = words01_max;
    ASSERT_EQ((words01_max_r + 1).digits[0], 0ull);
    ASSERT_EQ((words01_max_r + 1).digits[1], 0ull);
    ASSERT_EQ((words01_max_r + 1).digits[2], 1ull);
    ASSERT_EQ((words01_max_r + 1).digits[3], 0ull);

    // {0, 0, 1, 0} - 1 -> borrow through words 1 and 0

    constexpr uint_t word2_one = uint_t(1) << (2 * word_bits<uint_t>);
    static_assert((word2_one - 1).digits[0] == native_word_t<uint_t>(-1));
    static_assert((word2_one - 1).digits[1] == native_word_t<uint_t>(-1));
    static_assert((word2_one - 1).digits[2] == 0);
    static_assert((word2_one - 1).digits[3] == 0);
    const uint_t word2_one_r = word2_one;
    ASSERT_EQ((word2_one_r - 1).digits[0], native_word_t<uint_t>(-1));
    ASSERT_EQ((word2_one_r - 1).digits[1], native_word_t<uint_t>(-1));
    ASSERT_EQ((word2_one_r - 1).digits[2], 0ull);
    ASSERT_EQ((word2_one_r - 1).digits[3], 0ull);

    // {0, 0, 0, 1} - 1 -> borrow through all three words

    constexpr uint_t word3_one = uint_t(1) << (3 * word_bits<uint_t>);
    static_assert((word3_one - 1).digits[0] == native_word_t<uint_t>(-1));
    static_assert((word3_one - 1).digits[1] == native_word_t<uint_t>(-1));
    static_assert((word3_one - 1).digits[2] == native_word_t<uint_t>(-1));
    static_assert((word3_one - 1).digits[3] == 0);
    const uint_t word3_one_r = word3_one;
    ASSERT_EQ((word3_one_r - 1).digits[0], native_word_t<uint_t>(-1));
    ASSERT_EQ((word3_one_r - 1).digits[1], native_word_t<uint_t>(-1));
    ASSERT_EQ((word3_one_r - 1).digits[2], native_word_t<uint_t>(-1));
    ASSERT_EQ((word3_one_r - 1).digits[3], 0ull);

    // carry from word 1 to word 2

    constexpr uint_t word1_max = uint_t(native_word_t<uint_t>(-1)) << word_bits<uint_t>;
    constexpr uint_t word1_one = uint_t(1) << word_bits<uint_t>;
    static_assert((word1_max + word1_one).digits[0] == 0);
    static_assert((word1_max + word1_one).digits[1] == 0);
    static_assert((word1_max + word1_one).digits[2] == 1);
    static_assert((word1_max + word1_one).digits[3] == 0);
    const uint_t word1_max_r = word1_max;
    const uint_t word1_one_r = word1_one;
    ASSERT_EQ((word1_max_r + word1_one_r).digits[0], 0ull);
    ASSERT_EQ((word1_max_r + word1_one_r).digits[1], 0ull);
    ASSERT_EQ((word1_max_r + word1_one_r).digits[2], 1ull);
    ASSERT_EQ((word1_max_r + word1_one_r).digits[3], 0ull);

    // multiplication with carry from word 0 to word 1

    constexpr uint_t mul_carry_base = uint_t(native_word_t<uint_t>(-1));
    static_assert((mul_carry_base * 2).digits[0] == native_word_t<uint_t>(native_word_t<uint_t>(-1) - 1));
    static_assert((mul_carry_base * 2).digits[1] == 1);
    static_assert((mul_carry_base * 2).digits[2] == 0);
    static_assert((mul_carry_base * 2).digits[3] == 0);
    const uint_t mul_carry_base_r = mul_carry_base;
    ASSERT_EQ((mul_carry_base_r * 2).digits[0], native_word_t<uint_t>(native_word_t<uint_t>(-1) - 1));
    ASSERT_EQ((mul_carry_base_r * 2).digits[1], 1ull);
    ASSERT_EQ((mul_carry_base_r * 2).digits[2], 0ull);
    ASSERT_EQ((mul_carry_base_r * 2).digits[3], 0ull);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_256_shift_at_or_beyond_width_noop_tests()
{
    // shifts at or beyond the total bit width keep the value unchanged

    constexpr uint_t value = uint_t(native_word_t<uint_t>(0x11223344u))
                           + (uint_t(native_word_t<uint_t>(0x55667788u)) << word_bits<uint_t>)
                           + (uint_t(native_word_t<uint_t>(0x99aabbccu)) << (2 * word_bits<uint_t>))
                           + (uint_t(native_word_t<uint_t>(0xddeeff00u)) << (3 * word_bits<uint_t>));
    static_assert((value << total_bits<uint_t>) == value);
    static_assert((value >> total_bits<uint_t>) == value);
    static_assert((value << (total_bits<uint_t> + 1)) == value);
    static_assert((value >> (total_bits<uint_t> + 1)) == value);
    static_assert((value << (total_bits<uint_t> + 44)) == value);
    static_assert((value >> (total_bits<uint_t> + 44)) == value);

    const uint_t value_r = value;
    ASSERT_EQ(value_r << total_bits<uint_t>, value_r);
    ASSERT_EQ(value_r >> total_bits<uint_t>, value_r);
    ASSERT_EQ(value_r << (total_bits<uint_t> + 1), value_r);
    ASSERT_EQ(value_r >> (total_bits<uint_t> + 1), value_r);
    ASSERT_EQ(value_r << (total_bits<uint_t> + 44), value_r);
    ASSERT_EQ(value_r >> (total_bits<uint_t> + 44), value_r);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t>
void run_256_signed_shift_at_or_beyond_width_noop_tests()
{
    // shifts at or beyond the total bit width keep the value unchanged

    constexpr int_t int_value = -int_t(0x12345678u);
    static_assert((int_value << total_bits<int_t>) == int_value);
    static_assert((int_value >> total_bits<int_t>) == int_value);
    static_assert((int_value << (total_bits<int_t> + 1)) == int_value);
    static_assert((int_value >> (total_bits<int_t> + 1)) == int_value);
    static_assert((int_value << (total_bits<int_t> + 17)) == int_value);
    static_assert((int_value >> (total_bits<int_t> + 17)) == int_value);

    const int_t int_value_r = int_value;
    ASSERT_EQ(int_value_r << total_bits<int_t>, int_value_r);
    ASSERT_EQ(int_value_r >> total_bits<int_t>, int_value_r);
    ASSERT_EQ(int_value_r << (total_bits<int_t> + 1), int_value_r);
    ASSERT_EQ(int_value_r >> (total_bits<int_t> + 1), int_value_r);
    ASSERT_EQ(int_value_r << (total_bits<int_t> + 17), int_value_r);
    ASSERT_EQ(int_value_r >> (total_bits<int_t> + 17), int_value_r);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_128_shift_at_or_beyond_width_noop_tests()
{
    // unsigned shifts at or beyond the total bit width keep the value unchanged

    constexpr uint_t uint_value = uint_t(native_word_t<uint_t>(0xdeadbeefu))
                                + (uint_t(native_word_t<uint_t>(0xcafebabeu)) << word_bits<uint_t>);
    static_assert((uint_value << total_bits<uint_t>) == uint_value);
    static_assert((uint_value >> total_bits<uint_t>) == uint_value);
    static_assert((uint_value << (total_bits<uint_t> + 1)) == uint_value);
    static_assert((uint_value >> (total_bits<uint_t> + 1)) == uint_value);
    static_assert((uint_value << (total_bits<uint_t> + 17)) == uint_value);
    static_assert((uint_value >> (total_bits<uint_t> + 17)) == uint_value);

    const uint_t uint_value_r = uint_value;
    ASSERT_EQ(uint_value_r << total_bits<uint_t>, uint_value_r);
    ASSERT_EQ(uint_value_r >> total_bits<uint_t>, uint_value_r);
    ASSERT_EQ(uint_value_r << (total_bits<uint_t> + 1), uint_value_r);
    ASSERT_EQ(uint_value_r >> (total_bits<uint_t> + 1), uint_value_r);
    ASSERT_EQ(uint_value_r << (total_bits<uint_t> + 17), uint_value_r);
    ASSERT_EQ(uint_value_r >> (total_bits<uint_t> + 17), uint_value_r);

    // signed shifts at or beyond the total bit width follow the same no-op rule

    constexpr int_t int_value = -int_t(0x12345678u);
    static_assert((int_value << total_bits<int_t>) == int_value);
    static_assert((int_value >> total_bits<int_t>) == int_value);
    static_assert((int_value << (total_bits<int_t> + 1)) == int_value);
    static_assert((int_value >> (total_bits<int_t> + 1)) == int_value);
    static_assert((int_value << (total_bits<int_t> + 17)) == int_value);
    static_assert((int_value >> (total_bits<int_t> + 17)) == int_value);

    const int_t int_value_r = int_value;
    ASSERT_EQ(int_value_r << total_bits<int_t>, int_value_r);
    ASSERT_EQ(int_value_r >> total_bits<int_t>, int_value_r);
    ASSERT_EQ(int_value_r << (total_bits<int_t> + 1), int_value_r);
    ASSERT_EQ(int_value_r >> (total_bits<int_t> + 1), int_value_r);
    ASSERT_EQ(int_value_r << (total_bits<int_t> + 17), int_value_r);
    ASSERT_EQ(int_value_r >> (total_bits<int_t> + 17), int_value_r);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// compound_shift_tests
////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(compound_shift_tests, family_128_signed_and_unsigned_compound_assignment_operators)
{
    run_128_compound_assignment_tests<uint128_t, int128_t>();
    run_128_compound_assignment_tests<uint32x4_t, int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(compound_shift_tests, family_256_unsigned_compound_assignment_operators)
{
    run_256_compound_assignment_tests<uint256_t>();
    run_256_compound_assignment_tests<uint32x8_t>();
    run_256_compound_assignment_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(compound_shift_tests, family_128_signed_and_unsigned_bit_shift_edge_cases)
{
    run_128_bit_shift_edge_case_tests<uint128_t>();
    run_128_bit_shift_edge_case_tests<uint32x4_t>();
    run_128_signed_shift_tests<int128_t>();
    run_128_signed_shift_tests<int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(compound_shift_tests, family_256_signed_and_unsigned_bit_shift_edge_cases)
{
    run_256_bit_shift_edge_case_tests<uint256_t>();
    run_256_bit_shift_edge_case_tests<uint32x8_t>();
    run_256_bit_shift_edge_case_tests<uint64x4_t>();
    run_256_signed_shift_tests<int256_t>();
    run_256_signed_shift_tests<int32x8_t>();
    run_256_signed_shift_tests<int64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(compound_shift_tests, family_256_unsigned_multiword_carry_borrow)
{
    run_256_multiword_carry_borrow_tests<uint256_t>();
    run_256_multiword_carry_borrow_tests<uint32x8_t>();
    run_256_multiword_carry_borrow_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(compound_shift_tests, family_128_signed_and_unsigned_shift_at_or_beyond_width_is_noop)
{
    run_128_shift_at_or_beyond_width_noop_tests<uint128_t, int128_t>();
    run_128_shift_at_or_beyond_width_noop_tests<uint32x4_t, int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(compound_shift_tests, family_256_signed_and_unsigned_shift_at_or_beyond_width_is_noop)
{
    run_256_shift_at_or_beyond_width_noop_tests<uint256_t>();
    run_256_shift_at_or_beyond_width_noop_tests<uint32x8_t>();
    run_256_shift_at_or_beyond_width_noop_tests<uint64x4_t>();
    run_256_signed_shift_at_or_beyond_width_noop_tests<int256_t>();
    run_256_signed_shift_at_or_beyond_width_noop_tests<int32x8_t>();
    run_256_signed_shift_at_or_beyond_width_noop_tests<int64x4_t>();
}

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// compound_shift_tests.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////
