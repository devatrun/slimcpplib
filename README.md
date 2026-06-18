# SLIMCPP
# Simple long integer math library for C++
SLIMCPP is a C++ header-only library that implements long integers that exceed the maximum size of integer types normally used by a compiler. All classes, methods and functions were not created or designed to work with huge numbers, for which there are specialized big integer mathematical libraries. In some cases, it is necessary to temporarily perform calculations with precision exceeding the maximum supported size of integers, and then return the result to its native size again. The conditions are ideal for SLIMCPP.
The main features:
* easy to use: the library is header-only and dependency-free
* compile-time friendly: core integer types, operators, helpers, fixed dividers, and literals are `constexpr`
* cross-platform: requires a C++20-compatible MSVC, GCC, or Clang compiler
* speed: uses platform intrinsics when available and platform-independent `constexpr` fallbacks otherwise
* exception neutral: doesn't use exceptions, all methods are `noexcept`
* STL friendly: supports `std::numeric_limits`, `std::hash`, and standard streams

The library implements two main classes:
```c++
namespace slim
{
template<typename native_t = uintmax_t, uint_t size = 2>
class long_uint_t; // unsigned integer
template<typename native_t = uintmax_t, uint_t size = 2>
class long_int_t;  // signed integer

} // namespace slim
```
where `native_t` is an unsigned integer type supported by the library and providing a standard integer-like API. It may be built from a compiler-native unsigned integer or from another `long_uint_t`, allowing recursive composition. `size` must be at least 2 and must be a power of two. The total integer width is `bit_count_v<native_t> * size`.
## Implementation
* [long_math.h](include/slimcpplib/long_math.h) - common `constexpr` primitives, traits, and generic native/integer operations
* [long_math_array.h](include/slimcpplib/long_math_array.h) - generic array/limb algorithms for unsigned integer-like word types
* [long_math_long.h](include/slimcpplib/long_math_long.h) - long integer specializations and long arithmetic helpers
* [long_uint.h](include/slimcpplib/long_uint.h) - unsigned long integer class, predefined unsigned aliases, and unsigned literals
* [long_int.h](include/slimcpplib/long_int.h) - signed long integer class, predefined signed aliases, and signed literals (**Can be completely removed if not used**)
* [long_fixdiv.h](include/slimcpplib/long_fixdiv.h) - fixed divider helper (**Can be completely removed if not used**)
* [long_io.h](include/slimcpplib/long_io.h) - optional standard stream input/output (**Can be completely removed if not used**)
* [gcc/long_math.h](include/slimcpplib/gcc/long_math.h) and [gcc/long_math_array.h](include/slimcpplib/gcc/long_math_array.h) - optional GCC/Clang optimized overload layers, including 128-bit SIMD array addition/subtraction when available
* [msvc/long_math.h](include/slimcpplib/msvc/long_math.h) and [msvc/long_math_array.h](include/slimcpplib/msvc/long_math_array.h) - optional MSVC optimized overload layers, including 128-bit SIMD array addition/subtraction when available

