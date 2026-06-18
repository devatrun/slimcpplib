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

#include "long_math_array.h"

#include <functional>

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// type definitions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t, uint_t size>
class long_uint_t;
template<typename type_t, uint_t size>
class long_int_t;

} // namespace slim

namespace std
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// class numeric_limits<slim::long_uint_t<type_t, size>>
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, size_t size>
struct numeric_limits<slim::long_uint_t<type_t, size>> {
public:

    using long_uint_t = slim::long_uint_t<type_t, size>;

    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;

    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr float_denorm_style has_denorm = denorm_absent;
    static constexpr bool has_denorm_loss = false;
    static constexpr float_round_style round_style = round_toward_zero;

    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = true;

    static constexpr int digits = static_cast<int>(slim::bit_count_v<long_uint_t>);
    static constexpr int digits10 = digits * 301 / 1000;
    static constexpr int radix = 2;

    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;

    static constexpr bool traps = false;
    static constexpr bool tinyness_before = false;

    static constexpr long_uint_t min() noexcept
    {
        return long_uint_t(0);
    }

    static constexpr long_uint_t lowest() noexcept
    {
        return long_uint_t(0);
    }

    static constexpr long_uint_t max() noexcept
    {
        return long_uint_t(-1);
    }

    static constexpr long_uint_t epsilon() noexcept
    {
        return long_uint_t(0);
    }

    static constexpr long_uint_t round_error() noexcept
    {
        return long_uint_t(0);
    }

    static constexpr long_uint_t infinity() noexcept
    {
        return long_uint_t(0);
    }

    static constexpr long_uint_t quiet_NaN() noexcept
    {
        return long_uint_t(0);
    }

    static constexpr long_uint_t signaling_NaN() noexcept
    {
        return long_uint_t(0);
    }

    static constexpr long_uint_t denorm_min() noexcept
    {
        return long_uint_t(0);
    }
};



////////////////////////////////////////////////////////////////////////////////////////////////////
// class numeric_limits<slim::long_int_t<type_t, size>>
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, size_t size>
struct numeric_limits<slim::long_int_t<type_t, size>> {
public:
    using long_int_t = slim::long_int_t<type_t, size>;

    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = true;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;

    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr float_denorm_style has_denorm = denorm_absent;
    static constexpr bool has_denorm_loss = false;
    static constexpr float_round_style round_style = round_toward_zero;

    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = true;

    static constexpr int digits = static_cast<int>(slim::bit_count_v<long_int_t> - 1);
    static constexpr int digits10 = digits * 301 / 1000;
    static constexpr int radix = 2;

    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;

    static constexpr bool traps = false;
    static constexpr bool tinyness_before = false;

    static constexpr long_int_t min() noexcept
    {
        return slim::long_uint_t<type_t, size>(1) << (slim::bit_count_v<long_int_t> - 1);
    }

    static constexpr long_int_t lowest() noexcept
    {
        return min();
    }

    static constexpr long_int_t max() noexcept
    {
        return ~slim::long_uint_t<type_t, size>(min());
    }

    static constexpr long_int_t epsilon() noexcept
    {
        return long_int_t(0);
    }

    static constexpr long_int_t round_error() noexcept
    {
        return long_int_t(0);
    }

    static constexpr long_int_t infinity() noexcept
    {
        return long_int_t(0);
    }

    static constexpr long_int_t quiet_NaN() noexcept
    {
        return long_int_t(0);
    }

    static constexpr long_int_t signaling_NaN() noexcept
    {
        return long_int_t(0);
    }

    static constexpr long_int_t denorm_min() noexcept
    {
        return long_int_t(0);
    }
};



////////////////////////////////////////////////////////////////////////////////////////////////////
// class hash<slim::long_uint_t<type_t, size>>
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, size_t size>
struct hash<slim::long_uint_t<type_t, size>> {
public:
    using long_uint_t = slim::long_uint_t<type_t, size>;

    static constexpr size_t combine_hash(size_t value1, size_t value2) noexcept
    {
        return value1 ^ (value2 + size_t(0x9e3779b9) + (value1 << 6) + (value1 >> 2));
    }

    static constexpr size_t hash_word(type_t value) noexcept
    {
        if constexpr (sizeof(type_t) <= sizeof(size_t))
            return static_cast<size_t>(value);
        else
            return combine_hash(static_cast<size_t>(slim::half_lo(value)), static_cast<size_t>(slim::half_hi(value)));
    }

