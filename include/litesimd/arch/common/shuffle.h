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

#ifndef LITESIMD_ARCH_COMMON_SHUFFLE_H
#define LITESIMD_ARCH_COMMON_SHUFFLE_H

#include <litesimd/types.h>

namespace litesimd {

// High/Low insert
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
high_insert( simd_type< ValueType_T, Tag_T >, ValueType_T = 0 ){}

template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
low_insert( simd_type< ValueType_T, Tag_T >, ValueType_T = 0 ){}

/**
 * \ingroup shuffle
 * \brief Combine two SIMD registers using a mask to choose the values.
 *
 * The **blend** operation combines 2 SIMD registers using a mask, as a return of 
 * compare functions, to select each value.
 *
 * Example of blend function on SSE and int32_t
 *
 * | Index | 3 | 2 | 1 | 0 |
 * | :--- | :--: | :--: | :--: | :--: |
 * | litesimd::t_int32_simd **mask** | 0x00000000 | 0xFFFFFFFF | 0xFFFFFFFF | 0x00000000 |
 * | litesimd::t_int32_simd X( 1 ); | 1 | 1 | 1 | 1 |
 * | litesimd::t_int32_simd Y( 2 ); | 2 | 2 | 2 | 2 |
 * | litesimd::blend( mask, X, Y ); | 2 | 1 | 1 | 2 |
 *
 * \param mask Mask to select the values
 * \param trueVal Values select when mask is true
 * \param falseVal Values select when mask is false
 * \tparam ValueType_T Base type of original SIMD register
 * \returns Combined SIMD register
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/shuffle.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd mask( 0x00000000, 0xffffffff, 0Xffffffff, 0x00000000 );
 *     ls::t_int32_simd val1( 1 );
 *     ls::t_int32_simd val2( 2 );
 *     std::cout << "blend( mask, val1, val2 ): "
 *               << ls::blend( mask, val1, val2 ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * blend( mask, val1, val2 ): (2, 1, 1, 2)
 * ```
 */
template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
blend( simd_type< ValueType_T, Tag_T > mask,
       simd_type< ValueType_T, Tag_T > trueVal,
       simd_type< ValueType_T, Tag_T > falseVal ){}

// Get Set
// ---------------------------------------------------------------------------------------
template< int index, typename ValueType_T, typename Tag_T = default_tag >
struct get_functor
{
    inline ValueType_T operator()( simd_type< ValueType_T, Tag_T > ){}
};

template< int index, typename ValueType_T, typename Tag_T = default_tag >
struct set_functor
{
    inline simd_type< ValueType_T, Tag_T > operator()( simd_type< ValueType_T, Tag_T >, ValueType_T ){}
};

} // namespace litesimd

#endif // LITESIMD_ARCH_COMMON_SHUFFLE_H
