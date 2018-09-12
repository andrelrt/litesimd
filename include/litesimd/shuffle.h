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

// High/Low insert
// ---------------------------------------------------------------------------------------
template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
inline SimdType_T
high_insert( SimdType_T vec, typename SimdType_T::simd_value_type val )
{
    return high_insert< typename SimdType_T::simd_value_type,
                        typename SimdType_T::simd_tag >( vec, val );
}

template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
inline SimdType_T
low_insert( SimdType_T vec, typename SimdType_T::simd_value_type val )
{
    return low_insert< typename SimdType_T::simd_value_type,
                       typename SimdType_T::simd_tag >( vec, val );
}

// Blend
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

// Get Set
// ---------------------------------------------------------------------------------------
template< int index, typename ValueType_T, typename Tag_T = default_tag >
inline ValueType_T
get( simd_type< ValueType_T, Tag_T > vec )
{
    static_assert( 0 <= index && index < simd_type< ValueType_T, Tag_T >::simd_size,
                    "Index out of bounds" );

    return get_functor< index, ValueType_T, Tag_T >()( vec );
}

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
