////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Simple Long Integer Math for C++
// version 1.0
//
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020-2021 Yury Kalmykov <y_kalmykov@mail.ru>.
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
//
// $Id: long_math.h 154 2021-09-13 22:14:28Z ykalmykov $
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <climits>
#include <cstdint>

#include <array>
#include <optional>
#include <type_traits>

#if !(defined(_MSC_VER) && _MSC_VER >= 1910 && ((defined(_MSVC_LANG) && _MSVC_LANG > 201402)) || (__cplusplus > 201402))
#error "Library SLIMCPP requires a compiler that supports C++ 17!"
#endif

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// type definitions
////////////////////////////////////////////////////////////////////////////////////////////////////

using uint_t = uintptr_t;
using int_t = intptr_t;

template<typename type_t, uint_t size>
class long_uint_t;
template<typename type_t, uint_t size>
class long_int_t;



////////////////////////////////////////////////////////////////////////////////////////////////////
// byte_count_v and bit_count_v
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t>
constexpr uint_t byte_count_v = sizeof(type_t);
template<typename type_t>
constexpr uint_t bit_count_v = byte_count_v<type_t>* CHAR_BIT;



////////////////////////////////////////////////////////////////////////////////////////////////////
// is_unsigned_v and is_signed_v
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t>
inline constexpr bool is_unsigned_v = std::is_unsigned_v<type_t>;
template<typename type_t>
inline constexpr bool is_signed_v = std::is_signed_v<type_t>;



////////////////////////////////////////////////////////////////////////////////////////////////////
// make_unsigned_t
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t>
struct make_unsigned {
    using type = typename std::make_unsigned<type_t>::type;
};
template<typename type_t>
using make_unsigned_t = typename make_unsigned<type_t>::type;



////////////////////////////////////////////////////////////////////////////////////////////////////
// is_long_uint_v
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename>
inline constexpr bool is_long_uint_v = false;
template<typename type_t, uint_t size>
inline constexpr bool is_long_uint_v<long_uint_t<type_t, size>> = true;
template<typename type_t, uint_t size>
inline constexpr bool is_long_uint_v<const long_uint_t<type_t, size>> = true;




////////////////////////////////////////////////////////////////////////////////////////////////////
// is_long_int_v
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename>
inline constexpr bool is_long_int_v = false;
template<typename type_t, uint_t size>
inline constexpr bool is_long_int_v<long_int_t<type_t, size>> = true;
template<typename type_t, uint_t size>
inline constexpr bool is_long_int_v<const long_int_t<type_t, size>> = true;



////////////////////////////////////////////////////////////////////////////////////////////////////
// half
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t>
struct half {

    using type = type_t;

    static constexpr type_t hi(const type_t& value) noexcept { return value >> (bit_count_v<type_t> / 2); }
    static constexpr type_t lo(const type_t& value) noexcept { return value & (type_t(~type_t(0)) >> (bit_count_v<type_t> / 2)); }
    static constexpr type_t make(const type_t& value_hi, const type_t& value_lo) noexcept { return (value_hi << (bit_count_v<type_t> / 2)) | value_lo; }
};
template<typename type_t, uint_t size>
struct half<long_uint_t<type_t, size>> {

    using type = long_uint_t<type_t, size / 2>;

    static constexpr type hi(const long_uint_t<type_t, size>& value) noexcept
    {
        type result;

        for (uint_t n = 0; n < size / 2; n++)
            result.digits[n] = value.digits[n + size / 2];

        return result;
    }
    static constexpr type lo(const long_uint_t<type_t, size>& value) noexcept
    {
        type result;

        for (uint_t n = 0; n < size / 2; n++)
            result.digits[n] = value.digits[n];

        return result;
    }
    static constexpr long_uint_t<type_t, size> make(const type& value_hi, const type& value_lo) noexcept
    {
        long_uint_t<type_t, size> result;

        for (uint_t n = 0; n < size / 2; n++)
            result.digits[n] = value_lo.digits[n];
        for (uint_t n = size / 2; n < size; n++)
            result.digits[n] = value_hi.digits[n - size / 2];

        return result;
    }
};
template<typename type_t>
struct half<long_uint_t<type_t, 1>> {

    using type = type_t;

    static constexpr type_t hi(const long_uint_t<type_t, 1>& value) noexcept { return half<type_t>::hi(value.digits[0]); }
    static constexpr type_t lo(const long_uint_t<type_t, 1>& value) noexcept { return half<type_t>::lo(value.digits[0]); }
    static constexpr long_uint_t<type_t, 1> make(const type_t& value_hi, const type_t& value_lo) noexcept { return half<type_t>::make(value_hi, value_lo); };
};
template<typename type_t>
struct half<long_uint_t<type_t, 2>> {

