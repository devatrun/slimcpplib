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

#include "long_math_long.h"

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// long_uint_t class
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename native_t = uintmax_t, uint_t size = 2>
class long_uint_t
{
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // type and constant definition

    static constexpr uint_t lo = 0;
    static constexpr uint_t hi = size - 1;

    using native_array_t = std::array<native_t, size>;

    static_assert(unsigned_type<native_t>, "unsigned long integer native type must be unsigned.");
    static_assert(size >= 2, "native array size must be 2 or bigger.");
    static_assert((size & (size - 1)) == 0, "native array size must be power of 2.");

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // construction/destruction

    constexpr long_uint_t() noexcept = default;
    constexpr long_uint_t(const long_uint_t& that) noexcept = default;
    constexpr long_uint_t(long_uint_t&& that) noexcept = default;
    template<uint_t other_size>
    requires(other_size < size)
    constexpr long_uint_t(const long_uint_t<native_t, other_size>& that) noexcept;
    constexpr long_uint_t(native_array_t digits) noexcept;
    template<unsigned_native_type value_t>
    constexpr long_uint_t(value_t value) noexcept;
    template<signed_native_type value_t>
    constexpr long_uint_t(value_t value) noexcept;
    constexpr long_uint_t(bool value) noexcept;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // public methods

    constexpr void swap(long_uint_t& that) noexcept;
    constexpr long_uint_t& operator=(const long_uint_t& that) noexcept = default;
    constexpr long_uint_t& operator=(long_uint_t&& that) noexcept = default;

