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

#ifdef LITESIMD_HAS_SSE

#include <immintrin.h>
#include <type_traits>
#include "../common/types.h"

namespace litesimd {

template<> struct traits<  int8_t, sse_tag >{ using simd_type = __m128i; static inline simd_type zero() { return _mm_setzero_si128(); } };
template<> struct traits< int16_t, sse_tag >{ using simd_type = __m128i; static inline simd_type zero() { return _mm_setzero_si128(); } };
template<> struct traits< int32_t, sse_tag >{ using simd_type = __m128i; static inline simd_type zero() { return _mm_setzero_si128(); } };
template<> struct traits< int64_t, sse_tag >{ using simd_type = __m128i; static inline simd_type zero() { return _mm_setzero_si128(); } };
template<> struct traits<   float, sse_tag >{ using simd_type = __m128;  static inline simd_type zero() { return _mm_setzero_ps(); } };
template<> struct traits<  double, sse_tag >{ using simd_type = __m128d; static inline simd_type zero() { return _mm_setzero_pd(); } };

template<> inline simd_type<  int8_t, sse_tag > from_value<  int8_t, sse_tag >(  int8_t val ) { return _mm_set1_epi8( val ); }
template<> inline simd_type< int16_t, sse_tag > from_value< int16_t, sse_tag >( int16_t val ) { return _mm_set1_epi16( val ); }
template<> inline simd_type< int32_t, sse_tag > from_value< int32_t, sse_tag >( int32_t val ) { return _mm_set1_epi32( val ); }
template<> inline simd_type< int64_t, sse_tag > from_value< int64_t, sse_tag >( int64_t val ) { return _mm_set1_epi64x( val ); }
template<> inline simd_type<   float, sse_tag > from_value<   float, sse_tag >(   float val ) { return _mm_set1_ps( val ); }
template<> inline simd_type<  double, sse_tag > from_value<  double, sse_tag >(  double val ) { return _mm_set1_pd( val ); }

} // namespace litesimd

#endif // LITESIMD_HAS_SSE
#endif // LITESIMD_SSE_TYPES_H
