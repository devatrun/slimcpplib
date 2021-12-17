////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Simple Long Integer Math for C++
// version 1.3
//
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021 Yury Kalmykov <y_kalmykov@mail.ru>.
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

#include "../include/slimcpplib/long_int.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
//  define 128-bit and 256-bit integer types in your namespace
////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace slim::literals;
using uint128_t = slim::uint128_t;
using int128_t = slim::int128_t;
using uint256_t = slim::uint256_t;
using int256_t = slim::int256_t;

//using slim_uint64_t = slim::long_uint_t<uint32_t, 2>;
//using uint128_t = slim::long_uint_t<slim_uint64_t, 2>;
//using int128_t = slim::long_int_t<slim_uint64_t, 2>;
//using uint256_t = slim::long_uint_t<slim_uint64_t, 4>;
//using int256_t = slim::long_int_t<slim_uint64_t, 4>;



////////////////////////////////////////////////////////////////////////////////////////////////////
// standalone routines
////////////////////////////////////////////////////////////////////////////////////////////////////

static void initalization_and_conversion() noexcept
{
    // construct long integer

    uint128_t u1; // can't be used as constant expressions
    u1 = 0;
    assert(u1 == 0);


    // construct long integer from unsigned integer value

    constexpr uint256_t u2 = 1U;
    static_assert(u2 == 1);



    // construct long integer from signed integer value

    constexpr uint128_t u3 = -1; // sign will be expanded
    static_assert(u3 == -1);
    constexpr int128_t s4 = -1; // sign will be expanded
    static_assert(s4 == -1);



    // construct long integer from long integer with opposite sign

    constexpr int128_t s5 = uint128_t(-1);
    static_assert(u3 == -1);
    constexpr uint128_t u6 = int128_t(-1);
    static_assert(s4 == -1);



    // construct long integer from shorter long integer value

    constexpr int256_t s7 = int128_t(-1); // sign will be expanded
    static_assert(s7 == -1);
    const int256_t s8 = int128_t(-1); // sign will be expanded
    assert(s8 == -1);



    // construct long integer from longer long integer value

    //constexpr int s9 = u2; // error! implicit conversion not allowed
    constexpr int s9 = static_cast<int>(uint256_t(1));
    static_assert(s9 == 1);

    // constexpr int128_t s10 = u2; // error! implicit conversion not allowed
    constexpr int128_t s10 = static_cast<uint128_t>(uint256_t(-1));
    static_assert(s10 == -1);



    // construct long integer from long integer constant

    constexpr auto u11 = 0xaf5705a4'89525e79'a5120c42'daebbc57'd55f0277'53a05970'9fee8a5d'41e2ae79_ui256;
    static_assert(u11 == 0xaf5705a4'89525e79'a5120c42'daebbc57'd55f0277'53a05970'9fee8a5d'41e2ae79_ui256);
    constexpr auto s12 = -0xf473e8e5'f6e812c3'fde4523b'51b6d251_si128;
    static_assert(s12 == -0xf473e8e5'f6e812c3'fde4523b'51b6d251_si128);



    // construct long integer from boolean value

    constexpr uint128_t u13 = true; // implicitly converted to one as standard
    static_assert(u13 == 1);
    constexpr int128_t s14 = false; // implicitly converted to zero as standard
    static_assert(s14 == 0);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void comparison() noexcept
{
    // compare long unsigned integers

    constexpr auto uzero = 0_ui128;

    static_assert(uzero == 0);
    static_assert(uzero != 1);

    assert(uzero < -1);
    assert(uzero <= 0);
    assert(-1 > uzero);
    assert(-1 >= uzero);

    // compare long signed integers

    constexpr auto szero = 0_si128;

    static_assert(szero == 0);
    static_assert(szero != 1);

    assert(-1 < szero);
    assert(-1 <= szero);
    assert(szero > -1);
    assert(szero >= 0);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
static void bitwise_oprations() noexcept
{
    constexpr uint256_t zero = 0;
    constexpr uint256_t ones = -1;

    // inverse all bits
    static_assert(~zero == -1);
    static_assert(~ones == 0);

    // perform AND operation
    static_assert((zero & ones) == 0);

    // perform OR operation
    static_assert((zero | ones) == -1);

    // perform exclusive OR operation
    static_assert((zero ^ ones) == -1);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
static void bit_shift_operations() noexcept
{
    constexpr uint128_t bits = -1;
    constexpr uint128_t shift = 127;

    assert((bits >> static_cast<int>(shift)) == 1); // shift argument has native integer type
    assert((bits << 127) == 0x80000000'00000000'00000000'00000000_ui128);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
static void arithmetic_operations() noexcept
{
    constexpr auto uvalue = 0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128;
    constexpr auto svalue = -0xf473e8e5'f6e812c3'fde4523b'51b6d251_si128;

    // unary plus
    assert(+uvalue == 0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);
    assert(+svalue == -0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);

    // prefix increment
    assert(++uint128_t(uvalue) == 0xf473e8e5'f6e812c3'fde4523b'51b6d252_ui128);
    assert(++int128_t(svalue) == -0xf473e8e5'f6e812c3'fde4523b'51b6d250_ui128);

    // postfix increment
    assert(uint128_t(uvalue)++ == 0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);
    assert(int128_t(svalue)++ == -0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);

    // addition
    assert(uvalue + 1 == 0xf473e8e5'f6e812c3'fde4523b'51b6d252_ui128);
    assert(svalue + 1 == -0xf473e8e5'f6e812c3'fde4523b'51b6d250_ui128);



    // unary minus
    assert(-uvalue == -0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);
    assert(-svalue == 0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);

    // prefix decrement
    assert(--uint128_t(uvalue) == 0xf473e8e5'f6e812c3'fde4523b'51b6d250_ui128);
    assert(--int128_t(svalue) == -0xf473e8e5'f6e812c3'fde4523b'51b6d252_ui128);

    // postfix decrement
    assert(uint128_t(uvalue)-- == 0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);
    assert(int128_t(svalue)-- == -0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);

    // subtraction
    assert(uvalue - 1 == 0xf473e8e5'f6e812c3'fde4523b'51b6d250_ui128);
    assert(svalue - 1 == -0xf473e8e5'f6e812c3'fde4523b'51b6d252_ui128);



    // multiplication
    assert(uvalue * 1 == 0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);
    assert(svalue * 1 == -0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);



    // division
    assert(uvalue / 1 == 0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);
    // uvalue / 0; // fail! will be interrupted as standard integer division by zero
    assert(svalue / 1 == -0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);
    // svalue / 0; // fail! will be interrupted as standard integer division by zero



    // modulo remainder
    assert(uvalue % 1 == 0);
    // uvalue % 0; // fail! will be interrupted as standard integer division by zero
    assert(svalue % 1 == 0);
    // svalue % 0; // fail! will be interrupted as standard integer division by zero
}



////////////////////////////////////////////////////////////////////////////////////////////////////
static void muldiv_operation() noexcept
{
    constexpr auto uvalue = 0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128;
    constexpr auto svalue = -0xf473e8e5'f6e812c3'fde4523b'51b6d251_si128;

    // multiplication then division operation

    assert(muldiv(uvalue, uvalue, uvalue) == 0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);
    assert(muldiv(svalue, svalue, svalue) == -0xf473e8e5'f6e812c3'fde4523b'51b6d251_ui128);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// module main entry point
////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    initalization_and_conversion();
    comparison();
    bitwise_oprations();
    bit_shift_operations();
    arithmetic_operations();
    muldiv_operation();

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// End of main.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////
