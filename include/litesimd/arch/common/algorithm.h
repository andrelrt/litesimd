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

#ifndef LITESIMD_ARCH_COMMON_ALGORITHM_H
#define LITESIMD_ARCH_COMMON_ALGORITHM_H

#include "../../types.h"

namespace litesimd {

// Min max
// ---------------------------------------------------------------------------------------
template< typename Value_T, typename Tag_T > simd_type< Value_T, Tag_T >
min( simd_type< Value_T, Tag_T >, simd_type< Value_T, Tag_T > ){}

template< typename Value_T, typename Tag_T > simd_type< Value_T, Tag_T >
max( simd_type< Value_T, Tag_T >, simd_type< Value_T, Tag_T > ){}

// Horizontal min max
// ---------------------------------------------------------------------------------------
//template< typename Value_T, typename Tag_T > Value_T
//min( simd_type< Value_T, Tag_T > ){}
//
//template< typename Value_T, typename Tag_T > Value_T
//max( simd_type< Value_T, Tag_T > ){}

} // namespace litesimd

#endif // LITESIMD_ARCH_COMMON_ALGORITHM_H
