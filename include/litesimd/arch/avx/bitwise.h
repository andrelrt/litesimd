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

#ifndef LITESIMD_AVX_BITWISE_H
#define LITESIMD_AVX_BITWISE_H

#ifdef LITESIMD_HAS_AVX

#include "../../types.h"
#include "../common/bitwise.h"

namespace litesimd {

// Bit AND
// ---------------------------------------------------------------------------------------
#define DEF_BIT_AND( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, avx_tag > \
bit_and< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > lhs, \
                            simd_type< TYPE_T, avx_tag > rhs ) { \
    return CMD( lhs, rhs ); }

DEF_BIT_AND( int8_t,  _mm256_and_si256 )
DEF_BIT_AND( int16_t, _mm256_and_si256 )
DEF_BIT_AND( int32_t, _mm256_and_si256 )
DEF_BIT_AND( int64_t, _mm256_and_si256 )
DEF_BIT_AND( float,   _mm256_and_ps )
DEF_BIT_AND( double,  _mm256_and_pd )
#undef DEF_BIT_AND

// Bit OR
// ---------------------------------------------------------------------------------------
#define DEF_BIT_OR( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, avx_tag > \
bit_or< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > lhs, \
                           simd_type< TYPE_T, avx_tag > rhs ) { \
    return CMD( lhs, rhs ); }

DEF_BIT_OR( int8_t,  _mm256_or_si256 )
DEF_BIT_OR( int16_t, _mm256_or_si256 )
DEF_BIT_OR( int32_t, _mm256_or_si256 )
DEF_BIT_OR( int64_t, _mm256_or_si256 )
DEF_BIT_OR( float,   _mm256_or_ps )
DEF_BIT_OR( double,  _mm256_or_pd )
#undef DEF_BIT_OR

// Bit XOR
// ---------------------------------------------------------------------------------------
#define DEF_BIT_XOR( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, avx_tag > \
bit_xor< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > lhs, \
                            simd_type< TYPE_T, avx_tag > rhs ) { \
    return CMD( lhs, rhs ); }

DEF_BIT_XOR( int8_t,  _mm256_xor_si256 )
DEF_BIT_XOR( int16_t, _mm256_xor_si256 )
DEF_BIT_XOR( int32_t, _mm256_xor_si256 )
DEF_BIT_XOR( int64_t, _mm256_xor_si256 )
DEF_BIT_XOR( float,   _mm256_xor_ps )
DEF_BIT_XOR( double,  _mm256_xor_pd )
#undef DEF_BIT_XOR

#ifndef _WIN32
// Horizontal bit AND
// ---------------------------------------------------------------------------------------
template<> inline int8_t
bit_and< int8_t, avx_tag >( simd_type< int8_t, avx_tag > val )
{
    val = _mm256_and_si256( val, _mm256_shuffle_epi32( val, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    val = _mm256_and_si256( val, _mm256_shuffle_epi32( val, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    val = _mm256_and_si256( val, _mm256_permute4x64_epi64( val, _MM_SHUFFLE( 0, 0, 0, 2 ) ) );
    val = _mm256_and_si256( val, _mm256_shufflelo_epi16( val, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    val = _mm256_and_si256( val, _mm256_srli_epi16( val, 8 ) );
    return (int8_t)_mm256_cvtsi256_si32( val );
}

template<> inline int16_t
bit_and< int16_t, avx_tag >( simd_type< int16_t, avx_tag > val )
{
    val = _mm256_and_si256( val, _mm256_shuffle_epi32( val, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    val = _mm256_and_si256( val, _mm256_shuffle_epi32( val, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    val = _mm256_and_si256( val, _mm256_permute4x64_epi64( val, _MM_SHUFFLE( 0, 0, 0, 2 ) ) );
    val = _mm256_and_si256( val, _mm256_shufflelo_epi16( val, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    return (int16_t)_mm256_cvtsi256_si32( val );
}

template<> inline int32_t
bit_and< int32_t, avx_tag >( simd_type< int32_t, avx_tag > val )
{
    val = _mm256_and_si256( val, _mm256_shuffle_epi32( val, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    val = _mm256_and_si256( val, _mm256_shuffle_epi32( val, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    val = _mm256_and_si256( val, _mm256_permute4x64_epi64( val, _MM_SHUFFLE( 0, 0, 0, 2 ) ) );
    return _mm256_cvtsi256_si32( val );
}

template<> inline int64_t
bit_and< int64_t, avx_tag >( simd_type< int64_t, avx_tag > val )
{
    val = _mm256_and_si256( val, _mm256_permute4x64_epi64( val, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    val = _mm256_and_si256( val, _mm256_permute4x64_epi64( val, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    return _mm256_extract_epi64( val, 0 );
}

template<> inline float
bit_and< float, avx_tag >( simd_type< float, avx_tag > val )
{
    val = _mm256_and_ps( val, _mm256_shuffle_ps( val, val, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    val = _mm256_and_ps( val, _mm256_shuffle_ps( val, val, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    return _mm256_cvtss_f32( val );
}

template<> inline double
bit_and< double, avx_tag >( simd_type< double, avx_tag > val )
{
    val = _mm256_and_pd( val, _mm256_shuffle_pd( val, val, _MM_SHUFFLE2( 0, 1 ) ) );
    return _mm256_cvtsd_f64( val );
}
#endif // _WIN32


} // namespace litesimd

#endif // LITESIMD_HAS_AVX
#endif // LITESIMD_AVX_BITWISE_H
