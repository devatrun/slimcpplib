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
// Copyright (c) 2020 Yury Kalmykov <y_kalmykov@mail.ru>.
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
// $Id:$
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#if __GNUC__

#include <x86intrin.h>

namespace slim
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

// calculate leading zero bits

uint_t nlz(const uint8_t& value) noexcept;
uint_t nlz(const uint16_t& value) noexcept;
uint_t nlz(const uint32_t& value) noexcept;
uint_t nlz(const uint64_t& value) noexcept;

// add with carry

uint8_t addc(const uint8_t& value1, const uint8_t& value2, bool& carry) noexcept;
uint16_t addc(const uint16_t& value1, const uint16_t& value2, bool& carry) noexcept;
uint32_t addc(const uint32_t& value1, const uint32_t& value2, bool& carry) noexcept;
uint64_t addc(const uint64_t& value1, const uint64_t& value2, bool& carry) noexcept;

// subtract with borrow

uint8_t subb(const uint8_t& value1, const uint8_t& value2, bool& borrow) noexcept;
uint16_t subb(const uint16_t& value1, const uint16_t& value2, bool& borrow) noexcept;
uint32_t subb(const uint32_t& value1, const uint32_t& value2, bool& borrow) noexcept;
uint64_t subb(const uint64_t& value1, const uint64_t& value2, bool& borrow) noexcept;

// multiply with carry

uint8_t mulc(const uint8_t& value1, const uint8_t& value2, uint8_t& carry) noexcept;
uint16_t mulc(const uint16_t& value1, const uint16_t& value2, uint16_t& carry) noexcept;
uint32_t mulc(const uint32_t& value1, const uint32_t& value2, uint32_t& carry) noexcept;
uint64_t mulc(const uint64_t& value1, const uint64_t& value2, uint64_t& carry) noexcept;



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

inline uint_t nlz(const uint8_t& value) noexcept
{
    return value ? __builtin_clz(value) : bit_count_v<uint8_t>;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint_t nlz(const uint16_t& value) noexcept
{
    return value ? __builtin_clz(value) : bit_count_v<uint16_t>;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint_t nlz(const uint32_t& value) noexcept
{
    return value ? __builtin_clz(value) : bit_count_v<uint32_t>;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint_t nlz(const uint64_t& value) noexcept
{
#ifdef __x86_64__
    return value ? __builtin_clz(value) : bit_count_v<uint64_t>;
#else
    const uint32_t value_hi = static_cast<uint32_t>(half_hi(value));
    const uint32_t value_lo = static_cast<uint32_t>(half_lo(value));

    const uint_t count = nlz(value_hi);
    return count < bit_count_v<uint32_t> ? count : count + nlz(value_lo);
#endif // __x86_64__
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint8_t addc(const uint8_t& value1, const uint8_t& value2, bool& carry) noexcept
{
    uint8_t result;
    carry = __builtin_add_overflow(value1, value2, &result);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint16_t addc(const uint16_t& value1, const uint16_t& value2, bool& carry) noexcept
{
    uint16_t result;
    carry = __builtin_add_overflow(value1, value2, &result);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t addc(const uint32_t& value1, const uint32_t& value2, bool& carry) noexcept
{
    uint32_t result;
    carry = __builtin_add_overflow(value1, value2, &result);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint64_t addc(const uint64_t& value1, const uint64_t& value2, bool& carry) noexcept
{
#ifdef __x86_64__
    uint64_t result;
    carry = __builtin_add_overflow(value1, value2, &result);

    return result;
#else
    return addc<uint64_t>(value1, value2, carry);
#endif // __x86_64__
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint8_t subb(const uint8_t& value1, const uint8_t& value2, bool& borrow) noexcept
{
    uint8_t result;
    borrow = __builtin_sub_overflow(value1, value2, &result);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint16_t subb(const uint16_t& value1, const uint16_t& value2, bool& borrow) noexcept
{
    uint16_t result;
    borrow = __builtin_sub_overflow(value1, value2, &result);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t subb(const uint32_t& value1, const uint32_t& value2, bool& borrow) noexcept
{
    uint32_t result;
    borrow = __builtin_sub_overflow(value1, value2, &result);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint64_t subb(const uint64_t& value1, const uint64_t& value2, bool& borrow) noexcept
{
#ifdef __x86_64__
    uint64_t result;
    borrow = __builtin_sub_overflow(value1, value2, &result);

    return result;
#else
    return subb<uint64_t>(value1, value2, borrow);
#endif // __x86_64__
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint8_t mulc(const uint8_t& value1, const uint8_t& value2, uint8_t& carry) noexcept
{
    const uint16_t result = uint16_t(value1) * uint16_t(value2) + uint16_t(carry);
    carry = static_cast<uint8_t>(half_hi(result));

    return static_cast<uint8_t>(result);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint16_t mulc(const uint16_t& value1, const uint16_t& value2, uint16_t& carry) noexcept
{
    const uint32_t result = uint32_t(value1) * uint32_t(value2) + uint32_t(carry);
    carry = static_cast<uint16_t>(half_hi(result));

    return static_cast<uint16_t>(result);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t mulc(const uint32_t& value1, const uint32_t& value2, uint32_t& carry) noexcept
{
    const uint64_t result = uint64_t(value1) * uint64_t(value2) + uint64_t(carry);
    carry = static_cast<uint32_t>(half_hi(result));

    return static_cast<uint32_t>(result);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint64_t mulc(const uint64_t& value1, const uint64_t& value2, uint64_t& carry) noexcept
{
#ifdef __x86_64__
    const unsigned __int128 result = static_cast<unsigned __int128>(value1) * value2;
    uint64_t result_lo = result & 0xffffffffffffffff;
    uint64_t result_hi = result >> 64;

    bool add_carry = false;
    result_lo = addc(result_lo, carry, add_carry);
    result_hi = addc(result_hi, 0, add_carry);

    carry = result_hi;

    return result_lo;
#else
    return mulc<uint64_t>(value1, value2, carry);
#endif // __x86_64__
}

} // namespace slim

#endif // __GNUC__

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of long_math_gcc.h
////////////////////////////////////////////////////////////////////////////////////////////////////