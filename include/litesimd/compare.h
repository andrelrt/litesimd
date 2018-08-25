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
#include <intrin.h>
#include <type_traits>
#include "types.h"
#include "arch/compare.h"
#include "detail/helper_macros.h"

namespace litesimd {

// Bitmask to index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T,
          std::enable_if_t< std::is_integral< ValueType_T >::value >* = nullptr >
inline size_t
bitmask_last_index( uint32_t bmask )
{
    unsigned long index;
    return (0 == _BitScanReverse( &index, bmask ))
        ? 0
        : (index + 1) / sizeof(ValueType_T);
//    return (bmask == 0)
//        ? 0
//        : (_bit_scan_reverse( bmask ) + 1) / sizeof(ValueType_T);
}

template< typename ValueType_T,
          std::enable_if_t< std::is_integral< ValueType_T >::value >* = nullptr >
inline size_t
bitmask_first_index( uint32_t bmask )
{
    unsigned long index;
    return (0 == _BitScanForward( &index, bmask ))
        ? 0
        : index / sizeof(ValueType_T) + 1;
//    return (bmask == 0)
//        ? 0
//        : (_bit_scan_forward( bmask ) + sizeof(ValueType_T)) / sizeof(ValueType_T);
}

template< typename ValueType_T,
          std::enable_if_t< std::is_floating_point< ValueType_T >::value >* = nullptr >
inline size_t
bitmask_last_index( uint32_t bmask )
{
    unsigned long index;
    return (0 == _BitScanReverse( &index, bmask ))
        ? 0
        : index + 1;
//    return (bmask == 0)
//        ? 0
//        : _bit_scan_reverse( bmask ) + 1;
}

template< typename ValueType_T,
          std::enable_if_t< std::is_floating_point< ValueType_T >::value >* = nullptr >
inline size_t
bitmask_first_index( uint32_t bmask )
{
    unsigned long index;
    return (0 == _BitScanForward( &index, bmask ))
        ? 0
        : index + 1;
//    return (bmask == 0)
//        ? 0
//        : _bit_scan_forward( bmask ) + 1;
}

// Greater than
// ---------------------------------------------------------------------------------------
DEFINE_BINARY_FUNCTION_ADAPTORS( greater_than, type )

// Greater than bitmask
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename simd_type< ValueType_T, Tag_T >::bitmask_type
greater_than_bitmask( simd_type< ValueType_T, Tag_T > lhs,
                      simd_type< ValueType_T, Tag_T > rhs )
{
    return mask_to_bitmask< ValueType_T, Tag_T >(
                greater_than< ValueType_T, Tag_T >( lhs, rhs )
            );
}

DEFINE_BINARY_FUNCTION_ADAPTORS( greater_than_bitmask, bitmask_type )

// Greater than high index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename simd_type< ValueType_T, Tag_T >::bitmask_type
greater_than_last_index( typename simd_type< ValueType_T, Tag_T >::simd_type lhs,
                         typename simd_type< ValueType_T, Tag_T >::simd_type rhs )
{
    return bitmask_last_index< ValueType_T >(
                greater_than_bitmask< ValueType_T, Tag_T >( lhs, rhs )
            );
}

DEFINE_BINARY_FUNCTION_ADAPTORS( greater_than_last_index, bitmask_type )

// Greater than low index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename simd_type< ValueType_T, Tag_T >::bitmask_type
greater_than_first_index( typename simd_type< ValueType_T, Tag_T >::simd_type lhs,
                         typename simd_type< ValueType_T, Tag_T >::simd_type rhs )
{
    return bitmask_first_index< ValueType_T >(
                greater_than_bitmask< ValueType_T, Tag_T >( lhs, rhs )
            );
}

DEFINE_BINARY_FUNCTION_ADAPTORS( greater_than_first_index, bitmask_type )

// Equals
// ---------------------------------------------------------------------------------------
DEFINE_BINARY_FUNCTION_ADAPTORS( equals, type )

// Equals bitmask
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename simd_type< ValueType_T, Tag_T >::bitmask_type
equals_bitmask( simd_type< ValueType_T, Tag_T > lhs,
                      simd_type< ValueType_T, Tag_T > rhs )
{
    return mask_to_bitmask< ValueType_T, Tag_T >(
                equals< ValueType_T, Tag_T >( lhs, rhs )
            );
}

DEFINE_BINARY_FUNCTION_ADAPTORS( equals_bitmask, bitmask_type )

// Equals high index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename simd_type< ValueType_T, Tag_T >::bitmask_type
equals_last_index( typename simd_type< ValueType_T, Tag_T >::simd_type lhs,
                         typename simd_type< ValueType_T, Tag_T >::simd_type rhs )
{
    return bitmask_last_index< ValueType_T >(
                equals_bitmask< ValueType_T, Tag_T >( lhs, rhs )
            );
}

DEFINE_BINARY_FUNCTION_ADAPTORS( equals_last_index, bitmask_type )

// Equals low index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename simd_type< ValueType_T, Tag_T >::bitmask_type
equals_first_index( typename simd_type< ValueType_T, Tag_T >::simd_type lhs,
                         typename simd_type< ValueType_T, Tag_T >::simd_type rhs )
{
    return bitmask_first_index< ValueType_T >(
                equals_bitmask< ValueType_T, Tag_T >( lhs, rhs )
            );
}

DEFINE_BINARY_FUNCTION_ADAPTORS( equals_first_index, bitmask_type )

// Blend ternary
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
simd_type< ValueType_T, Tag_T >
blend( simd_type< ValueType_T, Tag_T > mask,
       ValueType_T trueVal,
       simd_type< ValueType_T, Tag_T > falseVal )
{
    return blend< ValueType_T, Tag_T >( mask,
                                        simd_type< ValueType_T, Tag_T >( trueVal ),
                                        falseVal );
}

template< typename ValueType_T, typename Tag_T = default_tag >
simd_type< ValueType_T, Tag_T >
blend( simd_type< ValueType_T, Tag_T > mask,
       simd_type< ValueType_T, Tag_T > trueVal,
       ValueType_T falseVal )
{
    return blend< ValueType_T, Tag_T >( mask,
                                        trueVal,
                                        simd_type< ValueType_T, Tag_T >( falseVal ) );
}

} // namespace litesimd

#endif // LITESIMD_COMPARE_H
