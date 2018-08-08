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

#ifndef LITESIMD_SSE_BITWISE_H
#define LITESIMD_SSE_BITWISE_H

#include "../../types.h"
#include "../common/bitwise.h"

namespace litesimd {

// Bit AND
// ---------------------------------------------------------------------------------------
#define DEF_BIT_AND( TYPE_T, CMD ) \
template<> inline typename traits< TYPE_T, sse_tag >::simd_type \
bit_and< TYPE_T, sse_tag >( typename traits< TYPE_T, sse_tag >::simd_type lhs, \
                            typename traits< TYPE_T, sse_tag >::simd_type rhs ) { \
    return CMD( lhs, rhs ); }

DEF_BIT_AND( int8_t,  _mm_and_si128 )
DEF_BIT_AND( int16_t, _mm_and_si128 )
DEF_BIT_AND( int32_t, _mm_and_si128 )
DEF_BIT_AND( int64_t, _mm_and_si128 )
DEF_BIT_AND( float,   _mm_and_ps )
DEF_BIT_AND( double,  _mm_and_pd )
#undef DEF_BIT_AND

} // namespace litesimd

#endif // LITESIMD_SSE_BITWISE_H

#endif //__SSE2__
