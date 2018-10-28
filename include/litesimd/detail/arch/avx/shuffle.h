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

#ifndef LITESIMD_AVX_SHUFFLE_H
#define LITESIMD_AVX_SHUFFLE_H

#ifdef LITESIMD_HAS_AVX

#include <litesimd/types.h>
#include <litesimd/arch/common/shuffle.h>
#include <litesimd/arch/avx/detail/compatibility.h>

namespace litesimd {

// Blend
// ---------------------------------------------------------------------------------------
#define DEF_BLEND( TYPE_T, BLEND_CMD ) \
template<> inline simd_type< TYPE_T, avx_tag > \
blend< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > mask, \
                          simd_type< TYPE_T, avx_tag > trueVal, \
                          simd_type< TYPE_T, avx_tag > falseVal ) { \
    return BLEND_CMD( falseVal, trueVal, mask ); }

DEF_BLEND( int8_t,  _mm256_blendv_epi8 )
DEF_BLEND( int16_t, _mm256_blendv_epi8 )
DEF_BLEND( int32_t, _mm256_blendv_epi8 )
DEF_BLEND( int64_t, _mm256_blendv_epi8 )
DEF_BLEND( float,   _mm256_blendv_ps )
DEF_BLEND( double,  _mm256_blendv_pd )
#undef DEF_BLEND

// Get
// ---------------------------------------------------------------------------------------
template< int index >
struct get_functor< index, int8_t, avx_tag >
{
    int8_t inline operator()( simd_type< int8_t, avx_tag > vec )
    {
        return (int8_t) _mm256_extract_epi8( vec, index );
    }
};

template< int index >
struct get_functor< index, int16_t, avx_tag >
{
    int16_t inline operator()( simd_type< int16_t, avx_tag > vec )
    {
        return (int16_t) _mm256_extract_epi16( vec, index );
    }
};

template< int index >
struct get_functor< index, int32_t, avx_tag >
{
    int32_t inline operator()( simd_type< int32_t, avx_tag > vec )
    {
        return (int32_t) _mm256_extract_epi32( vec, index );
    }
};

template< int index >
struct get_functor< index, int64_t, avx_tag >
{
    int64_t inline operator()( simd_type< int64_t, avx_tag > vec )
    {
        return (int64_t) _mm256_extract_epi64( vec, index );
    }
};

template< int index >
struct get_functor< index, float, avx_tag >
{
    float inline operator()( simd_type< float, avx_tag > vec )
    {
        return _mm_cvtss_f32( _mm_permute_ps( _mm256_extractf128_ps( vec, index >> 2 ),
                                              index & 3 ) );
    }
};

template< int index >
struct get_functor< index, double, avx_tag >
{
    double inline operator()( simd_type< double, avx_tag > vec )
    {
        return _mm256_cvtsd_f64( _mm256_permute4x64_pd( vec, index ) );
    }
};

// Set
// ---------------------------------------------------------------------------------------
template< int index >
struct set_functor< index, int8_t, avx_tag >
{
    simd_type< int8_t, avx_tag > inline
    operator()( simd_type< int8_t, avx_tag > vec, int8_t val )
    {
        return _mm256_insert_epi8( vec, val, index );
    }
};

template< int index >
struct set_functor< index, int16_t, avx_tag >
{
    simd_type< int16_t, avx_tag > inline
    operator()( simd_type< int16_t, avx_tag > vec, int16_t val )
    {
        return _mm256_insert_epi16( vec, val, index );
    }
};

template< int index >
struct set_functor< index, int32_t, avx_tag >
{
    simd_type< int32_t, avx_tag > inline
    operator()( simd_type< int32_t, avx_tag > vec, int32_t val )
    {
        return _mm256_insert_epi32( vec, val, index );
    }
};

template< int index >
struct set_functor< index, int64_t, avx_tag >
{
    simd_type< int64_t, avx_tag > inline
    operator()( simd_type< int64_t, avx_tag > vec, int64_t val )
    {
        return _mm256_insert_epi64( vec, val, index );
    }
};

template< int index >
struct set_functor< index, float, avx_tag >
{
    simd_type< float, avx_tag > inline
    operator()( simd_type< float, avx_tag > vec, float val )
    {
        auto mask = _mm256_cmp_ps(
                _mm256_set1_ps( index ),
                _mm256_set_ps( 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f ),
                _CMP_EQ_OQ );

        return _mm256_blendv_ps( vec, _mm256_set1_ps( val ), mask );
    }
};

template< int index >
struct set_functor< index, double, avx_tag >
{
    simd_type< double, avx_tag > inline
    operator()( simd_type< double, avx_tag > vec, double val )
    {
        auto mask = _mm256_cmp_pd(
                _mm256_set1_pd( index ),
                _mm256_set_pd( 3.0, 2.0, 1.0, 0.0 ),
                _CMP_EQ_OQ );

        return _mm256_blendv_pd( vec, _mm256_set1_pd( val ), mask );
    }
};

