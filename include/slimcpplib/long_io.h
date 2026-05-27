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

#pragma once

#include "long_uint.h"

#include <array>
#include <iostream>
#include <limits>
#include <optional>

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone methods
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename native_t, uint_t size, typename char_t, class traits_t = std::char_traits<char_t>>
std::basic_ostream<char_t, traits_t>& operator<<(std::basic_ostream<char_t, traits_t>& stream, const long_uint_t<native_t, size>& value);
template<typename native_t, uint_t size, typename char_t, class traits_t = std::char_traits<char_t>>
std::basic_ostream<char_t, traits_t>& operator<<(std::basic_ostream<char_t, traits_t>& stream, const long_int_t<native_t, size>& value);
template<typename native_t, uint_t size, typename char_t, class traits_t = std::char_traits<char_t>>
std::basic_istream<char_t, traits_t>& operator>>(std::basic_istream<char_t, traits_t>& stream, long_uint_t<native_t, size>& value);
template<typename native_t, uint_t size, typename char_t, class traits_t = std::char_traits<char_t>>
std::basic_istream<char_t, traits_t>& operator>>(std::basic_istream<char_t, traits_t>& stream, long_int_t<native_t, size>& value);

namespace impl
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// struct chunk_traits_t
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename native_t>
struct chunk_traits_t {
    native_t chunk_base = 1;
    uint_t digits_per_chunk = 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone helper methods
////////////////////////////////////////////////////////////////////////////////////////////////////

inline uint_t get_base(std::ios::fmtflags flags) noexcept;
template<typename char_t>
std::optional<uint_t> parse_digit(char_t ch, uint_t base) noexcept;
template<typename native_t>
chunk_traits_t<native_t> get_chunk_traits(uint_t base) noexcept;
template<typename long_uint_t>
bool append_chunk(long_uint_t& value, uint_t base, typename long_uint_t::native_array_t::value_type chunk, uint_t digit_count) noexcept;
template<typename long_uint_t, typename char_t, class traits_t = std::char_traits<char_t>>
void write_unsigned_integer(std::basic_ostream<char_t, traits_t>& stream, const long_uint_t& magnitude, const char_t* sign_prefix = nullptr, uint_t sign_prefix_size = 0);
template<typename long_uint_t, typename char_t, class traits_t = std::char_traits<char_t>>
std::optional<long_uint_t> read_unsigned_integer(std::basic_istream<char_t, traits_t>& stream);
template<typename char_t, class traits_t = std::char_traits<char_t>>
void write_formatted(std::basic_ostream<char_t, traits_t>& stream, const char_t* prefix, uint_t prefix_size, const char_t* digits, uint_t digit_size);

} // namespace impl

