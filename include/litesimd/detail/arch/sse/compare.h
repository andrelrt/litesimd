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

#ifndef LITESIMD_ARCH_SSE_COMPARE_H
#define LITESIMD_ARCH_SSE_COMPARE_H

#ifdef LITESIMD_HAS_SSE

#include <litesimd/types.h>
#include <litesimd/detail/arch/common/compare.h>
#include <litesimd/detail/helper_macros.h>

namespace litesimd {


// Bit scan
// ---------------------------------------------------------------------------------------
template<> inline std::pair<int, bool>
bit_scan_forward< sse_tag >( uint32_t bitmask )
{
#ifdef _WIN32
    unsigned long index;
    return (0 == _BitScanForward( &index, bitmask ))
        ? std::make_pair( -1, false )
        : std::make_pair( index, true );
#else
    return (bitmask == 0)
        ? std::make_pair( -1, false )
        : std::make_pair( _bit_scan_forward( bitmask ), true );
#endif
}

template<> inline std::pair<int, bool>
bit_scan_reverse< sse_tag >( uint32_t bitmask )
{
#ifdef _WIN32
    unsigned long index;
    return (0 == _BitScanReverse( &index, bitmask ))
        ? std::make_pair( -1, false )
        : std::make_pair( index, true );
#else
    return (bitmask == 0)
        ? std::make_pair( -1, false )
        : std::make_pair( _bit_scan_reverse( bitmask ), true );
#endif
}

// Mask to bitmask
// ---------------------------------------------------------------------------------------
#define DEF_MASK_TO_BITMASK( TYPE_T, CMD ) \
template<> inline typename simd_type< TYPE_T, sse_tag >::bitmask_type \
mask_to_bitmask< TYPE_T, sse_tag >( simd_type< TYPE_T, sse_tag > mask ) { \
    return CMD( mask ); \
}
DEF_MASK_TO_BITMASK( int8_t,  _mm_movemask_epi8 )
DEF_MASK_TO_BITMASK( int16_t, _mm_movemask_epi8 )
DEF_MASK_TO_BITMASK( int32_t, _mm_movemask_epi8 )
DEF_MASK_TO_BITMASK( int64_t, _mm_movemask_epi8 )
DEF_MASK_TO_BITMASK( float,   _mm_movemask_ps )
DEF_MASK_TO_BITMASK( double,  _mm_movemask_pd )
#undef DEF_MASK_TO_BITMASK

// Greater than
// ---------------------------------------------------------------------------------------
#define DEF_GREATER_THAN( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, sse_tag > \
greater< TYPE_T, sse_tag >( simd_type< TYPE_T, sse_tag > lhs, \
                                 simd_type< TYPE_T, sse_tag > rhs ) { \
    return CMD( lhs, rhs ); \
}

DEF_GREATER_THAN( int8_t,  _mm_cmpgt_epi8 )
DEF_GREATER_THAN( int16_t, _mm_cmpgt_epi16 )
DEF_GREATER_THAN( int32_t, _mm_cmpgt_epi32 )
DEF_GREATER_THAN( int64_t, _mm_cmpgt_epi64 )
DEF_GREATER_THAN( float,   _mm_cmpgt_ps )
DEF_GREATER_THAN( double,  _mm_cmpgt_pd )
#undef DEF_GREATER_THAN

// Equals
// ---------------------------------------------------------------------------------------
#define DEF_EQUALS( TYPE_T, CMD ) \
template<> inline simd_type< TYPE_T, sse_tag > \
equal_to< TYPE_T, sse_tag >( simd_type< TYPE_T, sse_tag > lhs, \
                           simd_type< TYPE_T, sse_tag > rhs ) { \
    return CMD( lhs, rhs ); \
}

DEF_EQUALS( int8_t,  _mm_cmpeq_epi8 )
DEF_EQUALS( int16_t, _mm_cmpeq_epi16 )
DEF_EQUALS( int32_t, _mm_cmpeq_epi32 )
DEF_EQUALS( int64_t, _mm_cmpeq_epi64 )
DEF_EQUALS( float,   _mm_cmpeq_ps )
DEF_EQUALS( double,  _mm_cmpeq_pd )
#undef DEF_EQUALS

// all_of
// ---------------------------------------------------------------------------------------
template< typename ValueType_T >
struct all_of_bitmask_op< ValueType_T,
              typename std::enable_if<std::is_integral<ValueType_T>::value, sse_tag>::type >
{
    inline bool operator()( typename simd_type< ValueType_T, sse_tag >::bitmask_type bitmask )
    {
        return (bitmask == 0xffff);
    }
};

template< typename ValueType_T >
struct all_of_bitmask_op< ValueType_T,
              typename std::enable_if<std::is_floating_point<ValueType_T>::value, sse_tag>::type >
{
    inline bool operator()( typename simd_type< ValueType_T, sse_tag >::bitmask_type bitmask )
    {
        constexpr static typename simd_type< ValueType_T, sse_tag >::bitmask_type true_mask =
            (1 << simd_type< ValueType_T, sse_tag >::simd_size) -1;
        return (bitmask == true_mask);
    }
};

template< typename ValueType_T >
struct all_of_op< ValueType_T,
              typename std::enable_if<std::is_integral<ValueType_T>::value, sse_tag>::type >
{
    inline bool operator()( simd_type< ValueType_T, sse_tag > mask )
    {
        return !!_mm_test_all_ones( mask );
    }
};

template< typename ValueType_T >
struct all_of_op< ValueType_T,
              typename std::enable_if<std::is_floating_point<ValueType_T>::value, sse_tag>::type >
{
    inline bool operator()( simd_type< ValueType_T, sse_tag > mask )
    {
        return all_of_bitmask_op< ValueType_T, sse_tag >()( mask_to_bitmask< ValueType_T, sse_tag >( mask ) );
    }
};

// none_of
// ---------------------------------------------------------------------------------------
template< typename ValueType_T >
struct none_of_op< ValueType_T,
              typename std::enable_if<std::is_integral<ValueType_T>::value, sse_tag>::type >
{
    inline bool operator()( simd_type< ValueType_T, sse_tag > mask )
    {
        return !!_mm_testz_si128( mask, simd_type< ValueType_T, sse_tag >::ones() );
    }
};

template< typename ValueType_T >
struct none_of_op< ValueType_T,
              typename std::enable_if<std::is_floating_point<ValueType_T>::value, sse_tag>::type >
{
    inline bool operator()( simd_type< ValueType_T, sse_tag > mask )
    {
        return (0 == mask_to_bitmask< ValueType_T, sse_tag >( mask ) );
    }
};

} // namespace litesimd

#endif // LITESIMD_HAS_SSE
#endif // LITESIMD_ARCH_SSE_COMPARE_H
