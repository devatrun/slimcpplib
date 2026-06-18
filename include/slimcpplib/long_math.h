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

#include <cassert>
#include <climits>
#include <cstdint>

#include <array>
#include <concepts>
#include <limits>
#include <optional>
#include <type_traits>

#if !(defined(_MSC_VER) && _MSC_VER >= 1929 && ((defined(_MSVC_LANG) && _MSVC_LANG > 201703)) || (__cplusplus > 201703))
#error "Library SLIMCPP requires a compiler that supports C++ 20!"
#endif

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// type definitions
////////////////////////////////////////////////////////////////////////////////////////////////////

using uint_t = std::uintptr_t;
using int_t = std::intptr_t;



////////////////////////////////////////////////////////////////////////////////////////////////////
// byte_count_v and bit_count_v
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t>
constexpr uint_t byte_count_v = sizeof(type_t);
template<typename type_t>
constexpr uint_t bit_count_v = byte_count_v<type_t> * std::numeric_limits<uint8_t>::digits;



////////////////////////////////////////////////////////////////////////////////////////////////////
// concepts
////////////////////////////////////////////////////////////////////////////////////////////////////

// integer type supported by library numeric limits
template<typename type_t>
concept integer_type = std::numeric_limits<type_t>::is_integer;

// unsigned integer type supported by library numeric limits
template<typename type_t>
concept unsigned_type = integer_type<type_t> && !std::numeric_limits<type_t>::is_signed;

// signed integer type supported by library numeric limits
template<typename type_t>
concept signed_type = integer_type<type_t> && std::numeric_limits<type_t>::is_signed;

// compiler-native integral type
template<typename native_t>
concept native_type = std::integral<native_t>;

// compiler-native integral type except bool
template<typename native_t>
concept native_integer_type = native_type<native_t> && !std::same_as<native_t, bool>;

// unsigned compiler-native integral type
template<typename native_t>
concept unsigned_native_type = native_type<native_t> && unsigned_type<native_t>;

// signed compiler-native integral type
template<typename native_t>
concept signed_native_type = native_type<native_t> && signed_type<native_t>;

// compiler-native integral type that fits into native word
template<typename native_t, typename word_t>
concept native_sized_type = native_type<native_t> && byte_count_v<native_t> <= byte_count_v<word_t>;

// library integer type that fits into native word
template<typename type_t, typename word_t>
concept sized_integer_type = integer_type<type_t> && byte_count_v<type_t> <= byte_count_v<word_t>;



////////////////////////////////////////////////////////////////////////////////////////////////////
// make_unsigned_t
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t>
struct make_unsigned;

template<native_integer_type native_t>
struct make_unsigned<native_t> {
    using type = std::make_unsigned_t<native_t>;
};
template<typename type_t>
using make_unsigned_t = typename make_unsigned<type_t>::type;



////////////////////////////////////////////////////////////////////////////////////////////////////
// make_signed_t
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t>
struct make_signed;

template<native_integer_type native_t>
struct make_signed<native_t> {
    using type = std::make_signed_t<native_t>;
};
template<typename type_t>
using make_signed_t = typename make_signed<type_t>::type;



////////////////////////////////////////////////////////////////////////////////////////////////////
// half_t
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t>
struct half_type;

template<native_integer_type native_t>
struct half_type<native_t> {
    using type = native_t;
};
template<typename type_t>
using half_t = typename half_type<type_t>::type;



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

// extract low half of unsigned integer

template<unsigned_native_type type_t>
constexpr type_t half_lo(type_t value) noexcept;

// extract high half of unsigned integer

template<unsigned_native_type type_t>
constexpr type_t half_hi(type_t value) noexcept;

// move low half to high

template<unsigned_native_type type_t>
constexpr type_t half_make_hi(type_t value) noexcept;

// return most significant bit

template<integer_type type_t>
constexpr bool sign(type_t value);

// propagate most significant bit to the right

template<typename type_t, uint_t byte_count = byte_count_v<type_t>>
requires unsigned_native_type<type_t>
constexpr type_t pmsbr(type_t value) noexcept;

// calculate number of set bits

template<unsigned_native_type type_t>
constexpr uint_t popcnt(type_t value) noexcept;

// calculate leading zero bits

template<unsigned_native_type type_t>
constexpr uint_t nlz(type_t value) noexcept;