// High insert
// ---------------------------------------------------------------------------------------
template<> inline simd_type< int8_t, avx_tag >
high_insert< int8_t, avx_tag >( simd_type< int8_t, avx_tag > vec, int8_t val )
{
    return set_functor<31, int8_t, avx_tag>()(
                set_functor<15, int8_t, avx_tag>()(
                        _mm256_srli_si256( vec, 1 ),
                        get_functor<16, int8_t, avx_tag>()( vec ) ),
                val );
}

template<> inline simd_type< int16_t, avx_tag >
high_insert< int16_t, avx_tag >( simd_type< int16_t, avx_tag > vec, int16_t val )
{
    return set_functor<15, int16_t, avx_tag>()(
                set_functor<7, int16_t, avx_tag>()(
                        _mm256_srli_si256( vec, 2 ),
                        get_functor<8, int16_t, avx_tag>()( vec ) ),
                val );
}

template<> inline simd_type< int32_t, avx_tag >
high_insert< int32_t, avx_tag >( simd_type< int32_t, avx_tag > vec, int32_t val )
{
    return set_functor<7, int32_t, avx_tag>()(
                set_functor<3, int32_t, avx_tag>()(
                        _mm256_srli_si256( vec, 4 ),
                        get_functor<4, int32_t, avx_tag>()( vec ) ),
                val );
}

template<> inline simd_type< int64_t, avx_tag >
high_insert< int64_t, avx_tag >( simd_type< int64_t, avx_tag > vec, int64_t val )
{
    return set_functor<3, int64_t, avx_tag>()(
                _mm256_permute4x64_epi64( vec, _MM_SHUFFLE( 3, 3, 2, 1 ) ),
                val );
}

template<> inline simd_type< float, avx_tag >
high_insert< float, avx_tag >( simd_type< float, avx_tag > vec,
                                float val )
{
    return set_functor<7, float, avx_tag>()(
                set_functor<3, float, avx_tag>()(
                        _mm256_permute_ps( vec, _MM_SHUFFLE( 3, 3, 2, 1 ) ),
                        get_functor<4, float, avx_tag>()( vec ) ),
                val );
}

template<> inline simd_type< double, avx_tag >
high_insert< double, avx_tag >( simd_type< double, avx_tag > vec,
                                double val )
{
    return set_functor<3, double, avx_tag>()(
                _mm256_permute4x64_pd( vec, _MM_SHUFFLE( 3, 3, 2, 1 ) ),
                val );
}

// Low insert
// ---------------------------------------------------------------------------------------
template<> inline simd_type< int8_t, avx_tag >
low_insert< int8_t, avx_tag >( simd_type< int8_t, avx_tag > vec, int8_t val )
{
    return set_functor<0, int8_t, avx_tag>()(
                set_functor<16, int8_t, avx_tag>()(
                        _mm256_slli_si256( vec, 1 ),
                        get_functor<15, int8_t, avx_tag>()( vec ) ),
                val );
}

template<> inline simd_type< int16_t, avx_tag >
low_insert< int16_t, avx_tag >( simd_type< int16_t, avx_tag > vec, int16_t val )
{
    return set_functor<0, int16_t, avx_tag>()(
                set_functor<8, int16_t, avx_tag>()(
                        _mm256_slli_si256( vec, 2 ),
                        get_functor<7, int16_t, avx_tag>()( vec ) ),
                val );
}

template<> inline simd_type< int32_t, avx_tag >
low_insert< int32_t, avx_tag >( simd_type< int32_t, avx_tag > vec, int32_t val )
{
    return set_functor<0, int32_t, avx_tag>()(
                set_functor<4, int32_t, avx_tag>()(
                        _mm256_slli_si256( vec, 4 ),
                        get_functor<3, int32_t, avx_tag>()( vec ) ),
                val );
}

template<> inline simd_type< int64_t, avx_tag >
low_insert< int64_t, avx_tag >( simd_type< int64_t, avx_tag > vec, int64_t val )
{
    return set_functor<0, int64_t, avx_tag>()(
                _mm256_permute4x64_epi64( vec, _MM_SHUFFLE( 2, 1, 0, 0 ) ),
                val );
}

template<> inline simd_type< float, avx_tag >
low_insert< float, avx_tag >( simd_type< float, avx_tag > vec,
                                float val )
{
    return set_functor<0, float, avx_tag>()(
                set_functor<4, float, avx_tag>()(
                        _mm256_permute_ps( vec, _MM_SHUFFLE( 2, 1, 0, 0 ) ),
                        get_functor<3, float, avx_tag>()( vec ) ),
                val );
}

template<> inline simd_type< double, avx_tag >
low_insert< double, avx_tag >( simd_type< double, avx_tag > vec,
                                double val )
{
    return set_functor<0, double, avx_tag>()(
                _mm256_permute4x64_pd( vec, _MM_SHUFFLE( 2, 1, 0, 0 ) ),
                val );
}

} // namespace litesimd

#endif // LITESIMD_HAS_AVX
#endif // LITESIMD_AVX_SHUFFLE_H
