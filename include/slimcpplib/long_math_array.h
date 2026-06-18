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

#if __has_include("gcc/long_math.h")
#include "gcc/long_math.h"
#endif // __has_include("gcc/long_math.h")

#if __has_include("msvc/long_math.h")
#include "msvc/long_math.h"
#endif // __has_include("msvc/long_math.h")

#include <span>

namespace slim::array
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace general
{
// add two vectors

template<unsigned_type type_t, uint_t size>
constexpr bool add(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept;
template<unsigned_type type_t, uint_t size>
constexpr bool addc(std::span<type_t, size> value1, std::span<const type_t, size> value2, bool carry) noexcept;

// subtract two vectors

template<unsigned_type type_t, uint_t size>
constexpr bool sub(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept;
template<unsigned_type type_t, uint_t size>
constexpr bool subb(std::span<type_t, size> value1, std::span<const type_t, size> value2, bool borrow) noexcept;

} // namespace general

// compare vectors

template<unsigned_type type_t, uint_t size>
constexpr bool cmp_equal(std::span<const type_t, size> value1, std::span<const type_t, size> value2) noexcept;
template<unsigned_type type_t, uint_t size>
constexpr bool cmp_less(std::span<const type_t, size> value1, std::span<const type_t, size> value2) noexcept;

// negate vector

template<unsigned_type type_t, uint_t size>
constexpr void negate(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept;

// bit operations

template<unsigned_type type_t, uint_t size>
constexpr void bit_not(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept;
template<unsigned_type type_t, uint_t size>
constexpr void bit_and(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept;
template<unsigned_type type_t, uint_t size>
constexpr void bit_or(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept;
template<unsigned_type type_t, uint_t size>
constexpr void bit_xor(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept;

// shift vector

template<unsigned_type type_t, uint_t size>
constexpr void shl(std::span<type_t, size> value, uint_t shift) noexcept;
template<unsigned_type type_t, uint_t size>
constexpr void shr(std::span<type_t, size> value, uint_t shift) noexcept;

// add two vectors

template<unsigned_type type_t, uint_t size>
constexpr bool add(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept;
template<unsigned_type type_t, uint_t size>
constexpr bool addc(std::span<type_t, size> value1, std::span<const type_t, size> value2, bool carry) noexcept;

// subtract two vectors

template<unsigned_type type_t, uint_t size>
constexpr bool sub(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept;
template<unsigned_type type_t, uint_t size>
constexpr bool subb(std::span<type_t, size> value1, std::span<const type_t, size> value2, bool borrow) noexcept;

// multiply two vectors

template<unsigned_type type_t, uint_t size>
constexpr void mul(std::span<type_t, size> value1, std::span<const type_t, size> value2, std::span<type_t, size> carry) noexcept;
template<unsigned_type type_t, uint_t size>
constexpr void mul_lo(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept;

}

#if __has_include("gcc/long_math_array.h")
#include "gcc/long_math_array.h"
#endif // __has_include("gcc/long_math_array.h")

#if __has_include("msvc/long_math_array.h")
#include "msvc/long_math_array.h"
#endif // __has_include("msvc/long_math_array.h")


namespace slim::array
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone methods
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace general
{
////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr bool add(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    bool carry = ::slim::add(value1[0], value2[0]);

    for (uint_t n = 1; n < size; ++n)
        carry = ::slim::addc(value1[n], value2[n], carry);

    return carry;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr bool addc(std::span<type_t, size> value1, std::span<const type_t, size> value2, bool carry) noexcept
{
    for (uint_t n = 0; n < size; ++n)
        carry = ::slim::addc(value1[n], value2[n], carry);

    return carry;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr bool sub(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    bool borrow = ::slim::sub(value1[0], value2[0]);

    for (uint_t n = 1; n < size; ++n)
        borrow = ::slim::subb(value1[n], value2[n], borrow);

    return borrow;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr bool subb(std::span<type_t, size> value1, std::span<const type_t, size> value2, bool borrow) noexcept
{
    for (uint_t n = 0; n < size; ++n)
        borrow = ::slim::subb(value1[n], value2[n], borrow);

    return borrow;
}

} // namespace general

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr bool cmp_equal(std::span<const type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    for (uint_t n = size; n-- > 0;) {

        if (!::slim::cmp_equal(value1[n], value2[n]))
            return false;
    }

    return true;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr bool cmp_less(std::span<const type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    for (uint_t n = size; n-- > 0;) {

        if (!::slim::cmp_equal(value1[n], value2[n]))
            return ::slim::cmp_less(value1[n], value2[n]);
    }

    return false;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr void negate(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    bool borrow = true;

    for (uint_t n = 0; n < size; ++n) {

        type_t digit = value2[n];
        borrow = ::slim::subb(digit, type_t(0), borrow);
        ::slim::bit_not(value1[n], digit);
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr void bit_not(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    for (uint_t n = 0; n < size; ++n)
        ::slim::bit_not(value1[n], value2[n]);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr void bit_and(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    for (uint_t n = 0; n < size; ++n)
        ::slim::bit_and(value1[n], value2[n]);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr void bit_or(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    for (uint_t n = 0; n < size; ++n)
        ::slim::bit_or(value1[n], value2[n]);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr void bit_xor(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    for (uint_t n = 0; n < size; ++n)
        ::slim::bit_xor(value1[n], value2[n]);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr void shl(std::span<type_t, size> value, uint_t shift) noexcept
{
    if (shift < bit_count_v<type_t> * size) {

        const uint_t offset = shift / bit_count_v<type_t>;
        const uint_t bit_shift = shift % bit_count_v<type_t>;

        for (uint_t destination = size; destination-- > offset;) {

            const uint_t source = destination - offset;
            const type_t value_lo = source > 0 ? value[source - 1] : type_t(0);
            value[destination] = ::slim::shl2(value[source], value_lo, bit_shift);
        }

        for (uint_t n = 0; n < offset; ++n)
            value[n] = 0;
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr void shr(std::span<type_t, size> value, uint_t shift) noexcept
{
    if (shift < bit_count_v<type_t> * size) {

        const uint_t offset = shift / bit_count_v<type_t>;
        const uint_t bit_shift = shift % bit_count_v<type_t>;

        for (uint_t destination = 0; destination < size - offset; ++destination) {

            const uint_t source = destination + offset;
            const type_t value_hi = source + 1 < size ? value[source + 1] : type_t(0);
            value[destination] = ::slim::shr2(value_hi, value[source], bit_shift);
        }

        for (uint_t n = size - offset; n < size; ++n)
            value[n] = 0;
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr bool add(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    return general::add(value1, value2);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr bool addc(std::span<type_t, size> value1, std::span<const type_t, size> value2, bool carry) noexcept
{
    return general::addc(value1, value2, carry);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr bool sub(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    return general::sub(value1, value2);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr bool subb(std::span<type_t, size> value1, std::span<const type_t, size> value2, bool borrow) noexcept
{
    return general::subb(value1, value2, borrow);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr void mul(std::span<type_t, size> value1, std::span<const type_t, size> value2, std::span<type_t, size> carry) noexcept
{
    std::array<type_t, size * 2> result = {};

    for (uint_t value2_idx = 0; value2_idx < size; ++value2_idx) {

        for (uint_t value1_idx = 0; value1_idx < size; ++value1_idx) {

            const uint_t result_idx = value1_idx + value2_idx;

            type_t product_lo = value1[value1_idx];
            const type_t product_hi = ::slim::mul(product_lo, value2[value2_idx]);

            bool add_carry = ::slim::add(result[result_idx], product_lo);
            add_carry = ::slim::addc(result[result_idx + 1], product_hi, add_carry);

            for (uint_t n = result_idx + 2; add_carry && n < std::size(result); ++n)
                add_carry = ::slim::add(result[n], type_t(1));
        }
    }

    for (uint_t n = 0; n < size; ++n) {

        value1[n] = result[n];
        carry[n] = result[n + size];
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned_type type_t, uint_t size>
constexpr void mul_lo(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    std::array<type_t, size> result = {};

    for (uint_t value2_idx = 0; value2_idx < size; ++value2_idx) {

        for (uint_t value1_idx = 0; value1_idx < size - value2_idx; ++value1_idx) {

            type_t product_lo = value1[value1_idx];
            const type_t product_hi = ::slim::mul(product_lo, value2[value2_idx]);

            const uint_t result_idx = value1_idx + value2_idx;

            bool add_carry = ::slim::add(result[result_idx], product_lo);

            if (result_idx < std::size(result) - 1) {

                add_carry = ::slim::addc(result[result_idx + 1], product_hi, add_carry);

                for (uint_t n = result_idx + 2; add_carry && n < std::size(result); ++n)
                    add_carry = ::slim::add(result[n], type_t(1));
            }
        }
    }

    for (uint_t n = 0; n < size; ++n)
        value1[n] = result[n];
}

} // namespace slim::array

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of long_math_array.h
////////////////////////////////////////////////////////////////////////////////////////////////////