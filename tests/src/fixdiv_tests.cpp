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

#include <slimcpplib/long_fixdiv.h>

using namespace slim::literals;

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
void run_128_unsigned_divide_by_one_tests()
{
    // divisor = 1: identity for all values

    const auto divider = make_fixed_divider(uint_t(1));
    const uint_t large_value = uint_t(12345) + (uint_t(67890) << 32) + (uint_t(13579) << 64) + (uint_t(24680) << 96);

    ASSERT_EQ(uint_t(0) / divider, uint_t(0));
    ASSERT_EQ(uint_t(1) / divider, uint_t(1));
    ASSERT_EQ(uint_t(2) / divider, uint_t(2));
    ASSERT_EQ(large_value / divider, large_value);
    ASSERT_EQ(uint_t(-1) / divider, uint_t(-1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_128_unsigned_divide_by_power_of_two_tests()
{
    // power-of-2 divisors use shift-only optimization branch

    const auto divider_2 = make_fixed_divider(uint_t(2));
    const auto divider_256 = make_fixed_divider(uint_t(256));
    const auto divider_2_pow_32 = make_fixed_divider(uint_t(1) << 32);

    const uint_t value = uint_t(12345) + (uint_t(67890) << 32) + (uint_t(13579) << 64) + (uint_t(24680) << 96);

    ASSERT_EQ(value / divider_2, value / uint_t(2));
    ASSERT_EQ(value / divider_256, value / uint_t(256));
    ASSERT_EQ(value / divider_2_pow_32, value / (uint_t(1) << 32));

    // boundary values

    ASSERT_EQ(uint_t(255) / divider_256, uint_t(0));
    ASSERT_EQ(uint_t(256) / divider_256, uint_t(1));
    ASSERT_EQ(uint_t(512) / divider_256, uint_t(2));
    ASSERT_EQ(uint_t(513) / divider_256, uint_t(2));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_128_unsigned_consistency_sweep_tests()
{
    // verify fixed divider matches operator/ for representative divisors

    const uint_t divisors[] = {
        uint_t(3),
        uint_t(7),
        uint_t(1234567890u),
        uint_t(1) << 32,
        (uint_t(1) << 64) + uint_t(3)
    };

    const uint_t dividends[] = {
        uint_t(0),
        uint_t(1),
        uint_t(999),
        uint_t(12345) + (uint_t(67890) << 32) + (uint_t(13579) << 64) + (uint_t(24680) << 96),
        uint_t(-1)
    };

    for (const auto& divisor : divisors)
    {
        const auto divider = make_fixed_divider(divisor);
        for (const auto& dividend : dividends)
            ASSERT_EQ(dividend / divider, dividend / divisor);
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_128_unsigned_swap_tests()
{
    // prepare representative dividends

    const auto dividend1 = uint_t(12345) + (uint_t(67890) << 32) + (uint_t(13579) << 64) + (uint_t(24680) << 96);
    const auto dividend2 = uint_t(-1);

    // prepare dividers with different division paths

    const uint_t divisor1 = 7;
    const uint_t divisor2 = (uint_t(1) << 64) + uint_t(3);

    auto divider1 = make_fixed_divider(divisor1);
    auto divider2 = make_fixed_divider(divisor2);

    // swap divider internals

    divider1.swap(divider2);

    // verify that swapped dividers now behave as the opposite divisors

    ASSERT_EQ(dividend1 / divider1, dividend1 / divisor2);
    ASSERT_EQ(dividend2 / divider1, dividend2 / divisor2);
    ASSERT_EQ(dividend1 / divider2, dividend1 / divisor1);
    ASSERT_EQ(dividend2 / divider2, dividend2 / divisor1);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_256_unsigned_fixed_divider_boundary_tests()
{
    // representative dividends cover zero, low values and high-bit boundaries

    const uint_t dividends[] = {
        uint_t(0),
        uint_t(1),
        uint_t(2),
        uint_t(-1),
        (uint_t(1) << 255) - 1,
        uint_t(1) << 255,
        (uint_t(1) << 255) + 1,
        uint_t(0x0123456789abcdefull) + (uint_t(0xfedcba9876543210ull) << 64) + (uint_t(0x0f1e2d3c4b5a6978ull) << 128) + (uint_t(0x8877665544332211ull) << 192)
    };

    // representative divisors cover trivial, native-width and top-bit cases

    const uint_t divisors[] = {
        uint_t(1),
        uint_t(2),
        uint_t(3),
        uint_t(native_word_t<uint_t>(-1)),
        uint_t(1) << 64,
        (uint_t(1) << 64) - 1,
        (uint_t(1) << 64) + 1,
        uint_t(1) << 255,
        (uint_t(1) << 255) - 1
    };

    for (const auto& divisor : divisors)
    {
        const auto divider = make_fixed_divider(divisor);

        // fixed-divider quotients must match operator/ and preserve div/mod invariants

        for (const auto& dividend : dividends)
        {
            const uint_t quotient = dividend / divider;
            const uint_t remainder = dividend % divisor;

            ASSERT_EQ(quotient, dividend / divisor);
            ASSERT_EQ(quotient * divisor + remainder, dividend);
            ASSERT_LT(remainder, divisor);
        }
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// fixdiv_tests
////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(fixdiv_tests, family_128_unsigned_divide_by_one)
{
    run_128_unsigned_divide_by_one_tests<uint128_t>();
    run_128_unsigned_divide_by_one_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(fixdiv_tests, family_128_unsigned_divide_by_power_of_two)
{
    run_128_unsigned_divide_by_power_of_two_tests<uint128_t>();
    run_128_unsigned_divide_by_power_of_two_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(fixdiv_tests, divide_by_large_prime)
{
    // prime divisor: exercises the general Granlund-Montgomery branch
    // 2^64 - 59 is prime
    constexpr uint128_t prime = uint128_t(0xffffffffffffffc5ull);
    const auto fdp = make_fixed_divider(prime);

    const uint128_t values[] = {
        uint128_t(0),
        uint128_t(1),
        prime - 1,
        prime,
        prime + 1,
        prime * 2,
        prime * 2 + 1,
        0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128,
        uint128_t(-1)
    };

    for (const auto& val : values)
        ASSERT_EQ(val / fdp, val / prime);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(fixdiv_tests, divide_by_large_divisor)
{
    // divisor > 2^63: requires the addition != 0 path in divide()
    constexpr uint128_t big = uint128_t(0x8000000000000001ull);  // 2^63 + 1
    const auto fdb = make_fixed_divider(big);

    const uint128_t values[] = {
        uint128_t(0),
        uint128_t(1),
        big - 1,
        big,
        big + 1,
        big * 2,
        0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128,
        uint128_t(-1)
    };

    for (const auto& val : values)
        ASSERT_EQ(val / fdb, val / big);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(fixdiv_tests, family_128_unsigned_consistency_sweep)
{
    run_128_unsigned_consistency_sweep_tests<uint128_t>();
    run_128_unsigned_consistency_sweep_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(fixdiv_tests, family_128_unsigned_swap)
{
    run_128_unsigned_swap_tests<uint128_t>();
    run_128_unsigned_swap_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(fixdiv_tests, family_256_unsigned_fixed_divider_boundary_values)
{
    run_256_unsigned_fixed_divider_boundary_tests<uint256_t>();
    run_256_unsigned_fixed_divider_boundary_tests<uint32x8_t>();
    run_256_unsigned_fixed_divider_boundary_tests<uint64x4_t>();
}

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// fixdiv_tests.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////
