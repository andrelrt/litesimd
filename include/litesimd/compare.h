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
#include "arch/sse/compare.h"
#include "arch/avx/compare.h"

namespace litesimd {

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
          class = typename SimdType_T::simd_value_type >
inline typename traits< typename SimdType_T::simd_value_type, Tag_T >::bitmask_type
greater_than_bitmask( SimdType_T lhs, SimdType_T rhs )
{
    return greater_than_bitmask< typename SimdType_T::simd_value_type, Tag_T >( lhs, rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          class = std::enable_if_t< std::is_arithmetic< ValueType_T >::value > >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
greater_than_bitmask( ValueType_T lhs,
                      typename traits< ValueType_T, Tag_T >::simd_type rhs )
{
    return greater_than_bitmask< ValueType_T, Tag_T >(
                from_value< ValueType_T, Tag_T >( lhs ),
                rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag,
          class = std::enable_if_t< std::is_arithmetic< ValueType_T >::value > >
inline typename traits< ValueType_T, Tag_T >::bitmask_type
greater_than_bitmask( typename traits< ValueType_T, Tag_T >::simd_type lhs,
                      ValueType_T rhs )
{
    return greater_than_bitmask< ValueType_T, Tag_T >(
                lhs,
                from_value< ValueType_T, Tag_T >( rhs ) );
}



} // namespace litesimd

#endif // LITESIMD_COMPARE_H