////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone methods
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename native_t, uint_t size, typename char_t, class traits_t>
inline std::basic_ostream<char_t, traits_t>& operator<<(std::basic_ostream<char_t, traits_t>& stream, const long_uint_t<native_t, size>& value)
{
    impl::write_unsigned_integer(stream, value);

    return stream;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size, typename char_t, class traits_t>
inline std::basic_ostream<char_t, traits_t>& operator<<(std::basic_ostream<char_t, traits_t>& stream, const long_int_t<native_t, size>& value)
{
    using long_uint_t = long_uint_t<native_t, size>;
    
    std::array<char_t, 1> sign_prefix;
    uint_t sign_prefix_size = 0;

    // prepare the optional sign prefix and format the magnitude as unsigned

    if (value.sign())
        sign_prefix[sign_prefix_size++] = char_t('-');
    else if ((stream.flags() & std::ios::showpos) != 0)
        sign_prefix[sign_prefix_size++] = char_t('+');

    impl::write_unsigned_integer(stream, value.sign() ? long_uint_t(-value) : long_uint_t(value), sign_prefix.data(), sign_prefix_size);

    return stream;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size, typename char_t, class traits_t>
inline std::basic_istream<char_t, traits_t>& operator>>(std::basic_istream<char_t, traits_t>& stream, long_uint_t<native_t, size>& value)
{
    using long_uint_t = long_uint_t<native_t, size>;
    
    const auto read = impl::read_unsigned_integer<long_uint_t>(stream);

    if (!read.has_value())
        return stream;

    value = *read;

    return stream;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size, typename char_t, class traits_t>
inline std::basic_istream<char_t, traits_t>& operator>>(std::basic_istream<char_t, traits_t>& stream, long_int_t<native_t, size>& value)
{
    using long_uint_t = long_uint_t<native_t, size>;
    using long_int_t = long_int_t<native_t, size>;
    
    const auto read = impl::read_unsigned_integer<long_uint_t>(stream);

    if (!read.has_value())
        return stream;

    // decode the wrapped unsigned representation and validate the signed range

    constexpr long_uint_t uint_min_magnitude = long_uint_t(1) << (bit_count_v<long_int_t> - 1);
    constexpr long_uint_t uint_max_magnitude = uint_min_magnitude - 1;
    const bool negative = sign(*read);
    const long_uint_t limit = negative ? uint_min_magnitude : uint_max_magnitude;
    const long_uint_t magnitude = negative ? -*read : *read;

    if (magnitude > limit) {

        stream.setstate(std::ios::failbit);
        return stream;
    }

    value = negative
        ? (magnitude == uint_min_magnitude ? long_int_t(uint_min_magnitude) : -long_int_t(magnitude))
        : long_int_t(magnitude);

    return stream;
}

namespace impl
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone helper methods
////////////////////////////////////////////////////////////////////////////////////////////////////

inline uint_t get_base(std::ios::fmtflags flags) noexcept
{
    if ((flags & std::ios::basefield) == std::ios::oct)
        return 8;

    if ((flags & std::ios::basefield) == std::ios::hex)
        return 16;

    return 10;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename char_t>
inline std::optional<uint_t> parse_digit(char_t ch, uint_t base) noexcept
{
    if (ch >= char_t('0') && ch <= char_t('9')) {

        const uint_t decimal_digit = ch - char_t('0');

        if (decimal_digit < base)
            return std::make_optional(decimal_digit);

    } else if (ch >= char_t('A') && ch <= char_t('F')) {

        const uint_t upper_hex_digit = ch - char_t('A') + 10;

        if (upper_hex_digit < base)
            return std::make_optional(upper_hex_digit);

    } else if (ch >= char_t('a') && ch <= char_t('f')) {

        const uint_t lower_hex_digit = ch - char_t('a') + 10;

        if (lower_hex_digit < base)
            return std::make_optional(lower_hex_digit);
    }

    return std::nullopt;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t>
inline chunk_traits_t<native_t> get_chunk_traits(uint_t base) noexcept
{
    constexpr native_t max_value = std::numeric_limits<native_t>::max();
    chunk_traits_t<native_t> result;

    // choose the largest base^k that still fits into a native word

    while (result.chunk_base <= max_value / static_cast<native_t>(base)) {

        result.chunk_base *= static_cast<native_t>(base);
        ++result.digits_per_chunk;
    }

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename long_uint_t>
inline bool append_chunk(long_uint_t& value, uint_t base, typename long_uint_t::native_array_t::value_type chunk, uint_t digit_count) noexcept
{
    using native_t = typename long_uint_t::native_array_t::value_type;

    if (digit_count == 0)
        return true;

    // fold one parsed base^k chunk into the current wide value

    native_t scale = 1;
    for (uint_t idx = 0; idx < digit_count; ++idx)
        scale *= static_cast<native_t>(base);

    long_uint_t tmp = value;
    const long_uint_t carry = mulc(tmp, long_uint_t(scale), long_uint_t(chunk));

    if (carry > 0)
        return false;

    value = tmp;
    return true;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename long_uint_t, typename char_t, class traits_t>
inline void write_unsigned_integer(std::basic_ostream<char_t, traits_t>& stream, const long_uint_t& magnitude, const char_t* sign_prefix, uint_t sign_prefix_size)
{
    using native_t = typename long_uint_t::native_array_t::value_type;

    constexpr uint_t max_chars = bit_count_v<long_uint_t> + 5;
    constexpr uint_t max_sign_prefix_size = 1;
    constexpr uint_t max_base_prefix_size = 2;
    constexpr uint_t max_prefix_size = max_sign_prefix_size + max_base_prefix_size;
    constexpr auto upper_digits = "0123456789ABCDEF";
    constexpr auto lower_digits = "0123456789abcdef";

    const bool uppercase = (stream.flags() & std::ios::uppercase) != 0;
    const auto digits_table = uppercase ? upper_digits : lower_digits;
    const uint_t base = get_base(stream.flags());
    
    std::array<char_t, max_chars> digits_buffer;
    char_t* digits_out = digits_buffer.data() + digits_buffer.size();

    // convert the magnitude into base^k chunks and emit digits backwards

    if (magnitude == 0) {

        *--digits_out = char_t('0');

    } else {

        const chunk_traits_t<native_t> traits = get_chunk_traits<native_t>(base);
        
        long_uint_t remainder = magnitude;
        long_uint_t chunk;

        while (remainder != 0) {

            remainder = divqr(remainder, long_uint_t(traits.chunk_base), chunk);
            native_t chunk_value = static_cast<native_t>(chunk);
            uint_t digit_count = traits.digits_per_chunk;

            do {

                *--digits_out = char_t(digits_table[static_cast<uint_t>(chunk_value % static_cast<native_t>(base))]);
                chunk_value /= static_cast<native_t>(base);

                if (digit_count > 0)
                    --digit_count;

            } while (chunk_value > 0 || digit_count > 0);
        }

        while ((digits_buffer.data() + digits_buffer.size() - digits_out) > 1 && *digits_out == char_t('0'))
            ++digits_out;
    }

    std::array<char_t, max_prefix_size> prefix_buffer;
    uint_t prefix_size = 0;
    assert(sign_prefix_size <= max_sign_prefix_size);

    // place sign first, then append the optional base prefix

    if (sign_prefix_size > 0) {

        for (uint_t idx = 0; idx < sign_prefix_size; ++idx)
            prefix_buffer[prefix_size++] = sign_prefix[idx];
    }

    const char_t* digits = digits_out;
    const uint_t digit_size = static_cast<uint_t>(digits_buffer.data() + digits_buffer.size() - digits);
    const bool is_zero = digit_size == 1 && digits[0] == char_t('0');

    if ((stream.flags() & std::ios::showbase) != 0 && !is_zero) {

        if (base == 8)
            prefix_buffer[prefix_size++] = char_t('0');
        else if (base == 16) {

            prefix_buffer[prefix_size++] = char_t('0');
            prefix_buffer[prefix_size++] = uppercase ? char_t('X') : char_t('x');
        }
    }

    write_formatted(stream, prefix_buffer.data(), prefix_size, digits, digit_size);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename long_uint_t, typename char_t, class traits_t>
inline std::optional<long_uint_t> read_unsigned_integer(std::basic_istream<char_t, traits_t>& stream)
{
    using native_t = typename long_uint_t::native_array_t::value_type;

    // use the standard formatted-input guard to honor stream state and skipws

    const typename std::basic_istream<char_t, traits_t>::sentry guard(stream);

    if (!guard)
        return std::nullopt;

    std::basic_streambuf<char_t, traits_t>* const buffer = stream.rdbuf();
    bool negative = false;

    const typename traits_t::int_type ich = buffer->sgetc();

    if (ich == traits_t::eof()) {

        stream.setstate(std::ios::eofbit | std::ios::failbit);
        return std::nullopt;
    }

    const char_t ch = std::char_traits<char_t>::to_char_type(ich);

    // handle an optional leading sign before digit parsing starts

    if (ch == char_t('+') || ch == char_t('-')) {

        negative = (ch == char_t('-'));
        buffer->sbumpc();
    }

    const uint_t base = get_base(stream.flags());
    const chunk_traits_t<native_t> traits = get_chunk_traits<native_t>(base);
    const native_t native_base = static_cast<native_t>(base);

    long_uint_t value = 0;
    bool has_digits = false;
    native_t chunk = 0;
    uint_t digit_count = 0;

    // accumulate input digits into native chunks and flush them into the wide value

    while (true) {

        const typename traits_t::int_type digit_char = buffer->sgetc();

        if (digit_char == traits_t::eof()) {

            stream.setstate(std::ios::eofbit);
            break;
        }

        const char_t digit_source = std::char_traits<char_t>::to_char_type(digit_char);
        const std::optional<uint_t> digit = parse_digit(digit_source, base);

        if (!digit.has_value())
            break;

        has_digits = true;
        const native_t digit_value = static_cast<native_t>(*digit);

        if (digit_count == traits.digits_per_chunk || chunk > (std::numeric_limits<native_t>::max() - digit_value) / native_base) {

            if (!append_chunk(value, base, chunk, digit_count)) {

                stream.setstate(std::ios::failbit);
                return std::nullopt;
            }

            chunk = digit_value;
            digit_count = 1;
            buffer->sbumpc();

            continue;
        }

        chunk = chunk * native_base + digit_value;
        ++digit_count;
        buffer->sbumpc();
    }

    // flush the trailing chunk and apply the parsed sign at the very end

    if (!append_chunk(value, base, chunk, digit_count) || !has_digits) {

        stream.setstate(std::ios::failbit);
        return std::nullopt;
    }

    if (negative)
        value = -value;

    return value;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename char_t, class traits_t>
inline void write_formatted(std::basic_ostream<char_t, traits_t>& stream, const char_t* prefix, uint_t prefix_size, const char_t* digits, uint_t digit_size)
{
    const std::streamsize width = stream.width(0);
    const std::streamsize total_size = static_cast<std::streamsize>(prefix_size + digit_size);
    const std::streamsize fill_size = std::max<std::streamsize>(0, width - total_size);

    const char_t fill = stream.fill();
    const auto adjust = stream.flags() & std::ios::adjustfield;

    std::streamsize leading_fill_size = 0;
    std::streamsize middle_fill_size = 0;
    std::streamsize trailing_fill_size = 0;

    if (fill_size > 0) {

        if (adjust == std::ios::left)
            trailing_fill_size = fill_size;
        else if (adjust == std::ios::internal && prefix_size > 0)
            middle_fill_size = fill_size;
        else
            leading_fill_size = fill_size;
    }

    for (std::streamsize idx = 0; idx < leading_fill_size; ++idx)
        stream.put(fill);

    if (prefix_size > 0)
        stream.write(prefix, static_cast<std::streamsize>(prefix_size));

    for (std::streamsize idx = 0; idx < middle_fill_size; ++idx)
        stream.put(fill);

    if (digit_size > 0)
        stream.write(digits, static_cast<std::streamsize>(digit_size));

    for (std::streamsize idx = 0; idx < trailing_fill_size; ++idx)
        stream.put(fill);
}

} // namespace impl
} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of long_io.h
////////////////////////////////////////////////////////////////////////////////////////////////////