    template<uint_t other_size>
    requires(other_size < size)
    explicit constexpr operator long_uint_t<native_t, other_size>() const noexcept;
    template<unsigned_native_type value_t>
    explicit constexpr operator value_t() const noexcept;
    template<signed_native_type value_t>
    explicit constexpr operator value_t() const noexcept;
    constexpr bool operator==(const long_uint_t& that) const noexcept;
    template<typename type_t>
    requires sized_integer_type<type_t, native_t>
    constexpr bool operator==(type_t value) const noexcept;
    constexpr bool operator!=(const long_uint_t& that) const noexcept;
    template<typename type_t>
    requires sized_integer_type<type_t, native_t>
    constexpr bool operator!=(type_t value) const noexcept;
    constexpr bool operator<(const long_uint_t& that) const noexcept;
    constexpr bool operator<=(const long_uint_t& that) const noexcept;
    constexpr bool operator>(const long_uint_t& that) const noexcept;
    constexpr bool operator>=(const long_uint_t& that) const noexcept;
    template<typename value_t>
    requires native_sized_type<value_t, native_t>
    constexpr bool operator<(value_t value) const noexcept;
    template<typename value_t>
    requires native_sized_type<value_t, native_t>
    constexpr bool operator<=(value_t value) const noexcept;
    template<typename value_t>
    requires native_sized_type<value_t, native_t>
    constexpr bool operator>(value_t value) const noexcept;
    template<typename value_t>
    requires native_sized_type<value_t, native_t>
    constexpr bool operator>=(value_t value) const noexcept;
    constexpr long_uint_t operator~() const noexcept;
    constexpr long_uint_t& operator&=(const long_uint_t& that) noexcept;
    template<typename type_t>
    requires sized_integer_type<type_t, native_t>
    constexpr long_uint_t& operator&=(type_t value) noexcept;
    constexpr long_uint_t operator&(const long_uint_t& that) const noexcept;
    template<typename type_t>
    requires sized_integer_type<type_t, native_t>
    constexpr long_uint_t operator&(type_t value) const noexcept;
    constexpr long_uint_t& operator|=(const long_uint_t& that) noexcept;
    template<typename type_t>
    requires sized_integer_type<type_t, native_t>
    constexpr long_uint_t& operator|=(type_t value) noexcept;
    constexpr long_uint_t operator|(const long_uint_t& that) const noexcept;
    template<typename type_t>
    requires sized_integer_type<type_t, native_t>
    constexpr long_uint_t operator|(type_t value) const noexcept;
    constexpr long_uint_t& operator^=(const long_uint_t& that) noexcept;
    template<typename type_t>
    requires sized_integer_type<type_t, native_t>
    constexpr long_uint_t& operator^=(type_t value) noexcept;
    constexpr long_uint_t operator^(const long_uint_t& that) const noexcept;
    template<typename type_t>
    requires sized_integer_type<type_t, native_t>
    constexpr long_uint_t operator^(type_t value) const noexcept;
    constexpr long_uint_t& operator<<=(uint_t shift) noexcept;
    constexpr long_uint_t operator<<(uint_t shift) const noexcept;
    constexpr long_uint_t& operator>>=(uint_t shift) noexcept;
    constexpr long_uint_t operator>>(uint_t shift) const noexcept;
    constexpr long_uint_t& operator+=(const long_uint_t& that) noexcept;
    template<typename type_t>
    requires sized_integer_type<type_t, native_t>
    constexpr long_uint_t& operator+=(type_t value) noexcept;
    constexpr long_uint_t operator+(const long_uint_t& that) const noexcept;
    template<typename type_t>
    requires sized_integer_type<type_t, native_t>
    constexpr long_uint_t operator+(type_t value) const noexcept;
    constexpr long_uint_t& operator++() noexcept;
    constexpr long_uint_t operator++(int) noexcept;
    constexpr long_uint_t operator+() const noexcept;
    constexpr long_uint_t& operator-=(const long_uint_t& that) noexcept;
    template<typename type_t>
    requires sized_integer_type<type_t, native_t>
    constexpr long_uint_t& operator-=(type_t value) noexcept;
    constexpr long_uint_t operator-(const long_uint_t& that) const noexcept;
    template<typename type_t>
    requires sized_integer_type<type_t, native_t>
    constexpr long_uint_t operator-(type_t value) const noexcept;
    constexpr long_uint_t& operator--() noexcept;
    constexpr long_uint_t operator--(int) noexcept;
    constexpr long_uint_t operator-() const noexcept;
    constexpr long_uint_t& operator*=(const long_uint_t& that) noexcept;
    constexpr long_uint_t operator*(const long_uint_t& that) const noexcept;
    constexpr long_uint_t& operator/=(const long_uint_t& that) noexcept;
    constexpr long_uint_t operator/(const long_uint_t& that) const noexcept;
    constexpr long_uint_t& operator%=(const long_uint_t& that) noexcept;
    constexpr long_uint_t operator%(const long_uint_t& that) const noexcept;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // data members

    native_array_t digits;
};



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone methods
////////////////////////////////////////////////////////////////////////////////////////////////////

template<unsigned_type type_t>
constexpr type_t muldiv(const type_t& value, const type_t& multiplier, const type_t& divider) noexcept;



////////////////////////////////////////////////////////////////////////////////////////////////////
// long_uint_t class
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

template<typename native_t, uint_t size>
template<uint_t other_size>
requires(other_size < size)
constexpr long_uint_t<native_t, size>::long_uint_t(const long_uint_t<native_t, other_size>& that) noexcept
{
    for (uint_t n = 0; n < other_size; ++n)
        digits[n] = that.digits[n];
    for (uint_t n = other_size; n < std::size(digits); ++n)
        digits[n] = native_t(0);
}

template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>::long_uint_t(native_array_t digits) noexcept
: digits(std::move(digits))
{
}

template<typename native_t, uint_t size>
template<unsigned_native_type value_t>
constexpr long_uint_t<native_t, size>::long_uint_t(value_t value) noexcept
{
    constexpr uint_t value_size = std::min(size, (byte_count_v<value_t> + byte_count_v<native_t> - 1) / byte_count_v<native_t>);

    for (uint_t n = 0; n < value_size; ++n) {

        digits[n] = value & ~native_t(0);
        value >>= std::min(bit_count_v<value_t> -1, bit_count_v<native_t>);
    }

    for (uint_t n = value_size; n < std::size(digits); ++n)
        digits[n] = native_t(0);
}

