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

#ifdef __SSE2__

#ifndef LITESIMD_SSE_SHUFFLE_H
#define LITESIMD_SSE_SHUFFLE_H

#include "../../types.h"
#include "../common/shuffle.h"

namespace litesimd {

// Low insert
// ---------------------------------------------------------------------------------------
template<> inline typename traits< int8_t, sse_tag >::simd_type
low_insert< int8_t, sse_tag >( typename traits< int8_t, sse_tag >::simd_type vec,
                               int8_t val )
{
    return _mm_insert_epi8(
                 _mm_shuffle_epi8( vec, 
                        _mm_set_epi8( 15, 15, 14, 13, 12, 11, 10, 9,
                                       8,  7,  6,  5,  4,  3,  2, 1 ) ),
                 (int)val, 15 );
}

template<> inline typename traits< int16_t, sse_tag >::simd_type
low_insert< int16_t, sse_tag >( typename traits< int16_t, sse_tag >::simd_type vec,
                                int16_t val )
{
    return _mm_insert_epi16(
                 _mm_shuffle_epi8( vec, 
                        _mm_set_epi8( 15, 15, 15, 14, 13, 12, 11, 10,
                                       9,  8,  7,  6,  5,  4,  3, 2 ) ),
                 (int)val, 7 );
}

template<> inline typename traits< int32_t, sse_tag >::simd_type
low_insert< int32_t, sse_tag >( typename traits< int32_t, sse_tag >::simd_type vec,
                                int32_t val )
{
    return _mm_insert_epi32(
                 _mm_shuffle_epi32( vec, _MM_SHUFFLE( 3, 3, 2, 1 ) ), 
                 (int) val, 3 );
}

template<> inline typename traits< int64_t, sse_tag >::simd_type
low_insert< int64_t, sse_tag >( typename traits< int64_t, sse_tag >::simd_type vec,
                                int64_t val )
{
    return _mm_insert_epi64(
                 _mm_shuffle_epi32( vec, _MM_SHUFFLE( 3, 3, 3, 2 ) ), 
                 val, 1 );
}

} // namespace litesimd

#endif // LITESIMD_SSE_SHUFFLE_H

#endif //__SSE2__
