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

#include "../compare.h"
#include "../shuffle.h"

namespace litesimd {

namespace {

template< int index, typename SimdType_T, typename Function_T >
struct litesimd_internal_for_each_loop
{
    bool operator()( bool forward, SimdType_T vec, Function_T func )
    {
        if( forward )
        {
            if( litesimd_internal_for_each_loop< index-1, SimdType_T >()( true, vec, func ) )
            {
                return func( index, get< index >( vec ) );
            }
        }
        else
        {
            if( func( index, get< index >( vec ) ) )
            {
                return litesimd_internal_for_each_loop< index-1, SimdType_T >()( false, vec, func );
            }
        }
        return false;
    }
};

template< typename SimdType_T, typename Function_T >
struct litesimd_internal_for_each_loop< 0, SimdType_T, Function_T >
{
    bool operator()( bool, SimdType_T vec, Function_T func )
    {
        return func( 0, get< 0 >( vec ) );
    }
};

} // empty namespace


template< typename SimdType_T, typename Function_T,
          typename SimdType_T::simd_value_type* = nullptr >
Function_T for_each( SimdType_T vec, Function_T func )
{
    using st = SimdType_T;
    litesimd_internal_for_each_loop< st::simd_size, st >()( true, vec, func );
    return std::move( func );
}

template< typename SimdType_T, typename Function_T,
          typename SimdType_T::simd_value_type* = nullptr >
Function_T for_each_backward( SimdType_T vec, Function_T func )
{
    using st = SimdType_T;
    litesimd_internal_for_each_loop< st::simd_size, st >()( false, vec, func );
    return std::move( func );
}

template< typename ValueType_T, typename Function_T >
void for_each_index( size_t bitmask, Function_T func )
{
    while( bitmask != 0 )
    {
        size_t idx = bitmask_first_index< ValueType_T >( bitmask );
        if( !func( idx ) )
            break;
        bitmask &= ~(size_t(1) << (idx-1));
    }
}

template< typename ValueType_T, typename Function_T >
void for_each_index_backward( size_t bitmask, Function_T func )
{
    while( bitmask != 0 )
    {
        size_t idx = bitmask_last_index< ValueType_T >( bitmask );
        if( !func( idx ) )
            break;
        bitmask &= ~(size_t(1) << (idx-1));
    }
}

} // namespace litesimd

#endif // LITESIMD_ALGORITHM_FOREACH_H
