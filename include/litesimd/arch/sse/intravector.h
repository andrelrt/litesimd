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

#ifndef LITESIMD_ARCH_SSE_INTRAVECTOR_H
#define LITESIMD_ARCH_SSE_INTRAVECTOR_H

#include "../common/intravector.h"

namespace litesimd {

template<>
struct intravector_op< int8_t, sse_tag >
{
    template< typename Function_T >
    int8_t inline operator()( simd_type< int8_t, sse_tag > vec, Function_T func )
    {
        vec = func( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
        vec = func( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        vec = func( vec, _mm_shufflelo_epi16( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        vec = func( vec, _mm_srli_epi16( vec, 8 ) );
        return (int8_t)_mm_cvtsi128_si32( vec );
    }
};

template<>
struct intravector_op< int16_t, sse_tag >
{
    template< typename Function_T >
    int16_t inline operator()( simd_type< int16_t, sse_tag > vec, Function_T func )
    {
        vec = func( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
        vec = func( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        vec = func( vec, _mm_shufflelo_epi16( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        return (int16_t)_mm_cvtsi128_si32( vec );
    }
};

template<>
struct intravector_op< int32_t, sse_tag >
{
    template< typename Function_T >
    int32_t inline operator()( simd_type< int32_t, sse_tag > vec, Function_T func )
    {
        vec = func( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
        vec = func( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        return _mm_cvtsi128_si32( vec );
    }
};

template<>
struct intravector_op< int64_t, sse_tag >
{
    template< typename Function_T >
    int64_t inline operator()( simd_type< int64_t, sse_tag > vec, Function_T func )
    {
        vec = func( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
        return _mm_cvtsi128_si64( vec );
    }
};

template<>
struct intravector_op< float, sse_tag >
{
    template< typename Function_T >
    float inline operator()( simd_type< float, sse_tag > vec, Function_T func )
    {
        vec = func( vec, _mm_shuffle_ps( vec, vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
        vec = func( vec, _mm_shuffle_ps( vec, vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        return _mm_cvtss_f32( vec );
    }
};

template<>
struct intravector_op< double, sse_tag >
{
    template< typename Function_T >
    double inline operator()( simd_type< double, sse_tag > vec, Function_T func )
    {
        vec = func( vec, _mm_shuffle_pd( vec, vec, _MM_SHUFFLE2( 0, 1 ) ) );
        return _mm_cvtsd_f64( vec );
    }
};

} // namespace litesimd

#endif // LITESIMD_ARCH_SSE_INTRAVECTOR_H
