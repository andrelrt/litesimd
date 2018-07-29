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

#ifdef __SSE2__
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

    typename ls::traits< TypeParam, ls::sse_tag >::bitmask_type mask = 0;
    val = 1;

    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_EQ( mask, (ls::greater_than_bitmask< TypeParam, ls::sse_tag >( val, cmp )) )
            << "val: " << val
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_than_bitmask< TypeParam, ls::sse_tag >( val, cmp );
        val += 2;
        mask <<= std::is_integral< TypeParam >::value ? sizeof(TypeParam) : 1;
        mask |= std::is_integral< TypeParam >::value ? (1 << sizeof(TypeParam)) - 1 : 1;
    }
}
#endif //__SSE2__

#ifdef __AVX2__
TYPED_TEST(SimdCompareTypes, GreaterThanAVXTypedTest)
{
    using simd = typename ls::traits< TypeParam, ls::avx_tag >::simd_type;
    constexpr size_t size = ls::traits< TypeParam, ls::avx_tag >::simd_size;

    simd cmp;
    TypeParam* pCmp = reinterpret_cast<TypeParam*>( &cmp );
    TypeParam val = 2;

    for( size_t i = 0; i < size; ++i )
    {
        pCmp[ i ] = val;
        val += 2;
    }

    typename ls::traits< TypeParam, ls::avx_tag >::bitmask_type mask = 0;
    val = 1;

    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_EQ( mask, (ls::greater_than_bitmask< TypeParam, ls::avx_tag >( val, cmp )) )
            << "val: " << val
            << " - hex: 0x" << std::hex << std::setw(8) << std::setfill( '0' )
            << ls::greater_than_bitmask< TypeParam, ls::avx_tag >( val, cmp );
        val += 2;
        mask <<= std::is_integral< TypeParam >::value ? sizeof(TypeParam) : 1;
        mask |= std::is_integral< TypeParam >::value ? (1 << sizeof(TypeParam)) - 1 : 1;
    }
}
#endif //__AVX2__

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

