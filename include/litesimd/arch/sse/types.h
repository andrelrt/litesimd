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

#ifndef LITESIMD_SSE_TYPES_H
#define LITESIMD_SSE_TYPES_H

#include <immintrin.h>
#include <type_traits>
#include "../../traits.h"

namespace litesimd {

struct sse_tag {};

template< typename ValueType_T >
struct traits< ValueType_T, sse_tag, 
               std::enable_if_t< std::is_integral< ValueType_T >::value > >
{
    using simd_type = __m128i;
    using mask_type = __m128i;
    using bitmask_type = uint16_t;
    constexpr static size_t simd_size = sizeof(simd_type)/sizeof(ValueType_T);

    static inline simd_type zero()
    {
        return _mm_setzero_si128();
    }

    template< class = std::enable_if_t< std::is_same< int8_t, ValueType_T >::value > >
    static inline simd_type from_value( int8_t val )
    {
        return _mm_set1_epi8( val );
    }

    template< class = std::enable_if_t< std::is_same< int16_t, ValueType_T >::value > >
    static inline simd_type from_value( int16_t val )
    {
        return _mm_set1_epi16( val );
    }

    template< class = std::enable_if_t< std::is_same< int32_t, ValueType_T >::value > >
    static inline simd_type from_value( int32_t val )
    {
        return _mm_set1_epi32( val );
    }

    template< class = std::enable_if_t< std::is_same< int64_t, ValueType_T >::value > >
    static inline simd_type from_value( int64_t val )
    {
        return _mm_set1_epi64x( val );
    }
};

template<> struct traits< float, sse_tag, void >
{
    using simd_type = __m128;
    using mask_type = __m128i;
    using bitmask_type = uint16_t;
    constexpr static size_t simd_size = sizeof(simd_type)/sizeof(float);

    static inline simd_type zero()
    {
        return _mm_setzero_ps();
    }

    static inline simd_type from_value( float val )
    {
        return _mm_set1_ps( val );
    }
};

template<> struct traits< double, sse_tag, void >
{
    using simd_type = __m128d;
    using mask_type = __m128i;
    using bitmask_type = uint16_t;
    constexpr static size_t simd_size = sizeof(simd_type)/sizeof(double);

    static inline simd_type zero()
    {
        return _mm_setzero_pd();
    }

    static inline simd_type from_value( double val )
    {
        return _mm_set1_pd( val );
    }
};

} // namespace litesimd

#endif //LITESIMD_SSE_TYPES_H

