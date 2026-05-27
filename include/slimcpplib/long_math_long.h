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

#include "long_math.h"

#include <functional>

#if __has_include("long_math_gcc.h")
#include "long_math_gcc.h"
#endif // __has_include("long_math_gcc.h")

#if __has_include("long_math_msvc.h")
#include "long_math_msvc.h"
#endif // __has_include("long_math_msvc.h")

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

// multiply with carry

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> mul(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> mulc(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, const long_uint_t<type_t, size>& carry) noexcept;

// divide

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> divq(const long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept;
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> divqr(const long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, long_uint_t<type_t, size>& remainder) noexcept;

// negate vector

template<typename type_t, std::enable_if_t<is_unsigned_array_v<type_t>, int> = 0>
constexpr void negate(type_t& value) noexcept;

// add two vectors

template<typename type_t, std::enable_if_t<is_unsigned_array_v<type_t>, int> = 0>
constexpr void add(type_t& value1, const type_t& value2) noexcept;

// subtract two vectors

template<typename type_t, std::enable_if_t<is_unsigned_array_v<type_t>, int> = 0>
constexpr void sub(type_t& value1, const type_t& value2) noexcept;

// multiply two vectors

template<typename value_t, std::enable_if_t<is_unsigned_array_v<value_t>, int> = 0>
constexpr value_t mul(value_t& value1, const value_t& value2) noexcept;



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
constexpr long_uint_t<type_t, size> mul(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    return mul(value1.digits, value2.digits);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> mulc(long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, const long_uint_t<type_t, size>& carry) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    long_uint_t carry_hi = mul(value1.digits, value2.digits);

    bool add_carry = false;

    for (uint_t n = 0; n < size; ++n)
        add_carry = addc(value1.digits[n], carry.digits[n], add_carry);

    for (uint_t n = 0; add_carry && n < size; ++n)
        add_carry = add(carry_hi.digits[n], type_t(1));

    return carry_hi;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> divq(const long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;
    using half_uint_t = half_t<long_uint_t>;

    const half_uint_t dividend_lo = half_lo(value1);
    const half_uint_t dividend_hi = half_hi(value1);
    const half_uint_t divider_lo = half_lo(value2);
    const half_uint_t divider_hi = half_hi(value2);

    long_uint_t quotient;

    if (divider_hi == 0) {

        half_uint_t quotient_lo;
        half_uint_t quotient_hi;

        if (divider_lo > dividend_hi) {

            if (dividend_hi == 0)
                quotient_lo = divq<half_uint_t>(dividend_lo, divider_lo);
            else
                quotient_lo = divq2<half_uint_t>(dividend_hi, dividend_lo, divider_lo);

            quotient_hi = 0;

        } else {

            quotient_lo = divq2<half_uint_t>(dividend_hi % divider_lo, dividend_lo, divider_lo);
            quotient_hi = dividend_hi / divider_lo;
        }

        quotient = half_make(quotient_hi, quotient_lo);

    } else
        quotient = divq2<long_uint_t>(0, value1, value2);
    
    return quotient;
}



////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning(push)
#pragma warning(disable : 4724)

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> divqr(const long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, long_uint_t<type_t, size>& remainder) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;
    using half_uint_t = half_t<long_uint_t>;

    const half_uint_t dividend_lo = half_lo(value1);
    const half_uint_t dividend_hi = half_hi(value1);
    const half_uint_t divider_lo = half_lo(value2);
    const half_uint_t divider_hi = half_hi(value2);

    long_uint_t quotient;

    if (divider_hi == 0) {

        half_uint_t quotient_lo;
        half_uint_t quotient_hi;
        half_uint_t remainder_lo;

        if (divider_lo > dividend_hi) {

            if (dividend_hi == 0)
                quotient_lo = divqr<half_uint_t>(dividend_lo, divider_lo, remainder_lo);
            else
                quotient_lo = divqr2<half_uint_t>(dividend_hi, dividend_lo, divider_lo, remainder_lo);

            quotient_hi = 0;

        } else {
            quotient_lo = divqr2<half_uint_t>(dividend_hi % divider_lo, dividend_lo, divider_lo, remainder_lo);
            quotient_hi = dividend_hi / divider_lo;
        }

        quotient = half_make(quotient_hi, quotient_lo);
        remainder = half_make(half_uint_t(0), remainder_lo);

    } else
        quotient = divqr2<long_uint_t>(0, value1, value2, remainder);
    
    return quotient;
}

#pragma warning(pop)



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_array_v<type_t>, int>>
constexpr void negate(type_t& value) noexcept
{
    using value_t = typename type_t::value_type;

    bool borrow = true;

    for (uint_t n = 0; n < std::size(value); ++n) {

        borrow = subb(value[n], value_t(0), borrow);
        value[n] = ~value[n];
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_array_v<type_t>, int>>
constexpr void add(type_t& value1, const type_t& value2) noexcept
{
    bool carry = add(value1[0], value2[0]);

    for (uint_t n = 1; n < std::size(value1); ++n)
        carry = addc(value1[n], value2[n], carry);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_array_v<type_t>, int>>
constexpr void sub(type_t& value1, const type_t& value2) noexcept
{
    bool borrow = sub(value1[0], value2[0]);

    for (uint_t n = 1; n < std::size(value1); ++n)
        borrow = subb(value1[n], value2[n], borrow);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename value_t, std::enable_if_t<is_unsigned_array_v<value_t>, int>>
constexpr value_t mul(value_t& value1, const value_t& value2) noexcept
{
    using native_t = typename value_t::value_type;

    std::array<native_t, std::tuple_size_v<value_t> * 2> result = {};

    for (uint_t value2_idx = 0; value2_idx < std::size(value2); ++value2_idx) {

        for (uint_t value1_idx = 0; value1_idx < std::size(value1); ++value1_idx) {

            const uint_t result_idx = value1_idx + value2_idx;

            native_t product_lo = value1[value1_idx];
            const native_t product_hi = mul(product_lo, value2[value2_idx]);

            bool carry = add(result[result_idx], product_lo);
            carry = addc(result[result_idx + 1], product_hi, carry);

            for (uint_t n = result_idx + 2; carry && n < std::size(result); ++n)
                carry = add(result[n], native_t(1));
        }
    }

    value_t carry;

    for (uint_t n = 0; n < std::size(value1); ++n) {

        value1[n] = result[n];
        carry[n] = result[n + std::size(value1)];
    }

    return carry;
}

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of long_math_long.h
////////////////////////////////////////////////////////////////////////////////////////////////////
