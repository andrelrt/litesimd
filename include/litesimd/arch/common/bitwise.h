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

#ifndef LITESIMD_ARCH_COMMON_BITWISE_H
#define LITESIMD_ARCH_COMMON_BITWISE_H

#include <litesimd/types.h>

namespace litesimd {

// Basic bitwise operations
// ---------------------------------------------------------------------------------------

/**
 * \ingroup bitwise
 * \brief Returns the bitwise AND operation between passed parameters.
 *
 * \param lhs, rhs SIMD registers to apply bitwise AND operation.
 * \returns SIMD register with the result of the operation.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/bitwise.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 3 ), b( 6 );
 *     std::cout << "bit_and( a, b ): " << ls::bit_and( a, b ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * bit_and( a, b ): (2, 2, 2, 2)
 * ```
 */
template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
bit_and( simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > ){}

/**
 * \ingroup bitwise
 * \brief Returns the bitwise OR operation between passed parameters.
 *
 * \param lhs, rhs SIMD registers to apply bitwise OR operation.
 * \returns SIMD register with the result of the operation.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/bitwise.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 3 ), b( 6 );
 *     std::cout << "bit_or( a, b ): " << ls::bit_or( a, b ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * bit_or( a, b ): (7, 7, 7, 7)
 * ```
 */
template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
bit_or( simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > ){}

/**
 * \ingroup bitwise
 * \brief Returns the bitwise XOR operation between passed parameters.
 *
 * \param lhs, rhs SIMD registers to apply bitwise XOR operation.
 * \returns SIMD register with the result of the operation.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/bitwise.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 3 ), b( 6 );
 *     std::cout << "bit_xor( a, b ): " << ls::bit_xor( a, b ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * bit_xor( a, b ): (5, 5, 5, 5)
 * ```
 */
template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
bit_xor( simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > ){}

/**
 * \ingroup bitwise
 * \brief Inverts all bits on a SIMD register.
 *
 * \param vec SIMD register to negate.
 * \returns SIMD register with all bit inverted.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/bitwise.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 0xfffffff0 );
 *     std::cout << "bit_not( a ): " << ls::bit_not( a ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * bit_not( a ): (15, 15, 15, 15)
 * ```
 */
template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
bit_not( simd_type< ValueType_T, Tag_T > vec )
{
    return bit_xor( vec, simd_type< ValueType_T, Tag_T >::ones() );
}

} // namespace litesimd

#endif // LITESIMD_ARCH_COMMON_BITWISE_H
