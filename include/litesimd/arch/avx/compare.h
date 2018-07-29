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

#ifndef LITESIMD_AVX_COMPARE_H
#define LITESIMD_AVX_COMPARE_H

#include "../../types.h"
#include "../compare.h"

namespace litesimd {

template<> inline typename traits< int8_t, avx_tag >::mask_type
greater_than< int8_t, avx_tag >( __m256i lhs, __m256i rhs )
{
    return _mm256_cmpgt_epi8( lhs, rhs );
}

template<> inline typename traits< int16_t, avx_tag >::mask_type
greater_than< int16_t, avx_tag >( __m256i lhs, __m256i rhs )
{
    return _mm256_cmpgt_epi16( lhs, rhs );
}

template<> inline typename traits< int32_t, avx_tag >::mask_type
greater_than< int32_t, avx_tag >( __m256i lhs, __m256i rhs )
{
    return _mm256_cmpgt_epi32( lhs, rhs );
}

template<> inline typename traits< int64_t, avx_tag >::mask_type
greater_than< int64_t, avx_tag >( __m256i lhs, __m256i rhs )
{
    return _mm256_cmpgt_epi64( lhs, rhs );
}

template<> inline typename traits< float, avx_tag >::mask_type
greater_than< float, avx_tag >( __m256 lhs, __m256 rhs )
{
    // Quietly ignore NaN
    return _mm256_cmp_ps( lhs, rhs, _CMP_GT_OQ );
}

template<> inline typename traits< double, avx_tag >::mask_type
greater_than< double, avx_tag >( __m256d lhs, __m256d rhs )
{
    // Quietly ignore NaN
    return _mm256_cmp_pd( lhs, rhs, _CMP_GT_OQ );
}

template<> inline typename traits< int8_t, avx_tag >::bitmask_type
mask_to_bitmask< int8_t, avx_tag >( typename traits< int8_t, avx_tag >::mask_type mask )
{
    return _mm256_movemask_epi8( mask );
}

template<> inline typename traits< int16_t, avx_tag >::bitmask_type
mask_to_bitmask< int16_t, avx_tag >( typename traits< int16_t, avx_tag >::mask_type mask )
{
    return _mm256_movemask_epi8( mask );
}

template<> inline typename traits< int32_t, avx_tag >::bitmask_type
mask_to_bitmask< int32_t, avx_tag >( typename traits< int32_t, avx_tag >::mask_type mask )
{
    return _mm256_movemask_epi8( mask );
}

template<> inline typename traits< int64_t, avx_tag >::bitmask_type
mask_to_bitmask< int64_t, avx_tag >( typename traits< int64_t, avx_tag >::mask_type mask )
{
    return _mm256_movemask_epi8( mask );
}

template<> inline typename traits< float, avx_tag >::bitmask_type
mask_to_bitmask< float, avx_tag >( typename traits< float, avx_tag >::mask_type mask )
{
    return _mm256_movemask_ps( mask );
}

template<> inline typename traits< double, avx_tag >::bitmask_type
mask_to_bitmask< double, avx_tag >( typename traits< double, avx_tag >::mask_type mask )
{
    return _mm256_movemask_pd( mask );
}

} // namespace litesimd

#endif // LITESIMD_AVX_COMPARE_H

#endif //__AVX2__