// bit operations

template<unsigned_native_type type_t>
constexpr void bit_not(type_t& value1, type_t value2) noexcept;
template<unsigned_native_type type_t>
constexpr void bit_and(type_t& value1, type_t value2) noexcept;
template<unsigned_native_type type_t>
constexpr void bit_or(type_t& value1, type_t value2) noexcept;
template<unsigned_native_type type_t>
constexpr void bit_xor(type_t& value1, type_t value2) noexcept;

// compare values

template<unsigned_native_type type_t>
constexpr bool cmp_equal(type_t value1, type_t value2) noexcept;
template<unsigned_native_type type_t>
constexpr bool cmp_less(type_t value1, type_t value2) noexcept;

// shift bits to the left

template<unsigned_native_type type_t>
constexpr type_t shl2(type_t value_hi, type_t value_lo, uint_t shift) noexcept;

// shift bits to the right

template<unsigned_native_type type_t>
constexpr type_t shr2(type_t value_hi, type_t value_lo, uint_t shift) noexcept;

// add with carry

template<unsigned_native_type type_t>
constexpr bool add(type_t& value1, type_t value2) noexcept;
template<unsigned_native_type type_t>
constexpr bool addc(type_t& value1, type_t value2, bool carry) noexcept;

// subtract with borrow

template<unsigned_native_type type_t>
constexpr bool sub(type_t& value1, type_t value2) noexcept;
template<unsigned_native_type type_t>
constexpr bool subb(type_t& value1, type_t value2, bool borrow) noexcept;

// multiply with carry

template<unsigned_native_type type_t>
constexpr type_t mul(type_t& value1, type_t value2) noexcept;
template<unsigned_native_type type_t>
constexpr type_t mulc(type_t& value1, type_t value2, type_t carry) noexcept;

// divide with remainder

template<unsigned_native_type type_t>
constexpr void div(type_t& value1, type_t value2) noexcept;
template<unsigned_native_type type_t>
constexpr type_t divr(type_t& value1, type_t value2) noexcept;
template<unsigned_native_type type_t>
constexpr void div2(type_t value1_hi, type_t& value1_lo, type_t value2) noexcept;
template<unsigned_native_type type_t>
constexpr type_t divr2(type_t value1_hi, type_t& value1_lo, type_t value2) noexcept;
template<unsigned_native_type type_t>
constexpr void mod(type_t& value1, type_t value2) noexcept;
template<unsigned_native_type type_t>
constexpr void mod2(type_t value1_hi, type_t& value1_lo, type_t value2) noexcept;



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

