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

using SimdTypes = ::testing::Types<int8_t, int16_t, int32_t, int64_t, float, double>;
TYPED_TEST_CASE(SimdCompareTypes, SimdTypes);

TYPED_TEST(SimdCompareTypes, GreaterThanSSETypedTest)
{
    using simd = typename ls::traits< TypeParam, ls::sse_tag >::simd_type;
    constexpr size_t size = ls::traits< TypeParam, ls::sse_tag >::simd_size;

    simd cmp;
    TypeParam* pCmp = reinterpret_cast<TypeParam*>( &cmp );
    TypeParam val = 2;

    for( size_t i = 0; i < size; ++i )
    {
        pCmp[ i ] = val;
        val += 2;
    }

    ls::t_bitmask mask = 0;
    val = 1;

    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_EQ( mask, ls::greater_than_bitmask( val, cmp ) )
            << "val: " << val
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_than_bitmask( val, cmp );
        val += 2;
        mask <<= std::is_integral< TypeParam >::value ? sizeof(TypeParam) : 1;
        mask |= std::is_integral< TypeParam >::value ? (1 << sizeof(TypeParam)) - 1 : 1;
    }
}

TEST(SimdCompareTest, GreaterThanDefault)
{
    ls::t_int32_simd cmp;
    int32_t* pCmp = reinterpret_cast<int32_t*>( &cmp );
    int32_t val = 10;
    for( size_t i = 0; i < ls::int32_simd_size; ++i )
    {
        pCmp[ i ] = val;
        val += 10;
    }
    ls::t_bitmask mask = 0;
    val = 5;

    for( size_t i = 0; i < ls::int32_simd_size; ++i )
    {
        EXPECT_EQ( mask, ls::greater_than_bitmask( val, cmp ) )
            << "val: " << val
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_than_bitmask( val, cmp );
        val += 10;
        mask <<= 4;
        mask |= 0xf;
    }
}

#ifdef __SSE2__
TEST(SimdCompareTest, GreaterThanSSE)
{
    using simd32 = typename ls::traits< int32_t, ls::sse_tag >::simd_type;
    constexpr size_t size32 = ls::traits< int32_t, ls::sse_tag >::simd_size;

    simd32 cmp;
    int32_t* pCmp = reinterpret_cast<int32_t*>( &cmp );
    int32_t val = 10;
    for( size_t i = 0; i < size32; ++i )
    {
        pCmp[ i ] = val;
        val += 10;
    }
    // cmp       = {40, 30, 20, 10}
    // cmp[i], i =   3   2   1   0

    EXPECT_EQ( 0x0000u, (ls::greater_than_bitmask< int32_t, ls::sse_tag >(  5, cmp )) ) << "hex: 0x" << std::hex
        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::sse_tag >( 5, cmp );
    EXPECT_EQ( 0x000fu, (ls::greater_than_bitmask< int32_t, ls::sse_tag >( 15, cmp )) ) << "hex: 0x" << std::hex
        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::sse_tag >( 15, cmp );
    EXPECT_EQ( 0x00ffu, (ls::greater_than_bitmask< int32_t, ls::sse_tag >( 25, cmp )) ) << "hex: 0x" << std::hex
        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::sse_tag >( 25, cmp );
    EXPECT_EQ( 0x0fffu, (ls::greater_than_bitmask< int32_t, ls::sse_tag >( 35, cmp )) ) << "hex: 0x" << std::hex
        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::sse_tag >( 35, cmp );
    EXPECT_EQ( 0xffffu, (ls::greater_than_bitmask< int32_t, ls::sse_tag >( 45, cmp )) ) << "hex: 0x" << std::hex
        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::sse_tag >( 45, cmp );

//    EXPECT_EQ( 0u, (ls::greater_than_index< int32_t, ls::sse_tag >(  5, cmp )) );
//    EXPECT_EQ( 0u, (ls::greater_than_index< int32_t, ls::sse_tag >( 10, cmp )) );
//    EXPECT_EQ( 1u, (ls::greater_than_index< int32_t, ls::sse_tag >( 15, cmp )) );
//    EXPECT_EQ( 1u, (ls::greater_than_index< int32_t, ls::sse_tag >( 20, cmp )) );
//    EXPECT_EQ( 2u, (ls::greater_than_index< int32_t, ls::sse_tag >( 25, cmp )) );
//    EXPECT_EQ( 2u, (ls::greater_than_index< int32_t, ls::sse_tag >( 30, cmp )) );
//    EXPECT_EQ( 3u, (ls::greater_than_index< int32_t, ls::sse_tag >( 35, cmp )) );
//    EXPECT_EQ( 3u, (ls::greater_than_index< int32_t, ls::sse_tag >( 40, cmp )) );
//    EXPECT_EQ( 4u, (ls::greater_than_index< int32_t, ls::sse_tag >( 45, cmp )) );

    pCmp[3] = 0x00007891;
    pCmp[2] = 0x00005576;
    pCmp[1] = 0x000041a7;
    pCmp[0] = 0x00002b83;

    EXPECT_EQ( 0x000fu, (ls::greater_than_bitmask< int32_t, ls::sse_tag >( 0x41a7, cmp )) ) << "hex: 0x" << std::hex
        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::sse_tag >( 0x41a7, cmp );
}
#endif //__SSE2__

