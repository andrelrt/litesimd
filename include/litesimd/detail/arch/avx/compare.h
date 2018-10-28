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

#ifndef LITESIMD_ARCH_AVX_COMPARE_H
#define LITESIMD_ARCH_AVX_COMPARE_H

#ifdef LITESIMD_HAS_AVX

#include <litesimd/types.h>
#include <litesimd/detail/arch/common/compare.h>
#include <litesimd/detail/arch/sse/compare.h>
#include <litesimd/detail/arch/avx/bitwise.h>

namespace litesimd {

// Bit scan
// ---------------------------------------------------------------------------------------
template<> inline std::pair<int, bool>
bit_scan_forward< avx_tag >( uint32_t bitmask )
{
    return bit_scan_forward< sse_tag >( bitmask );
}

template<> inline std::pair<int, bool>
bit_scan_reverse< avx_tag >( uint32_t bitmask )
{
    return bit_scan_reverse< sse_tag >( bitmask );
}

// Mask to bitmask
// ---------------------------------------------------------------------------------------
#define DEF_MASK_TO_BITMASK( TYPE_T, CMD ) \
template<> inline typename simd_type< TYPE_T, avx_tag >::bitmask_type \
mask_to_bitmask< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > mask ) { \
    return CMD( mask ); \
}

DEF_MASK_TO_BITMASK( int8_t,  _mm256_movemask_epi8 )
DEF_MASK_TO_BITMASK( int16_t, _mm256_movemask_epi8 )
DEF_MASK_TO_BITMASK( int32_t, _mm256_movemask_epi8 )
DEF_MASK_TO_BITMASK( int64_t, _mm256_movemask_epi8 )
DEF_MASK_TO_BITMASK( float,   _mm256_movemask_ps )
DEF_MASK_TO_BITMASK( double,  _mm256_movemask_pd )

#undef DEF_MASK_TO_BITMASK

// Greater than
// ---------------------------------------------------------------------------------------
#define DEF_GREATER_THAN( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, avx_tag > \
greater< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > lhs, \
                                 simd_type< TYPE_T, avx_tag > rhs ) { \
    return CMD( lhs, rhs ); \
}

DEF_GREATER_THAN( int8_t,  _mm256_cmpgt_epi8 )
DEF_GREATER_THAN( int16_t, _mm256_cmpgt_epi16 )
DEF_GREATER_THAN( int32_t, _mm256_cmpgt_epi32 )
DEF_GREATER_THAN( int64_t, _mm256_cmpgt_epi64 )

#undef DEF_GREATER_THAN

template<> inline simd_type< float, avx_tag >
greater< float, avx_tag >( simd_type< float, avx_tag > lhs,
                                simd_type< float, avx_tag > rhs )
{
    // Quietly ignore NaN
    return _mm256_cmp_ps( lhs, rhs, _CMP_GT_OQ );
}

template<> inline simd_type< double, avx_tag >
greater< double, avx_tag >( simd_type< double, avx_tag > lhs,
                                 simd_type< double, avx_tag > rhs )
{
    // Quietly ignore NaN
    return _mm256_cmp_pd( lhs, rhs, _CMP_GT_OQ );
}

// Equals
// ---------------------------------------------------------------------------------------
#define DEF_EQUALS( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, avx_tag > \
equal_to< TYPE_T, avx_tag >( simd_type< TYPE_T, avx_tag > lhs, \
                           simd_type< TYPE_T, avx_tag > rhs ) { \
    return CMD( lhs, rhs ); \
}

DEF_EQUALS( int8_t,  _mm256_cmpeq_epi8 )
DEF_EQUALS( int16_t, _mm256_cmpeq_epi16 )
DEF_EQUALS( int32_t, _mm256_cmpeq_epi32 )
DEF_EQUALS( int64_t, _mm256_cmpeq_epi64 )

#undef DEF_EQUALS

template<> inline simd_type< float, avx_tag >
equal_to< float, avx_tag >( simd_type< float, avx_tag > lhs,
                          simd_type< float, avx_tag > rhs )
{
    // Quietly ignore NaN
    return _mm256_cmp_ps( lhs, rhs, _CMP_EQ_OQ );
}

template<> inline simd_type< double, avx_tag >
equal_to< double, avx_tag >( simd_type< double, avx_tag > lhs,
                           simd_type< double, avx_tag > rhs )
{
    // Quietly ignore NaN
    return _mm256_cmp_pd( lhs, rhs, _CMP_EQ_OQ );
}

// none_of
// ---------------------------------------------------------------------------------------
template< typename ValueType_T >
struct none_of_op< ValueType_T, avx_tag >
{
    inline bool operator()( simd_type< ValueType_T, avx_tag > mask )
    {
        return !!_mm256_testz_si256( mask, simd_type< ValueType_T, avx_tag >::ones() );
    }
};

// all_of
// ---------------------------------------------------------------------------------------
template< typename ValueType_T >
struct all_of_op< ValueType_T, avx_tag >
{
    inline bool operator()( simd_type< ValueType_T, avx_tag > mask )
    {
        return none_of_op< ValueType_T, avx_tag >()( bit_not( mask ) );
    }
};

template< typename ValueType_T >
struct all_of_bitmask_op< ValueType_T,
              typename std::enable_if<std::is_integral<ValueType_T>::value, avx_tag>::type >
{
    inline bool operator()( typename simd_type< ValueType_T, avx_tag >::bitmask_type bitmask )
    {
        return (bitmask == 0xffffffff);
    }
};

template< typename ValueType_T >
struct all_of_bitmask_op< ValueType_T,
              typename std::enable_if<std::is_floating_point<ValueType_T>::value, avx_tag>::type >
{
    inline bool operator()( typename simd_type< ValueType_T, avx_tag >::bitmask_type bitmask )
    {
        constexpr static typename simd_type< ValueType_T, avx_tag >::bitmask_type true_mask =
            (1 << simd_type< ValueType_T, avx_tag >::simd_size) -1;
        return (bitmask == true_mask);
    }
};

} // namespace litesimd

#endif // LITESIMD_HAS_AVX
#endif // LITESIMD_ARCH_AVX_COMPARE_H