template<unsigned_native_type type_t>
constexpr type_t half_lo(type_t value) noexcept
{
    return value & (type_t(~type_t(0)) >> (bit_count_v<type_t> / 2));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr type_t half_hi(type_t value) noexcept
{
    return value >> (bit_count_v<type_t> / 2);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr type_t half_make_hi(type_t value) noexcept
{
    return value << (bit_count_v<type_t> / 2);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<integer_type type_t>
constexpr bool sign(type_t value)
{
    return make_signed_t<type_t>(value) < 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t byte_count>
requires unsigned_native_type<type_t>
constexpr type_t pmsbr(type_t value) noexcept
{
    if constexpr (byte_count == 1) {

        value |= (value >> 1);
        value |= (value >> 2);
        value |= (value >> 4);

    } else {

        value = pmsbr<type_t, byte_count / 2>(value);
        value |= (value >> (4 * byte_count));
    }

    return value;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t bit_count = bit_count_v<type_t>, uint_t mask_count = bit_count_v<type_t> / bit_count>
requires unsigned_native_type<type_t>
constexpr type_t popcnt_msk() noexcept
{
    if constexpr (mask_count == 1) {

        return pmsbr<type_t>(type_t(1) << (bit_count / 2 - 1));

    } else {

        constexpr type_t mask = popcnt_msk<type_t, bit_count, mask_count - 1>();
        return mask | (mask << (bit_count));
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t byte_count = byte_count_v<type_t>>
requires unsigned_native_type<type_t>
constexpr type_t popcnt_impl(type_t value) noexcept
{
    if constexpr (byte_count <= 4) {

        constexpr type_t mask2 = popcnt_msk<type_t, 2>();
        constexpr type_t mask4 = popcnt_msk<type_t, 4>();
        constexpr type_t mask8 = popcnt_msk<type_t, 8>();

        value = value - ((value >> 1) & mask2);
        value = (value & mask4) + ((value >> 2) & mask4);
        value = (value + (value >> 4)) & mask8;

        if constexpr (byte_count >= 2)
            value = value + (value >> 8);
        if constexpr (byte_count >= 3)
            value = value + (value >> 16);

    } else {

        value = popcnt_impl<type_t, byte_count / 2>(value);
        value = value + (value >> 4 * byte_count);
    }

    return value;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr uint_t popcnt(type_t value) noexcept
{
    return static_cast<uint_t>(popcnt_impl<type_t>(value) & ((bit_count_v<type_t> << 2) - 1));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr uint_t nlz(type_t value) noexcept
{
    return bit_count_v<type_t> - popcnt(pmsbr(value));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr void bit_not(type_t& value1, type_t value2) noexcept
{
    value1 = ~value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr void bit_and(type_t& value1, type_t value2) noexcept
{
    value1 &= value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr void bit_or(type_t& value1, type_t value2) noexcept
{
    value1 |= value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr void bit_xor(type_t& value1, type_t value2) noexcept
{
    value1 ^= value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr bool cmp_equal(type_t value1, type_t value2) noexcept
{
    return value1 == value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr bool cmp_less(type_t value1, type_t value2) noexcept
{
    return value1 < value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
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
template<unsigned_native_type type_t>
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
template<unsigned_native_type type_t>
constexpr bool add(type_t& value1, type_t value2) noexcept
{
    value1 += value2;
    return value1 < value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr bool addc(type_t& value1, type_t value2, bool carry) noexcept
{
    value1 += value2;
    bool carry_new = value1 < value2;
    value1 += carry;
    carry_new = carry_new || (value1 < type_t(carry));

    return carry_new;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr bool sub(type_t& value1, type_t value2) noexcept
{
    const type_t tmp = value1;
    value1 -= value2;
    return value1 > tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr bool subb(type_t& value1, type_t value2, bool borrow) noexcept
{
    type_t tmp = value1;
    value1 -= value2;
    bool borrow_new = value1 > tmp;
    tmp = value1;
    value1 -= borrow;
    borrow_new = borrow_new || (value1 > tmp);

    return borrow_new;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr type_t mul(type_t& value1, type_t value2) noexcept
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

    value1 = result_lo;

    return result_hi;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr type_t mulc(type_t& value1, type_t value2, type_t carry) noexcept
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

    result_hi += add(result_lo, carry);
    value1 = result_lo;

    return result_hi;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr void div(type_t& value1, type_t value2) noexcept
{
    value1 /= value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr type_t divr(type_t& value1, type_t value2) noexcept
{
    const type_t dividend = value1;

    value1 /= value2;

    return static_cast<type_t>(dividend - value1 * value2);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr void div2(type_t value1_hi, type_t& value1_lo, type_t value2) noexcept
{
    if (value2 != 0 && value1_hi >= value2) {

        value1_lo = type_t(~type_t(0));
        return;
    }

    const uint_t shift = nlz(value2);
    const type_t svalue2 = value2 << shift;

    const type_t nvalue2_hi = half_hi(svalue2);
    const type_t nvalue2_lo = half_lo(svalue2);

    const type_t nvalue1_32 = shl2(value1_hi, value1_lo, shift);
    const type_t nvalue1_10 = value1_lo << shift;

    const type_t nvalue1_hi = half_hi(nvalue1_10);
    const type_t nvalue1_lo = half_lo(nvalue1_10);

    type_t quotient_hi = nvalue1_32;
    const type_t remainder_hi = divr(quotient_hi, nvalue2_hi);

    const type_t t1 = quotient_hi * nvalue2_lo;
    const type_t t2 = half_make_hi(remainder_hi) | nvalue1_hi;

    if (t1 > t2) {

        --quotient_hi;

        if (t1 - t2 > svalue2)
            --quotient_hi;
    }

    const type_t nvalue1_21 = half_make_hi(nvalue1_32) + nvalue1_hi - quotient_hi * svalue2;

    type_t quotient_lo = nvalue1_21;
    const type_t remainder_lo = divr(quotient_lo, nvalue2_hi);

    const type_t t3 = quotient_lo * nvalue2_lo;
    const type_t t4 = half_make_hi(remainder_lo) | nvalue1_lo;

    if (t3 > t4) {

        --quotient_lo;

        if (t3 - t4 > svalue2)
            --quotient_lo;
    }

    value1_lo = half_make_hi(quotient_hi) | quotient_lo;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr type_t divr2(type_t value1_hi, type_t& value1_lo, type_t value2) noexcept
{
    if (value2 != 0 && value1_hi >= value2) {

        value1_lo = type_t(~type_t(0));
        return value1_lo;
    }

    const uint_t shift = nlz(value2);
    const type_t svalue2 = value2 << shift;

    const type_t nvalue2_hi = half_hi(svalue2);
    const type_t nvalue2_lo = half_lo(svalue2);

    const type_t nvalue1_32 = shl2(value1_hi, value1_lo, shift);
    const type_t nvalue1_10 = value1_lo << shift;

    const type_t nvalue1_hi = half_hi(nvalue1_10);
    const type_t nvalue1_lo = half_lo(nvalue1_10);

    type_t quotient_hi = nvalue1_32;
    const type_t remainder_hi = divr(quotient_hi, nvalue2_hi);

    const type_t t1 = quotient_hi * nvalue2_lo;
    const type_t t2 = half_make_hi(remainder_hi) | nvalue1_hi;

    if (t1 > t2) {

        --quotient_hi;

        if (t1 - t2 > svalue2)
            --quotient_hi;
    }

    const type_t nvalue1_21 = half_make_hi(nvalue1_32) + nvalue1_hi - quotient_hi * svalue2;

    type_t quotient_lo = nvalue1_21;
    const type_t remainder_lo = divr(quotient_lo, nvalue2_hi);

    const type_t t3 = quotient_lo * nvalue2_lo;
    const type_t t4 = half_make_hi(remainder_lo) | nvalue1_lo;

    if (t3 > t4) {

        --quotient_lo;

        if (t3 - t4 > svalue2)
            --quotient_lo;
    }

    value1_lo = half_make_hi(quotient_hi) | quotient_lo;

    return static_cast<type_t>((half_make_hi(nvalue1_21) + nvalue1_lo - quotient_lo * svalue2) >> shift);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable : 4724)

template<unsigned_native_type type_t>
constexpr void mod(type_t& value1, type_t value2) noexcept
{
    value1 %= value2;
}

#pragma warning(pop)



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_native_type type_t>
constexpr void mod2(type_t value1_hi, type_t& value1_lo, type_t value2) noexcept
{
    if (value2 != 0 && value1_hi >= value2) {

        value1_lo = type_t(~type_t(0));
        return;
    }

    const uint_t shift = nlz(value2);
    const type_t svalue2 = value2 << shift;

    const type_t nvalue2_hi = half_hi(svalue2);
    const type_t nvalue2_lo = half_lo(svalue2);

    const type_t nvalue1_32 = shl2(value1_hi, value1_lo, shift);
    const type_t nvalue1_10 = value1_lo << shift;

    const type_t nvalue1_hi = half_hi(nvalue1_10);
    const type_t nvalue1_lo = half_lo(nvalue1_10);

    type_t quotient_hi = nvalue1_32;
    const type_t remainder_hi = divr(quotient_hi, nvalue2_hi);

    const type_t t1 = quotient_hi * nvalue2_lo;
    const type_t t2 = half_make_hi(remainder_hi) | nvalue1_hi;

    if (t1 > t2) {

        --quotient_hi;

        if (t1 - t2 > svalue2)
            --quotient_hi;
    }

    const type_t nvalue1_21 = half_make_hi(nvalue1_32) + nvalue1_hi - quotient_hi * svalue2;

    type_t quotient_lo = nvalue1_21;
    const type_t remainder_lo = divr(quotient_lo, nvalue2_hi);

    const type_t t3 = quotient_lo * nvalue2_lo;
    const type_t t4 = half_make_hi(remainder_lo) | nvalue1_lo;

    if (t3 > t4) {

        --quotient_lo;

        if (t3 - t4 > svalue2)
            --quotient_lo;
    }

    value1_lo = static_cast<type_t>((half_make_hi(nvalue1_21) + nvalue1_lo - quotient_lo * svalue2) >> shift);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of long_math.h
////////////////////////////////////////////////////////////////////////////////////////////////////