//TEST(SimdCompareTest, GreaterThanAVX)
//{
//    using simd32 = typename ls::traits< int32_t, ls::avx_tag >::simd_type;
//    constexpr size_t size32 = ls::traits< int32_t, ls::avx_tag >::simd_size;
//
//    simd32 cmp;
//    int32_t* pCmp = reinterpret_cast<int32_t*>( &cmp );
//    int32_t val = 10;
//    for( size_t i = 0; i < size32; ++i )
//    {
//        pCmp[ i ] = val;
//        val += 10;
//    }
//    // cmp       = {80, 70, 60, 50, 40, 30, 20, 10}
//    // cmp[i], i =   7   6   5   4   3   2   1   0
//
//    EXPECT_EQ( 0x00000000u, (ls::greater_than_bitmask< int32_t, ls::avx_tag >(  5, cmp )) ) << "hex: 0x" << std::hex
//        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::avx_tag >(  5, cmp );
//    EXPECT_EQ( 0x0000000fu, (ls::greater_than_bitmask< int32_t, ls::avx_tag >( 15, cmp )) ) << "hex: 0x" << std::hex
//        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::avx_tag >( 15, cmp );
//    EXPECT_EQ( 0x000000ffu, (ls::greater_than_bitmask< int32_t, ls::avx_tag >( 25, cmp )) ) << "hex: 0x" << std::hex
//        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::avx_tag >( 25, cmp );
//    EXPECT_EQ( 0x00000fffu, (ls::greater_than_bitmask< int32_t, ls::avx_tag >( 35, cmp )) ) << "hex: 0x" << std::hex
//        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::avx_tag >( 35, cmp );
//    EXPECT_EQ( 0x0000ffffu, (ls::greater_than_bitmask< int32_t, ls::avx_tag >( 45, cmp )) ) << "hex: 0x" << std::hex
//        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::avx_tag >( 45, cmp );
//    EXPECT_EQ( 0x000fffffu, (ls::greater_than_bitmask< int32_t, ls::avx_tag >( 55, cmp )) ) << "hex: 0x" << std::hex
//        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::avx_tag >( 55, cmp );
//    EXPECT_EQ( 0x00ffffffu, (ls::greater_than_bitmask< int32_t, ls::avx_tag >( 65, cmp )) ) << "hex: 0x" << std::hex
//        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::avx_tag >( 65, cmp );
//    EXPECT_EQ( 0x0fffffffu, (ls::greater_than_bitmask< int32_t, ls::avx_tag >( 75, cmp )) ) << "hex: 0x" << std::hex
//        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::avx_tag >( 75, cmp );
//    EXPECT_EQ( 0xffffffffu, (ls::greater_than_bitmask< int32_t, ls::avx_tag >( 85, cmp )) ) << "hex: 0x" << std::hex
//        << std::setw(8) << std::setfill( '0' ) << ls::greater_than_bitmask< int32_t, ls::avx_tag >( 85, cmp );
//
//    EXPECT_EQ( 0u, (ls::greater_than_index< int32_t, ls::avx_tag >(  5, cmp )) );
//    EXPECT_EQ( 0u, (ls::greater_than_index< int32_t, ls::avx_tag >( 10, cmp )) );
//    EXPECT_EQ( 1u, (ls::greater_than_index< int32_t, ls::avx_tag >( 15, cmp )) );
//    EXPECT_EQ( 1u, (ls::greater_than_index< int32_t, ls::avx_tag >( 20, cmp )) );
//    EXPECT_EQ( 2u, (ls::greater_than_index< int32_t, ls::avx_tag >( 25, cmp )) );
//    EXPECT_EQ( 2u, (ls::greater_than_index< int32_t, ls::avx_tag >( 30, cmp )) );
//    EXPECT_EQ( 3u, (ls::greater_than_index< int32_t, ls::avx_tag >( 35, cmp )) );
//    EXPECT_EQ( 3u, (ls::greater_than_index< int32_t, ls::avx_tag >( 40, cmp )) );
//    EXPECT_EQ( 4u, (ls::greater_than_index< int32_t, ls::avx_tag >( 45, cmp )) );
//    EXPECT_EQ( 4u, (ls::greater_than_index< int32_t, ls::avx_tag >( 50, cmp )) );
//    EXPECT_EQ( 5u, (ls::greater_than_index< int32_t, ls::avx_tag >( 55, cmp )) );
//    EXPECT_EQ( 5u, (ls::greater_than_index< int32_t, ls::avx_tag >( 60, cmp )) );
//    EXPECT_EQ( 6u, (ls::greater_than_index< int32_t, ls::avx_tag >( 65, cmp )) );
//    EXPECT_EQ( 6u, (ls::greater_than_index< int32_t, ls::avx_tag >( 70, cmp )) );
//    EXPECT_EQ( 7u, (ls::greater_than_index< int32_t, ls::avx_tag >( 75, cmp )) );
//    EXPECT_EQ( 7u, (ls::greater_than_index< int32_t, ls::avx_tag >( 80, cmp )) );
//    EXPECT_EQ( 8u, (ls::greater_than_index< int32_t, ls::avx_tag >( 85, cmp )) );
//}
//
