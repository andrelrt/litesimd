// MIT License
//
// Copyright (c) 2018 André Tupinambá
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef LITESIMD_ARCH_COMMON_ARITHMETIC_H
#define LITESIMD_ARCH_COMMON_ARITHMETIC_H

#include "../../types.h"

namespace litesimd {

// Basic operations
// ---------------------------------------------------------------------------------------
/**
 * \ingroup arithmetic
 * \brief Adds each value inside the packed SIMD register.
 *
 * Each value inside the packed SIMD register is added indenpendently and the result is
 * saved on the same index inside the packed SIMD register.
 *
 * | Index | 3 | 2 | 1 | 0 |
 * | :--- | :--: | :--: | :--: | :--: |
 * | Register X | a | b | c | d |
 * | Register Y | e | f | g | h |
 * | litesimd::add( X, Y ) | a + e | b + f | c + g | d + h |
 *
 * \param lhs, rhs SIMD registers to be added
 * \returns SIMD register with the sum of each packed value
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/algorithm.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *     t_int32_simd a( 10 ), b( 1 );
 *     std::cout << "add( a, b ): " << ls::add( a, b ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * add( a, b ): ( 11, 11, 11, 11 )
 * ```
 *
 * \see sub
 */
template< typename ValueType_T, typename Tag_T = default_tag >
simd_type< ValueType_T, Tag_T >
add( simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > ){}

/**
 * \ingroup arithmetic
 * \brief Subtract each value inside the packed SIMD register.
 *
 * Each value inside the packed SIMD register is subtracted indenpendently and the result is
 * saved on the same index inside the packed SIMD register.
 *
 * | Index | 3 | 2 | 1 | 0 |
 * | :--- | :--: | :--: | :--: | :--: |
 * | Register X | a | b | c | d |
 * | Register Y | e | f | g | h |
 * | litesimd::sub( X, Y ) | a - e | b - f | c - g | d - h |
 *
 * \param lhs, rhs SIMD registers to be subtracted
 * \returns SIMD register with the difference of each packed value
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/algorithm.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *     t_int32_simd a( 10 ), b( 1 );
 *     std::cout << "sub( a, b ): " << ls::sub( a, b ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * sub( a, b ): ( 9, 9, 9, 9 )
 * ```
 *
 * \see add
 */
template< typename ValueType_T, typename Tag_T = default_tag >
simd_type< ValueType_T, Tag_T >
sub( simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > ){}

/**
 * \ingroup arithmetic
 * \brief Multiply each value inside the packed SIMD register and save the lower bits.
 *
 * Each value inside the packed SIMD register are multiplied indenpendently and the
 * lower bit of the result is saved on the same index inside the packed SIMD register.
 *
 * | Index | 3 | 2 | 1 | 0 |
 * | :--- | :--: | :--: | :--: | :--: |
 * | Register X | a | b | c | d |
 * | Register Y | e | f | g | h |
 * | litesimd::sub( X, Y ) | a - e | b - f | c - g | d - h |
 *
 * \param lhs, rhs SIMD registers to be subtracted
 * \returns SIMD register with the difference of each packed value
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/algorithm.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *     t_int32_simd a( 10 ), b( 1 );
 *     std::cout << "sub( a, b ): " << ls::sub( a, b ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * sub( a, b ): ( 9, 9, 9, 9 )
 * ```
 *
 * \see add
 */
template< typename ValueType_T, typename Tag_T = default_tag >
simd_type< ValueType_T, Tag_T >
mullo( simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > ){}

template< typename ValueType_T, typename Tag_T = default_tag >
simd_type< ValueType_T, Tag_T >
mulhi( simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > ){}

template< typename ValueType_T, typename Tag_T = default_tag >
simd_type< ValueType_T, Tag_T >
div( simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > ){}

} // namespace litesimd

#endif // LITESIMD_ARCH_COMMON_ARITHMETIC_H

