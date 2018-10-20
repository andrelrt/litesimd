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

#include <litesimd/types.h>

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
 * \tparam ValueType_T Type of value inside packed SIMD register.
 *         Only for signed numerical types (int8_t, int16_t, int32_t, int64_t, float and double)
 * \tparam Tag_T Metaprogramming tag for instruction set selection.
 * \returns SIMD register with the sum of each packed value
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
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
 * \tparam ValueType_T Type of value inside packed SIMD register.
 *         Only for signed numerical types (int8_t, int16_t, int32_t, int64_t, float and double)
 * \tparam Tag_T Metaprogramming tag for instruction set selection.
 * \returns SIMD register with the difference of each packed value
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
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
 * \brief Multiply each value inside the packed SIMD register and save the least significant bits.
 *
 * Each value inside the packed SIMD register are multiplied independently and the
 * least significant bits of the result are saved on the same index inside the packed SIMD register.
 *
 * For 16 bits
 * | Index | 3 | 2 | 1 | 0 |
 * | :--- | :--: | :--: | :--: | :--: |
 * | Register X | a | b | c | d |
 * | Register Y | e | f | g | h |
 * | litesimd::mullo( X, Y ) | (a * e) & 0xffff | (b * f) & 0xffff | (c * g) & 0xffff | (d * h) & 0xffff |
 *
 * \param lhs, rhs SIMD registers to be multiplied
 * \tparam ValueType_T Type of value inside packed SIMD register. Only int16_t and int32_t are available for this function.
 * \tparam Tag_T Metaprogramming tag for instruction set selection.
 * \returns SIMD register with the least significant bit of multiplication result
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/algorithm.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *     t_int16_simd a( 0x1002 ), b( 0x10 );
 *     std::cout << "mullo( a, b ): " << ls::mullo( a, b ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * mullo( a, b ): (32, 32, 32, 32, 32, 32, 32, 32)
 * ```
 *
 * \remarks This functions works only on int16_t and int32_t on Intel archtecture
 * \see mulhi
 */
template< typename ValueType_T, typename Tag_T = default_tag >
simd_type< ValueType_T, Tag_T >
mullo( simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > ){}

/**
 * \ingroup arithmetic
 * \brief Multiply each value inside the packed SIMD register and save the most significant bits.
 *
 * Each value inside the packed SIMD register are multiplied independently and the
 * most significant bits of the result are saved on the same index inside the packed SIMD register.
 *
 * For 16 bits
 * | Index | 3 | 2 | 1 | 0 |
 * | :--- | :--: | :--: | :--: | :--: |
 * | Register X | a | b | c | d |
 * | Register Y | e | f | g | h |
 * | litesimd::mulhi( X, Y ) | (a * e) >> 16 | (b * f) >> 16 | (c * g) >> 16 | (d * h) >> 16 |
 *
 * \param lhs, rhs SIMD registers to be multiplied
 * \tparam ValueType_T Type of value inside packed SIMD register. Only int16_t is available for this function.
 * \tparam Tag_T Metaprogramming tag for instruction set selection.
 * \returns SIMD register with the most significant bit of multiplication result
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/algorithm.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *     t_int16_simd a( 0x1002 ), b( 0x10 );
 *     std::cout << "mulhi( a, b ): " << ls::mulhi( a, b ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * mulhi( a, b ): (16, 16, 16, 16, 16, 16, 16, 16)
 * ```
 *
 * \remarks This functions works only on int16_t on Intel archtecture
 * \see mullo
 */
template< typename ValueType_T, typename Tag_T = default_tag >
simd_type< ValueType_T, Tag_T >
mulhi( simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > ){}

template< typename ValueType_T, typename Tag_T = default_tag >
simd_type< ValueType_T, Tag_T >
div( simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > ){}

} // namespace litesimd

#endif // LITESIMD_ARCH_COMMON_ARITHMETIC_H
