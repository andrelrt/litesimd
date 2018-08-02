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

#ifdef __AVX2__

#ifndef LITESIMD_AVX_TYPES_H
#define LITESIMD_AVX_TYPES_H

#include <immintrin.h>
#include <type_traits>
#include "../../traits.h"

namespace litesimd {

#define STRUCT_TRAITS_INT_AVX( INT_T ) \
template<> struct traits< INT_T, avx_tag > { \
    struct simd_type { __m256i _; typedef INT_T simd_value_type; }; \
    typedef __m256i  mask_type; \
    typedef uint32_t bitmask_type; \
    constexpr static size_t simd_size = sizeof(simd_type)/sizeof(INT_T); \
    static inline simd_type zero() { return {_mm256_setzero_si256()}; } \
}
STRUCT_TRAITS_INT_AVX( int8_t );
STRUCT_TRAITS_INT_AVX( int16_t );
STRUCT_TRAITS_INT_AVX( int32_t );
STRUCT_TRAITS_INT_AVX( int64_t );
#undef STRUCT_TRAITS_INT_AVX

template<> struct traits< float, avx_tag >
{
    struct simd_type { __m256 _; typedef float simd_value_type; };
    typedef __m256i  mask_type;
    typedef uint32_t bitmask_type;
    constexpr static size_t simd_size = sizeof(simd_type)/sizeof(float);

    static inline simd_type zero()
    {
        return {_mm256_setzero_ps()};
    }
};

template<> struct traits< double, avx_tag >
{
    struct simd_type { __m256 _; typedef double simd_value_type; };
    typedef __m256i  mask_type;
    typedef uint32_t bitmask_type;
    constexpr static size_t simd_size = sizeof(simd_type)/sizeof(double);

    static inline simd_type zero()
    {
        return {_mm256_setzero_pd()};
    }
};

template<> inline typename traits< int8_t, avx_tag >::simd_type
from_value< int8_t, avx_tag >( int8_t val )
{
    return {_mm256_set1_epi8( val )};
}

template<> inline typename traits< int16_t, avx_tag >::simd_type
from_value< int16_t, avx_tag >( int16_t val )
{
    return {_mm256_set1_epi16( val )};
}

template<> inline typename traits< int32_t, avx_tag >::simd_type
from_value< int32_t, avx_tag >( int32_t val )
{
    return {_mm256_set1_epi32( val )};
}

template<> inline typename traits< int64_t, avx_tag >::simd_type
from_value< int64_t, avx_tag >( int64_t val )
{
    return {_mm256_set1_epi64x( val )};
}

template<> inline typename traits< float, avx_tag >::simd_type
from_value< float, avx_tag >( float val )
{
    return {_mm256_set1_ps( val )};
}

template<> inline typename traits< double, avx_tag >::simd_type
from_value< double, avx_tag >( double val )
{
    return {_mm256_set1_pd( val )};
}

} // namespace litesimd

#endif //LITESIMD_AVX_TYPES_H

#endif //__AVX2__
