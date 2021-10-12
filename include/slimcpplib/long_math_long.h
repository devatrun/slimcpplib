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

#include "long_math.h"

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// type definitions
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t, uint_t size>
class long_uint_t;
template<typename type_t, uint_t size>
class long_int_t;



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
constexpr long_uint_t<type_t, size / 2> half_lo(long_uint_t<type_t, size> value) noexcept;
template<typename type_t>
constexpr type_t half_lo(long_uint_t<type_t, 2> value) noexcept;

// extract high half of unsigned integer

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size / 2> half_hi(long_uint_t<type_t, size> value) noexcept;
template<typename type_t>
constexpr type_t half_hi(long_uint_t<type_t, 2> value) noexcept;

// make unsigned integer from low and high

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> half_make(long_uint_t<type_t, size / 2> high, long_uint_t<type_t, size / 2> low) noexcept;
template<typename type_t>
constexpr long_uint_t<type_t, 2> half_make(type_t high, type_t low) noexcept;

// move low half to high

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> half_make_hi(long_uint_t<type_t, size> value) noexcept;

// calculate leading zero bits

template<typename type_t, uint_t size>
constexpr uint_t nlz(long_uint_t<type_t, size> value) noexcept;

// multiply with carry

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> mulc(long_uint_t<type_t, size> value1, long_uint_t<type_t, size> value2, long_uint_t<type_t, size>& carry) noexcept;

// divide with remainder

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> divr(long_uint_t<type_t, size> value1, long_uint_t<type_t, size> value2, std::optional<long_uint_t<type_t, size>>& remainder) noexcept;



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size / 2> half_lo(long_uint_t<type_t, size> value) noexcept
{
    constexpr uint_t half_size = size / 2;
    long_uint_t<type_t, half_size> result;

    for (uint_t n = 0; n < half_size; n++)
        result.digits[n] = value.digits[n];

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t>
constexpr type_t half_lo(long_uint_t<type_t, 2> value) noexcept
{
    return value.digits[0];
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size / 2> half_hi(long_uint_t<type_t, size> value) noexcept
{
    constexpr uint_t half_size = size / 2;
    long_uint_t<type_t, half_size> result;

    for (uint_t n = 0; n < half_size; n++)
        result.digits[n] = value.digits[n + half_size];
  
    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t>
constexpr type_t half_hi(long_uint_t<type_t, 2> value) noexcept
{
    return value.digits[1];
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> half_make(long_uint_t<type_t, size / 2> value_hi, long_uint_t<type_t, size / 2> value_lo) noexcept
{
    constexpr uint_t half_size = size / 2;
    long_uint_t<type_t, half_size> result;

    for (uint_t n = 0; n < half_size; n++)
        result.digits[n] = value_lo.digits[n];
    for (uint_t n = half_size; n < size; n++)
        result.digits[n] = value_hi.digits[n - half_size];

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t>
constexpr long_uint_t<type_t, 2> half_make(type_t value_hi, type_t value_lo) noexcept
{
    return long_uint_t<type_t, 2>({ value_lo, value_hi });
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> half_make_hi(long_uint_t<type_t, size> value) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;

    constexpr uint_t half_size = size / 2;
    long_uint_t result;

    for (uint_t n = 0; n < half_size; n++)
        result.digits[n] = 0;
    for (uint_t n = half_size; n < size; n++)
        result.digits[n] = value.digits[n - half_size];

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr uint_t nlz(long_uint_t<type_t, size> value) noexcept
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
constexpr long_uint_t<type_t, size> mulc(long_uint_t<type_t, size> value1, long_uint_t<type_t, size> value2, long_uint_t<type_t, size>& carry) noexcept
{
    using long_uint_t = long_uint_t<type_t, size>;
    using half_uint_t = half_t<long_uint_t>;
    
    const half_uint_t value1_lo = half_lo(value1);
    const half_uint_t value1_hi = half_hi(value1);
    const half_uint_t value2_lo = half_lo(value2);
    const half_uint_t value2_hi = half_hi(value2);

    half_uint_t t0_hi = 0;
    const half_uint_t t0_lo = mulc(value1_lo, value2_lo, t0_hi);
    half_uint_t t1_hi = t0_hi;
    const half_uint_t t1_lo = mulc(value1_hi, value2_lo, t1_hi);
    half_uint_t t2_hi = t1_lo;
    const half_uint_t t2_lo = mulc(value1_lo, value2_hi, t2_hi);
    half_uint_t t3_hi = t2_hi;
    const half_uint_t t3_lo = mulc(value1_hi, value2_hi, t3_hi);

    half_uint_t result_lo_lo = t0_lo;
    half_uint_t result_lo_hi = t2_lo;

    bool add_carry = false;
    const half_uint_t& result_hi_lo = addc(t3_lo, t1_hi, add_carry);
    const half_uint_t& result_hi_hi = t3_hi + add_carry;

    add_carry = false;

    if (carry != 0) {

        result_lo_lo = addc(result_lo_lo, half_lo(carry), add_carry);
        result_lo_hi = addc(result_lo_hi, half_hi(carry), add_carry);
    }

    const half_uint_t& carry_lo = addc(result_hi_lo, half_uint_t(0), add_carry);
    const half_uint_t& carry_hi = result_hi_hi + add_carry;

    carry = half_make(carry_hi, carry_lo);

    return half_make(result_lo_hi, result_lo_lo);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
constexpr long_uint_t<type_t, size> divr(long_uint_t<type_t, size> value1, long_uint_t<type_t, size> value2, std::optional<long_uint_t<type_t, size>>& remainder) noexcept
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

        quotient = half_make(quotient_hi, quotient_lo);

        if (remainder)
            remainder = half_make(half_uint_t(0), *remainder_lo);

    } else {

        quotient = divr2<long_uint_t>(0, value1, value2, remainder);
    }

    return quotient;
}

} // namespace slim

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of long_math_long.h
////////////////////////////////////////////////////////////////////////////////////////////////////
