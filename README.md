# SLIMCPP
# Simple long integer math library for C++
SLIMCPP is a C++ header-only library that implements long integers that exceed maximum size of native type supported by a specific compiler by 2-4 times. All classes, methods and functions were not created or designed to work with huge numbers, for which there are specialized big integer mathematical libraries. In some cases, it is necessary to temporarily perform calculations with precision exceeding the maximum supported size of integers, and then return the result to its native size again. The conditions are ideal for SLIMCPP.
The main features:
* easy to use: the library is header-only
* small: consists of few header files, there is no dependencies
* speed: use intrinsics if supported
* cross-platform: supports MSVC, GCC and CLANG C++20 compilers
* exception neutral: doesn't use exceptions, all methods are noexсept
* STL friendly: supports `std::numeric_limits` and `std::hash`

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
where native_t may be one of base unsigned type and size must by power of two.
## Implementation
* [long_int.h](include/slimcpplib/long_int.h) - signed long integer class (**Can be completely removed if not used**)
* [long_uint.h](include/slimcpplib/long_uint.h) - unsigned long integer class
* [long_math.h](include/slimcpplib/long_math.h) - cross-platform helper classes and functions
* [long_math_long.h](include/slimcpplib/long_math_long.h) - cross-platform helper classes and functions (long_uint_t/long_int_t specializations)
* [long_math_gcc.h](include/slimcpplib/long_math_gcc.h) - GCC, CLANG helper classes and functions (**Can be completely removed if irrelevant**)
* [long_math_msvc.h](include/slimcpplib/long_math_msvc.h) - MSVC helper classes and functions (**Can be completely removed if irrelevant**)
* [long_io.h](include/slimcpplib/long_io.h) - standard stream input/output (**Can be completely removed if not used**)
## Integration
The library implements four predefined types: uint128_t, uint256_t, int128_t, int256_t. You can use them in your project by include code below:
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
The library implements the muldiv method for faster calculation of the following expressions: (a * b / c). It can be used with signed and unsigned integers.
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
* Almost all operations can be evaluated at compile time. The exceptions are operations that may cause undefined behavior, where compile-time evaluation is not guaranteed.
* Location of digits always corresponds to little-endian, regardless of the platform, which should be taken into account when serialization/deserialization. The digits themselves are always in platform natural order.
## Tests and examples
* [base_tests.cpp](tests/src/base_tests.cpp) - basic construction, conversions, constants, swaps, and core type behavior.
* [arithmetic_tests.cpp](tests/src/arithmetic_tests.cpp) - arithmetic, bitwise, comparison, and shift operations for unsigned long integers.
* [compound_shift_tests.cpp](tests/src/compound_shift_tests.cpp) - compound left and right shift scenarios across limb boundaries.
* [signed_tests.cpp](tests/src/signed_tests.cpp) - signed arithmetic, comparisons, sign handling, division, and modulo behavior.
* [overflow_boundary_tests.cpp](tests/src/overflow_boundary_tests.cpp) - overflow-sensitive and boundary-value arithmetic cases.
* [literal_mixed_tests.cpp](tests/src/literal_mixed_tests.cpp) - literal parsing and mixed operations with native integer types.
* [io_tests.cpp](tests/src/io_tests.cpp) - standard stream input/output formatting and round-trip parsing.
* [stl_properties_tests.cpp](tests/src/stl_properties_tests.cpp) - STL integration, including `std::numeric_limits`, `std::hash`, and related properties.
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
