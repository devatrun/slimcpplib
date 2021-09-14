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

#if _MSC_VER

#include <intrin.h>

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

// shift bits to left

uint64_t shl2(const uint64_t& value_hi, const uint64_t& value_lo, uint_t shift) noexcept;

// shift bits to right

uint64_t shr2(const uint64_t& value_hi, const uint64_t& value_lo, uint_t shift) noexcept;

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
    unsigned long result = 0;
    result = static_cast<uint8_t>(_BitScanReverse(&result, value) ? (31 - result) - 24 : bit_count_v<uint8_t>);

    return static_cast<uint8_t>(result);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint_t nlz(const uint16_t& value) noexcept
{
    unsigned long result = 0;
    result = static_cast<uint16_t>(_BitScanReverse(&result, value) ? (31 - result) - 16 : bit_count_v<uint16_t>);

    return static_cast<uint16_t>(result);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint_t nlz(const uint32_t& value) noexcept
{
    unsigned long result = 0;
    result = static_cast<uint32_t>(_BitScanReverse(&result, value) ? 31 - result : bit_count_v<uint32_t>);

    return static_cast<uint32_t>(result);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint_t nlz(const uint64_t& value) noexcept
{
#ifdef _M_X64
    unsigned long result = 0;
    result = _BitScanReverse64(&result, value) ? 63 - result : static_cast<long>(bit_count_v<uint64_t>);

    return static_cast<uint64_t>(result);
#else
    const uint32_t value_hi = static_cast<uint32_t>(half_hi(value));
    const uint32_t value_lo = static_cast<uint32_t>(half_lo(value));

    const uint_t count = nlz(value_hi);
    return count < bit_count_v<uint32_t> ? count : count + nlz(value_lo);
#endif // _M_X64
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint64_t shl2(const uint64_t& value_hi, const uint64_t& value_lo, uint_t shift) noexcept
{
#ifdef _M_X64
    if (shift < bit_count_v<uint64_t>)
        return __shiftleft128(value_lo, value_hi, static_cast<uint8_t>(shift));
    else
        return value_lo << (shift - bit_count_v<uint64_t>);
#else
    return shl2<uint64_t>(value_hi, value_lo, shift);
#endif // _M_X64
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint64_t shr2(const uint64_t& value_hi, const uint64_t& value_lo, uint_t shift) noexcept
{
#ifdef _M_X64
    if (shift < bit_count_v<uint64_t>)
        return __shiftright128(value_lo, value_hi, static_cast<uint8_t>(shift));
    else
        return value_hi >> (shift - bit_count_v<uint64_t>);
#else
    return shr2<uint64_t>(value_hi, value_lo, shift);
#endif // _M_X64
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint8_t addc(const uint8_t& value1, const uint8_t& value2, bool& carry) noexcept
{
    uint8_t result;
    carry = _addcarry_u8(carry, value1, value2, &result);

    return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint16_t addc(const uint16_t& value1, const uint16_t& value2, bool& carry) noexcept
{
    uint16_t result;
    carry = _addcarry_u16(carry, value1, value2, &result);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t addc(const uint32_t& value1, const uint32_t& value2, bool& carry) noexcept
{
    uint32_t result;
    carry = _addcarry_u32(carry, value1, value2, &result);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint64_t addc(const uint64_t& value1, const uint64_t& value2, bool& carry) noexcept
{
#ifdef _M_X64
    uint64_t result;
    carry = _addcarry_u64(carry, value1, value2, &result);

    return result;
#else
    return addc<uint64_t>(value1, value2, carry);
#endif // _M_X64
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint8_t subb(const uint8_t& value1, const uint8_t& value2, bool& borrow) noexcept
{
    uint8_t result;
    borrow = _subborrow_u8(borrow, value1, value2, &result);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint16_t subb(const uint16_t& value1, const uint16_t& value2, bool& borrow) noexcept
{
    uint16_t result;
    borrow = _subborrow_u16(static_cast<uint8_t>(borrow), value1, value2, &result);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t subb(const uint32_t& value1, const uint32_t& value2, bool& borrow) noexcept
{
    uint32_t result;
    borrow = _subborrow_u32(static_cast<uint8_t>(borrow), value1, value2, &result);

    return result;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint64_t subb(const uint64_t& value1, const uint64_t& value2, bool& borrow) noexcept
{
#ifdef _M_X64
    uint64_t result;
    borrow = _subborrow_u64(static_cast<uint8_t>(borrow), value1, value2, &result);

    return result;
#else
    return subb<uint64_t>(value1, value2, borrow);
#endif // _M_X64
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
#ifdef _M_X64
    uint64_t result_lo;
    uint64_t result_hi;

    result_lo = _umul128(value1, value2, &result_hi);

    bool add_carry = false;
    result_lo = addc(result_lo, carry, add_carry);
    result_hi = addc(result_hi, 0, add_carry);

    carry = result_hi;

    return result_lo;
#else
    return mulc<uint64_t>(value1, value2, carry);
#endif // _M_X64
}

} // namespace slim

#endif // _MSC_VER

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of long_math_msvc.h
////////////////////////////////////////////////////////////////////////////////////////////////////
