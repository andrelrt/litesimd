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

#include <litesimd/types.h>
#include <litesimd/compare.h>
#include "gtest/gtest.h"

namespace ls = litesimd;

template <typename T> class SimdCompareTypes: public ::testing::Test {};

using SimdTypes = ::testing::Types<
#ifdef __SSE2__
    std::pair<int8_t, ls::sse_tag>, std::pair<int16_t, ls::sse_tag>,
    std::pair<int32_t, ls::sse_tag>, std::pair<int64_t, ls::sse_tag>,
    std::pair<float, ls::sse_tag>, std::pair<double, ls::sse_tag>
#ifdef __AVX2__
    , std::pair<int8_t, ls::avx_tag>, std::pair<int16_t, ls::avx_tag>,
    std::pair<int32_t, ls::avx_tag>, std::pair<int64_t, ls::avx_tag>,
    std::pair<float, ls::avx_tag>, std::pair<double, ls::avx_tag>
#endif //__AVX2__
#endif //__SSE2__
>;
TYPED_TEST_CASE(SimdCompareTypes, SimdTypes);

#ifdef __SSE2__
TYPED_TEST(SimdCompareTypes, GreaterThanTypedTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;
    using bitmask_type = typename ls::simd_type< type, tag >::bitmask_type;
    constexpr size_t size = ls::simd_type< type, tag >::simd_size;

    simd cmp;
    type* pCmp = reinterpret_cast<type*>( &cmp );
    type val = 2;

    for( size_t i = 0; i < size; ++i )
    {
        pCmp[ i ] = val;
        val += 2;
    }

    bitmask_type mask = 0;
    val = 1;

    for( bitmask_type i = 0; i < size+1; ++i )
    {
        EXPECT_EQ( mask, (ls::greater_bitmask< type, tag >( val, cmp )) )
            << "val: " << (int) val
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_bitmask< type, tag >( val, cmp );
        EXPECT_EQ( mask, (ls::greater_bitmask< type, tag >( val + 1, cmp )) )
            << "val: " << (int) val + 1
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_bitmask< type, tag >( val, cmp );

        EXPECT_EQ( i-1, (ls::greater_last_index< type, tag >( val, cmp )) )
            << "val: " << (int) val
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_last_index< type, tag >( val, cmp );
        EXPECT_EQ( i-1, (ls::greater_last_index< type, tag >( val + 1, cmp )) )
            << "val: " << (int) val + 1
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_last_index< type, tag >( val, cmp );

        val += 2;
        mask <<= std::is_integral< type >::value ? sizeof(type) : 1;
        mask |= std::is_integral< type >::value ? (1 << sizeof(type)) - 1 : 1;
    }
}

TYPED_TEST(SimdCompareTypes, EqualsTypedTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;
    using bitmask_type = typename ls::simd_type< type, tag >::bitmask_type;
    constexpr size_t size = ls::simd_type< type, tag >::simd_size;

    simd cmp;
    type* pCmp = reinterpret_cast<type*>( &cmp );
    type val = 2;

    for( size_t i = 0; i < size; ++i )
    {
        pCmp[ i ] = val;
        val += 2;
    }

    bitmask_type mask = 0;
    val = 0;

    for( bitmask_type i = 0; i < size+1; ++i )
    {
        EXPECT_EQ( mask, (ls::equal_to_bitmask< type, tag >( val, cmp )) )
            << "val: " << (int)val
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::equal_to_bitmask< type, tag >( val, cmp );
        EXPECT_EQ( 0u, (ls::equal_to_bitmask< type, tag >( val + 1, cmp )) )
            << "val: " << (int)val + 1
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::equal_to_bitmask< type, tag >( val, cmp );

        EXPECT_EQ( i-1, (ls::equal_to_last_index< type, tag >( val, cmp )) )
            << "val: " << (int) val
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::equal_to_last_index< type, tag >( val, cmp );
        EXPECT_EQ( -1, (ls::equal_to_last_index< type, tag >( val + 1, cmp )) )
            << "val: " << (int) val + 1
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::equal_to_last_index< type, tag >( val, cmp );

        val += 2;
        mask <<= std::is_integral< type >::value ? sizeof(type) : 1;
        if( i == 0 )
            mask = std::is_integral< type >::value ? (1 << sizeof(type)) - 1 : 1;
    }
}
#endif //__SSE2__

TEST(SimdCompareTest, GreaterThanDefault)
{
    ls::t_int32_simd cmp;
    int32_t* pCmp = reinterpret_cast<int32_t*>( &cmp );
    int32_t val = 2;
    for( size_t i = 0; i < ls::int32_simd_size; ++i )
    {
        pCmp[ i ] = val;
        val += 2;
    }
    ls::t_bitmask mask = 0;
    val = 1;

    for( ls::t_bitmask i = 0; i < ls::int32_simd_size + 1; ++i )
    {
        EXPECT_EQ( mask, ls::greater_bitmask( val, cmp ) )
            << "val: " << val
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_bitmask( val, cmp );
        EXPECT_EQ( mask, ls::greater_bitmask( val + 1, cmp ) )
            << "val: " << val + 1
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_bitmask( val, cmp );

        EXPECT_EQ( i-1, ls::greater_last_index( val, cmp ) )
            << "val: " << val
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_last_index( val, cmp );
        EXPECT_EQ( i-1, ls::greater_last_index( val + 1, cmp ) )
            << "val: " << val + 1
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_last_index( val, cmp );

        val += 2;
        mask <<= 4;
        mask |= 0xf;
    }
}

TEST(SimdCompareTest, GreaterThanDefaultSimd)
{
    ls::t_int32_simd cmp;
    int32_t* pCmp = reinterpret_cast<int32_t*>( &cmp );
    int32_t val = 2;
    for( size_t i = 0; i < ls::int32_simd_size; ++i )
    {
        pCmp[ i ] = val;
        val += 2;
    }
    ls::t_bitmask mask = 0;
    val = 1;

    for( size_t i = 0; i < ls::int32_simd_size + 1; ++i )
    {
        auto simdVal = ls::from_value( val );
        EXPECT_EQ( mask, ls::greater_bitmask( simdVal, cmp ) )
            << "val: " << val
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_bitmask( simdVal, cmp );
        EXPECT_EQ( i, 1 + ls::greater_last_index( simdVal, cmp ) )
            << "val: " << val
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_last_index( simdVal, cmp );

        simdVal = ls::from_value( val + 1 );
        EXPECT_EQ( mask, ls::greater_bitmask( simdVal, cmp ) )
            << "val: " << val + 1
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_bitmask( simdVal, cmp );
        EXPECT_EQ( i, 1 + ls::greater_last_index( simdVal, cmp ) )
            << "val: " << val + 1
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_last_index( simdVal, cmp );

        val += 2;
        mask <<= 4;
        mask |= 0xf;
    }
}
