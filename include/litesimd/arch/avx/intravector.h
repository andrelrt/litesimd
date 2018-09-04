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

#ifndef LITESIMD_ARCH_AVX_INTRAVECTOR_H
#define LITESIMD_ARCH_AVX_INTRAVECTOR_H

#include "../common/intravector.h"

namespace litesimd {

template<>
struct intravector_op< int8_t, avx_tag >
{
    template< typename Function_T >
    int8_t inline operator()( simd_type< int8_t, avx_tag > vec, Function_T func )
    {
        vec = func( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
        vec = func( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        vec = func( vec, _mm256_permute4x64_epi64( vec, _MM_SHUFFLE( 0, 0, 0, 2 ) ) );
        vec = func( vec, _mm256_shufflelo_epi16( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        vec = func( vec, _mm256_srli_epi16( vec, 8 ) );
        return (int8_t)_mm256_cvtsi256_si32( vec );
    }
};

template<>
struct intravector_op< int16_t, avx_tag >
{
    template< typename Function_T >
    int16_t inline operator()( simd_type< int16_t, avx_tag > vec, Function_T func )
    {
        vec = func( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
        vec = func( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        vec = func( vec, _mm256_permute4x64_epi64( vec, _MM_SHUFFLE( 0, 0, 0, 2 ) ) );
        vec = func( vec, _mm256_shufflelo_epi16( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        return (int16_t)_mm256_cvtsi256_si32( vec );
    }
};

template<>
struct intravector_op< int32_t, avx_tag >
{
    template< typename Function_T >
    int32_t inline operator()( simd_type< int32_t, avx_tag > vec, Function_T func )
    {
        vec = func( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
        vec = func( vec, _mm256_shuffle_epi32( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        vec = func( vec, _mm256_permute4x64_epi64( vec, _MM_SHUFFLE( 0, 0, 0, 2 ) ) );
        return _mm256_cvtsi256_si32( vec );
    }
};

template<>
struct intravector_op< int64_t, avx_tag >
{
    template< typename Function_T >
    int64_t inline operator()( simd_type< int64_t, avx_tag > vec, Function_T func )
    {
        vec = func( vec, _mm256_permute4x64_epi64( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
        vec = func( vec, _mm256_permute4x64_epi64( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        return _mm256_extract_epi64( vec, 0 );
    }
};

template<>
struct intravector_op< float, avx_tag >
{
    template< typename Function_T >
    float inline operator()( simd_type< float, avx_tag > vec, Function_T func )
    {
        vec = func( vec, _mm256_permute2f128_ps( vec, vec, 1 ) );
        vec = func( vec, _mm256_permute_ps( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
        vec = func( vec, _mm256_permute_ps( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        return _mm256_cvtss_f32( vec );
    }
};

template<>
struct intravector_op< double, avx_tag >
{
    template< typename Function_T >
    double inline operator()( simd_type< double, avx_tag > vec, Function_T func )
    {
        vec = func( vec, _mm256_permute4x64_pd( vec, _MM_SHUFFLE( 0, 0, 3, 2 ) ) );
        vec = func( vec, _mm256_permute4x64_pd( vec, _MM_SHUFFLE( 0, 0, 0, 1 ) ) );
        return _mm256_cvtsd_f64( vec );
    }
};

} // namespace litesimd

#endif // LITESIMD_ARCH_AVX_INTRAVECTOR_H

