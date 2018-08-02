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

#ifndef LITESIMD_SSE_TYPES_H
#define LITESIMD_SSE_TYPES_H

#include <immintrin.h>
#include <type_traits>
#include "../../traits.h"

namespace litesimd {

#define STRUCT_TRAITS_INT_SSE( INT_T ) \
template<> struct traits< INT_T, sse_tag > { \
    struct simd_type { __m128i _; typedef INT_T simd_value_type; }; \
    typedef __m128i  mask_type; \
    typedef uint16_t bitmask_type; \
    constexpr static size_t simd_size = sizeof(simd_type)/sizeof(INT_T); \
    static inline simd_type zero() { return {_mm_setzero_si128()}; } \
}
STRUCT_TRAITS_INT_SSE( int8_t );
STRUCT_TRAITS_INT_SSE( int16_t );
STRUCT_TRAITS_INT_SSE( int32_t );
STRUCT_TRAITS_INT_SSE( int64_t );
#undef STRUCT_TRAITS_INT_SSE

template<> struct traits< float, sse_tag >
{
    struct simd_type { __m128 _; typedef float simd_value_type; };
    typedef __m128i  mask_type;
    typedef uint16_t bitmask_type;
    constexpr static size_t simd_size = sizeof(simd_type)/sizeof(float);

    static inline simd_type zero()
    {
        return {_mm_setzero_ps()};
    }
};

template<> struct traits< double, sse_tag >
{
    struct simd_type { __m128d _; typedef double simd_value_type; };
    typedef __m128i  mask_type;
    typedef uint16_t bitmask_type;
    constexpr static size_t simd_size = sizeof(simd_type)/sizeof(double);

    static inline simd_type zero()
    {
        return {_mm_setzero_pd()};
    }
};

template<> inline typename traits< int8_t, sse_tag >::simd_type
from_value< int8_t, sse_tag >( int8_t val )
{
    return {_mm_set1_epi8( val )};
}

template<> inline typename traits< int16_t, sse_tag >::simd_type
from_value< int16_t, sse_tag >( int16_t val )
{
    return {_mm_set1_epi16( val )};
}

template<> inline typename traits< int32_t, sse_tag >::simd_type
from_value< int32_t, sse_tag >( int32_t val )
{
    return {_mm_set1_epi32( val )};
}

template<> inline typename traits< int64_t, sse_tag >::simd_type
from_value< int64_t, sse_tag >( int64_t val )
{
    return {_mm_set1_epi64x( val )};
}

template<> inline typename traits< float, sse_tag >::simd_type
from_value< float, sse_tag >( float val )
{
    return {_mm_set1_ps( val )};
}

template<> inline typename traits< double, sse_tag >::simd_type
from_value< double, sse_tag >( double val )
{
    return {_mm_set1_pd( val )};
}

} // namespace litesimd

#endif //LITESIMD_SSE_TYPES_H

#endif //__SSE2__
