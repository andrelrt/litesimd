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

#ifndef LITESIMD_AVX_ARITHMETIC_H
#define LITESIMD_AVX_ARITHMETIC_H

#ifdef LITESIMD_HAS_AVX

#include "../../types.h"
#include "../common/arithmetic.h"

namespace litesimd {

// Add
// ---------------------------------------------------------------------------------------
#define DEF_ADD( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, avx_tag > \
add< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > lhs, \
                        simd_type< TYPE_T, avx_tag > rhs ) { \
    return CMD( lhs, rhs ); }

DEF_ADD( int8_t,  _mm256_add_epi8 )
DEF_ADD( int16_t, _mm256_add_epi16 )
DEF_ADD( int32_t, _mm256_add_epi32 )
DEF_ADD( int64_t, _mm256_add_epi64 )
DEF_ADD( float,   _mm256_add_ps )
DEF_ADD( double,  _mm256_add_pd )
#undef DEF_ADD

} // namespace litesimd

#endif // LITESIMD_HAS_AVX
#endif // LITESIMD_AVX_ARITHMETIC_H
