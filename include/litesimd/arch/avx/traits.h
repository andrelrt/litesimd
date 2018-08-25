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

#ifndef LITESIMD_ARCH_AVX_TRAITS_H
#define LITESIMD_ARCH_AVX_TRAITS_H

#ifdef LITESIMD_HAS_AVX

#include <immintrin.h>
#include "../common/traits.h"

namespace litesimd {

template<> struct traits<  int8_t, avx_tag >{
    using simd_type = __m256i;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm256_setzero_si256(); }
    static inline simd_type ones() { return _mm256_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int8_t v ) { return _mm256_set1_epi8( v ); }
};

template<> struct traits<  int16_t, avx_tag >{
    using simd_type = __m256i;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm256_setzero_si256(); }
    static inline simd_type ones() { return _mm256_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int16_t v ) { return _mm256_set1_epi16( v ); }
};

template<> struct traits<  int32_t, avx_tag >{
    using simd_type = __m256i;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm256_setzero_si256(); }
    static inline simd_type ones() { return _mm256_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int32_t v ) { return _mm256_set1_epi32( v ); }
};

template<> struct traits<  int64_t, avx_tag >{
    using simd_type = __m256i;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm256_setzero_si256(); }
    static inline simd_type ones() { return _mm256_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int64_t v ) { return _mm256_set1_epi64x( v ); }
};

template<> struct traits<   float, avx_tag >{
    using simd_type = __m256;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm256_setzero_ps(); }
    static inline simd_type ones() { return _mm256_cmp_ps( zero(), zero(), _CMP_GT_OQ ); }
    static inline simd_type from_value( float v ) { return _mm256_set1_ps( v ); }
};

template<> struct traits<  double, avx_tag >{
    using simd_type = __m256d;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm256_setzero_pd(); }
    static inline simd_type ones() { return _mm256_cmp_pd( zero(), zero(), _CMP_GT_OQ ); }
    static inline simd_type from_value( double v ) { return _mm256_set1_pd( v ); }
};

} // namespace litesimd

#endif // LITESIMD_HAS_AVX
#endif // LITESIMD_ARCH_AVX_TRAITS_H
