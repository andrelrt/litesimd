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

#ifndef LITESIMD_SSE_SHUFFLE_H
#define LITESIMD_SSE_SHUFFLE_H

#ifdef LITESIMD_HAS_SSE

#include <smmintrin.h>
#include <litesimd/types.h>
#include <litesimd/detail/arch/common/shuffle.h>

namespace litesimd {

// Blend
// ---------------------------------------------------------------------------------------
#define DEF_BLEND( TYPE_T, BLEND_CMD ) \
template<> inline simd_type< TYPE_T, sse_tag > \
blend< TYPE_T, sse_tag >( simd_type< TYPE_T, sse_tag > mask, \
                          simd_type< TYPE_T, sse_tag > trueVal, \
                          simd_type< TYPE_T, sse_tag > falseVal ) { \
    return BLEND_CMD( falseVal, trueVal, mask ); }

DEF_BLEND( int8_t,  _mm_blendv_epi8 )
DEF_BLEND( int16_t, _mm_blendv_epi8 )
DEF_BLEND( int32_t, _mm_blendv_epi8 )
DEF_BLEND( int64_t, _mm_blendv_epi8 )
DEF_BLEND( float,   _mm_blendv_ps )
DEF_BLEND( double,  _mm_blendv_pd )
#undef DEF_BLEND

// Get
// ---------------------------------------------------------------------------------------
template< int index >
struct get_functor< index, int8_t, sse_tag >
{
    int8_t inline operator()( simd_type< int8_t, sse_tag > vec )
    {
        return _mm_extract_epi8( vec, index );
    }
};

template< int index >
struct get_functor< index, int16_t, sse_tag >
{
    int16_t inline operator()( simd_type< int16_t, sse_tag > vec )
    {
        return _mm_extract_epi16( vec, index );
    }
};

template< int index >
struct get_functor< index, int32_t, sse_tag >
{
    int32_t inline operator()( simd_type< int32_t, sse_tag > vec )
    {
        return _mm_extract_epi32( vec, index );
    }
};

template< int index >
struct get_functor< index, int64_t, sse_tag >
{
    int64_t inline operator()( simd_type< int64_t, sse_tag > vec )
    {
        return _mm_extract_epi64( vec, index );
    }
};

template< int index >
struct get_functor< index, float, sse_tag >
{
    float inline operator()( simd_type< float, sse_tag > vec )
    {
        return _mm_cvtss_f32( _mm_shuffle_ps( vec, vec, index ) );
    }
};

template< int index >
struct get_functor< index, double, sse_tag >
{
    double inline operator()( simd_type< double, sse_tag > vec )
    {
        return _mm_cvtsd_f64( _mm_shuffle_pd( vec, vec, index ) );
    }
};

// Set
// ---------------------------------------------------------------------------------------
template< int index >
struct set_functor< index, int8_t, sse_tag >
{
    simd_type< int8_t, sse_tag > inline
    operator()( simd_type< int8_t, sse_tag > vec, int8_t val )
    {
        return _mm_insert_epi8( vec, val, index );
    }
};

template< int index >
struct set_functor< index, int16_t, sse_tag >
{
    simd_type< int16_t, sse_tag > inline
    operator()( simd_type< int16_t, sse_tag > vec, int16_t val )
    {
        return _mm_insert_epi16( vec, val, index );
    }
};

template< int index >
struct set_functor< index, int32_t, sse_tag >
{
    simd_type< int32_t, sse_tag > inline
    operator()( simd_type< int32_t, sse_tag > vec, int32_t val )
    {
        return _mm_insert_epi32( vec, val, index );
    }
};

template< int index >
struct set_functor< index, int64_t, sse_tag >
{
    simd_type< int64_t, sse_tag > inline
    operator()( simd_type< int64_t, sse_tag > vec, int64_t val )
    {
        return _mm_insert_epi64( vec, val, index );
    }
};

template< int index >
struct set_functor< index, float, sse_tag >
{
    simd_type< float, sse_tag > inline
    operator()( simd_type< float, sse_tag > vec, float val )
    {
        return _mm_insert_ps( vec, _mm_set1_ps( val ), _MM_SHUFFLE( 0, index, 0, 0 ) );
    }
};

template<>
struct set_functor< 0, double, sse_tag >
{
    simd_type< double, sse_tag > inline
    operator()( simd_type< double, sse_tag > vec, double val )
    {
        return _mm_shuffle_pd( _mm_set1_pd( val ), vec, 2 );
    }
};

template<>
struct set_functor< 1, double, sse_tag >
{
    simd_type< double, sse_tag > inline
    operator()( simd_type< double, sse_tag > vec, double val )
    {
        return _mm_shuffle_pd( vec, _mm_set1_pd( val ), 0 );
    }
};

// High insert
// ---------------------------------------------------------------------------------------
template<> inline simd_type< int8_t, sse_tag >
high_insert< int8_t, sse_tag >( simd_type< int8_t, sse_tag > vec, int8_t val )
{
    return set_functor<15, int8_t, sse_tag>()( _mm_srli_si128( vec, 1 ), val );
}

template<> inline simd_type< int16_t, sse_tag >
high_insert< int16_t, sse_tag >( simd_type< int16_t, sse_tag > vec, int16_t val )
{
    return set_functor<7, int16_t, sse_tag>()( _mm_srli_si128( vec, 2 ), val );
}

template<> inline simd_type< int32_t, sse_tag >
high_insert< int32_t, sse_tag >( simd_type< int32_t, sse_tag > vec, int32_t val )
{
    return set_functor<3, int32_t, sse_tag>()( _mm_shuffle_epi32( vec, _MM_SHUFFLE( 3, 3, 2, 1 ) ), val );
}

template<> inline simd_type< int64_t, sse_tag >
high_insert< int64_t, sse_tag >( simd_type< int64_t, sse_tag > vec, int64_t val )
{
    return set_functor<1, int64_t, sse_tag>()( _mm_shuffle_epi32( vec, _MM_SHUFFLE( 3, 3, 3, 2 ) ), val );
}

template<> inline simd_type< float, sse_tag >
high_insert< float, sse_tag >( simd_type< float, sse_tag > vec,
                                float val )
{
    return set_functor<3, float, sse_tag>()( _mm_shuffle_ps( vec, vec, _MM_SHUFFLE( 3, 3, 2, 1 ) ), val );
}

template<> inline simd_type< double, sse_tag >
high_insert< double, sse_tag >( simd_type< double, sse_tag > vec,
                                double val )
{
    return set_functor<1, double, sse_tag>()( _mm_shuffle_pd( vec, vec, 1 ), val );
}

// Low insert
// ---------------------------------------------------------------------------------------
template<> inline simd_type< int8_t, sse_tag >
low_insert< int8_t, sse_tag >( simd_type< int8_t, sse_tag > vec, int8_t val )
{
    return set_functor<0, int8_t, sse_tag>()( _mm_slli_si128( vec, 1 ), val );
}

template<> inline simd_type< int16_t, sse_tag >
low_insert< int16_t, sse_tag >( simd_type< int16_t, sse_tag > vec, int16_t val )
{
    return set_functor<0, int16_t, sse_tag>()( _mm_slli_si128( vec, 2 ), val );
}

template<> inline simd_type< int32_t, sse_tag >
low_insert< int32_t, sse_tag >( simd_type< int32_t, sse_tag > vec, int32_t val )
{
    return set_functor<0, int32_t, sse_tag>()( _mm_shuffle_epi32( vec, _MM_SHUFFLE( 2, 1, 0, 0 ) ), val );
}

template<> inline simd_type< int64_t, sse_tag >
low_insert< int64_t, sse_tag >( simd_type< int64_t, sse_tag > vec, int64_t val )
{
    return set_functor<0, int64_t, sse_tag>()( _mm_shuffle_epi32( vec, _MM_SHUFFLE( 1, 0, 0, 0 ) ), val );
}

template<> inline simd_type< float, sse_tag >
low_insert< float, sse_tag >( simd_type< float, sse_tag > vec,
                                float val )
{
    return set_functor<0, float, sse_tag>()( _mm_shuffle_ps( vec, vec, _MM_SHUFFLE( 2, 1, 0, 0 ) ), val );
}

template<> inline simd_type< double, sse_tag >
low_insert< double, sse_tag >( simd_type< double, sse_tag > vec,
                                double val )
{
    return set_functor<0, double, sse_tag>()( _mm_shuffle_pd( vec, vec, 0 ), val );
}

} // namespace litesimd

#endif // LITESIMD_HAS_SSE
#endif // LITESIMD_SSE_SHUFFLE_H
