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

#if defined(_MSC_VER)

#include <intrin.h>

namespace slim::array
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _M_X64

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
requires(unsigned_type<type_t> && bit_count_v<type_t> * size == 128)
constexpr void add(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    if (std::is_constant_evaluated())
        return general::add(value1, value2);
    else {
        __m128i v1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(value1.data()));
        const __m128i v2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(value2.data()));

        __m128i result = _mm_add_epi64(v1, v2);
        const __m128i sign_mask = _mm_set1_epi64x(0x8000000000000000);
        const __m128i v1_flip = _mm_xor_si128(v1, sign_mask);
        const __m128i result_flip = _mm_xor_si128(result, sign_mask);
        const __m128i cmp = _mm_unpacklo_epi64(_mm_setzero_si128(), _mm_cmpgt_epi64(v1_flip, result_flip));
        result = _mm_sub_epi64(result, cmp);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(value1.data()), result);
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename type_t, uint_t size>
requires(unsigned_type<type_t> && bit_count_v<type_t> * size == 128)
constexpr void sub(std::span<type_t, size> value1, std::span<const type_t, size> value2) noexcept
{
    if (std::is_constant_evaluated())
        return general::sub(value1, value2);
    else {

        __m128i v1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(value1.data()));
        const __m128i v2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(value2.data()));

        __m128i result = _mm_sub_epi64(v1, v2);
        const __m128i sign_mask = _mm_set1_epi64x(0x8000000000000000);
        const __m128i v1_flip = _mm_xor_si128(v1, sign_mask);
        const __m128i result_flip = _mm_xor_si128(result, sign_mask);
        const __m128i cmp = _mm_unpacklo_epi64(_mm_setzero_si128(), _mm_cmpgt_epi64(result_flip, v1_flip));
        result = _mm_add_epi64(result, cmp);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(value1.data()), result);
    }
}

#endif // _M_X64

} // namespace slim::array

#endif // defined(_MSC_VER)

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of long_math_array.h
////////////////////////////////////////////////////////////////////////////////////////////////////