template<typename native_t, uint_t size>
template<signed_native_type value_t>
constexpr long_uint_t<native_t, size>::long_uint_t(value_t value) noexcept
{
    constexpr uint_t value_size = std::min(size, (byte_count_v<value_t> + byte_count_v<native_t> - 1) / byte_count_v<native_t>);
    const native_t extension = static_cast<native_t>(value >= 0 ? 0 : native_t(~0));

    for (uint_t n = 0; n < value_size; ++n) {

        digits[n] = value & ~native_t(0);
        value >>= std::min(bit_count_v<value_t> - 1, bit_count_v<native_t>);
    }

    for (uint_t n = value_size; n < std::size(digits); ++n)
        digits[n] = extension;
}

template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>::long_uint_t(bool value) noexcept
{
    digits[0] = value;

    for (uint_t n = 1; n < std::size(digits); ++n)
        digits[n] = native_t(0);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// public methods

template<typename native_t, uint_t size>
constexpr void long_uint_t<native_t, size>::swap(long_uint_t& that) noexcept
{
    using std::swap;

    swap(digits, that.digits);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<uint_t other_size>
requires(other_size < size)
constexpr long_uint_t<native_t, size>::operator long_uint_t<native_t, other_size>() const noexcept
{
    long_uint_t<native_t, other_size> tmp;

    for (uint_t n = 0; n < other_size; ++n)
        tmp.digits[n] = digits[n];

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<unsigned_native_type value_t>
constexpr long_uint_t<native_t, size>::operator value_t() const noexcept
{
    return static_cast<value_t>(digits[lo]);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<signed_native_type value_t>
constexpr long_uint_t<native_t, size>::operator value_t() const noexcept
{
    return static_cast<value_t>(digits[lo]);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr bool long_uint_t<native_t, size>::operator==(const long_uint_t& that) const noexcept
{
    return array::cmp_equal(std::span(digits), std::span(that.digits));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename type_t>
requires sized_integer_type<type_t, native_t>
constexpr bool long_uint_t<native_t, size>::operator==(type_t value) const noexcept
{
    native_t extension = native_t(0);

    if constexpr (signed_type<type_t>) {

        if (value < 0)
            extension = native_t(~native_t(0));
    }

    if (!cmp_equal(digits[0], native_t(value)))
        return false;

    for (uint_t n = 1; n < std::size(digits); ++n) {

        if (!cmp_equal(digits[n], extension))
            return false;
    }

    return true;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr bool long_uint_t<native_t, size>::operator!=(const long_uint_t& that) const noexcept
{
    return !operator==(that);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename type_t>
requires sized_integer_type<type_t, native_t>
constexpr bool long_uint_t<native_t, size>::operator!=(type_t value) const noexcept
{
    return !operator==(value);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr bool long_uint_t<native_t, size>::operator<(const long_uint_t& that) const noexcept
{
    return array::cmp_less(std::span(digits), std::span(that.digits));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr bool long_uint_t<native_t, size>::operator<=(const long_uint_t& that) const noexcept
{
    return !that.operator<(*this);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr bool long_uint_t<native_t, size>::operator>(const long_uint_t& that) const noexcept
{
    return that.operator<(*this);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr bool long_uint_t<native_t, size>::operator>=(const long_uint_t& that) const noexcept
{
    return !operator<(that);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename value_t>
requires native_sized_type<value_t, native_t>
constexpr bool long_uint_t<native_t, size>::operator<(value_t value) const noexcept
{
    native_t extension = native_t(0);

    if constexpr (signed_type<value_t>) {

        if (value < 0)
            extension = native_t(~native_t(0));
    }

    for (uint_t n = std::size(digits); n-- > 1;) {

        if (!cmp_equal(digits[n], extension))
            return cmp_less(digits[n], extension);
    }

    return cmp_less(digits[0], native_t(value));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename value_t>
requires native_sized_type<value_t, native_t>
constexpr bool long_uint_t<native_t, size>::operator<=(value_t value) const noexcept
{
    return operator<(value) || operator==(value);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename value_t>
requires native_sized_type<value_t, native_t>
constexpr bool long_uint_t<native_t, size>::operator>(value_t value) const noexcept
{
    return !operator<=(value);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename value_t>
requires native_sized_type<value_t, native_t>
constexpr bool long_uint_t<native_t, size>::operator>=(value_t value) const noexcept
{
    return !operator<(value);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator~() const noexcept
{
    long_uint_t tmp;

    array::bit_not(std::span(tmp.digits), std::span(digits));

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator&=(const long_uint_t& that) noexcept
{
    array::bit_and(std::span(digits), std::span(that.digits));

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename type_t>
requires sized_integer_type<type_t, native_t>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator&=(type_t value) noexcept
{
    native_t extension = native_t(0);

    if constexpr (signed_type<type_t>) {

        if (value < 0)
            extension = native_t(~native_t(0));
    }

    bit_and(digits[0], native_t(value));

    for (uint_t n = 1; n < std::size(digits); ++n)
        bit_and(digits[n], extension);

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator&(const long_uint_t& that) const noexcept
{
    long_uint_t tmp = *this;

    tmp &= that;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename type_t>
requires sized_integer_type<type_t, native_t>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator&(type_t value) const noexcept
{
    long_uint_t tmp = *this;

    tmp &= value;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator|=(const long_uint_t& that) noexcept
{
    array::bit_or(std::span(digits), std::span(that.digits));

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename type_t>
requires sized_integer_type<type_t, native_t>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator|=(type_t value) noexcept
{
    native_t extension = native_t(0);

    if constexpr (signed_type<type_t>) {

        if (value < 0)
            extension = native_t(~native_t(0));
    }

    bit_or(digits[0], native_t(value));

    for (uint_t n = 1; n < std::size(digits); ++n)
        bit_or(digits[n], extension);

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator|(const long_uint_t& that) const noexcept
{
    long_uint_t tmp = *this;

    tmp |= that;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename type_t>
requires sized_integer_type<type_t, native_t>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator|(type_t value) const noexcept
{
    long_uint_t tmp = *this;

    tmp |= value;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator^=(const long_uint_t& that) noexcept
{
    array::bit_xor(std::span(digits), std::span(that.digits));

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator^(const long_uint_t& that) const noexcept
{
    long_uint_t tmp = *this;

    tmp ^= that;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename type_t>
requires sized_integer_type<type_t, native_t>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator^(type_t value) const noexcept
{
    long_uint_t tmp = *this;

    tmp ^= value;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename type_t>
requires sized_integer_type<type_t, native_t>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator^=(type_t value) noexcept
{
    native_t extension = native_t(0);

    if constexpr (signed_type<type_t>) {

        if (value < 0)
            extension = native_t(~native_t(0));
    }

    bit_xor(digits[0], native_t(value));

    for (uint_t n = 1; n < std::size(digits); ++n)
        bit_xor(digits[n], extension);

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator<<=(uint_t shift) noexcept
{
    array::shl(std::span(digits), shift);

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator<<(uint_t shift) const noexcept
{
    long_uint_t tmp = *this;

    tmp <<= shift;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator>>=(uint_t shift) noexcept
{
    array::shr(std::span(digits), shift);

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator>>(uint_t shift) const noexcept
{
    long_uint_t tmp = *this;

    tmp >>= shift;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator+=(const long_uint_t& that) noexcept
{
    array::add(std::span(digits), std::span(that.digits));

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename type_t>
requires sized_integer_type<type_t, native_t>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator+=(type_t value) noexcept
{
    native_t extension = native_t(0);

    if constexpr (signed_type<type_t>) {

        if (value < 0)
            extension = native_t(~native_t(0));
    }

    bool carry = add(digits[0], native_t(value));

    for (uint_t n = 1; n < std::size(digits); ++n)
        carry = addc(digits[n], extension, carry);

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator+(const long_uint_t& that) const noexcept
{
    long_uint_t tmp = *this;

    tmp += that;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename type_t>
requires sized_integer_type<type_t, native_t>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator+(type_t value) const noexcept
{
    long_uint_t tmp = *this;

    tmp += value;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator++() noexcept
{
    bool carry = add(digits[0], native_t(1));

    for (uint_t n = 1; n < std::size(digits); ++n)
        carry = addc(digits[n], native_t(0), carry);

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator++(int) noexcept
{
    long_uint_t tmp = *this;

    operator++();

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator+() const noexcept
{
    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator-=(const long_uint_t& that) noexcept
{
    array::sub(std::span(digits), std::span(that.digits));

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename type_t>
requires sized_integer_type<type_t, native_t>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator-=(type_t value) noexcept
{
    native_t extension = native_t(0);

    if constexpr (signed_type<type_t>) {

        if (value < 0)
            extension = native_t(~native_t(0));
    }

    bool borrow = sub(digits[0], native_t(value));

    for (uint_t n = 1; n < std::size(digits); ++n)
        borrow = subb(digits[n], extension, borrow);

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator-(const long_uint_t& that) const noexcept
{
    long_uint_t tmp = *this;

    tmp -= that;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
template<typename type_t>
requires sized_integer_type<type_t, native_t>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator-(type_t value) const noexcept
{
    long_uint_t tmp = *this;

    tmp -= value;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator--() noexcept
{
    bool borrow = sub(digits[0], native_t(1));

    for (uint_t n = 1; n < std::size(digits); ++n)
        borrow = subb(digits[n], native_t(0), borrow);

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator--(int) noexcept
{
    long_uint_t tmp = *this;

    operator--();

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator-() const noexcept
{
    long_uint_t tmp;

    array::negate(std::span(tmp.digits), std::span(digits));

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator*=(const long_uint_t& that) noexcept
{
    array::mul_lo(std::span(digits), std::span(that.digits));

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator*(const long_uint_t& that) const noexcept
{
    long_uint_t tmp = *this;

    tmp *= that;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator/=(const long_uint_t& that) noexcept
{
    div(*this, that);

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator/(const long_uint_t& that) const noexcept
{
    long_uint_t tmp = *this;

    tmp /= that;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size>& long_uint_t<native_t, size>::operator%=(const long_uint_t& that) noexcept
{
    mod(*this, that);

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> long_uint_t<native_t, size>::operator%(const long_uint_t& that) const noexcept
{
    long_uint_t tmp = *this;

    tmp %= that;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone methods
////////////////////////////////////////////////////////////////////////////////////////////////////

template<native_type value_t, typename native_t, uint_t size>
constexpr bool operator==(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    return long_uint_t<native_t, size>(value1) == value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr bool operator!=(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    return long_uint_t<native_t, size>(value1) != value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr bool operator<(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    if constexpr (byte_count_v<value_t> <= byte_count_v<native_t>)
        return value2 > value1;
    else
        return long_uint_t<native_t, size>(value1) < value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr bool operator<=(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    if constexpr (byte_count_v<value_t> <= byte_count_v<native_t>)
        return value2 >= value1;
    else
        return long_uint_t<native_t, size>(value1) <= value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr bool operator>(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    if constexpr (byte_count_v<value_t> <= byte_count_v<native_t>)
        return value2 < value1;
    else
        return long_uint_t<native_t, size>(value1) > value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr bool operator>=(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    if constexpr (byte_count_v<value_t> <= byte_count_v<native_t>)
        return value2 <= value1;
    else
        return long_uint_t<native_t, size>(value1) >= value2;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> operator&(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    long_uint_t<native_t, size> tmp = value2;

    tmp &= value1;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> operator|(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    long_uint_t<native_t, size> tmp = value2;

    tmp |= value1;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> operator^(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    long_uint_t<native_t, size> tmp = value2;

    tmp ^= value1;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> operator<<(long_uint_t<native_t, size> value, uint_t shift) noexcept
{
    value <<= shift;

    return value;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> operator>>(long_uint_t<native_t, size> value, uint_t shift) noexcept
{
    value >>= shift;

    return value;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> operator+(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    long_uint_t<native_t, size> tmp = value2;

    tmp += value1;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> operator-(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    long_uint_t<native_t, size> tmp(value1);

    tmp -= value2;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> operator*(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    long_uint_t<native_t, size> tmp(value1);

    tmp *= value2;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> operator/(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    long_uint_t<native_t, size> tmp(value1);

    tmp /= value2;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<native_type value_t, typename native_t, uint_t size>
constexpr long_uint_t<native_t, size> operator%(value_t value1, const long_uint_t<native_t, size>& value2) noexcept
{
    long_uint_t<native_t, size> tmp(value1);

    tmp %= value2;

    return tmp;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone methods
////////////////////////////////////////////////////////////////////////////////////////////////////

template<unsigned_type type_t>
constexpr type_t muldiv(const type_t& value, const type_t& multiplier, const type_t& divider) noexcept
{
    type_t mul_lo = value;
    const type_t mul_hi = mul(mul_lo, multiplier);

    if (mul_hi == 0)
        div(mul_lo, divider);
    else
        div2(mul_hi, mul_lo, divider);

    return mul_lo;
}

namespace impl
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// enum parse_result
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class parse_result { ok,
    unexpected,
    overflow };
constexpr uint8_t kSeparator = 0xff;



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone methods
////////////////////////////////////////////////////////////////////////////////////////////////////

template<uint_t literal_size, typename digit_parser_t>
constexpr std::optional<std::array<uint8_t, literal_size>> parse_digits(const std::array<char, literal_size>& literal, const digit_parser_t& digit_parser) noexcept
{
    std::array<uint8_t, literal_size> digits;

    for (uint_t n = 0; n < std::size(digits); ++n) {

        const std::optional<uint8_t> digit = digit_parser(literal[n]);

        if (!digit)
            return std::nullopt;

        digits[n] = *digit;
    }

    return digits;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename long_t, uint8_t base, uint_t literal_size, typename digit_parser_t>
constexpr std::pair<parse_result, long_t> parse_literal(const std::array<char, literal_size>& literal, const digit_parser_t& digit_parser) noexcept
{
    const std::optional<std::array<uint8_t, literal_size>> literal_digits = parse_digits(literal, digit_parser);

    if (!literal_digits)
        return std::make_pair(parse_result::unexpected, long_t(0));

    using native_t = typename long_t::native_array_t::value_type;
    typename long_t::native_array_t digits = { 0, 0 };

    for (uint_t literal_idx = 0; literal_idx < literal_digits->size(); ++literal_idx) {

        native_t carry = (*literal_digits)[literal_idx];

        if (carry == kSeparator)
            continue;

        for (auto& digit : digits)
            carry = mulc<native_t>(digit, native_t(base), carry);

        if (carry > 0)
            return std::make_pair(parse_result::overflow, long_t(0));
    }

    return std::make_pair(parse_result::ok, long_t(digits));
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename long_t, typename... char_t>
constexpr std::pair<parse_result, long_t> parse_oct_literal(char, char_t... chars) noexcept
{
    constexpr uint_t literal_size = sizeof...(chars);
    const std::array<char, literal_size> literal = { chars... };

    return parse_literal<long_t, 8>(literal, [](char ch) -> std::optional<uint8_t> {

        if (ch == '\'')
            return kSeparator;

        if (ch >= '0' && ch <= '7')
            return uint8_t(uint_t(ch) - '0');

        return std::nullopt;
    });
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename long_t, typename... char_t>
constexpr std::pair<parse_result, long_t> parse_dec_literal(char_t... chars) noexcept
{
    constexpr uint_t literal_size = sizeof...(chars);
    const std::array<char, literal_size> literal = { chars... };

    return parse_literal<long_t, 10>(literal, [](char ch) -> std::optional<uint8_t> {

        if (ch == '\'')
            return kSeparator;

        if (ch >= '0' && ch <= '9')
            return uint8_t(uint_t(ch) - '0');

        return std::nullopt;
    });
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename long_t, typename... char_t>
constexpr std::pair<parse_result, long_t> parse_hex_literal(char, char, char_t... chars) noexcept
{
    constexpr uint_t literal_size = sizeof...(chars);
    const std::array<char, literal_size> literal = { chars... };

    return parse_literal<long_t, 16>(literal, [](char ch) -> std::optional<uint8_t> {

        if (ch == '\'')
            return kSeparator;

        if (ch >= '0' && ch <= '9')
            return uint8_t(uint_t(ch) - '0');
        if (ch >= 'A' && ch <= 'F')
            return uint8_t(10 + uint_t(ch) - 'A');
        else if (ch >= 'a' && ch <= 'f')
            return uint8_t(10 + uint_t(ch) - 'a');

        return std::nullopt;
    });
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename long_uint_t, char... chars>
constexpr long_uint_t parse_literal() noexcept
{
    constexpr uint_t char_count = sizeof...(chars);
    constexpr std::array<char, char_count> literal = { chars... };

    static_assert(literal[0] >= '0' && literal[0] <= '9', "unsigned long integer literal must be a number.");

    if constexpr (literal[0] == '0') {

        if constexpr (char_count == 1 || (literal[1] != 'X' && literal[1] != 'x')) {

            constexpr std::pair<parse_result, long_uint_t> value = parse_oct_literal<long_uint_t>(chars...);
            static_assert(value.first != parse_result::unexpected, "unexpected symbol unsigned in long integer octal literal");
            static_assert(value.first != parse_result::overflow, "overflow in unsigned long integer octal literal");

            return value.second;

        } else {

            constexpr std::pair<parse_result, long_uint_t> value = parse_hex_literal<long_uint_t>(chars...);
            static_assert(value.first != parse_result::unexpected, "unexpected symbol in unsigned long integer hexadecimal literal");
            static_assert(value.first != parse_result::overflow, "overflow in unsigned long integer hexadecimal literal");

            return value.second;
        }

    } else {

        constexpr std::pair<parse_result, long_uint_t> value = parse_dec_literal<long_uint_t>(chars...);
        static_assert(value.first != parse_result::unexpected, "unexpected symbol in unsigned long integer decimal literal");
        static_assert(value.first != parse_result::overflow, "overflow in unsigned long integer decimal literal");

        return value.second;
    }
}
} // namespace impl

////////////////////////////////////////////////////////////////////////////////////////////////////
// long integer literal accept operator
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef UINT64_MAX

using uint128_t = long_uint_t<uint64_t, 2>;
using uint256_t = long_uint_t<uint64_t, 4>;

namespace literals
{

template<char... chars>
constexpr uint128_t operator"" _ui128() noexcept
{
    return slim::impl::parse_literal<uint128_t, chars...>();
}

template<char... chars>
constexpr uint256_t operator"" _ui256() noexcept
{
    return slim::impl::parse_literal<uint256_t, chars...>();
}

} // namespace literals

#endif // UINT64_MAX

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of long_uint.h
////////////////////////////////////////////////////////////////////////////////////////////////////