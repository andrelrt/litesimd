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

#ifndef LITESIMD_SSE_COMPARE_H
#define LITESIMD_SSE_COMPARE_H

#ifdef LITESIMD_HAS_SSE

#include "../../types.h"
#include "../common/compare.h"
#include "../../detail/helper_macros.h"

namespace litesimd {

// Mask to bitmask
// ---------------------------------------------------------------------------------------
#define DEF_MASK_TO_BITMASK( TYPE_T, CMD ) \
template<> inline typename traits< TYPE_T, sse_tag >::bitmask_type \
mask_to_bitmask< TYPE_T, sse_tag >( typename traits< TYPE_T, sse_tag >::mask_type mask ) { \
    return CMD( mask ); \
}

DEF_MASK_TO_BITMASK( int8_t,  _mm_movemask_epi8 )
DEF_MASK_TO_BITMASK( int16_t, _mm_movemask_epi8 )
DEF_MASK_TO_BITMASK( int32_t, _mm_movemask_epi8 )
DEF_MASK_TO_BITMASK( int64_t, _mm_movemask_epi8 )
DEF_MASK_TO_BITMASK( float,   _mm_movemask_ps )
DEF_MASK_TO_BITMASK( double,  _mm_movemask_pd )
#undef DEF_MASK_TO_BITMASK

// Greater than
// ---------------------------------------------------------------------------------------
#define DEF_GREATER_THAN( TYPE_T, CMD ) \
template<> inline typename traits< TYPE_T, sse_tag >::mask_type \
greater_than< TYPE_T, sse_tag >( typename traits< TYPE_T, sse_tag >::simd_type lhs, \
                                 typename traits< TYPE_T, sse_tag >::simd_type rhs ) { \
    return CMD( lhs, rhs ); \
}

DEF_GREATER_THAN( int8_t,  _mm_cmpgt_epi8 )
DEF_GREATER_THAN( int16_t, _mm_cmpgt_epi16 )
DEF_GREATER_THAN( int32_t, _mm_cmpgt_epi32 )
DEF_GREATER_THAN( int64_t, _mm_cmpgt_epi64 )
DEF_GREATER_THAN( float,   _mm_cmpgt_ps )
DEF_GREATER_THAN( double,  _mm_cmpgt_pd )
#undef DEF_GREATER_THAN

// Equals
// ---------------------------------------------------------------------------------------
#define DEF_EQUALS( TYPE_T, CMD ) \
template<> inline typename traits< TYPE_T, sse_tag >::mask_type \
equals< TYPE_T, sse_tag >( typename traits< TYPE_T, sse_tag >::simd_type lhs, \
                           typename traits< TYPE_T, sse_tag >::simd_type rhs ) { \
    return CMD( lhs, rhs ); \
}

DEF_EQUALS( int8_t,  _mm_cmpeq_epi8 )
DEF_EQUALS( int16_t, _mm_cmpeq_epi16 )
DEF_EQUALS( int32_t, _mm_cmpeq_epi32 )
DEF_EQUALS( int64_t, _mm_cmpeq_epi64 )
DEF_EQUALS( float,   _mm_cmpeq_ps )
DEF_EQUALS( double,  _mm_cmpeq_pd )
#undef DEF_EQUALS

// Blend ternary
// ---------------------------------------------------------------------------------------
#define DEF_BLEND( TYPE_T, BLEND_CMD ) \
template<> inline typename traits< TYPE_T, sse_tag >::simd_type \
blend< TYPE_T, sse_tag >( typename traits< TYPE_T, sse_tag >::mask_type mask, \
                          typename traits< TYPE_T, sse_tag >::simd_type trueVal, \
                          typename traits< TYPE_T, sse_tag >::simd_type falseVal ) { \
    return BLEND_CMD( falseVal, trueVal, mask ); }

DEF_BLEND( int8_t,  _mm_blendv_epi8 )
DEF_BLEND( int16_t, _mm_blendv_epi8 )
DEF_BLEND( int32_t, _mm_blendv_epi8 )
DEF_BLEND( int64_t, _mm_blendv_epi8 )
DEF_BLEND( float,   _mm_blendv_ps )
DEF_BLEND( double,  _mm_blendv_pd )
#undef DEF_BLEND

} // namespace litesimd

#endif // LITESIMD_HAS_SSE
#endif // LITESIMD_SSE_COMPARE_H
