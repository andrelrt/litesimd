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

#ifndef LITESIMD_ARCH_AVX_ALGORITHM_H
#define LITESIMD_ARCH_AVX_ALGORITHM_H

#include "../common/algorithm.h"

namespace litesimd {

// Min max
// ---------------------------------------------------------------------------------------
#define DEF_BINARY_MIN( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, avx_tag > \
min< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > lhs, \
                        simd_type< TYPE_T, avx_tag > rhs ) { \
    return CMD( lhs, rhs ); }

DEF_BINARY_MIN( int8_t,  _mm256_min_epi8 )
DEF_BINARY_MIN( int16_t, _mm256_min_epi16 )
DEF_BINARY_MIN( int32_t, _mm256_min_epi32 )
DEF_BINARY_MIN( float,   _mm256_min_ps )
DEF_BINARY_MIN( double,  _mm256_min_pd )
#undef DEF_BINARY_MIN

#define DEF_BINARY_MAX( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, avx_tag > \
max< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > lhs, \
                        simd_type< TYPE_T, avx_tag > rhs ) { \
    return CMD( lhs, rhs ); }

DEF_BINARY_MAX( int8_t,  _mm256_max_epi8 )
DEF_BINARY_MAX( int16_t, _mm256_max_epi16 )
DEF_BINARY_MAX( int32_t, _mm256_max_epi32 )
DEF_BINARY_MAX( float,   _mm256_max_ps )
DEF_BINARY_MAX( double,  _mm256_max_pd )
#undef DEF_BINARY_MAX

// Horizontal min max
// ---------------------------------------------------------------------------------------
template<> inline int8_t
min< int8_t, avx_tag >( simd_type< int8_t, avx_tag > vec )
{
    vec = _mm256_min_epi8( vec, _mm256_permute2x128_si256( vec, vec, 1 ) );
    vec = _mm256_min_epi8( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm256_min_epi8( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    vec = _mm256_min_epi8( vec, _mm256_shufflelo_epi16( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    vec = _mm256_min_epi8( vec, _mm256_srli_epi16( vec, 8 ) );
    return (int8_t)_mm256_cvtsi256_si32( vec );
}

template<> inline int16_t
min< int16_t, avx_tag >( simd_type< int16_t, avx_tag > vec )
{
    vec = _mm256_min_epi16( vec, _mm256_permute2x128_si256( vec, vec, 1 ) );
    vec = _mm256_min_epi16( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm256_min_epi16( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    vec = _mm256_min_epi16( vec, _mm256_shufflelo_epi16( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    return (int16_t)_mm256_cvtsi256_si32( vec );
}

template<> inline int32_t
min< int32_t, avx_tag >( simd_type< int32_t, avx_tag > vec )
{
    vec = _mm256_min_epi32( vec, _mm256_permute2x128_si256( vec, vec, 1 ) );
    vec = _mm256_min_epi32( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm256_min_epi32( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    return (int32_t)_mm256_cvtsi256_si32( vec );
}

template<> inline float
min< float, avx_tag >( simd_type< float, avx_tag > vec )
{
    vec = _mm256_min_ps( vec, _mm256_permute2f128_ps( vec, vec, 1 ) );
    vec = _mm256_min_ps( vec, _mm256_permute_ps( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm256_min_ps( vec, _mm256_permute_ps( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    return _mm256_cvtss_f32( vec );
}

template<> inline double
min< double, avx_tag >( simd_type< double, avx_tag > vec )
{
    vec = _mm256_min_pd( vec, _mm256_permute4x64_pd( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm256_min_pd( vec, _mm256_permute_pd( vec, 1 ) );
    return _mm256_cvtsd_f64( vec );
}

template<> inline int8_t
max< int8_t, avx_tag >( simd_type< int8_t, avx_tag > vec )
{
    vec = _mm256_max_epi8( vec, _mm256_permute2x128_si256( vec, vec, 1 ) );
    vec = _mm256_max_epi8( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm256_max_epi8( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    vec = _mm256_max_epi8( vec, _mm256_shufflelo_epi16( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    vec = _mm256_max_epi8( vec, _mm256_srli_epi16( vec, 8 ) );
    return (int8_t)_mm256_cvtsi256_si32( vec );
}

template<> inline int16_t
max< int16_t, avx_tag >( simd_type< int16_t, avx_tag > vec )
{
    vec = _mm256_max_epi16( vec, _mm256_permute2x128_si256( vec, vec, 1 ) );
    vec = _mm256_max_epi16( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm256_max_epi16( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    vec = _mm256_max_epi16( vec, _mm256_shufflelo_epi16( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    return (int16_t)_mm256_cvtsi256_si32( vec );
}

template<> inline int32_t
max< int32_t, avx_tag >( simd_type< int32_t, avx_tag > vec )
{
    vec = _mm256_max_epi32( vec, _mm256_permute2x128_si256( vec, vec, 1 ) );
    vec = _mm256_max_epi32( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm256_max_epi32( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    return (int32_t)_mm256_cvtsi256_si32( vec );
}

template<> inline float
max< float, avx_tag >( simd_type< float, avx_tag > vec )
{
    vec = _mm256_max_ps( vec, _mm256_permute2f128_ps( vec, vec, 1 ) );
    vec = _mm256_max_ps( vec, _mm256_permute_ps( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm256_max_ps( vec, _mm256_permute_ps( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    return _mm256_cvtss_f32( vec );
}

template<> inline double
max< double, avx_tag >( simd_type< double, avx_tag > vec )
{
    vec = _mm256_max_pd( vec, _mm256_permute4x64_pd( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm256_max_pd( vec, _mm256_permute_pd( vec, 1 ) );
    return _mm256_cvtsd_f64( vec );
}

} // namespace litesimd

#endif // LITESIMD_ARCH_AVX_ALGORITHM_H
