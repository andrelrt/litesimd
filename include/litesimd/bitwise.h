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

#ifndef LITESIMD_BITWISE_H
#define LITESIMD_BITWISE_H

#include <type_traits>
#include "types.h"
#include "arch/bitwise.h"

namespace litesimd {

// Bit AND
// ---------------------------------------------------------------------------------------
template< typename SimdType_T, typename Tag_T = default_tag,
          typename SimdType_T::simd_value_type* = nullptr >
inline typename traits< typename SimdType_T::simd_value_type, Tag_T >::mask_type
bit_and( SimdType_T lhs, SimdType_T rhs )
{
    return bit_and< typename SimdType_T::simd_value_type, Tag_T >( lhs, rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag >
inline typename traits< ValueType_T, Tag_T >::mask_type
bit_and( ValueType_T lhs, typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return bit_and< ValueType_T, Tag_T >( from_value< ValueType_T, Tag_T >( lhs ), rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag >
inline typename traits< ValueType_T, Tag_T >::mask_type
bit_and( typename traits< ValueType_T, Tag_T >::simd_type lhs, ValueType_T rhs )
{
    return bit_and< ValueType_T, Tag_T >( lhs, from_value< ValueType_T, Tag_T >( rhs ) );
}


} // namespace litesimd

#endif // LITESIMD_BITWISE_H
