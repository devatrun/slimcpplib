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
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <slimcpplib/long_int.h>

using uint128_t = slim::uint128_t;
using int128_t = slim::int128_t;
using uint256_t = slim::uint256_t;
using int256_t = slim::int256_t;

using uint32x4_t = slim::long_uint_t<uint32_t, 4>;
using int32x4_t = slim::long_int_t<uint32_t, 4>;
using uint32x8_t = slim::long_uint_t<uint32_t, 8>;
using int32x8_t = slim::long_int_t<uint32_t, 8>;
using uint64x4_t = slim::long_uint_t<uint64_t, 4>;
using int64x4_t = slim::long_int_t<uint64_t, 4>;
