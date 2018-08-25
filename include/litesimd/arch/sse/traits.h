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

#ifndef LITESIMD_ARCH_SSE_TRAITS_H
#define LITESIMD_ARCH_SSE_TRAITS_H

#ifdef LITESIMD_HAS_SSE

#include <smmintrin.h>
#include "../common/traits.h"

namespace litesimd {

template<> struct traits<  int8_t, sse_tag >{
    using simd_type = __m128i;
    using bitmask_type = uint16_t;
    static inline simd_type zero() { return _mm_setzero_si128(); }
    static inline simd_type ones() { return _mm_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int8_t v ) { return _mm_set1_epi8( v ); }
};

template<> struct traits<  int16_t, sse_tag >{
    using simd_type = __m128i;
    using bitmask_type = uint16_t;
    static inline simd_type zero() { return _mm_setzero_si128(); }
    static inline simd_type ones() { return _mm_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int16_t v ) { return _mm_set1_epi16( v ); }
};

template<> struct traits<  int32_t, sse_tag >{
    using simd_type = __m128i;
    using bitmask_type = uint16_t;
    static inline simd_type zero() { return _mm_setzero_si128(); }
    static inline simd_type ones() { return _mm_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int32_t v ) { return _mm_set1_epi32( v ); }
};

template<> struct traits<  int64_t, sse_tag >{
    using simd_type = __m128i;
    using bitmask_type = uint16_t;
    static inline simd_type zero() { return _mm_setzero_si128(); }
    static inline simd_type ones() { return _mm_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int64_t v ) { return _mm_set1_epi64x( v ); }
};

template<> struct traits<   float, sse_tag >{
    using simd_type = __m128;
    using bitmask_type = uint16_t;
    static inline simd_type zero() { return _mm_setzero_ps(); }
    static inline simd_type ones() { return _mm_cmpeq_ps( zero(), zero() ); }
    static inline simd_type from_value( float v ) { return _mm_set1_ps( v ); }
};

template<> struct traits<  double, sse_tag >{
    using simd_type = __m128d;
    using bitmask_type = uint16_t;
    static inline simd_type zero() { return _mm_setzero_pd(); }
    static inline simd_type ones() { return _mm_cmpeq_pd( zero(), zero() ); }
    static inline simd_type from_value( double v ) { return _mm_set1_pd( v ); }
};

} // namespace litesimd

#endif // LITESIMD_HAS_SSE
#endif // LITESIMD_ARCH_SSE_TRAITS_H