Files in [gcc](include/slimcpplib/gcc/) and [msvc](include/slimcpplib/msvc/) are optional optimization layers. If they are removed or unavailable, the library falls back to the default platform-independent implementation from the common headers.
## Integration
The library implements four predefined types: uint128_t, uint256_t, int128_t, int256_t. You can use them in your project by including the code below:
```c++
#include <slimcpplib/long_int.h>  // Include all integers support
// or
#include <slimcpplib/long_uint.h> // Include only unsigned integers support

namespace your_namespace
{
    using uint128_t = slim::uint128_t;
    using uint256_t = slim::uint256_t;
    using int128_t  = slim::int128_t;
    using int256_t  = slim::int256_t;
    using namespace slim::literals;

} // namespace your_namespace
```
The library also provides `std::numeric_limits` specializations for `long_uint_t` and `long_int_t`, and `std::hash` specializations for use with STL hash containers such as `std::unordered_set` and `std::unordered_map`.
## Constant declaration:
```c++
constexpr auto uo = 03766713523035452062041773345651416625031020_ui128;  // octal unsigned integer
constexpr auto ud = 338770000845734292534325025077361652240_ui128;       // decimal unsigned integer
constexpr auto uh = 0xfedcba9876543210fedcba9876543210_ui128;            // hexadecimal unsigned integer

constexpr auto io = -03766713523035452062041773345651416625031020_si128; // octal signed integer
constexpr auto id = -338770000845734292534325025077361652240_si128;      // decimal signed integer
constexpr auto ih = -0xfedcba9876543210fedcba9876543210_si128;           // hexadecimal signed integer
```
also supported (') separator for integer literals:
```c++
constexpr auto u = 0xfedcba98'76543210'fedcba98'76543210_ui128;          // hexadecimal unsigned integer
```
## Construction:
```c++
const uint128_t u;                // construct uninitialized unsigned integer
const uint128_t u = 1U;           // construction from unsigned integer
const int128_t  s = -1;           // construction from signed integer
const uint128_t u = 10000_ui128;  // construction from long unsigned integer
const int128_t  u = -10000_si128; // construction from long signed integer
const uint128_t u = true;         // construction from boolean value
```
## Operators
* long_uint_t type supports following operators:
` ==, !=, <, <=, >, >=, <<=, <<, >>=, >>, +=, +, ++, -=, -, --, *=, *, /=, /, %=, %, ~, &=, &, |=, |, ^=, ^`

* long_int_t type supports following operators:
`==, !=, <, <=, >, >=, +=, +, ++, -=, -, --, *=, *, /=, /, %=, %`
## MulDiv
The library implements the `constexpr` muldiv method for faster calculation of the following expressions: (a * b / c). It can be used with signed and unsigned integer types supported by the library.
```c++
template<typename type_t>
constexpr type_t muldiv(const type_t& value, const type_t& multiplier, const type_t& divider) noexcept;
```
## Standard stream input/output
```c++
std::cout << std::oct << 338770000845734292534325025077361652240_ui128 << "\n";       // octal
std::cout << std::dec << 03766713523035452062041773345651416625031020_ui128 << " \n"; // decimal
std::cout << std::hex << 0xfedcba9876543210fedcba9876543210_ui128 << "\n";            // hexadecimal
```
## Limitations
* The design of long integers tries to completely repeat the behavior of native integers, but still differs. For example, the propagation of integer types always occurs from a signed integer to an unsigned integer, and an implicit conversion from a larger integer to a smaller integer does not cause a warning, but a compilation error.
* Core operations are implemented as `constexpr`. Actual compile-time evaluation follows normal C++ constant-evaluation rules. Optimized runtime intrinsics do not prevent compile-time use because `constexpr` fallbacks are used during constant evaluation.
* Location of digits always corresponds to little-endian, regardless of the platform, which should be taken into account when serialization/deserialization. The digits themselves are always in platform natural order.
## Tests and examples
* [base_tests.cpp](src/tests/base_tests.cpp) - basic construction, conversions, constants, swaps, and core type behavior.
* [arithmetic_tests.cpp](src/tests/arithmetic_tests.cpp) - arithmetic, bitwise, comparison, and shift operations for unsigned long integers.
* [compound_shift_tests.cpp](src/tests/compound_shift_tests.cpp) - compound left and right shift scenarios across limb boundaries.
* [signed_tests.cpp](src/tests/signed_tests.cpp) - signed arithmetic, comparisons, sign handling, division, and modulo behavior.
* [overflow_boundary_tests.cpp](src/tests/overflow_boundary_tests.cpp) - overflow-sensitive and boundary-value arithmetic cases.
* [literal_mixed_tests.cpp](src/tests/literal_mixed_tests.cpp) - literal parsing and mixed operations with native integer types.
* [fixdiv_tests.cpp](src/tests/fixdiv_tests.cpp) - fixed divider creation and division behavior from `long_fixdiv.h`.
* [io_tests.cpp](src/tests/io_tests.cpp) - standard stream input/output formatting and round-trip parsing.
* [stl_properties_tests.cpp](src/tests/stl_properties_tests.cpp) - STL integration, including `std::numeric_limits`, `std::hash`, and related properties.
## Performance
All measurements are not intended to be a strong performance tests and are provided simply for relative comparison of the operation costs. All measurements were taken on Intel (R) Core (TM) i5-9400F CPU @ 2.90GHz in a 64-bit configuration with 128-bit integers.
| Operation  | Average time (in ns.)  |
|--:|---|
| Unsigned addition | 0.25 |
| Signed addition  | 0.25 |
| Unsigned subtraction | 0.25 |
| Signed subtraction | 0.25 |
| Unsigned multiplication | 7.00 |
| Signed multiplication | 7.00 |
| Unsigned division | 15.00 |
| Signed division | 20.00 |
| Unsigned muldiv() | 20.00 |
| Signed muldiv() | 25.00 |