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

#ifndef LITESIMD_ALGORITHM_DETAIL_FOREACHLOOP_H
#define LITESIMD_ALGORITHM_DETAIL_FOREACHLOOP_H

namespace litesimd {
namespace detail {

template< bool forward, int index, typename SimdType_T, typename Function_T >
struct for_each_loop
{
    using type = typename SimdType_T::simd_value_type;
    using tag = typename SimdType_T::simd_tag;
    bool operator()( SimdType_T vec, Function_T func )
    {
        if( for_each_loop< true, index-1, SimdType_T, Function_T >()( vec, func ) )
        {
            return func( index, get< index, type, tag >( vec ) );
        }
        return false;
    }
};

template< int index, typename SimdType_T, typename Function_T >
struct for_each_loop< false, index, SimdType_T, Function_T >
{
    using type = typename SimdType_T::simd_value_type;
    using tag = typename SimdType_T::simd_tag;
    bool operator()( SimdType_T vec, Function_T func )
    {
        if( func( index, get< index, type, tag >( vec ) ) )
        {
            return for_each_loop< false, index-1, SimdType_T, Function_T >()( vec, func );
        }
        return false;
    }
};

template< typename SimdType_T, typename Function_T >
struct for_each_loop< true, 0, SimdType_T, Function_T >
{
    using type = typename SimdType_T::simd_value_type;
    using tag = typename SimdType_T::simd_tag;
    bool operator()( SimdType_T vec, Function_T func )
    {
        return func( 0, get< 0, type, tag >( vec ) );
    }
};

template< typename SimdType_T, typename Function_T >
struct for_each_loop< false, 0, SimdType_T, Function_T >
{
    using type = typename SimdType_T::simd_value_type;
    using tag = typename SimdType_T::simd_tag;
    bool operator()( SimdType_T vec, Function_T func )
    {
        return func( 0, get< 0, type, tag >( vec ) );
    }
};

}} // namespace litesimd::detail

#endif // LITESIMD_ALGORITHM_DETAIL_FOREACHLOOP_H
