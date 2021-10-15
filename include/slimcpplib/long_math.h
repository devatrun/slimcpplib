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
// is_unsigned_array_v
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t>
inline constexpr bool is_unsigned_array_v = false;
template<typename type_t, uint_t size>
inline constexpr bool is_unsigned_array_v<std::array<type_t, size>> = is_unsigned_v<type_t> && true;
template<typename type_t, uint_t size>
inline constexpr bool is_unsigned_array_v<const std::array<type_t, size>> = is_unsigned_v<type_t> && true;



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
// half_t
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t>
struct half_type {
    using type = type_t;
};
template<typename type_t>
using half_t = typename half_type<type_t>::type;



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

// make array from specified array

template<uint_t size_out, typename type_t, uint_t size_in, typename func_t>
constexpr std::array<type_t, size_out> make_array(const std::array<type_t, size_in>& arr, const func_t& func);
template<uint_t size_out, typename type_t, uint_t size_in, typename func_t, uint_t... idx>
constexpr std::array<type_t, size_out> make_array(const std::array<type_t, size_in>& arr, const func_t& func, std::integer_sequence<uint_t, idx...>);

// extract low half of unsigned integer

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t half_lo(type_t value) noexcept;

// extract high half of unsigned integer

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t half_hi(type_t value) noexcept;

// move low half to high

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t half_make_hi(type_t value) noexcept;

// calculate leading zero bits

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr uint_t nlz(type_t value) noexcept;

// shift bits to left

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t shl2(type_t value_hi, type_t value_lo, uint_t shift) noexcept;

// shift bits to right

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t shr2(type_t value_hi, type_t value_lo, uint_t shift) noexcept;

// add with carry

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t addc(type_t value1, type_t value2, bool& carry) noexcept;

// subtract with borrow

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t subb(type_t value1, type_t value2, bool& borrow) noexcept;

// multiply with carry

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t mulc(type_t value1, type_t value2, type_t& carry) noexcept;

// divide with remainder

template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t divr(type_t value1, type_t value2, std::optional<type_t>& remainder) noexcept;
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t divr2(type_t value1_hi, type_t value1_lo, type_t value2, std::optional<type_t>& remainder) noexcept;



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

template<uint_t size_out, typename type_t, uint_t size_in, typename func_t>
constexpr std::array<type_t, size_out> make_array(const std::array<type_t, size_in>& arr, const func_t& func)
{
    constexpr uint_t size = std::min(size_in, size_out);
    return make_array<size_out>(arr, func, std::make_integer_sequence<uint_t, size>{});
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<uint_t size_out, typename type_t, uint_t size_in, typename func_t, uint_t... idx>
constexpr std::array<type_t, size_out> make_array(const std::array<type_t, size_in>& arr, const func_t& func, std::integer_sequence<uint_t, idx...>)
{
    return std::array<type_t, size_out>{ (func(arr, idx))... };
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t half_lo(type_t value) noexcept
{
    return value & (type_t(~type_t(0)) >> (bit_count_v<type_t> / 2));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t half_hi(type_t value) noexcept
{
    return value >> (bit_count_v<type_t> / 2);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t half_make_hi(type_t value) noexcept
{
    return value << (bit_count_v<type_t> / 2);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr uint_t nlz(type_t value) noexcept
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
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t shl2(type_t value_hi, type_t value_lo, uint_t shift) noexcept
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
constexpr type_t shr2(type_t value_hi, type_t value_lo, uint_t shift) noexcept
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
constexpr type_t addc(type_t value1, const type_t value2, bool& carry) noexcept
{
    bool carry_new = false;

    type_t result = value1;
    result += value2;
    carry_new = carry_new || (result < value2);
    result += carry;
    carry_new = carry_new || (result < type_t(carry));

    carry = carry_new;
    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t subb(type_t value1, type_t value2, bool& borrow) noexcept
{
    bool borrow_new = false;

    type_t result = value1;
    result -= value2;
    borrow_new = borrow_new || (result > value1);
    type_t result_tmp = result;
    result -= borrow;
    borrow_new = borrow_new || (result > result_tmp);

    borrow = borrow_new;
    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t mulc_classic(type_t value1, type_t value2, type_t& carry) noexcept
{
    const type_t value1_lo = half_lo(value1);
    const type_t value1_hi = half_hi(value1);
    const type_t value2_lo = half_lo(value2);
    const type_t value2_hi = half_hi(value2);

    const type_t t0 = value1_lo * value2_lo;
    const type_t t1 = value1_hi * value2_lo + half_hi(t0);
    const type_t t2 = value1_lo * value2_hi + half_lo(t1);
    const type_t t3 = value1_hi * value2_hi + half_hi(t2);

    const type_t result_lo = half_make_hi(half_lo(t2)) + half_lo(t0);
    const type_t result_hi = t3 + half_hi(t1);

    bool add_carry = false;
    const type_t result = addc(result_lo, carry, add_carry);
    carry = result_hi + add_carry;

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int> = 0>
constexpr type_t mulc_karatsuba(type_t value1, type_t value2, type_t& carry) noexcept
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
    result_hi += z_hi + add_carry;
    add_carry = false;
    result_lo = addc(result_lo, carry, add_carry);
    carry = result_hi + add_carry;

    return result_lo;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t mulc(type_t value1, type_t value2, type_t& carry) noexcept
{
    //return mulc_karatsuba(value1, value2, carry);
    return mulc_classic(value1, value2, carry);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t divr(type_t value1, type_t value2, std::optional<type_t>& remainder) noexcept
{
    type_t quotient = value1 / value2;

    if (remainder)
        remainder = static_cast<type_t>(value1 - quotient * value2);

    return quotient;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, std::enable_if_t<is_unsigned_v<type_t>, int>>
constexpr type_t divr2(type_t value1_hi, type_t value1_lo, type_t value2, std::optional<type_t>& remainder) noexcept
{
    if (value2 != 0 && value1_hi >= value2) {

        if (remainder)
            remainder = type_t(~type_t(0));

        return type_t(~type_t(0));
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
