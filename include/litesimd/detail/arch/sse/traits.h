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
#include <litesimd/detail/arch/common/traits.h>

namespace litesimd {

template<> struct traits<  int8_t, sse_tag >{
    using simd_type = __m128i;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm_setzero_si128(); }
    static inline simd_type ones() { return _mm_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int8_t v ) { return _mm_set1_epi8( v ); }
    static inline simd_type from_values( int8_t v15, int8_t v14, int8_t v13, int8_t v12,
                                         int8_t v11, int8_t v10, int8_t  v9, int8_t  v8,
                                         int8_t  v7, int8_t  v6, int8_t  v5, int8_t  v4,
                                         int8_t  v3, int8_t  v2, int8_t  v1, int8_t  v0 )
    {
        return _mm_set_epi8( v15, v14, v13, v12, v11, v10,  v9,  v8,
                              v7,  v6,  v5,  v4,  v3,  v2,  v1,  v0 );
    }
    static inline simd_type iota() { return from_values( 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 ); }
    static inline simd_type load_unaligned( const void* ptr )
    {
        return _mm_lddqu_si128( reinterpret_cast<const simd_type*>( ptr ) );
    }
};

template<> struct traits<  int16_t, sse_tag >{
    using simd_type = __m128i;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm_setzero_si128(); }
    static inline simd_type ones() { return _mm_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int16_t v ) { return _mm_set1_epi16( v ); }
    static inline simd_type from_values( int16_t v7, int16_t v6, int16_t v5, int16_t v4,
                                         int16_t v3, int16_t v2, int16_t v1, int16_t v0 )
    {
        return _mm_set_epi16( v7, v6, v5, v4, v3, v2, v1, v0 );
    }
    static inline simd_type iota() { return from_values( 7, 6, 5, 4, 3, 2, 1, 0 ); }
    static inline simd_type load_unaligned( const void* ptr )
    {
        return _mm_lddqu_si128( reinterpret_cast<const simd_type*>( ptr ) );
    }
};

template<> struct traits<  int32_t, sse_tag >{
    using simd_type = __m128i;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm_setzero_si128(); }
    static inline simd_type ones() { return _mm_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int32_t v ) { return _mm_set1_epi32( v ); }
    static inline simd_type from_values( int32_t v3, int32_t v2, int32_t v1, int32_t v0 )
    {
        return _mm_set_epi32( v3, v2, v1, v0 );
    }
    static inline simd_type iota() { return from_values( 3, 2, 1, 0 ); }
    static inline simd_type load_unaligned( const void* ptr )
    {
        return _mm_lddqu_si128( reinterpret_cast<const simd_type*>( ptr ) );
    }
};

template<> struct traits<  int64_t, sse_tag >{
    using simd_type = __m128i;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm_setzero_si128(); }
    static inline simd_type ones() { return _mm_cmpeq_epi8( zero(), zero() ); }
    static inline simd_type from_value( int64_t v ) { return _mm_set1_epi64x( v ); }
    static inline simd_type from_values( int64_t v1, int64_t v0 )
    {
        return _mm_set_epi64x( v1, v0 );
    }
    static inline simd_type iota() { return from_values( 1, 0 ); }
    static inline simd_type load_unaligned( const void* ptr )
    {
        return _mm_lddqu_si128( reinterpret_cast<const simd_type*>( ptr ) );
    }
};

template<> struct traits<   float, sse_tag >{
    using simd_type = __m128;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm_setzero_ps(); }
    static inline simd_type ones() { return _mm_cmpeq_ps( zero(), zero() ); }
    static inline simd_type from_value( float v ) { return _mm_set1_ps( v ); }
    static inline simd_type from_values( float v3, float v2, float v1, float v0 )
    {
        return _mm_set_ps( v3, v2, v1, v0 );
    }
    static inline simd_type iota() { return from_values( 3, 2, 1, 0 ); }
    static inline simd_type load_unaligned( const void* ptr )
    {
        return _mm_load_ps( reinterpret_cast<const float*>( ptr ) );
    }
};

template<> struct traits<  double, sse_tag >{
    using simd_type = __m128d;
    using bitmask_type = uint32_t;
    static inline simd_type zero() { return _mm_setzero_pd(); }
    static inline simd_type ones() { return _mm_cmpeq_pd( zero(), zero() ); }
    static inline simd_type from_value( double v ) { return _mm_set1_pd( v ); }
    static inline simd_type from_values( double v1, double v0 )
    {
        return _mm_set_pd( v1, v0 );
    }
    static inline simd_type iota() { return from_values( 1, 0 ); }
    static inline simd_type load_unaligned( const void* ptr )
    {
        return _mm_load_pd( reinterpret_cast<const double*>( ptr ) );
    }
};

} // namespace litesimd

#endif // LITESIMD_HAS_SSE
#endif // LITESIMD_ARCH_SSE_TRAITS_H