    using type = type_t;

    static constexpr type_t hi(const long_uint_t<type_t, 2>& value) noexcept { return value.digits[1]; }
    static constexpr type_t lo(const long_uint_t<type_t, 2>& value) noexcept { return value.digits[0]; }
    static constexpr long_uint_t<type_t, 2> make(const type_t& value_hi, const type_t& value_lo) noexcept { return long_uint_t<type_t, 2>({ value_lo, value_hi }); };
};



////////////////////////////////////////////////////////////////////////////////////////////////////
// divr_helper
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t>
struct divr_helper {
    static constexpr type_t calc(const type_t& value1, const type_t& value2, std::optional<type_t>& remainder) noexcept { return divr(value1, value2, remainder); }
};
template<typename type_t, uint_t size>
struct divr_helper<long_uint_t<type_t, size>> {
    static constexpr long_uint_t<type_t, size> calc(const long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, std::optional<long_uint_t<type_t, size>>& remainder) noexcept { return divr(value1, value2, remainder); }
};
template<typename type_t>
struct divr_helper<long_uint_t<type_t, 1>> {
    static constexpr long_uint_t<type_t, 1> calc(const long_uint_t<type_t, 1>& value1, const long_uint_t<type_t, 1>& value2, std::optional<long_uint_t<type_t, 1>>& remainder) noexcept { 
        return divr(value1.digits[0], value2.digits[0], remainder.digits[0]); 
    }
};



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

// extract low half of unsigned integer

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t half_lo(const type_t& value) noexcept;
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> half_lo(const long_uint_t<type_t, size>& value) noexcept;

// extract high half of unsigned integer

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t half_hi(const type_t& value) noexcept;
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> half_hi(const long_uint_t<type_t, size>& value) noexcept;

// move low half to high

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t half_make_hi(const type_t& value) noexcept;
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> half_make_hi(const long_uint_t<type_t, size>& value) noexcept;

// calculate leading zero bits

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr uint_t nlz(const type_t& value) noexcept;
template<typename type_t, uint_t size>
constexpr uint_t nlz(const long_uint_t<type_t, size>& value) noexcept;

// shift bits to left

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t shl2(const type_t& value_hi, const type_t& value_lo, uint_t shift) noexcept;

// shift bits to right

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t shr2(const type_t& value_hi, const type_t& value_lo, uint_t shift) noexcept;

// add with carry

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t addc(const type_t& value1, const type_t& value2, bool& carry) noexcept;

// subtract with borrow

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t subb(const type_t& value1, const type_t& value2, bool& borrow) noexcept;

// multiply with carry

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t mulc(const type_t& value1, const type_t& value2, type_t& carry) noexcept;

