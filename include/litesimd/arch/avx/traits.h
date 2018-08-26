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
    static inline simd_type from_values( int8_t v31, int8_t v30, int8_t v29, int8_t v28,
                                         int8_t v27, int8_t v26, int8_t v25, int8_t v24,
                                         int8_t v23, int8_t v22, int8_t v21, int8_t v20,
                                         int8_t v19, int8_t v18, int8_t v17, int8_t v16,
                                         int8_t v15, int8_t v14, int8_t v13, int8_t v12,
                                         int8_t v11, int8_t v10, int8_t  v9, int8_t  v8,
                                         int8_t  v7, int8_t  v6, int8_t  v5, int8_t  v4,
                                         int8_t  v3, int8_t  v2, int8_t  v1, int8_t  v0 )
    {
        return _mm256_set_epi8( v31, v30, v29, v28, v27, v26, v25, v24,
                                v23, v22, v21, v20, v19, v18, v17, v16,
                                v15, v14, v13, v12, v11, v10,  v9,  v8,
                                 v7,  v6,  v5,  v4,  v3,  v2,  v1,  v0 );
    }
};

template<> struct traits<  int16_t, avx_tag >{
    using simd_type = __m256i;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm256_setzero_si256(); }
    static inline simd_type ones() { return _mm256_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int16_t v ) { return _mm256_set1_epi16( v ); }
    static inline simd_type from_values( int16_t v15, int16_t v14, int16_t v13, int16_t v12,
                                         int16_t v11, int16_t v10, int16_t  v9, int16_t  v8,
                                         int16_t  v7, int16_t  v6, int16_t  v5, int16_t  v4,
                                         int16_t  v3, int16_t  v2, int16_t  v1, int16_t  v0 )
    {
        return _mm256_set_epi16( v15, v14, v13, v12, v11, v10,  v9,  v8,
                                  v7,  v6,  v5,  v4,  v3,  v2,  v1,  v0 );
    }
};

template<> struct traits<  int32_t, avx_tag >{
    using simd_type = __m256i;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm256_setzero_si256(); }
    static inline simd_type ones() { return _mm256_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int32_t v ) { return _mm256_set1_epi32( v ); }
    static inline simd_type from_values( int32_t v7, int32_t v6, int32_t v5, int32_t v4,
                                         int32_t v3, int32_t v2, int32_t v1, int32_t v0 )
    {
        return _mm256_set_epi32( v7, v6, v5, v4, v3, v2, v1, v0 );
    }
};

template<> struct traits<  int64_t, avx_tag >{
    using simd_type = __m256i;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm256_setzero_si256(); }
    static inline simd_type ones() { return _mm256_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int64_t v ) { return _mm256_set1_epi64x( v ); }
    static inline simd_type from_values( int64_t v3, int64_t v2, int64_t v1, int64_t v0 )
    {
        return _mm256_set_epi64x( v3, v2, v1, v0 );
    }
};

template<> struct traits<   float, avx_tag >{
    using simd_type = __m256;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm256_setzero_ps(); }
    static inline simd_type ones() { return _mm256_cmp_ps( zero(), zero(), _CMP_GT_OQ ); }
    static inline simd_type from_value( float v ) { return _mm256_set1_ps( v ); }
    static inline simd_type from_values( float v7, float v6, float v5, float v4,
                                         float v3, float v2, float v1, float v0 )
    {
        return _mm256_set_ps( v7, v6, v5, v4, v3, v2, v1, v0 );
    }
};

template<> struct traits<  double, avx_tag >{
    using simd_type = __m256d;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm256_setzero_pd(); }
    static inline simd_type ones() { return _mm256_cmp_pd( zero(), zero(), _CMP_GT_OQ ); }
    static inline simd_type from_value( double v ) { return _mm256_set1_pd( v ); }
    static inline simd_type from_values( double v3, double v2, double v1, double v0 )
    {
        return _mm256_set_pd( v3, v2, v1, v0 );
    }
};

} // namespace litesimd

#endif // LITESIMD_HAS_AVX
#endif // LITESIMD_ARCH_AVX_TRAITS_H
