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

#include <slimcpplib/long_io.h>

#include <sstream>
#include <string>

using namespace slim::literals;
namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone functions
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_128_unsigned_decimal_round_trip_tests()
{
    const uint_t original = uint_t(12345) + (uint_t(67890) << 32) + (uint_t(13579) << 64) + (uint_t(24680) << 96);

    std::ostringstream oss;
    oss << original;

    uint_t recovered;
    std::istringstream iss(oss.str());
    iss >> recovered;

    ASSERT_EQ(original, recovered);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t>
void run_128_signed_decimal_round_trip_tests()
{
    // positive signed value

    const int_t positive_value = int_t(12345) + (int_t(67890) << 32);
    {
        std::ostringstream oss;
        oss << positive_value;
        int_t recovered;
        std::istringstream iss(oss.str());
        iss >> recovered;
        ASSERT_EQ(positive_value, recovered);
    }

    // negative signed value

    const int_t negative_value = -positive_value;
    {
        std::ostringstream oss;
        oss << negative_value;
        const std::string value_string = oss.str();
        ASSERT_EQ(value_string[0], '-');  // must start with '-'
        int_t recovered;
        std::istringstream iss(value_string);
        iss >> recovered;
        ASSERT_EQ(negative_value, recovered);
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_128_unsigned_hex_input_tests()
{
    // lowercase hexadecimal input

    std::istringstream lower_stream("deadbeef");
    uint_t lower_value;
    lower_stream >> std::hex >> lower_value;
    ASSERT_EQ(lower_value, uint_t(0xdeadbeefull));

    // uppercase hexadecimal input

    std::istringstream upper_stream("DEADBEEF");
    uint_t upper_value;
    upper_stream >> std::hex >> upper_value;
    ASSERT_EQ(upper_value, uint_t(0xdeadbeefull));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_128_unsigned_octal_input_tests()
{
    // zero in octal form

    std::istringstream zero_stream("0");
    uint_t zero_value;
    zero_stream >> std::oct >> zero_value;
    ASSERT_EQ(zero_value, uint_t(0));

    // non-trivial octal value

    std::istringstream simple_stream("777");
    uint_t simple_value;
    simple_stream >> std::oct >> simple_value;
    ASSERT_EQ(simple_value, uint_t(0777));

    // octal power boundary

    std::istringstream power_stream("10");
    uint_t power_value;
    power_stream >> std::oct >> power_value;
    ASSERT_EQ(power_value, uint_t(8));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_128_signed_and_unsigned_decimal_input_with_whitespace_and_leading_zeros_tests()
{
    // unsigned input with leading whitespace and zeros

    std::istringstream unsigned_stream("   0000012345  ");
    uint_t unsigned_value;
    unsigned_stream >> unsigned_value;
    ASSERT_EQ(unsigned_value, uint_t(12345));

    // signed input with whitespace and a negative sign

    std::istringstream signed_stream(" \t\n-00042 ");
    int_t signed_value;
    signed_stream >> signed_value;
    ASSERT_EQ(signed_value, int_t(-42));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename int_t>
void run_128_signed_input_sign_handling_tests()
{
    // explicit positive value

    std::istringstream positive_stream("42");
    int_t positive_value;
    positive_stream >> positive_value;
    ASSERT_EQ(positive_value, int_t(42));

    // explicit negative value

    std::istringstream negative_stream("-42");
    int_t negative_value;
    negative_stream >> negative_value;
    ASSERT_EQ(negative_value, int_t(-42));

    // negative zero normalizes to zero

    std::istringstream zero_stream("-0");
    int_t zero_value;
    zero_stream >> zero_value;
    ASSERT_EQ(zero_value, int_t(0));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t, size_t shift_value>
void run_special_value_decimal_round_trip_tests()
{
    const uint_t wide_unsigned = (uint_t(0x0123456789abcdefull) << 192)
                               + (uint_t(0xfedcba9876543210ull) << 128)
                               + (uint_t(0x13579bdf2468ace0ull) << 64)
                               + uint_t(0x0badf00dcafebeefull);
    const int_t signed_min = int_t(uint_t(1) << shift_value);
    const int_t signed_max = int_t(~(uint_t(1) << shift_value));
    const int_t wide_signed_negative = -int_t((uint_t(0x1234567890abcdefull) << 64) + uint_t(0x0fedcba987654321ull));

    {
        std::ostringstream oss;
        oss << wide_unsigned;
        uint_t recovered;
        std::istringstream iss(oss.str());
        iss >> recovered;
        ASSERT_EQ(recovered, wide_unsigned);
    }

    {
        std::ostringstream oss;
        oss << signed_min;
        int_t recovered;
        std::istringstream iss(oss.str());
        iss >> recovered;
        ASSERT_EQ(recovered, signed_min);
    }

    {
        std::ostringstream oss;
        oss << signed_max;
        int_t recovered;
        std::istringstream iss(oss.str());
        iss >> recovered;
        ASSERT_EQ(recovered, signed_max);
    }

    {
        std::ostringstream oss;
        oss << wide_signed_negative;
        const std::string serialized = oss.str();
        ASSERT_EQ(serialized[0], '-');

        int_t recovered;
        std::istringstream iss(serialized);
        iss >> recovered;
        ASSERT_EQ(recovered, wide_signed_negative);
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_input_edge_and_error_behavior_tests()
{
    // explicit plus sign is accepted for unsigned values

    {
        uint_t unsigned_value = uint_t(0);
        std::istringstream unsigned_stream("+42");
        unsigned_stream >> unsigned_value;
        ASSERT_EQ(unsigned_value, uint_t(42));
    }

    // explicit plus sign is accepted for signed values

    {
        int_t signed_value = int_t(0);
        std::istringstream signed_stream("+42");
        signed_stream >> signed_value;
        ASSERT_EQ(signed_value, int_t(42));
    }

    // a standalone minus sign is invalid and leaves the target unchanged

    {
        int_t minus_only_value = int_t(123);
        std::istringstream minus_only_stream("-");
        minus_only_stream >> minus_only_value;
        ASSERT_TRUE(minus_only_stream.fail());
        ASSERT_EQ(minus_only_value, int_t(123));
    }

    // parsing stops exactly at the separating whitespace and preserves the next token

    {
        uint_t first_value;
        uint_t second_value;
        std::istringstream chained_stream("123 456");
        chained_stream >> first_value >> second_value;
        ASSERT_EQ(first_value, uint_t(123));
        ASSERT_EQ(second_value, uint_t(456));
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t, typename int_t>
void run_stream_state_contract_tests()
{
    // exact unsigned token reaches eof without fail

    {
        uint_t value = 0;
        std::istringstream stream("42");
        stream >> value;
        ASSERT_EQ(value, uint_t(42));
        ASSERT_TRUE(stream.eof());
        ASSERT_FALSE(stream.fail());
        ASSERT_FALSE(stream.bad());
    }

    // exact signed token reaches eof without fail

    {
        int_t value = 0;
        std::istringstream stream("-42");
        stream >> value;
        ASSERT_EQ(value, int_t(-42));
        ASSERT_TRUE(stream.eof());
        ASSERT_FALSE(stream.fail());
        ASSERT_FALSE(stream.bad());
    }

    // chained extraction keeps the stream usable between tokens

    {
        uint_t first_value = 0;
        uint_t second_value = 0;
        std::istringstream stream("123 456");
        stream >> first_value;
        ASSERT_EQ(first_value, uint_t(123));
        ASSERT_FALSE(stream.eof());
        ASSERT_FALSE(stream.fail());
        stream >> second_value;
        ASSERT_EQ(second_value, uint_t(456));
        ASSERT_TRUE(stream.eof());
        ASSERT_FALSE(stream.fail());
    }

    // invalid trailing characters stop extraction after the valid prefix

    {
        uint_t value = 0;
        std::istringstream stream("12z34 56");
        stream >> value;
        ASSERT_EQ(value, uint_t(12));
        ASSERT_EQ(stream.peek(), 'z');
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_decimal_output_tests()
{
    std::ostringstream decimal_stream;
    decimal_stream << uint_t(0);
    ASSERT_EQ(decimal_stream.str(), "0");

    decimal_stream.str("");
    decimal_stream.clear();
    decimal_stream << uint_t(1);
    ASSERT_EQ(decimal_stream.str(), "1");

    decimal_stream.str("");
    decimal_stream.clear();
    decimal_stream << uint_t(10);
    ASSERT_EQ(decimal_stream.str(), "10");

    decimal_stream.str("");
    decimal_stream.clear();
    decimal_stream << uint_t(12345678901234567890ull);
    ASSERT_EQ(decimal_stream.str(), "12345678901234567890");
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_hex_output_tests()
{
    std::ostringstream hex_stream;
    hex_stream << std::hex << uint_t(0xdeadbeefull);
    ASSERT_EQ(hex_stream.str(), "deadbeef");

    hex_stream.str("");
    hex_stream.clear();
    hex_stream << std::hex << std::uppercase << uint_t(0xdeadbeefull);
    ASSERT_EQ(hex_stream.str(), "DEADBEEF");
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_octal_output_tests()
{
    std::ostringstream octal_stream;
    octal_stream << std::oct << uint_t(0);
    ASSERT_EQ(octal_stream.str(), "0");

    octal_stream.str("");
    octal_stream.clear();
    octal_stream << std::oct << uint_t(8);
    ASSERT_EQ(octal_stream.str(), "10");

    octal_stream.str("");
    octal_stream.clear();
    octal_stream << std::oct << uint_t(0777);
    ASSERT_EQ(octal_stream.str(), "777");

    octal_stream.str("");
    octal_stream.clear();
    octal_stream << std::oct << uint_t(1);
    ASSERT_EQ(octal_stream.str(), "1");
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_stream_format_switching_tests()
{
    const uint_t value = uint_t(0xdeadbeefull);
    std::ostringstream mixed_format_stream;
    mixed_format_stream << std::hex << value << ' ' << std::dec << value << ' ' << std::oct << value;
    ASSERT_EQ(mixed_format_stream.str(), "deadbeef 3735928559 33653337357");
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_uppercase_and_nouppercase_output_tests()
{
    const uint_t value = (uint_t(0xaf5705a489525e79ull) << 64) + uint_t(0xa5120c42daebbc57ull);

    std::ostringstream lowercase_stream;
    lowercase_stream << std::hex << std::nouppercase << value;
    ASSERT_EQ(lowercase_stream.str(), "af5705a489525e79a5120c42daebbc57");

    std::ostringstream uppercase_stream;
    uppercase_stream << std::hex << std::uppercase << value;
    ASSERT_EQ(uppercase_stream.str(), "AF5705A489525E79A5120C42DAEBBC57");
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_unsigned_input_error_tests()
{
    std::istringstream decimal_stream("12z34");
    uint_t decimal_value = 0;
    decimal_stream >> decimal_value;
    ASSERT_EQ(decimal_value, uint_t(12));
    ASSERT_EQ(decimal_stream.peek(), 'z');

    std::istringstream octal_stream("128");
    uint_t octal_value = 0;
    octal_stream >> std::oct >> octal_value;
    ASSERT_EQ(octal_value, uint_t(10));
    ASSERT_EQ(octal_stream.peek(), '8');

    std::istringstream negative_unsigned_stream("-1");
    uint_t negative_unsigned_value = 0;
    negative_unsigned_stream >> negative_unsigned_value;
    ASSERT_EQ(negative_unsigned_value, uint_t(-1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename uint_t>
void run_unsigned_input_overflow_tests()
{
    std::istringstream overflow_stream("99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
    uint_t value = uint_t(123);
    overflow_stream >> value;
    ASSERT_TRUE(overflow_stream.fail());
    ASSERT_EQ(value, uint_t(123));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// io_tests
////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(io_tests, decimal_output)
{
    run_decimal_output_tests<uint128_t>();
    run_decimal_output_tests<uint32x4_t>();
    run_decimal_output_tests<uint256_t>();
    run_decimal_output_tests<uint32x8_t>();
    run_decimal_output_tests<uint64x4_t>();

    // 2^128 - 1 = 340282366920938463463374607431768211455
    std::ostringstream ss;
    ss.str("");
    ss << uint128_t(-1);
    ASSERT_EQ(ss.str(), "340282366920938463463374607431768211455");
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, hex_output)
{
    run_hex_output_tests<uint128_t>();
    run_hex_output_tests<uint32x4_t>();
    run_hex_output_tests<uint256_t>();
    run_hex_output_tests<uint32x8_t>();
    run_hex_output_tests<uint64x4_t>();

    // full 128-bit value
    std::ostringstream ss;
    ss.str("");
    ss << std::hex << 0xaf5705a4'89525e79'a5120c42'daebbc57_ui128;
    ASSERT_EQ(ss.str(), "af5705a489525e79a5120c42daebbc57");
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, octal_output)
{
    run_octal_output_tests<uint128_t>();
    run_octal_output_tests<uint32x4_t>();
    run_octal_output_tests<uint256_t>();
    run_octal_output_tests<uint32x8_t>();
    run_octal_output_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, stream_format_switching)
{
    run_stream_format_switching_tests<uint128_t>();
    run_stream_format_switching_tests<uint32x4_t>();
    run_stream_format_switching_tests<uint256_t>();
    run_stream_format_switching_tests<uint32x8_t>();
    run_stream_format_switching_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, uppercase_and_nouppercase_output)
{
    run_uppercase_and_nouppercase_output_tests<uint128_t>();
    run_uppercase_and_nouppercase_output_tests<uint32x4_t>();
    run_uppercase_and_nouppercase_output_tests<uint256_t>();
    run_uppercase_and_nouppercase_output_tests<uint32x8_t>();
    run_uppercase_and_nouppercase_output_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, family_128_unsigned_decimal_round_trip)
{
    run_128_unsigned_decimal_round_trip_tests<uint128_t>();
    run_128_unsigned_decimal_round_trip_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, family_128_signed_decimal_round_trip)
{
    run_128_signed_decimal_round_trip_tests<int128_t>();
    run_128_signed_decimal_round_trip_tests<int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, family_128_unsigned_hex_input)
{
    run_128_unsigned_hex_input_tests<uint128_t>();
    run_128_unsigned_hex_input_tests<uint32x4_t>();

    // mixed-case hexadecimal input

    std::istringstream mixed_stream("Af5705A489525E79A5120C42DAEBBC57");
    uint128_t mixed_value;
    mixed_stream >> std::hex >> mixed_value;
    ASSERT_EQ(mixed_value, 0xaf5705a4'89525e79'a5120c42'daebbc57_ui128);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, family_128_unsigned_octal_input)
{
    run_128_unsigned_octal_input_tests<uint128_t>();
    run_128_unsigned_octal_input_tests<uint32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, family_128_signed_and_unsigned_decimal_input_with_whitespace_and_leading_zeros)
{
    run_128_signed_and_unsigned_decimal_input_with_whitespace_and_leading_zeros_tests<uint128_t, int128_t>();
    run_128_signed_and_unsigned_decimal_input_with_whitespace_and_leading_zeros_tests<uint32x4_t, int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, invalid_input_sets_fail_or_stops_at_valid_prefix)
{
    run_unsigned_input_error_tests<uint128_t>();
    run_unsigned_input_error_tests<uint32x4_t>();
    run_unsigned_input_error_tests<uint256_t>();
    run_unsigned_input_error_tests<uint32x8_t>();
    run_unsigned_input_error_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, family_128_signed_input_sign_handling)
{
    run_128_signed_input_sign_handling_tests<int128_t>();
    run_128_signed_input_sign_handling_tests<int32x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, input_overflow_sets_failbit)
{
    run_unsigned_input_overflow_tests<uint128_t>();
    run_unsigned_input_overflow_tests<uint32x4_t>();
    run_unsigned_input_overflow_tests<uint256_t>();
    run_unsigned_input_overflow_tests<uint32x8_t>();
    run_unsigned_input_overflow_tests<uint64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, family_256_signed_and_unsigned_special_value_decimal_round_trip)
{
    run_special_value_decimal_round_trip_tests<uint256_t, int256_t, 255>();
    run_special_value_decimal_round_trip_tests<uint32x8_t, int32x8_t, 255>();
    run_special_value_decimal_round_trip_tests<uint64x4_t, int64x4_t, 255>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, family_256_signed_and_unsigned_input_edge_and_error_behavior)
{
    run_input_edge_and_error_behavior_tests<uint256_t, int256_t>();
    run_input_edge_and_error_behavior_tests<uint32x8_t, int32x8_t>();
    run_input_edge_and_error_behavior_tests<uint64x4_t, int64x4_t>();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(io_tests, family_256_signed_and_unsigned_stream_state_contract)
{
    run_stream_state_contract_tests<uint256_t, int256_t>();
    run_stream_state_contract_tests<uint32x8_t, int32x8_t>();
    run_stream_state_contract_tests<uint64x4_t, int64x4_t>();
}

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// io_tests.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////
