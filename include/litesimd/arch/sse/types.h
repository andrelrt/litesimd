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

#ifndef LITESIMD_SSE_TYPES_H
#define LITESIMD_SSE_TYPES_H

#include <immintrin.h>
#include <type_traits>
#include "../../traits.h"

namespace litesimd {

struct sse_tag {};

template< typename ValueType_T >
struct traits< ValueType_T, sse_tag, 
               std::enable_if_t< std::is_integral< ValueType_T >::value > >
{
    using simd_type = __m128i;
    constexpr static size_t simd_size = sizeof(simd_type)/sizeof(ValueType_T);
};

template<> struct traits< float, sse_tag, void >
{
    using simd_type = __m128;
    constexpr static size_t simd_size = sizeof(simd_type)/sizeof(float);
};

template<> struct traits< double, sse_tag, void >
{
    using simd_type = __m128d;
    constexpr static size_t simd_size = sizeof(simd_type)/sizeof(double);
};

} // namespace litesimd

#endif //LITESIMD_SSE_TYPES_H

