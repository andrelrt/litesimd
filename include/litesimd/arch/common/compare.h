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

#ifndef LITESIMD_ARCH_COMMON_COMPARE_H
#define LITESIMD_ARCH_COMMON_COMPARE_H

#include <litesimd/types.h>

namespace litesimd {

/**
 * \ingroup compare
 * \brief Converts a SIMD mask to a bitmask
 *
 * \param mask SIMD mask to be converted
 * \tparam ValueType_T Base type of original SIMD register
 * \returns Bitmask equivalent
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/compare.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd x( 9, 8, 7, 6 );
 *     ls::t_int32_simd y( 9, 8, 5, 6 );
 *     auto mask = ls::equal_to( x, y ); // (0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF)
 *     std::cout << "mask_to_bitmask< int32_t >( mask ): " << std::hex
 *               << ls::mask_to_bitmask< int32_t >( mask ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * mask_to_bitmask< int32_t >( mask ): ff0f
 * ```
 */
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename simd_type< ValueType_T, Tag_T >::bitmask_type
mask_to_bitmask( simd_type< ValueType_T, Tag_T > mask )
{
    return 0;
}

/**
 * \ingroup compare
 * \brief Compares two SIMD registers and returns a mask representing the values of the first parameter is greater than the second parameter
 *
 * \param lhs, rhs Values to be compared
 * \tparam ValueType_T Base type of SIMD register
 * \returns Mask representing the values of the lhs parameter which are greater than the rhs parameter
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/compare.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd x( 9, 8, 7, 6 );
 *     ls::t_int32_simd y( 9, 8, 5, 6 );
 *     std::cout << "greater( x, y ): " << std::hex << ls::greater( x, y ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * greater( x, y ): (0, 0, ffffffff, 0)
 * ```
 */
template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
greater( simd_type< ValueType_T, Tag_T > lhs, simd_type< ValueType_T, Tag_T > rhs ){}

/**
 * \ingroup compare
 * \brief Compares two SIMD registers and returns a mask of equal values
 *
 * \param lhs, rhs Values to be compared
 * \tparam ValueType_T Base type of SIMD register
 * \returns Mask representing the values of the lhs parameter which are equal to the rhs parameter
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/compare.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd x( 9, 8, 7, 6 );
 *     ls::t_int32_simd y( 9, 8, 5, 6 );
 *     std::cout << "equal_to( x, y ): " << std::hex << ls::equal_to( x, y ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * equal_to( x, y ): (ffffffff, ffffffff, 0, ffffffff)
 * ```
 */
template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
equal_to( simd_type< ValueType_T, Tag_T > lhs, simd_type< ValueType_T, Tag_T > rhs ){}

} // namespace litesimd

#endif // LITESIMD_ARCH_COMMON_COMPARE_H
