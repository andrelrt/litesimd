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

#include <type_traits>
#include "types.h"
#include "arch/compare.h"

namespace litesimd {

// Bitmask to index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T,
          std::enable_if_t< std::is_integral< ValueType_T >::value >* = nullptr >
inline size_t
bitmask_high_index( uint32_t bmask )
{
    return (bmask == 0)
        ? 0
        : (_bit_scan_reverse( bmask ) + 1) / sizeof(ValueType_T);
}

template< typename ValueType_T,
          std::enable_if_t< std::is_integral< ValueType_T >::value >* = nullptr >
inline size_t
bitmask_low_index( uint32_t bmask )
{
    return (bmask == 0)
        ? 0
        : (_bit_scan_forward( bmask ) + sizeof(ValueType_T)) / sizeof(ValueType_T);
}

template< typename ValueType_T,
          std::enable_if_t< std::is_floating_point< ValueType_T >::value >* = nullptr >
inline size_t
bitmask_high_index( uint32_t bmask )
{
    return (bmask == 0)
        ? 0
        : _bit_scan_reverse( bmask ) + 1;
}

template< typename ValueType_T,
          std::enable_if_t< std::is_floating_point< ValueType_T >::value >* = nullptr >
inline size_t
bitmask_low_index( uint32_t bmask )
{
    return (bmask == 0)
        ? 0
        : _bit_scan_forward( bmask ) + 1;
}

// Greater than
// ---------------------------------------------------------------------------------------
template< typename SimdType_T, typename Tag_T = default_tag,
          typename SimdType_T::simd_value_type* = nullptr >
inline typename traits< typename SimdType_T::simd_value_type, Tag_T >::mask_type
greater_than( SimdType_T lhs, SimdType_T rhs )
{
    return greater_than< typename SimdType_T::simd_value_type, Tag_T >( lhs, rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag >
inline typename traits< ValueType_T, Tag_T >::mask_type
greater_than( ValueType_T lhs,
              typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return greater_than< ValueType_T, Tag_T >(
                  from_value< ValueType_T, Tag_T >( lhs ),
                  rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag >
inline typename traits< ValueType_T, Tag_T >::mask_type
greater_than( typename traits< ValueType_T, Tag_T >::simd_type lhs,
              ValueType_T rhs )
{
    return greater_than< ValueType_T, Tag_T >(
                  lhs,
                  from_value< ValueType_T, Tag_T >( rhs ) );
}

// Greater than bitmask
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
greater_than_bitmask( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                      typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return mask_to_bitmask< ValueType_T, Tag_T >(
                greater_than< ValueType_T, Tag_T >( lhs, rhs )
            );
}

template< typename SimdType_T, typename Tag_T = default_tag,
          typename SimdType_T::simd_value_type* = nullptr >
inline typename traits< typename SimdType_T::simd_value_type, Tag_T >::bitmask_type
greater_than_bitmask( SimdType_T lhs, SimdType_T rhs )
{
    return greater_than_bitmask< typename SimdType_T::simd_value_type, Tag_T >( lhs, rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          std::enable_if_t< std::is_arithmetic< ValueType_T >::value >* = nullptr >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
greater_than_bitmask( ValueType_T lhs,
                      typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return greater_than_bitmask< ValueType_T, Tag_T >(
                from_value< ValueType_T, Tag_T >( lhs ),
                rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          std::enable_if_t< std::is_arithmetic< ValueType_T >::value >* = nullptr >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
greater_than_bitmask( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                      ValueType_T rhs )
{
    return greater_than_bitmask< ValueType_T, Tag_T >(
                lhs,
                from_value< ValueType_T, Tag_T >( rhs ) );
}

// Greater than high index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
greater_than_high_index( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                         typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return bitmask_high_index< ValueType_T >(
                greater_than_bitmask< ValueType_T, Tag_T >( lhs, rhs )
            );
}

template< typename SimdType_T, typename Tag_T = default_tag,
          typename SimdType_T::simd_value_type* = nullptr >
inline typename traits< typename SimdType_T::simd_value_type, Tag_T >::bitmask_type
greater_than_high_index( SimdType_T lhs, SimdType_T rhs )
{
    return greater_than_high_index< typename SimdType_T::simd_value_type, Tag_T >( lhs, rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          std::enable_if_t< std::is_arithmetic< ValueType_T >::value >* = nullptr >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
greater_than_high_index( ValueType_T lhs,
                      typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return greater_than_high_index< ValueType_T, Tag_T >(
                from_value< ValueType_T, Tag_T >( lhs ),
                rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          std::enable_if_t< std::is_arithmetic< ValueType_T >::value >* = nullptr >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
greater_than_high_index( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                      ValueType_T rhs )
{
    return greater_than_high_index< ValueType_T, Tag_T >(
                lhs,
                from_value< ValueType_T, Tag_T >( rhs ) );
}

// Greater than low index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
greater_than_low_index( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                         typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return bitmask_low_index< ValueType_T >(
                greater_than_bitmask< ValueType_T, Tag_T >( lhs, rhs )
            );
}

template< typename SimdType_T, typename Tag_T = default_tag,
          typename SimdType_T::simd_value_type* = nullptr >
inline typename traits< typename SimdType_T::simd_value_type, Tag_T >::bitmask_type
greater_than_low_index( SimdType_T lhs, SimdType_T rhs )
{
    return greater_than_low_index< typename SimdType_T::simd_value_type, Tag_T >( lhs, rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          std::enable_if_t< std::is_arithmetic< ValueType_T >::value >* = nullptr >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
greater_than_low_index( ValueType_T lhs,
                      typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return greater_than_low_index< ValueType_T, Tag_T >(
                from_value< ValueType_T, Tag_T >( lhs ),
                rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          std::enable_if_t< std::is_arithmetic< ValueType_T >::value >* = nullptr >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
greater_than_low_index( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                      ValueType_T rhs )
{
    return greater_than_low_index< ValueType_T, Tag_T >(
                lhs,
                from_value< ValueType_T, Tag_T >( rhs ) );
}

// Equals
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename traits< ValueType_T, Tag_T >::mask_type
equals( ValueType_T lhs,
        typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return equals< ValueType_T, Tag_T >(
           traits< ValueType_T, Tag_T >::from_value( lhs ),
           rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag >
inline typename traits< ValueType_T, Tag_T >::mask_type
equals( typename traits< ValueType_T, Tag_T >::simd_type lhs,
        ValueType_T rhs )
{
    return equals< ValueType_T, Tag_T >(
                  lhs,
                  traits< ValueType_T, Tag_T >::from_value( rhs ) );
}

// Equals bitmask
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
equals_bitmask( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                      typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return mask_to_bitmask< ValueType_T, Tag_T >(
                equals< ValueType_T, Tag_T >( lhs, rhs )
            );
}

template< typename SimdType_T, typename Tag_T = default_tag,
          typename SimdType_T::simd_value_type* = nullptr >
inline typename traits< typename SimdType_T::simd_value_type, Tag_T >::bitmask_type
equals_bitmask( SimdType_T lhs, SimdType_T rhs )
{
    return equals_bitmask< typename SimdType_T::simd_value_type, Tag_T >( lhs, rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          std::enable_if_t< std::is_arithmetic< ValueType_T >::value >* = nullptr >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
equals_bitmask( ValueType_T lhs,
                      typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return equals_bitmask< ValueType_T, Tag_T >(
                from_value< ValueType_T, Tag_T >( lhs ),
                rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          std::enable_if_t< std::is_arithmetic< ValueType_T >::value >* = nullptr >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
equals_bitmask( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                      ValueType_T rhs )
{
    return equals_bitmask< ValueType_T, Tag_T >(
                lhs,
                from_value< ValueType_T, Tag_T >( rhs ) );
}

// Equals high index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
equals_high_index( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                         typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return bitmask_high_index< ValueType_T >(
                equals_bitmask< ValueType_T, Tag_T >( lhs, rhs )
            );
}

template< typename SimdType_T, typename Tag_T = default_tag,
          typename SimdType_T::simd_value_type* = nullptr >
inline typename traits< typename SimdType_T::simd_value_type, Tag_T >::bitmask_type
equals_high_index( SimdType_T lhs, SimdType_T rhs )
{
    return equals_high_index< typename SimdType_T::simd_value_type, Tag_T >( lhs, rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          std::enable_if_t< std::is_arithmetic< ValueType_T >::value >* = nullptr >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
equals_high_index( ValueType_T lhs,
                      typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return equals_high_index< ValueType_T, Tag_T >(
                from_value< ValueType_T, Tag_T >( lhs ),
                rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          std::enable_if_t< std::is_arithmetic< ValueType_T >::value >* = nullptr >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
equals_high_index( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                      ValueType_T rhs )
{
    return equals_high_index< ValueType_T, Tag_T >(
                lhs,
                from_value< ValueType_T, Tag_T >( rhs ) );
}

// Equals low index
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
equals_low_index( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                         typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return bitmask_low_index< ValueType_T >(
                equals_bitmask< ValueType_T, Tag_T >( lhs, rhs )
            );
}

template< typename SimdType_T, typename Tag_T = default_tag,
          typename SimdType_T::simd_value_type* = nullptr >
inline typename traits< typename SimdType_T::simd_value_type, Tag_T >::bitmask_type
equals_low_index( SimdType_T lhs, SimdType_T rhs )
{
    return equals_low_index< typename SimdType_T::simd_value_type, Tag_T >( lhs, rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          std::enable_if_t< std::is_arithmetic< ValueType_T >::value >* = nullptr >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
equals_low_index( ValueType_T lhs,
                      typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return equals_low_index< ValueType_T, Tag_T >(
                from_value< ValueType_T, Tag_T >( lhs ),
                rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          std::enable_if_t< std::is_arithmetic< ValueType_T >::value >* = nullptr >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
equals_low_index( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                      ValueType_T rhs )
{
    return equals_low_index< ValueType_T, Tag_T >(
                lhs,
                from_value< ValueType_T, Tag_T >( rhs ) );
}

// Blend ternary
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
typename traits< ValueType_T, Tag_T >::simd_type
blend( typename traits< ValueType_T, Tag_T >::mask_type mask,
       ValueType_T trueVal,
       typename traits< ValueType_T, Tag_T >::simd_type falseVal )
{
    return blend< ValueType_T, Tag_T >( mask,
                                      from_value< ValueType_T, Tag_T >( trueVal ),
                                      falseVal );
}

template< typename ValueType_T, typename Tag_T = default_tag >
typename traits< ValueType_T, Tag_T >::simd_type
blend( typename traits< ValueType_T, Tag_T >::mask_type mask,
       typename traits< ValueType_T, Tag_T >::simd_type trueVal,
       ValueType_T falseVal )
{
    return blend< ValueType_T, Tag_T >( mask,
                                      trueVal,
                                      from_value< ValueType_T, Tag_T >( falseVal ) );
}

} // namespace litesimd

#endif // LITESIMD_COMPARE_H