// divide with remainder

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t divr(const type_t& value1, const type_t& value2, std::optional<type_t>& remainder) noexcept;
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> divr(const long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, std::optional<long_uint_t<type_t, size>>& remainder) noexcept;
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t divr2(const type_t& value1_hi, const type_t& value1_lo, const type_t& value2, std::optional<type_t>& remainder) noexcept;



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t half_lo(const type_t& value) noexcept
{
    return value & (type_t(~type_t(0)) >> (bit_count_v<type_t> / 2));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> half_lo(const long_uint_t<type_t, size>& value) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    constexpr uint_t half = size / 2;
    long_uint_t result;

    for (uint_t n = 0; n < half; n++)
        result.digits[n] = value.digits[n];
    for (uint_t n = half; n < size; n++)
        result.digits[n] = 0;

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t half_hi(const type_t& value) noexcept
{
    return value >> (bit_count_v<type_t> / 2);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> half_hi(const long_uint_t<type_t, size>& value) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    constexpr uint_t half = size / 2;
    long_uint_t result;

    for (uint_t n = 0; n < half; n++)
        result.digits[n] = value.digits[n + half];
    for (uint_t n = half; n < size; n++)
        result.digits[n] = 0;

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t half_make_hi(const type_t& value) noexcept
{
    return value << (bit_count_v<type_t> / 2);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> half_make_hi(const long_uint_t<type_t, size>& value) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    constexpr uint_t half = size / 2;
    long_uint_t result;

    for (uint_t n = 0; n < half; n++)
        result.digits[n] = 0;
    for (uint_t n = half; n < size; n++)
        result.digits[n] = value.digits[n - half];

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr uint_t nlz(const type_t& value) noexcept
{
    uint_t result = 0;
    type_t mask = type_t(1) << (bit_count_v<type_t> - 1);

    while ((~value & mask) != 0) {

        mask >>= 1;
        result++;
    }

    return result;
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
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t shl2(const type_t& value_hi, const type_t& value_lo, uint_t shift) noexcept
{
    const type_t result_lo = value_lo;
    type_t result_hi = value_hi;

    if (shift > 0 && shift < bit_count_v<type_t> * 2) {

        if (shift < bit_count_v<type_t>)
            result_hi = (result_hi << shift) | (result_lo >> (bit_count_v<type_t> - shift));
        else
            result_hi = value_lo << (shift - bit_count_v<type_t>);
    }

    return result_hi;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t shr2(const type_t& value_hi, const type_t& value_lo, uint_t shift) noexcept
{
    type_t result_lo = value_lo;
    const type_t result_hi = value_hi;

    if (shift > 0 && shift < bit_count_v<type_t> * 2) {

        if (shift < bit_count_v<type_t>)
            result_lo = (result_lo >> shift) | (result_hi << (bit_count_v<type_t> - shift));
        else
            result_lo = value_hi >> (shift - bit_count_v<type_t>);
    }

    return result_lo;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t addc(const type_t& value1, const type_t& value2, bool& carry) noexcept
{
    const type_t tmp = value2 + carry;
    const type_t result = value1 + tmp;
    carry = (tmp < value2) || (result < value1);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t subb(const type_t& value1, const type_t& value2, bool& borrow) noexcept
{
    const type_t tmp = value1 - borrow;
    const type_t result = tmp - value2;
    borrow = (tmp > value1) || (result > tmp);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t mulc_classic(const type_t& value1, const type_t& value2, type_t& carry) noexcept
{
    const type_t value1_lo = half_lo(value1);
    const type_t value1_hi = half_hi(value1);
    const type_t value2_lo = half_lo(value2);
    const type_t value2_hi = half_hi(value2);

    const type_t t0 = value1_lo * value2_lo;
    const type_t t1 = value1_hi * value2_lo + half_hi(t0);
    const type_t t2 = value1_lo * value2_hi + half_lo(t1);
    const type_t t3 = value1_hi * value2_hi + half_hi(t2);

    type_t result_lo = half_make_hi(half_lo(t2)) + half_lo(t0);
    type_t result_hi = t3 + half_hi(t1);

    bool add_carry = false;
    result_lo = addc(result_lo, carry, add_carry);
    carry = addc(result_hi, type_t(0), add_carry);

    return result_lo;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t mulc_karatsuba(const type_t& value1, const type_t& value2, type_t& carry) noexcept
{
    const type_t value1_lo = half_lo(value1);
    const type_t value1_hi = half_hi(value1);
    const type_t value2_lo = half_lo(value2);
    const type_t value2_hi = half_hi(value2);

    const type_t x = value1_hi * value2_hi;
    const type_t y = value1_lo * value2_lo;

    const type_t a_plus_b = value1_lo + value1_hi;
    const type_t c_plus_d = value2_lo + value2_hi;
    assert(half_hi(a_plus_b) <= 1);
    assert(half_hi(c_plus_d) <= 1);
    const type_t a_plus_b_lo = half_lo(a_plus_b);
    const type_t a_plus_b_hi = half_hi(a_plus_b);
    const type_t c_plus_d_lo = half_lo(c_plus_d);
    const type_t c_plus_d_hi = half_hi(c_plus_d);

    type_t z_lo = a_plus_b_lo * c_plus_d_lo;
    type_t z_hi = 0;

    if (a_plus_b_hi != 0) {

        bool add_carry = false;
        z_lo = addc(z_lo, half_make_hi(c_plus_d_lo), add_carry);
        z_hi += add_carry;
    }

    if (c_plus_d_hi != 0) {

        bool add_carry = false;
        z_lo = addc(z_lo, half_make_hi(a_plus_b_lo), add_carry);
        z_hi += add_carry;
    }

    if (half_hi(a_plus_b) != 0 && half_hi(c_plus_d) != 0)
        ++z_hi;

    bool sub_borrow = false;
    z_lo = subb(z_lo, x, sub_borrow);
    z_hi -= sub_borrow;
    sub_borrow = false;
    z_lo = subb(z_lo, y, sub_borrow);
    z_hi -= sub_borrow;
    assert(z_hi <= 1);

    z_hi = shl2(z_hi, z_lo, bit_count_v<type_t> / 2);
    z_lo <<= bit_count_v<type_t> / 2;

    type_t result_lo = y;
    type_t result_hi = x;

    bool add_carry = false;
    result_lo = addc(result_lo, z_lo, add_carry);
    result_hi = addc(result_hi, z_hi, add_carry);
    add_carry = false;
    result_lo = addc(result_lo, carry, add_carry);
    carry = addc(result_hi, 0, add_carry);

    return result_lo;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t mulc(const type_t& value1, const type_t& value2, type_t& carry) noexcept
{
    //return mulc_karatsuba(value1, value2, carry);
    return mulc_classic(value1, value2, carry);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t divr(const type_t& value1, const type_t& value2, std::optional<type_t>& remainder) noexcept
{
    type_t quotient = value1 / value2;

    if (remainder)
        remainder = static_cast<type_t>(value1 - quotient * value2);

    return quotient;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> divr(const long_uint_t<type_t, size>& value1, const long_uint_t<type_t, size>& value2, std::optional<long_uint_t<type_t, size>>& remainder) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;
    using half_uint_t = typename half<long_uint_t>::type;

    const half_uint_t dividend_lo = half<long_uint_t>::lo(value1);
    const half_uint_t dividend_hi = half<long_uint_t>::hi(value1);
    const half_uint_t divider_lo = half<long_uint_t>::lo(value2);
    const half_uint_t divider_hi = half<long_uint_t>::hi(value2);

    long_uint_t quotient;

    if (divider_hi == 0) {

        half_uint_t quotient_lo;
        half_uint_t quotient_hi;
        std::optional<half_uint_t> remainder_lo = remainder ? half_uint_t() : std::optional<half_uint_t>();

        if (divider_lo > dividend_hi) {

            if (dividend_hi == 0)
                quotient_lo = divr<half_uint_t>(dividend_lo, divider_lo, remainder_lo);
            else 
                quotient_lo = divr2<half_uint_t>(dividend_hi, dividend_lo, divider_lo, remainder_lo);

            quotient_hi = 0;

        } else {

            quotient_lo = divr2<half_uint_t>(dividend_hi % divider_lo, dividend_lo, divider_lo, remainder_lo);
            quotient_hi = dividend_hi / divider_lo;
        }

        quotient = half<long_uint_t>::make(quotient_hi, quotient_lo);

        if (remainder)
            remainder = half<long_uint_t>::make(0, *remainder_lo);

    } else {

        quotient = divr2<long_uint_t>(0, value1, value2, remainder);
    }

    return quotient;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t divr2(const type_t& value1_hi, const type_t& value1_lo, const type_t& value2, std::optional<type_t>& remainder) noexcept
{
    if (value1_hi >= value2) {

        if (remainder)
            remainder = type_t(~0);

        return type_t(~0);
    }

    const uint_t shift = nlz(value2);
    const type_t svalue2 = value2 << shift;

    const type_t nvalue2_hi = half_hi(svalue2);
    const type_t nvalue2_lo = half_lo(svalue2);

    const type_t nvalue1_32 = shl2(value1_hi, value1_lo, shift);
    const type_t nvalue1_10 = value1_lo << shift;

    const type_t nvalue1_hi = half_hi(nvalue1_10);
    const type_t nvalue1_lo = half_lo(nvalue1_10);

    std::optional<type_t> remainder_hi = type_t();
    type_t quotient_hi = divr(nvalue1_32, nvalue2_hi, remainder_hi);

    const type_t t1 = quotient_hi * nvalue2_lo;
    const type_t t2 = half_make_hi(*remainder_hi) | nvalue1_hi;
    
    if (t1 > t2) {

        --quotient_hi;

        if (t1 - t2 > svalue2)
            --quotient_hi;
    }

    const type_t nvalue1_21 = half_make_hi(nvalue1_32) + nvalue1_hi - quotient_hi * svalue2;

    std::optional<type_t> remainder_lo = type_t();
    type_t quotient_lo = divr(nvalue1_21, nvalue2_hi, remainder_lo);
    
    const type_t t3 = quotient_lo * nvalue2_lo;
    const type_t t4 = half_make_hi(*remainder_lo) | nvalue1_lo;

    if (t3 > t4) {

        --quotient_lo;

        if (t3 - t4 > svalue2)
            --quotient_lo;
    }

    if (remainder)
        remainder = static_cast<type_t>((half_make_hi(nvalue1_21) + nvalue1_lo - quotient_lo * svalue2) >> shift);

    return half_make_hi(quotient_hi) | quotient_lo;
}

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of long_math.h
////////////////////////////////////////////////////////////////////////////////////////////////////
