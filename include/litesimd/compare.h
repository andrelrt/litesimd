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

#ifndef LITESIMD_COMPARE_H
#define LITESIMD_COMPARE_H

#include <immintrin.h>
#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif
#include <type_traits>
#include <litesimd/types.h>
#include <litesimd/arch/compare.h>
#include <litesimd/detail/helper_macros.h>

namespace litesimd {

/**
 * \defgroup compare Comparision operations
 *
 * In litesimd, compare group has functions to compare values, with `grater than`
 * and `equal to` operators, and to manipulate the result.
 *
 * All this functions are accessable at `<litesimd/compare.h>`
 *
 * The Litesimd comparision functions have 3 result types:
 *
 * - **mask**: Is the result of the comparison and have the same SIMD type of
 *   operands. It represents the result as all bits 0 for `false` and all bits 1
 *   for `true`. Some litesimd functions can use this mask as a parameter to execute
 *   the operation based on `true` and `false` values. \see blend()
 * - **bitmask**: Scalar version of the _mask_ result. It is calculated as the
 *   most significant bit of each 8 bits of SIMD register. It can be useful to calculate
 *   the _index_ of `true` result and to use SIMD result on an `if` statement.
 *   ```{.cpp}
 *   auto bitmask = litesimd::mask_to_bitmask( mask );
 *   if( bitmask == 0 ) {
 *      // No bit set on mask
 *   }
 *   ``` 
 * - **index**: Position inside the SIMD register with `true` result and is generated
 *   using the bitmask. There are 2 functions to calculate indexes, one returns the
 *   first index of the bitmask and the another returns the last index of the bitmask.
 *   \see for_each_index()
 *
 * Example of this results on SSE and int32_t
 *
 * | Index | 3 | 2 | 1 | 0 |
 * | :--- | :--: | :--: | :--: | :--: |
 * | litesimd::t_int32_simd X( 9, 8, 7, 6 ); | 9 | 8 | 7 | 6 |
 * | litesimd::t_int32_simd Y( 9, 8, 5, 6 ); | 9 | 8 | 5 | 6 |
 * | litesimd::t_int32_simd **mask** = litesimd::equal_to( X, Y ); | 0xFFFFFFFF | 0xFFFFFFFF | 0x00000000 | 0xFFFFFFFF |
 * | uint16_t **bitmask** = litesimd::mask_to_bitmask( _mask_ ); | 0xFF0F ||||
 * | int **first_index** = litesimd::bitmask_first_index( _bitmask_ ); | 0 ||||
 * | int **last_index** = litesimd::bitmask_last_index( _bitmask_ ); | 3 ||||
 *
 */

/**
 * \ingroup compare
 * \brief Find the last index on a bitmask
 *
 * \param bmask Bitmask to be searched
 * \tparam ValueType_T Base type of original SIMD register
 * \returns The highest index set on the bitmask
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
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
 *     auto bitmask = ls::mask_to_bitmask( mask ); // 0xFF0F
 *     std::cout << "bitmask_last_index< int32_t >( bitmask ): "
 *               << ls::bitmask_last_index< int32_t >( bitmask ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * bitmask_last_index< int32_t >( bitmask ): 3
 * ```
 * @{
 */
template< typename ValueType_T,
          typename std::enable_if< std::is_integral< ValueType_T >::value >::type* = nullptr >
inline int
bitmask_last_index( uint32_t bmask )
{
#ifdef _WIN32
    unsigned long index;
    return (0 == _BitScanReverse( &index, bmask ))
        ? -1
        : index / sizeof(ValueType_T);
#else
    return (bmask == 0)
        ? -1
        : _bit_scan_reverse( bmask ) / sizeof(ValueType_T);
#endif
}

template< typename ValueType_T,
          typename std::enable_if< std::is_floating_point< ValueType_T >::value >::type* = nullptr >
inline int
bitmask_last_index( uint32_t bmask )
{
#ifdef _WIN32
    unsigned long index;
    return (0 == _BitScanReverse( &index, bmask ))
        ? -1
        : index;
#else
    return (bmask == 0)
        ? -1
        : _bit_scan_reverse( bmask );
#endif
}
/**@}*/

/**
 * \ingroup compare
 * \brief Find the first index on a bitmask
 *
 * \param bmask Bitmask to be searched
 * \tparam ValueType_T Base type of original SIMD register
 * \returns The lowest index set on the bitmask
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
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
 *     auto bitmask = ls::mask_to_bitmask( mask ); // 0xFF0F
 *     std::cout << "bitmask_first_index< int32_t >( bitmask ): "
 *               << ls::bitmask_first_index< int32_t >( bitmask ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * bitmask_first_index< int32_t >( bitmask ): 0
 * ```
 */
///@{
template< typename ValueType_T,
          typename std::enable_if< std::is_integral< ValueType_T >::value >::type* = nullptr >
inline int
bitmask_first_index( uint32_t bmask )
{
#ifdef _WIN32
    unsigned long index;
    return (0 == _BitScanForward( &index, bmask ))
        ? -1
        : index / sizeof(ValueType_T);
#else
    return (bmask == 0)
        ? -1
        : _bit_scan_forward( bmask ) / sizeof(ValueType_T);
#endif
}

template< typename ValueType_T,
          typename std::enable_if< std::is_floating_point< ValueType_T >::value >::type* = nullptr >
inline int
bitmask_first_index( uint32_t bmask )
{
#ifdef _WIN32
    unsigned long index;
    return (0 == _BitScanForward( &index, bmask ))
        ? -1
        : index;
#else
    return (bmask == 0)
        ? -1
        : _bit_scan_forward( bmask );
#endif
}
///@}

// Greater than
// ---------------------------------------------------------------------------------------
DEFINE_BINARY_FUNCTION_ADAPTORS( greater, type )

// Greater than bitmask
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename simd_type< ValueType_T, Tag_T >::bitmask_type
greater_bitmask( simd_type< ValueType_T, Tag_T > lhs,
                      simd_type< ValueType_T, Tag_T > rhs )
{
    return mask_to_bitmask< ValueType_T, Tag_T >(
                greater< ValueType_T, Tag_T >( lhs, rhs )
            );
}

DEFINE_BINARY_FUNCTION_ADAPTORS( greater_bitmask, bitmask_type )

// Greater than high index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline int
greater_last_index( simd_type< ValueType_T, Tag_T > lhs,
                         simd_type< ValueType_T, Tag_T > rhs )
{
    return bitmask_last_index< ValueType_T >(
                greater_bitmask< ValueType_T, Tag_T >( lhs, rhs )
            );
}

DEFINE_BINARY_FUNCTION_ADAPTORS( greater_last_index, index_type )

// Greater than low index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline int
greater_first_index( simd_type< ValueType_T, Tag_T > lhs,
                         simd_type< ValueType_T, Tag_T > rhs )
{
    return bitmask_first_index< ValueType_T >(
                greater_bitmask< ValueType_T, Tag_T >( lhs, rhs )
            );
}

DEFINE_BINARY_FUNCTION_ADAPTORS( greater_first_index, index_type )

// Equals
// ---------------------------------------------------------------------------------------
DEFINE_BINARY_FUNCTION_ADAPTORS( equal_to, type )

// Equals bitmask
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename simd_type< ValueType_T, Tag_T >::bitmask_type
equal_to_bitmask( simd_type< ValueType_T, Tag_T > lhs,
                      simd_type< ValueType_T, Tag_T > rhs )
{
    return mask_to_bitmask< ValueType_T, Tag_T >(
                equal_to< ValueType_T, Tag_T >( lhs, rhs )
            );
}

DEFINE_BINARY_FUNCTION_ADAPTORS( equal_to_bitmask, bitmask_type )

// Equals high index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline int
equal_to_last_index( simd_type< ValueType_T, Tag_T > lhs,
                         simd_type< ValueType_T, Tag_T > rhs )
{
    return bitmask_last_index< ValueType_T >(
                equal_to_bitmask< ValueType_T, Tag_T >( lhs, rhs )
            );
}

DEFINE_BINARY_FUNCTION_ADAPTORS( equal_to_last_index, index_type )

// Equals low index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline int
equal_to_first_index( simd_type< ValueType_T, Tag_T > lhs,
                    simd_type< ValueType_T, Tag_T > rhs )
{
    return bitmask_first_index< ValueType_T >(
                equal_to_bitmask< ValueType_T, Tag_T >( lhs, rhs )
            );
}

DEFINE_BINARY_FUNCTION_ADAPTORS( equal_to_first_index, index_type )

} // namespace litesimd

#endif // LITESIMD_COMPARE_H
