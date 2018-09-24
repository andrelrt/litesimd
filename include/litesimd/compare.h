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
 * - **mask**: Is the result of comparision intrincs. Is have the same SIMD type of
 *   operands and hold the result as all bits 0 for `false` result and all bits 1 for
 *   `true` result. Some SIMD intrics can use this mask as parameter to execute the
 *   operation based on `true` and `false` values. @see blend
 * - **bitmask**:
 */

/**
 * \ingroup compare
 * \brief Find the last index on a bitmask
 *
 *
 *
 * \param vec SIMD register to apply `AND` operator
 * \returns Result of `AND` operation among all values
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/bitwise.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 0x71, 0x3c, 0x1e, 0x0f );
 *     std::cout << "bit_and( a ): " << ls::bit_and( a ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * bit_and( a ): 8
 * ```
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
