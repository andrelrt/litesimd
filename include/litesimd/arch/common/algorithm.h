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

#ifndef LITESIMD_ARCH_COMMON_ALGORITHM_H
#define LITESIMD_ARCH_COMMON_ALGORITHM_H

#include "../../types.h"

namespace litesimd {

// Min max
// ---------------------------------------------------------------------------------------
/**
 * \ingroup algorithm
 * \brief Compares each value inside the SIMD register and returns the lesser of each pair.
 *
 * Each value inside the packed SIMD register is compared indenpendently and the result is
 * the lesser of the two values on the same index inside the packed SIMD register.
 *
 * | Index | 3 | 2 | 1 | 0 |
 * | :--- | :--: | :--: | :--: | :--: |
 * | Register X | a | b | c | d |
 * | Register Y | e | d | f | g |
 * | litesimd::min( X, Y ) | min( a, e ) | min( b, d ) | min( c, f ) | min( d, g ) |
 *
 * \param lhs, rhs SIMD registers to compare
 * \returns SIMD register with the lesser of each packed value
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
 *     t_int32_simd a( 4, 1, 3, 2 ), b( 2, 2, 1, 4 );
 *     std::cout << "min( a, b ): " << ls::min( a, b ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * min( a, b ): ( 2, 1, 1, 2 )
 * ```
 *
 * \see max
 */
template< typename Value_T, typename Tag_T > inline simd_type< Value_T, Tag_T >
min( simd_type< Value_T, Tag_T >, simd_type< Value_T, Tag_T > ){}

/**
 * \ingroup algorithm
 * \brief Compares each value inside the SIMD register and returns the greater of each pair.
 *
 * Each value inside the packed SIMD register is compared indenpendently and the result is
 * the greater of the two values on the same index inside the packed SIMD register.
 *
 * | Index | 3 | 2 | 1 | 0 |
 * | :--- | :--: | :--: | :--: | :--: |
 * | Register X | a | b | c | d |
 * | Register Y | e | d | f | g |
 * | litesimd::max( X, Y ) | max( a, e ) | max( b, d ) | max( c, f ) | max( d, g ) |
 *
 * \param lhs, rhs SIMD registers to compare
 * \returns SIMD register with the greater of each packed value
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
 *     t_int32_simd a( 4, 1, 3, 2 ), b( 2, 2, 1, 4 );
 *     std::cout << "max( a, b ): " << ls::max( a, b ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * max( a, b ): ( 4, 2, 3, 4 )
 * ```
 *
 * \see min
 */
template< typename Value_T, typename Tag_T > inline simd_type< Value_T, Tag_T >
max( simd_type< Value_T, Tag_T >, simd_type< Value_T, Tag_T > ){}

} // namespace litesimd

#endif // LITESIMD_ARCH_COMMON_ALGORITHM_H