    constexpr size_t operator()(const long_uint_t& value) const noexcept
    {
        size_t result = hash_word(value.digits[0]);

        for (slim::uint_t n = 1; n < size; ++n)
            result = combine_hash(result, hash_word(value.digits[n]));

        return result;
    }
};



////////////////////////////////////////////////////////////////////////////////////////////////////
// class hash<slim::long_int_t<type_t, size>>
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, size_t size>
struct hash<slim::long_int_t<type_t, size>> {
public:
    using long_uint_t = slim::long_uint_t<type_t, size>;
    using long_int_t = slim::long_int_t<type_t, size>;

    constexpr size_t operator()(const long_int_t& value) const noexcept
    {
        return hash<long_uint_t>()(static_cast<const long_uint_t&>(value));
    }
};

} // namespace std

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// make_unsigned_t
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename native_t, uint_t size>
struct make_unsigned<long_uint_t<native_t, size>> {
    using type = long_uint_t<native_t, size>;
};
template<typename native_t, uint_t size>
struct make_unsigned<long_int_t<native_t, size>> {
    using type = long_uint_t<native_t, size>;
};



////////////////////////////////////////////////////////////////////////////////////////////////////
// make_signed_t
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename native_t, uint_t size>
struct make_signed<long_uint_t<native_t, size>> {
    using type = long_int_t<native_t, size>;
};
template<typename native_t, uint_t size>
struct make_signed<long_int_t<native_t, size>> {
    using type = long_int_t<native_t, size>;
};



////////////////////////////////////////////////////////////////////////////////////////////////////
// half_t
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t, uint_t size>
struct half_type<long_uint_t<type_t, size>> {
    using type = long_uint_t<type_t, size / 2>;
};
template<typename type_t>
struct half_type<long_uint_t<type_t, 2>> {
    using type = type_t;
};



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

// extract low half of unsigned integer

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size / 2> half_lo(const long_uint_t<type_t, size>& value) noexcept;
template<typename type_t>
constexpr type_t half_lo(const long_uint_t<type_t, 2>& value) noexcept;

// extract high half of unsigned integer

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size / 2> half_hi(const long_uint_t<type_t, size>& value) noexcept;
template<typename type_t>
constexpr type_t half_hi(const long_uint_t<type_t, 2>& value) noexcept;

// make unsigned integer from low and high

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size * 2> half_make(const long_uint_t<type_t, size>& high, const long_uint_t<type_t, size>& low) noexcept;
template<typename type_t>
constexpr long_uint_t<type_t, 2> half_make(const type_t& high, const type_t& low) noexcept;

// move low half to high

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> half_make_hi(const long_uint_t<type_t, size>& value) noexcept;

// return most significant bit

template<typename type_t, uint_t size>
constexpr bool sign(const long_uint_t<type_t, size>& value) noexcept;
template<typename type_t, uint_t size>
constexpr bool sign(const long_int_t<type_t, size>& value) noexcept;

// calculate leading zero bits

template<typename type_t, uint_t size>
constexpr uint_t nlz(const long_uint_t<type_t, size>& value) noexcept;
template<typename type_t, uint_t size>
constexpr bool half_hi_zero(const long_uint_t<type_t, size>& value) noexcept;

// bit operations

