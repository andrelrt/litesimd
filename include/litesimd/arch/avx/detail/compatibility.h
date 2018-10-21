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

#ifndef LITESIMD_AVX_DETAIL_COMPATIBILITY_H
#define LITESIMD_AVX_DETAIL_COMPATIBILITY_H

#ifdef LITESIMD_HAS_AVX

#if (__GNUC__ < 7) && !defined(__clang__)
// old GCC workarounds
static inline float _mm256_cvtss_f32( __m256 vec ) { return _mm_cvtss_f32( _mm256_extractf128_ps( vec, 0 ) ); }
static inline double _mm256_cvtsd_f64( __m256d vec ) { return _mm_cvtsd_f64( _mm256_extractf128_pd( vec, 0 ) ); }
#endif

#if defined(_MSC_VER)
static inline __m256i _mm256_insert_epi64( __m256i vec, int64_t val, const int imm8 )
{
    return _mm256_blend_epi32( vec, _mm_set1_epi64x( val ), 3 << (2*imm8) );
}
#endif _MSC_VER

#endif // LITESIMD_HAS_AVX
#endif // LITESIMD_AVX_DETAIL_COMPATIBILITY_H
