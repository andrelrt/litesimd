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

#ifndef LITESIMD_ALGORITHM_FOREACH_H
#define LITESIMD_ALGORITHM_FOREACH_H

#include <utility>
#include <type_traits>
#include "../compare.h"
#include "../shuffle.h"
#include "detail/for_each_loop.h"

namespace litesimd {

template< typename SimdType_T, typename Function_T,
          typename SimdType_T::simd_value_type* = nullptr >
inline Function_T for_each( SimdType_T vec, Function_T func )
{
    using st = SimdType_T;
    detail::for_each_loop< st::simd_size-1, st, Function_T >()( true, vec, func );
    return std::move( func );
}

template< typename SimdType_T, typename Function_T,
          typename SimdType_T::simd_value_type* = nullptr >
inline Function_T for_each_backward( SimdType_T vec, Function_T func )
{
    using st = SimdType_T;
    detail::for_each_loop< st::simd_size-1, st, Function_T >()( false, vec, func );
    return std::move( func );
}

template< typename ValueType_T, typename Function_T,
          typename std::enable_if_t<std::is_integral<ValueType_T>::value>* = nullptr >
inline Function_T for_each_index( uint32_t bitmask, Function_T func )
{
    constexpr uint32_t mask = (1 << sizeof(ValueType_T)) -1;
    while( bitmask != 0 )
    {
        int idx = bitmask_first_index< ValueType_T >( bitmask );
        if( !func( idx ) )
            break;
        bitmask &= ~(mask << (idx*sizeof(ValueType_T)));
    }
    return std::move( func );
}

template< typename ValueType_T, typename Function_T,
          typename std::enable_if_t<std::is_floating_point<ValueType_T>::value>* = nullptr >
inline Function_T for_each_index( uint32_t bitmask, Function_T func )
{
    while( bitmask != 0 )
    {
        int idx = bitmask_first_index< ValueType_T >( bitmask );
        if( !func( idx ) )
            break;
        bitmask &= ~(1 << idx);
    }
    return std::move( func );
}

template< typename ValueType_T, typename Function_T,
          typename std::enable_if_t<std::is_integral<ValueType_T>::value>* = nullptr >
inline Function_T for_each_index_backward( uint32_t bitmask, Function_T func )
{
    constexpr uint32_t mask = (1 << sizeof(ValueType_T)) -1;
    while( bitmask != 0 )
    {
        int idx = bitmask_last_index< ValueType_T >( bitmask );
        if( !func( idx ) )
            break;
        bitmask &= ~(mask << (idx*sizeof(ValueType_T)));
    }
    return std::move( func );
}

template< typename ValueType_T, typename Function_T,
          typename std::enable_if_t<std::is_floating_point<ValueType_T>::value>* = nullptr >
inline Function_T for_each_index_backward( uint32_t bitmask, Function_T func )
{
    while( bitmask != 0 )
    {
        int idx = bitmask_last_index< ValueType_T >( bitmask );
        if( !func( idx ) )
            break;
        bitmask &= ~(1 << idx);
    }
    return std::move( func );
}

} // namespace litesimd

#endif // LITESIMD_ALGORITHM_FOREACH_H
