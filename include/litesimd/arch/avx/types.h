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

#ifndef LITESIMD_AVX_TYPES_H
#define LITESIMD_AVX_TYPES_H

#ifdef LITESIMD_HAS_AVX

#include <immintrin.h>
#include <type_traits>
#include "../common/types.h"

namespace litesimd {

template<> struct traits<  int8_t, avx_tag >{ using simd_type = __m256i; static inline simd_type zero() { return _mm256_setzero_si256(); } };
template<> struct traits< int16_t, avx_tag >{ using simd_type = __m256i; static inline simd_type zero() { return _mm256_setzero_si256(); } };
template<> struct traits< int32_t, avx_tag >{ using simd_type = __m256i; static inline simd_type zero() { return _mm256_setzero_si256(); } };
template<> struct traits< int64_t, avx_tag >{ using simd_type = __m256i; static inline simd_type zero() { return _mm256_setzero_si256(); } };
template<> struct traits<   float, avx_tag >{ using simd_type = __m256;  static inline simd_type zero() { return _mm256_setzero_ps(); } };
template<> struct traits<  double, avx_tag >{ using simd_type = __m256d; static inline simd_type zero() { return _mm256_setzero_pd(); } };

template<> inline simd_type<  int8_t, avx_tag > from_value<  int8_t, avx_tag >(  int8_t val ) { return _mm256_set1_epi8( val ); }
template<> inline simd_type< int16_t, avx_tag > from_value< int16_t, avx_tag >( int16_t val ) { return _mm256_set1_epi16( val ); }
template<> inline simd_type< int32_t, avx_tag > from_value< int32_t, avx_tag >( int32_t val ) { return _mm256_set1_epi32( val ); }
template<> inline simd_type< int64_t, avx_tag > from_value< int64_t, avx_tag >( int64_t val ) { return _mm256_set1_epi64x( val ); }
template<> inline simd_type<   float, avx_tag > from_value<   float, avx_tag >(   float val ) { return _mm256_set1_ps( val ); }
template<> inline simd_type<  double, avx_tag > from_value<  double, avx_tag >(  double val ) { return _mm256_set1_pd( val ); }

} // namespace litesimd

#endif //LITESIMD_HAS_AVX
#endif //LITESIMD_AVX_TYPES_H