template<typename type_t, uint_t size>
constexpr void bit_not(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;
template<typename type_t, uint_t size>
constexpr void bit_and(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;
template<typename type_t, uint_t size>
constexpr void bit_or(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;
template<typename type_t, uint_t size>
constexpr void bit_xor(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;

// shift bits to the left

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> shl2(const long_uint_t<type_t, size>& value_hi, const long_uint_t<type_t, size>& value_lo, uint_t shift) noexcept;
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> shr2(const long_uint_t<type_t, size>& value_hi, const long_uint_t<type_t, size>& value_lo, uint_t shift) noexcept;

// add with carry

template<typename type_t, uint_t size>
constexpr bool add(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;
template<typename type_t, uint_t size>
constexpr bool addc(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, bool carry) noexcept;

// subtract with borrow

template<typename type_t, uint_t size>
constexpr bool sub(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;
template<typename type_t, uint_t size>
constexpr bool subb(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, bool borrow) noexcept;

// compare values

template<typename type_t, uint_t size>
constexpr bool cmp_equal(const long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;
template<typename type_t, uint_t size>
constexpr bool cmp_less(const long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;

// multiply with carry

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> mul(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> mulc(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, const long_uint_t<type_t, size>& carry) noexcept;

// divide

template<typename type_t, uint_t size>
constexpr void div(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> divr(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;
template<typename type_t, uint_t size>
constexpr void div2(long_uint_t<type_t, size> value1_hi, long_uint_t<type_t, size>& value1_lo, long_uint_t<type_t, size> value2) noexcept;
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> divr2(long_uint_t<type_t, size> value1_hi, long_uint_t<type_t, size>& value1_lo, long_uint_t<type_t, size> value2) noexcept;
template<typename type_t, uint_t size>
constexpr void mod(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;
template<typename type_t, uint_t size>
constexpr void mod2(long_uint_t<type_t, size> value1_hi, long_uint_t<type_t, size>& value1_lo, long_uint_t<type_t, size> value2) noexcept;




////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size / 2> half_lo(const long_uint_t<type_t, size>& value) noexcept
{
    constexpr uint_t half_size = size / 2;
    long_uint_t<type_t, half_size> result;

    for (uint_t n = 0; n < half_size; ++n)
        result.digits[n] = value.digits[n];

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t>
constexpr type_t half_lo(const long_uint_t<type_t, 2>& value) noexcept
{
    return value.digits[0];
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size / 2> half_hi(const long_uint_t<type_t, size>& value) noexcept
{
    constexpr uint_t half_size = size / 2;
    long_uint_t<type_t, half_size> result;

    for (uint_t n = 0; n < half_size; ++n)
        result.digits[n] = value.digits[n + half_size];

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t>
constexpr type_t half_hi(const long_uint_t<type_t, 2>& value) noexcept
{
    return value.digits[1];
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size * 2> half_make(const long_uint_t<type_t, size>& value_hi, const long_uint_t<type_t, size>& value_lo) noexcept
{
    constexpr uint_t double_size = size * 2;
    long_uint_t<type_t, double_size> result;

    for (uint_t n = 0; n < size; ++n)
        result.digits[n] = value_lo.digits[n];
    for (uint_t n = size; n < double_size; ++n)
        result.digits[n] = value_hi.digits[n - size];

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t>
constexpr long_uint_t<type_t, 2> half_make(const type_t& value_hi, const type_t& value_lo) noexcept
{
    return long_uint_t<type_t, 2>({ value_lo, value_hi });
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> half_make_hi(const long_uint_t<type_t, size>& value) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    constexpr uint_t half_size = size / 2;
    long_uint_t result;

    for (uint_t n = 0; n < half_size; ++n)
        result.digits[n] = 0;
    for (uint_t n = half_size; n < size; ++n)
        result.digits[n] = value.digits[n - half_size];

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr bool sign(const long_uint_t<type_t, size>& value) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    return make_signed_t<type_t>(value.digits[long_uint_t::hi]) < 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr bool sign(const long_int_t<type_t, size>& value) noexcept
{
    return value.sign();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr uint_t nlz(const long_uint_t<type_t, size>& value) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    uint_t count = 0;

    for (uint_t n = std::size(value.digits); n-- > 0;) {

        const uint_t scount = nlz(value.digits[n]);
        count += scount;

        if (scount < bit_count_v<type_t>)
            break;
    }

    return count;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr bool half_hi_zero(const long_uint_t<type_t, size>& value) noexcept
{
    for (uint_t n = size / 2; n < size; ++n) {

        if (value.digits[n] != 0)
            return false;
    }

    return true;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr void bit_not(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    array::bit_not(std::span(value1.digits), std::span(value2.digits));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr void bit_and(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    array::bit_and(std::span(value1.digits), std::span(value2.digits));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr void bit_or(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    array::bit_or(std::span(value1.digits), std::span(value2.digits));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr void bit_xor(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    array::bit_xor(std::span(value1.digits), std::span(value2.digits));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> shl2(const long_uint_t<type_t, size>& value_hi, const long_uint_t<type_t, size>& value_lo, uint_t shift) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    if (shift == 0 || shift >= bit_count_v<long_uint_t> * 2)
        return value_hi;

    if (shift < bit_count_v<long_uint_t>) {
        long_uint_t result = value_hi;
        long_uint_t carry = value_lo;

        array::shl(std::span(result.digits), shift);
        array::shr(std::span(carry.digits), bit_count_v<long_uint_t> - shift);
        bit_or(result, carry);

        return result;
    }

    long_uint_t result = value_lo;
    array::shl(std::span(result.digits), shift - bit_count_v<long_uint_t>);
    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> shr2(const long_uint_t<type_t, size>& value_hi, const long_uint_t<type_t, size>& value_lo, uint_t shift) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    if (shift == 0 || shift >= bit_count_v<long_uint_t> * 2)
        return value_lo;

    if (shift < bit_count_v<long_uint_t>) {
        long_uint_t result = value_lo;
        long_uint_t carry = value_hi;

        array::shr(std::span(result.digits), shift);
        array::shl(std::span(carry.digits), bit_count_v<long_uint_t> - shift);
        bit_or(result, carry);

        return result;
    }

    long_uint_t result = value_hi;
    array::shr(std::span(result.digits), shift - bit_count_v<long_uint_t>);
    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr bool add(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    return array::add(std::span(value1.digits), std::span(value2.digits));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr bool addc(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, bool carry) noexcept
{
    return array::addc(std::span(value1.digits), std::span(value2.digits), carry);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr bool sub(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    return array::sub(std::span(value1.digits), std::span(value2.digits));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr bool subb(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, bool borrow) noexcept
{
    return array::subb(std::span(value1.digits), std::span(value2.digits), borrow);
}



template<typename type_t, uint_t size>
constexpr bool cmp_equal(const long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    return array::cmp_equal(std::span(value1.digits), std::span(value2.digits));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr bool cmp_less(const long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    return array::cmp_less(std::span(value1.digits), std::span(value2.digits));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> mul(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    long_uint_t carry;

    array::mul(std::span(value1.digits), std::span(value2.digits), std::span(carry.digits));

    return carry;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> mulc(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, const long_uint_t<type_t, size>& carry) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    long_uint_t carry_hi;
    array::mul(std::span(value1.digits), std::span(value2.digits), std::span(carry_hi.digits));

    const bool add_carry = array::add(std::span(value1.digits), std::span(carry.digits));

    bool carry_digit = add_carry;

    for (uint_t n = 0; carry_digit && n < size; ++n)
        carry_digit = add(carry_hi.digits[n], type_t(1));

    return carry_hi;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr void div2(long_uint_t<type_t, size> value1_hi, long_uint_t<type_t, size>& value1_lo, long_uint_t<type_t, size> value2) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    if (value2 != 0 && value1_hi >= value2) {

        value1_lo = ~long_uint_t(0);
        return;
    }

    const uint_t shift = nlz(value2);
    long_uint_t svalue2 = value2;
    array::shl(std::span(svalue2.digits), shift);

    const long_uint_t nvalue2_hi = half_hi(svalue2);
    const long_uint_t nvalue2_lo = half_lo(svalue2);

    const long_uint_t nvalue1_32 = shl2(value1_hi, value1_lo, shift);
    long_uint_t nvalue1_10 = value1_lo;
    array::shl(std::span(nvalue1_10.digits), shift);

    const long_uint_t nvalue1_hi = half_hi(nvalue1_10);
    const long_uint_t nvalue1_lo = half_lo(nvalue1_10);

    long_uint_t quotient_hi = nvalue1_32;
    const long_uint_t remainder_hi = divr(quotient_hi, nvalue2_hi);

    long_uint_t t1 = quotient_hi;
    mul(t1, nvalue2_lo);
    long_uint_t t2 = half_make_hi(remainder_hi);
    bit_or(t2, nvalue1_hi);

    if (cmp_less(t2, t1)) {

        --quotient_hi;

        long_uint_t delta = t1;
        sub(delta, t2);

        if (cmp_less(svalue2, delta))
            --quotient_hi;
    }

    long_uint_t nvalue1_21 = half_make_hi(nvalue1_32);
    add(nvalue1_21, nvalue1_hi);
    long_uint_t product_hi = quotient_hi;
    mul(product_hi, svalue2);
    sub(nvalue1_21, product_hi);

    long_uint_t quotient_lo = nvalue1_21;
    const long_uint_t remainder_lo = divr(quotient_lo, nvalue2_hi);

    long_uint_t t3 = quotient_lo;
    mul(t3, nvalue2_lo);
    long_uint_t t4 = half_make_hi(remainder_lo);
    bit_or(t4, nvalue1_lo);

    if (cmp_less(t4, t3)) {

        --quotient_lo;

        long_uint_t delta = t3;
        sub(delta, t4);

        if (cmp_less(svalue2, delta))
            --quotient_lo;
    }

    value1_lo = half_make_hi(quotient_hi);
    bit_or(value1_lo, quotient_lo);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> divr2(long_uint_t<type_t, size> value1_hi, long_uint_t<type_t, size>& value1_lo, long_uint_t<type_t, size> value2) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    if (value2 != 0 && value1_hi >= value2) {

        value1_lo = ~long_uint_t(0);
        return value1_lo;
    }

    const uint_t shift = nlz(value2);
    long_uint_t svalue2 = value2;
    array::shl(std::span(svalue2.digits), shift);

    const long_uint_t nvalue2_hi = half_hi(svalue2);
    const long_uint_t nvalue2_lo = half_lo(svalue2);

    const long_uint_t nvalue1_32 = shl2(value1_hi, value1_lo, shift);
    long_uint_t nvalue1_10 = value1_lo;
    array::shl(std::span(nvalue1_10.digits), shift);

    const long_uint_t nvalue1_hi = half_hi(nvalue1_10);
    const long_uint_t nvalue1_lo = half_lo(nvalue1_10);

    long_uint_t quotient_hi = nvalue1_32;
    const long_uint_t remainder_hi = divr(quotient_hi, nvalue2_hi);

    long_uint_t t1 = quotient_hi;
    mul(t1, nvalue2_lo);
    long_uint_t t2 = half_make_hi(remainder_hi);
    bit_or(t2, nvalue1_hi);

    if (cmp_less(t2, t1)) {

        --quotient_hi;

        long_uint_t delta = t1;
        sub(delta, t2);

        if (cmp_less(svalue2, delta))
            --quotient_hi;
    }

    long_uint_t nvalue1_21 = half_make_hi(nvalue1_32);
    add(nvalue1_21, nvalue1_hi);
    long_uint_t product_hi = quotient_hi;
    mul(product_hi, svalue2);
    sub(nvalue1_21, product_hi);

    long_uint_t quotient_lo = nvalue1_21;
    const long_uint_t remainder_lo = divr(quotient_lo, nvalue2_hi);

    long_uint_t t3 = quotient_lo;
    mul(t3, nvalue2_lo);
    long_uint_t t4 = half_make_hi(remainder_lo);
    bit_or(t4, nvalue1_lo);

    if (cmp_less(t4, t3)) {

        --quotient_lo;

        long_uint_t delta = t3;
        sub(delta, t4);

        if (cmp_less(svalue2, delta))
            --quotient_lo;
    }

    value1_lo = half_make_hi(quotient_hi);
    bit_or(value1_lo, quotient_lo);

    long_uint_t result = half_make_hi(nvalue1_21);
    add(result, nvalue1_lo);
    long_uint_t product_lo = quotient_lo;
    mul(product_lo, svalue2);
    sub(result, product_lo);
    array::shr(std::span(result.digits), shift);
    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr void mod2(long_uint_t<type_t, size> value1_hi, long_uint_t<type_t, size>& value1_lo, long_uint_t<type_t, size> value2) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    if (value2 != 0 && value1_hi >= value2) {

        value1_lo = ~long_uint_t(0);
        return;
    }

    const uint_t shift = nlz(value2);
    long_uint_t svalue2 = value2;
    array::shl(std::span(svalue2.digits), shift);

    const long_uint_t nvalue2_hi = half_hi(svalue2);
    const long_uint_t nvalue2_lo = half_lo(svalue2);

    const long_uint_t nvalue1_32 = shl2(value1_hi, value1_lo, shift);
    long_uint_t nvalue1_10 = value1_lo;
    array::shl(std::span(nvalue1_10.digits), shift);

    const long_uint_t nvalue1_hi = half_hi(nvalue1_10);
    const long_uint_t nvalue1_lo = half_lo(nvalue1_10);

    long_uint_t quotient_hi = nvalue1_32;
    const long_uint_t remainder_hi = divr(quotient_hi, nvalue2_hi);

    long_uint_t t1 = quotient_hi;
    mul(t1, nvalue2_lo);
    long_uint_t t2 = half_make_hi(remainder_hi);
    bit_or(t2, nvalue1_hi);

    if (cmp_less(t2, t1)) {

        --quotient_hi;

        long_uint_t delta = t1;
        sub(delta, t2);

        if (cmp_less(svalue2, delta))
            --quotient_hi;
    }

    long_uint_t nvalue1_21 = half_make_hi(nvalue1_32);
    add(nvalue1_21, nvalue1_hi);
    long_uint_t product_hi = quotient_hi;
    mul(product_hi, svalue2);
    sub(nvalue1_21, product_hi);

    long_uint_t quotient_lo = nvalue1_21;
    const long_uint_t remainder_lo = divr(quotient_lo, nvalue2_hi);

    long_uint_t t3 = quotient_lo;
    mul(t3, nvalue2_lo);
    long_uint_t t4 = half_make_hi(remainder_lo);
    bit_or(t4, nvalue1_lo);

    if (cmp_less(t4, t3)) {

        --quotient_lo;

        long_uint_t delta = t3;
        sub(delta, t4);

        if (cmp_less(svalue2, delta))
            --quotient_lo;
    }

    long_uint_t result = half_make_hi(nvalue1_21);
    add(result, nvalue1_lo);
    long_uint_t product_lo = quotient_lo;
    mul(product_lo, svalue2);
    sub(result, product_lo);
    array::shr(std::span(result.digits), shift);
    value1_lo = result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr void div(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;
    using half_uint_t = half_t<long_uint_t>;

    if (!half_hi_zero(value2)) {
        div2(long_uint_t(0), value1, value2);
        return;
    }

    const half_uint_t dividend_hi = half_hi(value1);
    const half_uint_t divider_lo = half_lo(value2);
    half_uint_t quotient_lo = half_lo(value1);
    half_uint_t quotient_hi;

    if (divider_lo > dividend_hi) {

        if (dividend_hi == 0) {
            div(quotient_lo, divider_lo);
        } else {
            div2(dividend_hi, quotient_lo, divider_lo);
        }

        quotient_hi = 0;

    } else {
        half_uint_t remainder_hi = dividend_hi;
        mod(remainder_hi, divider_lo);
        div2(remainder_hi, quotient_lo, divider_lo);

        quotient_hi = dividend_hi;
        div(quotient_hi, divider_lo);
    }

    value1 = half_make(quotient_hi, quotient_lo);
}



////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning(push)
#pragma warning(disable : 4724)

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> divr(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;
    using half_uint_t = half_t<long_uint_t>;

    if (!half_hi_zero(value2))
        return divr2(long_uint_t(0), value1, value2);

    const half_uint_t dividend_hi = half_hi(value1);
    const half_uint_t divider_lo = half_lo(value2);
    half_uint_t quotient_lo = half_lo(value1);
    half_uint_t quotient_hi;
    half_uint_t remainder_lo;

    if (divider_lo > dividend_hi) {

        if (dividend_hi == 0) {
            remainder_lo = divr(quotient_lo, divider_lo);
        } else {
            remainder_lo = divr2(dividend_hi, quotient_lo, divider_lo);
        }

        quotient_hi = 0;

    } else {
        half_uint_t remainder_hi = dividend_hi;
        mod(remainder_hi, divider_lo);
        remainder_lo = divr2(remainder_hi, quotient_lo, divider_lo);

        quotient_hi = dividend_hi;
        div(quotient_hi, divider_lo);
    }

    value1 = half_make(quotient_hi, quotient_lo);
    return half_make(half_uint_t(0), remainder_lo);
}

#pragma warning(pop)



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr void mod(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;
    using half_uint_t = half_t<long_uint_t>;

    if (!half_hi_zero(value2)) {
        mod2(long_uint_t(0), value1, value2);
        return;
    }

    const half_uint_t dividend_hi = half_hi(value1);
    const half_uint_t divider_lo = half_lo(value2);
    half_uint_t remainder_lo = half_lo(value1);

    if (divider_lo > dividend_hi) {

        if (dividend_hi == 0)
            mod(remainder_lo, divider_lo);
        else
            mod2(dividend_hi, remainder_lo, divider_lo);

    } else {

        half_uint_t remainder_hi = dividend_hi;
        mod(remainder_hi, divider_lo);
        mod2(remainder_hi, remainder_lo, divider_lo);
    }

    value1 = half_make(half_uint_t(0), remainder_lo);
}



} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of long_math_long.h
////////////////////////////////////////////////////////////////////////////////////////////////////