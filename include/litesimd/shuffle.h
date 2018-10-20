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

#ifndef LITESIMD_SHUFFLE_H
#define LITESIMD_SHUFFLE_H

#include <litesimd/types.h>
#include <litesimd/arch/shuffle.h>
#include <litesimd/detail/helper_macros.h>

namespace litesimd {

/**
 * \defgroup shuffle Register manipulation
 *
 * In litesimd, the shuffle group has functions to manipulate the SIMD register.
 *
 * All this functions are accessable at `<litesimd/shuffle.h>`
 */

/**
 * \ingroup shuffle
 * \brief Shift all values one index lower inside the SIMD register and
 *        insert a value on the highest index.
 *
 * \param vec SIMD register
 * \param val Value to be inserted
 * \tparam ValueType_T Base type of original SIMD register
 * \returns SIMD register with the highest index modified
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
 *     ls::t_int32_simd x( 3, 2, 1, 0 );
 *     std::cout << "high_insert( x, 9 ): "
 *               << ls::high_insert( x, 9 ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * high_insert( x, 9 ): (9, 3, 2, 1)
 * ```
 */
template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
inline SimdType_T
high_insert( SimdType_T vec, typename SimdType_T::simd_value_type val )
{
    return high_insert< typename SimdType_T::simd_value_type,
                        typename SimdType_T::simd_tag >( vec, val );
}

/**
 * \ingroup shuffle
 * \brief Shift all values one index lower inside the SIMD register and
 *        insert a value on the lowest index.
 *
 * \param vec SIMD register
 * \param val Value to be inserted
 * \tparam ValueType_T Base type of original SIMD register
 * \returns SIMD register with the lowest index modified
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
 *     ls::t_int32_simd x( 3, 2, 1, 0 );
 *     std::cout << "low_insert( x, 9 ): "
 *               << ls::low_insert( x, 9 ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * low_insert( x, 9 ): (2, 1, 0, 9)
 * ```
 */
template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
inline SimdType_T
low_insert( SimdType_T vec, typename SimdType_T::simd_value_type val )
{
    return low_insert< typename SimdType_T::simd_value_type,
                       typename SimdType_T::simd_tag >( vec, val );
}

// Blend helper functions (comments on arch/common)
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
blend( simd_type< ValueType_T, Tag_T > mask,
       ValueType_T trueVal,
       simd_type< ValueType_T, Tag_T > falseVal )
{
    return blend< ValueType_T, Tag_T >( mask,
                                        simd_type< ValueType_T, Tag_T >( trueVal ),
                                        falseVal );
}

template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
blend( simd_type< ValueType_T, Tag_T > mask,
       simd_type< ValueType_T, Tag_T > trueVal,
       ValueType_T falseVal )
{
    return blend< ValueType_T, Tag_T >( mask,
                                        trueVal,
                                        simd_type< ValueType_T, Tag_T >( falseVal ) );
}

/**
 * \ingroup shuffle
 * \brief Extract one value from SIMD register.
 *
 * The `index` template parameter is verified with `static_assert` and the compilation will
 * fail with <b>`"Index out of bounds"`</b> error on invalid values.
 *
 * \param vec SIMD register
 * \tparam index Index of the value inside the SIMD register
 * \tparam ValueType_T Base type of original SIMD register
 * \returns The value extracted
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/shuffle.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd x( 9, 7, 5, 2 );
 *     std::cout << "get<2>( x ): "
 *               << ls::get<2>( x ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * get<2>( x ): 7
 * ```
 */
template< int index, typename ValueType_T, typename Tag_T = default_tag >
inline ValueType_T
get( simd_type< ValueType_T, Tag_T > vec )
{
    static_assert( 0 <= index && index < simd_type< ValueType_T, Tag_T >::simd_size,
                    "Index out of bounds" );

    return get_functor< index, ValueType_T, Tag_T >()( vec );
}

/**
 * \ingroup shuffle
 * \brief Extract one value from SIMD register.
 *
 * The `index` template parameter is verified with `static_assert` and the compilation will
 * fail with <b>`"Index out of bounds"`</b> error on invalid values.
 *
 * \param vec SIMD register
 * \tparam index Index of the value inside the SIMD register
 * \tparam ValueType_T Base type of original SIMD register
 * \returns The value extracted
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
 *     ls::t_int32_simd x( 9, 7, 5, 2 );
 *     std::cout << "set<2>( x, 11 ): "
 *               << ls::set<2>( x, 11 ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * set<2>( x, 11 ): (9, 11, 5, 2)
 * ```
 */
template< int index, typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
set( simd_type< ValueType_T, Tag_T > vec, ValueType_T val )
{
    static_assert( 0 <= index && index < simd_type< ValueType_T, Tag_T >::simd_size,
                    "Index out of bounds" );

    return set_functor< index, ValueType_T, Tag_T >()( vec, val );
}

} // namespace litesimd

#endif // LITESIMD_SHUFFLE_H
