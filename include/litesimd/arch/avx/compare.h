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

#ifndef LITESIMD_AVX_COMPARE_H
#define LITESIMD_AVX_COMPARE_H

#ifdef LITESIMD_HAS_AVX

#include "../../types.h"
#include "../common/compare.h"

namespace litesimd {

// Mask to bitmask
// ---------------------------------------------------------------------------------------
#define DEF_MASK_TO_BITMASK( TYPE_T, CMD ) \
template<> inline typename simd_type< TYPE_T, avx_tag >::bitmask_type \
mask_to_bitmask< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > mask ) { \
    return CMD( mask ); \
}

DEF_MASK_TO_BITMASK( int8_t,  _mm256_movemask_epi8 )
DEF_MASK_TO_BITMASK( int16_t, _mm256_movemask_epi8 )
DEF_MASK_TO_BITMASK( int32_t, _mm256_movemask_epi8 )
DEF_MASK_TO_BITMASK( int64_t, _mm256_movemask_epi8 )
DEF_MASK_TO_BITMASK( float,   _mm256_movemask_ps )
DEF_MASK_TO_BITMASK( double,  _mm256_movemask_pd )

#undef DEF_MASK_TO_BITMASK

// Greater than
// ---------------------------------------------------------------------------------------
#define DEF_GREATER_THAN( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, avx_tag > \
greater_than< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > lhs, \
                                 simd_type< TYPE_T, avx_tag > rhs ) { \
    return CMD( lhs, rhs ); \
}

DEF_GREATER_THAN( int8_t,  _mm256_cmpgt_epi8 )
DEF_GREATER_THAN( int16_t, _mm256_cmpgt_epi16 )
DEF_GREATER_THAN( int32_t, _mm256_cmpgt_epi32 )
DEF_GREATER_THAN( int64_t, _mm256_cmpgt_epi64 )

#undef DEF_GREATER_THAN

template<> inline simd_type< float, avx_tag >
greater_than< float, avx_tag >( simd_type< float, avx_tag > lhs,
                                simd_type< float, avx_tag > rhs )
{
    // Quietly ignore NaN
    return _mm256_cmp_ps( lhs, rhs, _CMP_GT_OQ );
}

template<> inline simd_type< double, avx_tag >
greater_than< double, avx_tag >( simd_type< double, avx_tag > lhs,
                                 simd_type< double, avx_tag > rhs )
{
    // Quietly ignore NaN
    return _mm256_cmp_pd( lhs, rhs, _CMP_GT_OQ );
}

// Equals
// ---------------------------------------------------------------------------------------
#define DEF_EQUALS( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, avx_tag > \
equals< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > lhs, \
                           simd_type< TYPE_T, avx_tag > rhs ) { \
    return CMD( lhs, rhs ); \
}

DEF_EQUALS( int8_t,  _mm256_cmpeq_epi8 )
DEF_EQUALS( int16_t, _mm256_cmpeq_epi16 )
DEF_EQUALS( int32_t, _mm256_cmpeq_epi32 )
DEF_EQUALS( int64_t, _mm256_cmpeq_epi64 )

#undef DEF_EQUALS

template<> inline simd_type< float, avx_tag >
equals< float, avx_tag >( simd_type< float, avx_tag > lhs,
                          simd_type< float, avx_tag > rhs )
{
    // Quietly ignore NaN
    return _mm256_cmp_ps( lhs, rhs, _CMP_EQ_OQ );
}

template<> inline simd_type< double, avx_tag >
equals< double, avx_tag >( simd_type< double, avx_tag > lhs,
                           simd_type< double, avx_tag > rhs )
{
    // Quietly ignore NaN
    return _mm256_cmp_pd( lhs, rhs, _CMP_EQ_OQ );
}

} // namespace litesimd

#endif // LITESIMD_HAS_AVX
#endif // LITESIMD_AVX_COMPARE_H
