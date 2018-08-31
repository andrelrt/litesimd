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

#ifndef LITESIMD_ARCH_COMMON_SHUFFLE_H
#define LITESIMD_ARCH_COMMON_SHUFFLE_H

#include "../../types.h"

namespace litesimd {

// Low insert
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
simd_type< ValueType_T, Tag_T >
high_insert( simd_type< ValueType_T, Tag_T >, ValueType_T = 0 ){}

// Blend
// ---------------------------------------------------------------------------------------
template< typename ValueType_T, typename Tag_T = default_tag >
simd_type< ValueType_T, Tag_T >
blend( simd_type< ValueType_T, Tag_T >,
       simd_type< ValueType_T, Tag_T >,
       simd_type< ValueType_T, Tag_T > )
{
    return simd_type< ValueType_T, Tag_T >::zero();
}

// Get Set
// ---------------------------------------------------------------------------------------
template< int index, typename ValueType_T, typename Tag_T = default_tag >
struct get_functor
{
    ValueType_T operator()( simd_type< ValueType_T, Tag_T > ){}
};

template< int index, typename ValueType_T, typename Tag_T = default_tag >
struct set_functor
{
    simd_type< ValueType_T, Tag_T > operator()( simd_type< ValueType_T, Tag_T >, ValueType_T ){}
};

} // namespace litesimd

#endif // LITESIMD_ARCH_COMMON_SHUFFLE_H
