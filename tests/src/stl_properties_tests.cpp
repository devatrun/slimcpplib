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

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <limits>
#include <numeric>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone functions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t, size_t expected_size>
void run_size_and_alignment_tests()
{
    static_assert(sizeof(type_t) == expected_size);

    // alignment must be at least the alignment of the native word type

    static_assert(alignof(type_t) >= alignof(typename type_t::native_array_t::value_type));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_unsigned_numeric_limits_tests()
{
    // unsigned long integers report the standard integral category properties

    static_assert(std::numeric_limits<uint_t>::is_integer);
    static_assert(!std::numeric_limits<uint_t>::is_signed);
    static_assert(std::numeric_limits<uint_t>::is_specialized);
    static_assert(std::numeric_limits<uint_t>::is_exact);
    static_assert(std::numeric_limits<uint_t>::is_bounded);
    static_assert(std::numeric_limits<uint_t>::is_modulo);

    // radix and digit counts follow the wrapped binary representation

    static_assert(std::numeric_limits<uint_t>::radix == 2);
    static_assert(std::numeric_limits<uint_t>::digits == bit_count_v<uint_t>);
    static_assert(std::numeric_limits<uint_t>::digits10 == std::numeric_limits<uint_t>::digits * 301 / 1000);

    // boundary accessors expose the expected zero and all-ones values

    static_assert(std::numeric_limits<uint_t>::min() == uint_t(0));
    static_assert(std::numeric_limits<uint_t>::lowest() == uint_t(0));
    static_assert(std::numeric_limits<uint_t>::max() == uint_t(-1));

    // floating-point-only members stay inactive for integer specializations

    static_assert(std::numeric_limits<uint_t>::epsilon() == uint_t(0));
    static_assert(std::numeric_limits<uint_t>::round_error() == uint_t(0));
    static_assert(!std::numeric_limits<uint_t>::has_infinity);
    static_assert(!std::numeric_limits<uint_t>::has_quiet_NaN);
    static_assert(!std::numeric_limits<uint_t>::has_signaling_NaN);
    static_assert(std::numeric_limits<uint_t>::denorm_min() == uint_t(0));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t, typename uint_t, size_t shift_value>
void run_signed_numeric_limits_tests()
{
    // signed long integers report the standard integral category properties

    static_assert(std::numeric_limits<int_t>::is_integer);
    static_assert(std::numeric_limits<int_t>::is_signed);
    static_assert(std::numeric_limits<int_t>::is_specialized);
    static_assert(std::numeric_limits<int_t>::is_exact);
    static_assert(std::numeric_limits<int_t>::is_bounded);
    static_assert(std::numeric_limits<int_t>::is_modulo);

    // radix and digit counts reflect the dedicated sign bit

    static_assert(std::numeric_limits<int_t>::radix == 2);
    static_assert(std::numeric_limits<int_t>::digits == bit_count_v<int_t> - 1);
    static_assert(std::numeric_limits<int_t>::digits10 == std::numeric_limits<int_t>::digits * 301 / 1000);

    // boundary accessors expose the expected minimum and maximum values

    static_assert(std::numeric_limits<int_t>::min() == int_t(uint_t(1) << shift_value));
    static_assert(std::numeric_limits<int_t>::lowest() == std::numeric_limits<int_t>::min());
    static_assert(std::numeric_limits<int_t>::max() == int_t(~(uint_t(1) << shift_value)));
    static_assert(std::numeric_limits<int_t>::min() < int_t(0));
    static_assert(std::numeric_limits<int_t>::max() > int_t(0));

    // floating-point-only members stay inactive for integer specializations

    static_assert(std::numeric_limits<int_t>::epsilon() == int_t(0));
    static_assert(std::numeric_limits<int_t>::round_error() == int_t(0));
    static_assert(!std::numeric_limits<int_t>::has_infinity);
    static_assert(!std::numeric_limits<int_t>::has_quiet_NaN);
    static_assert(!std::numeric_limits<int_t>::has_signaling_NaN);
    static_assert(std::numeric_limits<int_t>::denorm_min() == int_t(0));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_128_digits_array_layout_tests()
{
    // 1 << word_bits has digits[0]=0, digits[1]=1

    constexpr uint_t shifted_word = uint_t(1) << word_bits<uint_t>;
    static_assert(shifted_word.digits[0] == 0);
    static_assert(shifted_word.digits[1] == 1);

    // known value is in low word

    constexpr native_word_t<uint_t> known_low_word = native_word_t<uint_t>(0x89abcdefu);
    constexpr uint_t known = uint_t(known_low_word);
    static_assert(known.digits[0] == known_low_word);
    static_assert(known.digits[1] == 0);

    // direct digits[] write then compare

    uint_t mutable_value;
    mutable_value.digits[0] = native_word_t<uint_t>(0xdeadbeefu);
    mutable_value.digits[1] = native_word_t<uint_t>(0x01020304u);
    ASSERT_EQ(mutable_value.digits[0], native_word_t<uint_t>(0xdeadbeefu));
    ASSERT_EQ(mutable_value.digits[1], native_word_t<uint_t>(0x01020304u));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_256_digits_array_layout_tests()
{
    // 1 << (3 * word_bits) has digits[3]=1, others=0

    constexpr uint_t shifted_word3 = uint_t(1) << (3 * word_bits<uint_t>);
    static_assert(shifted_word3.digits[0] == 0);
    static_assert(shifted_word3.digits[1] == 0);
    static_assert(shifted_word3.digits[2] == 0);
    static_assert(shifted_word3.digits[3] == 1);

    // direct digits[] count iteration

    uint_t mutable_value = uint_t(0);
    mutable_value.digits[2] = 1;
    
    size_t nonzero = 0;
    for (size_t n = 0; n < std::size(mutable_value.digits); ++n) {

        if (mutable_value.digits[n] != 0)
            ++nonzero;
    }

    ASSERT_EQ(nonzero, size_t(1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_unsigned_256_explicit_conversion_tests()
{
    // conversion to the same native-word 128-bit type keeps low 128 bits

    constexpr uint_t all_ones = uint_t(-1);
    constexpr uint128_t truncated_128 = static_cast<uint128_t>(all_ones);
    static_assert(truncated_128 == uint128_t(-1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_unsigned_128_explicit_conversion_tests()
{
    // conversion to native type extracts digits[0]

    constexpr native_word_t<uint_t> low_word = native_word_t<uint_t>(0x89abcdefu);
    constexpr uint_t mixed_words = uint_t(low_word) + (uint_t(1) << word_bits<uint_t>);
    static_assert(static_cast<native_word_t<uint_t>>(mixed_words) == low_word);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t>
void run_signed_256_explicit_conversion_tests()
{
    // conversion to the same native-word 128-bit signed type keeps low 128 bits

    constexpr int_t neg_value = int_t(-1);
    constexpr int128_t truncated_int128 = static_cast<int128_t>(neg_value);
    static_assert(truncated_int128 == int128_t(-1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t>
void run_type_trait_tests()
{
    static_assert(std::is_default_constructible_v<type_t>);
    static_assert(std::is_copy_constructible_v<type_t>);
    static_assert(std::is_move_constructible_v<type_t>);
    static_assert(std::is_copy_assignable_v<type_t>);
    static_assert(std::is_move_assignable_v<type_t>);
    static_assert(std::is_trivially_copyable_v<type_t>);
    static_assert(std::is_standard_layout_v<type_t>);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_unsigned_stl_algorithm_tests()
{
    std::array<uint_t, 4> values = {
        uint_t(5),
        uint_t(1),
        uint_t(9),
        uint_t(3)
    };

    // ordering works with standard sorting algorithms

    std::sort(values.begin(), values.end());
    ASSERT_EQ(values[0], uint_t(1));
    ASSERT_EQ(values[1], uint_t(3));
    ASSERT_EQ(values[2], uint_t(5));
    ASSERT_EQ(values[3], uint_t(9));

    // accumulation uses the custom arithmetic operators transparently

    const uint_t sum = std::accumulate(values.begin(), values.end(), uint_t(0));
    ASSERT_EQ(sum, uint_t(18));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t>
void run_signed_stl_algorithm_tests()
{
    std::array<int_t, 4> values = {
        int_t(-5),
        int_t(1),
        int_t(-1),
        int_t(3)
    };

    // ordering works with standard sorting algorithms

    std::sort(values.begin(), values.end());
    ASSERT_EQ(values[0], int_t(-5));
    ASSERT_EQ(values[1], int_t(-1));
    ASSERT_EQ(values[2], int_t(1));
    ASSERT_EQ(values[3], int_t(3));

    // accumulation uses the custom arithmetic operators transparently

    const int_t sum = std::accumulate(values.begin(), values.end(), int_t(0));
    ASSERT_EQ(sum, int_t(-2));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_numeric_limits_usage_tests()
{
    const uint_t uint_min = std::numeric_limits<uint_t>::min();
    const uint_t uint_lowest = std::numeric_limits<uint_t>::lowest();
    const uint_t uint_max = std::numeric_limits<uint_t>::max();
    const int_t int_min = std::numeric_limits<int_t>::min();
    const int_t int_lowest = std::numeric_limits<int_t>::lowest();
    const int_t int_max = std::numeric_limits<int_t>::max();

    // unsigned boundary accessors behave like modular integer endpoints

    ASSERT_EQ(uint_min, uint_t(0));
    ASSERT_EQ(uint_lowest, uint_t(0));
    ASSERT_EQ(++uint_t(uint_max), uint_min);
    ASSERT_EQ(--uint_t(uint_min), uint_max);

    // signed boundary accessors preserve the expected wraparound ordering

    ASSERT_EQ(int_lowest, int_min);
    ASSERT_LT(int_min, int_t(0));
    ASSERT_GT(int_max, int_t(0));
    ASSERT_EQ(++int_t(int_max), int_min);
    ASSERT_EQ(--int_t(int_min), int_max);
    ASSERT_EQ(uint_max + uint_t(1), uint_min);
    ASSERT_EQ(int_max + int_t(1), int_min);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_unsigned_hash_tests()
{
    constexpr uint_t zero = uint_t(0);
    constexpr uint_t small = uint_t(0x12345u);
    constexpr uint_t multiword = (uint_t(0x0123456789abcdefull) << word_bits<uint_t>) + uint_t(0x0fedcba987654321ull);
    constexpr uint_t all_ones = uint_t(-1);
    constexpr std::hash<uint_t> hasher = {};

    // equal unsigned values must produce equal hash values in constexpr and runtime code

    static_assert(std::hash<uint_t>()(zero) == std::hash<uint_t>()(uint_t(0)));
    static_assert(std::hash<uint_t>()(small) == std::hash<uint_t>()(uint_t(0x12345u)));
    static_assert(std::hash<uint_t>()(multiword) == std::hash<uint_t>()((uint_t(0x0123456789abcdefull) << word_bits<uint_t>) + uint_t(0x0fedcba987654321ull)));
    static_assert(std::hash<uint_t>()(all_ones) == std::hash<uint_t>()(uint_t(-1)));

    ASSERT_EQ(hasher(zero), hasher(uint_t(0)));
    ASSERT_EQ(hasher(small), hasher(uint_t(0x12345u)));
    ASSERT_EQ(hasher(multiword), hasher((uint_t(0x0123456789abcdefull) << word_bits<uint_t>) + uint_t(0x0fedcba987654321ull)));
    ASSERT_EQ(hasher(all_ones), hasher(uint_t(-1)));

    // unsigned values must be usable as keys in unordered_set

    std::unordered_set<uint_t> values = {
        zero,
        small,
        multiword,
        all_ones
    };

    ASSERT_NE(values.find(zero), values.end());
    ASSERT_NE(values.find(small), values.end());
    ASSERT_NE(values.find(multiword), values.end());
    ASSERT_NE(values.find(all_ones), values.end());

    // unsigned values must be usable as keys in unordered_map

    std::unordered_map<uint_t, int> mapped_values = {
        { zero, 0 },
        { small, 1 },
        { multiword, 2 },
        { all_ones, 3 }
    };

    ASSERT_EQ(mapped_values[zero], 0);
    ASSERT_EQ(mapped_values[small], 1);
    ASSERT_EQ(mapped_values[multiword], 2);
    ASSERT_EQ(mapped_values[all_ones], 3);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t, typename uint_t>
void run_signed_hash_tests()
{
    constexpr int_t zero = int_t(0);
    constexpr int_t positive = int_t(0x12345u);
    constexpr int_t negative = -int_t(0x12345u);
    constexpr int_t multiword_negative = -int_t((uint_t(0x1234567890abcdefull) << 64) + uint_t(0x0fedcba987654321ull));

    constexpr std::hash<int_t> signed_hasher = {};
    constexpr std::hash<uint_t> unsigned_hasher = {};

    // equal signed values must produce equal hash values in constexpr and runtime code

    static_assert(std::hash<int_t>()(zero) == std::hash<int_t>()(int_t(0)));
    static_assert(std::hash<int_t>()(positive) == std::hash<int_t>()(int_t(0x12345u)));
    static_assert(std::hash<int_t>()(negative) == std::hash<int_t>()(-int_t(0x12345u)));
    static_assert(std::hash<int_t>()(multiword_negative) == std::hash<int_t>()(-int_t((uint_t(0x1234567890abcdefull) << 64) + uint_t(0x0fedcba987654321ull))));

    ASSERT_EQ(signed_hasher(zero), signed_hasher(int_t(0)));
    ASSERT_EQ(signed_hasher(positive), signed_hasher(int_t(0x12345u)));
    ASSERT_EQ(signed_hasher(negative), signed_hasher(-int_t(0x12345u)));
    ASSERT_EQ(signed_hasher(multiword_negative), signed_hasher(-int_t((uint_t(0x1234567890abcdefull) << 64) + uint_t(0x0fedcba987654321ull))));

    // signed hash follows the underlying unsigned bit representation in constexpr and runtime code

    static_assert(std::hash<int_t>()(int_t(-1)) == std::hash<uint_t>()(uint_t(-1)));
    static_assert(std::hash<int_t>()(negative) == std::hash<uint_t>()(static_cast<uint_t>(negative)));
    static_assert(std::hash<int_t>()(multiword_negative) == std::hash<uint_t>()(static_cast<uint_t>(multiword_negative)));

    ASSERT_EQ(signed_hasher(int_t(-1)), unsigned_hasher(uint_t(-1)));
    ASSERT_EQ(signed_hasher(negative), unsigned_hasher(static_cast<uint_t>(negative)));
    ASSERT_EQ(signed_hasher(multiword_negative), unsigned_hasher(static_cast<uint_t>(multiword_negative)));

    // signed values must be usable as keys in unordered_set

    std::unordered_set<int_t> values = {
        zero,
        positive,
        negative,
        multiword_negative
    };

    ASSERT_NE(values.find(zero), values.end());
    ASSERT_NE(values.find(positive), values.end());
    ASSERT_NE(values.find(negative), values.end());
    ASSERT_NE(values.find(multiword_negative), values.end());

    // signed values must be usable as keys in unordered_map

    std::unordered_map<int_t, int> mapped_values = {
        { zero, 0 },
        { positive, 1 },
        { negative, 2 },
        { multiword_negative, 3 }
    };

    ASSERT_EQ(mapped_values[zero], 0);
    ASSERT_EQ(mapped_values[positive], 1);
    ASSERT_EQ(mapped_values[negative], 2);
    ASSERT_EQ(mapped_values[multiword_negative], 3);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// stl_properties_tests
////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(stl_properties_tests, family_128_signed_and_unsigned_size_and_alignment)
{
    run_size_and_alignment_tests<uint128_t, 16>();
    run_size_and_alignment_tests<int128_t, 16>();
    run_size_and_alignment_tests<uint32x4_t, 16>();
    run_size_and_alignment_tests<int32x4_t, 16>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_256_signed_and_unsigned_size_and_alignment)
{
    run_size_and_alignment_tests<uint256_t, 32>();
    run_size_and_alignment_tests<int256_t, 32>();
    run_size_and_alignment_tests<uint32x8_t, 32>();
    run_size_and_alignment_tests<int32x8_t, 32>();
    run_size_and_alignment_tests<uint64x4_t, 32>();
    run_size_and_alignment_tests<int64x4_t, 32>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_128_signed_and_unsigned_numeric_limits)
{
    run_unsigned_numeric_limits_tests<uint128_t>();
    run_unsigned_numeric_limits_tests<uint32x4_t>();
    run_signed_numeric_limits_tests<int128_t, uint128_t, 127>();
    run_signed_numeric_limits_tests<int32x4_t, uint32x4_t, 127>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_256_signed_and_unsigned_numeric_limits)
{
    run_unsigned_numeric_limits_tests<uint256_t>();
    run_unsigned_numeric_limits_tests<uint32x8_t>();
    run_unsigned_numeric_limits_tests<uint64x4_t>();
    run_signed_numeric_limits_tests<int256_t, uint256_t, 255>();
    run_signed_numeric_limits_tests<int32x8_t, uint32x8_t, 255>();
    run_signed_numeric_limits_tests<int64x4_t, uint64x4_t, 255>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_128_unsigned_digits_array_layout)
{
    run_128_digits_array_layout_tests<uint128_t>();
    run_128_digits_array_layout_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_256_unsigned_digits_array_layout)
{
    run_256_digits_array_layout_tests<uint256_t>();
    run_256_digits_array_layout_tests<uint32x8_t>();
    run_256_digits_array_layout_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_128_unsigned_explicit_conversions)
{
    run_unsigned_128_explicit_conversion_tests<uint128_t>();
    run_unsigned_128_explicit_conversion_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_256_signed_and_unsigned_explicit_conversions)
{
    run_unsigned_256_explicit_conversion_tests<uint256_t>();
    run_unsigned_256_explicit_conversion_tests<uint64x4_t>();
    run_signed_256_explicit_conversion_tests<int256_t>();
    run_signed_256_explicit_conversion_tests<int64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_128_signed_and_unsigned_type_traits)
{
    run_type_trait_tests<uint128_t>();
    run_type_trait_tests<int128_t>();
    run_type_trait_tests<uint32x4_t>();
    run_type_trait_tests<int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_256_signed_and_unsigned_type_traits)
{
    run_type_trait_tests<uint256_t>();
    run_type_trait_tests<int256_t>();
    run_type_trait_tests<uint32x8_t>();
    run_type_trait_tests<int32x8_t>();
    run_type_trait_tests<uint64x4_t>();
    run_type_trait_tests<int64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_128_signed_and_unsigned_stl_algorithms_and_containers)
{
    run_unsigned_stl_algorithm_tests<uint128_t>();
    run_unsigned_stl_algorithm_tests<uint32x4_t>();
    run_signed_stl_algorithm_tests<int128_t>();
    run_signed_stl_algorithm_tests<int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_128_unsigned_hash)
{
    run_unsigned_hash_tests<uint128_t>();
    run_unsigned_hash_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_128_signed_hash)
{
    run_signed_hash_tests<int128_t, uint128_t>();
    run_signed_hash_tests<int32x4_t, uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_256_signed_and_unsigned_stl_algorithms_and_containers)
{
    run_unsigned_stl_algorithm_tests<uint256_t>();
    run_unsigned_stl_algorithm_tests<uint32x8_t>();
    run_unsigned_stl_algorithm_tests<uint64x4_t>();
    run_signed_stl_algorithm_tests<int256_t>();
    run_signed_stl_algorithm_tests<int32x8_t>();
    run_signed_stl_algorithm_tests<int64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_256_unsigned_hash)
{
    run_unsigned_hash_tests<uint256_t>();
    run_unsigned_hash_tests<uint32x8_t>();
    run_unsigned_hash_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_256_signed_hash)
{
    run_signed_hash_tests<int256_t, uint256_t>();
    run_signed_hash_tests<int32x8_t, uint32x8_t>();
    run_signed_hash_tests<int64x4_t, uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(stl_properties_tests, family_256_signed_and_unsigned_numeric_limits_usage)
{
    run_numeric_limits_usage_tests<uint256_t, int256_t>();
    run_numeric_limits_usage_tests<uint32x8_t, int32x8_t>();
    run_numeric_limits_usage_tests<uint64x4_t, int64x4_t>();
}

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// stl_properties_tests.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////
