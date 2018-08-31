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

#ifndef LITESIMD_SSE_BITWISE_H
#define LITESIMD_SSE_BITWISE_H

#ifdef LITESIMD_HAS_SSE

#include "../../types.h"
#include "../common/bitwise.h"

namespace litesimd {

// Bit AND
// ---------------------------------------------------------------------------------------
#define DEF_BIT_AND( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, sse_tag > \
bit_and< TYPE_T, sse_tag >( simd_type< TYPE_T, sse_tag > lhs, \
                            simd_type< TYPE_T, sse_tag > rhs ) { \
    return CMD( lhs, rhs ); }

DEF_BIT_AND( int8_t,  _mm_and_si128 )
DEF_BIT_AND( int16_t, _mm_and_si128 )
DEF_BIT_AND( int32_t, _mm_and_si128 )
DEF_BIT_AND( int64_t, _mm_and_si128 )
DEF_BIT_AND( float,   _mm_and_ps )
DEF_BIT_AND( double,  _mm_and_pd )
#undef DEF_BIT_AND

// Bit OR
// ---------------------------------------------------------------------------------------
#define DEF_BIT_OR( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, sse_tag > \
bit_or< TYPE_T, sse_tag >( simd_type< TYPE_T, sse_tag > lhs, \
                           simd_type< TYPE_T, sse_tag > rhs ) { \
    return CMD( lhs, rhs ); }

DEF_BIT_OR( int8_t,  _mm_or_si128 )
DEF_BIT_OR( int16_t, _mm_or_si128 )
DEF_BIT_OR( int32_t, _mm_or_si128 )
DEF_BIT_OR( int64_t, _mm_or_si128 )
DEF_BIT_OR( float,   _mm_or_ps )
DEF_BIT_OR( double,  _mm_or_pd )
#undef DEF_BIT_OR

// Bit XOR
// ---------------------------------------------------------------------------------------
#define DEF_BIT_XOR( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, sse_tag > \
bit_xor< TYPE_T, sse_tag >( simd_type< TYPE_T, sse_tag > lhs, \
                            simd_type< TYPE_T, sse_tag > rhs ) { \
    return CMD( lhs, rhs ); }

DEF_BIT_XOR( int8_t,  _mm_xor_si128 )
DEF_BIT_XOR( int16_t, _mm_xor_si128 )
DEF_BIT_XOR( int32_t, _mm_xor_si128 )
DEF_BIT_XOR( int64_t, _mm_xor_si128 )
DEF_BIT_XOR( float,   _mm_xor_ps )
DEF_BIT_XOR( double,  _mm_xor_pd )
#undef DEF_BIT_XOR

// Horizontal bit AND
// ---------------------------------------------------------------------------------------
template<> inline int8_t
bit_and< int8_t, sse_tag >( simd_type< int8_t, sse_tag > vec )
{
    vec = _mm_and_si128( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm_and_si128( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    vec = _mm_and_si128( vec, _mm_shufflelo_epi16( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    vec = _mm_and_si128( vec, _mm_srli_epi16( vec, 8 ) );
    return (int8_t)_mm_cvtsi128_si32( vec );
}

template<> inline int16_t
bit_and< int16_t, sse_tag >( simd_type< int16_t, sse_tag > vec )
{
    vec = _mm_and_si128( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm_and_si128( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    vec = _mm_and_si128( vec, _mm_shufflelo_epi16( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    return (int16_t)_mm_cvtsi128_si32( vec );
}

template<> inline int32_t
bit_and< int32_t, sse_tag >( simd_type< int32_t, sse_tag > vec )
{
    vec = _mm_and_si128( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm_and_si128( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    return _mm_cvtsi128_si32( vec );
}

template<> inline int64_t
bit_and< int64_t, sse_tag >( simd_type< int64_t, sse_tag > vec )
{
    vec = _mm_and_si128( vec, _mm_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    return _mm_cvtsi128_si64( vec );
}

template<> inline float
bit_and< float, sse_tag >( simd_type< float, sse_tag > vec )
{
    vec = _mm_and_ps( vec, _mm_shuffle_ps( vec, vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
    vec = _mm_and_ps( vec, _mm_shuffle_ps( vec, vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
    return _mm_cvtss_f32( vec );
}

template<> inline double
bit_and< double, sse_tag >( simd_type< double, sse_tag > vec )
{
    vec = _mm_and_pd( vec, _mm_shuffle_pd( vec, vec, _MM_SHUFFLE2( 0, 1 ) ) );
    return _mm_cvtsd_f64( vec );
}

} // namespace litesimd

#endif // LITESIMD_HAS_SSE
#endif // LITESIMD_SSE_BITWISE_H
