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
#include <litesimd/arithmetic.h>
#include "gtest/gtest.h"

namespace ls = litesimd;

template <typename T> class ArithmeticTypedTest: public ::testing::Test {};

using TestTypes = ::testing::Types<
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
TYPED_TEST_CASE(ArithmeticTypedTest, TestTypes);

using TagTypes = ::testing::Types<
#ifdef __SSE2__
ls::sse_tag
#ifdef __AVX2__
, ls::avx_tag
#endif //__AVX2__
#endif //__SSE2__
>;

TYPED_TEST_CASE(ArithmeticTaggedTest, TagTypes);

#ifdef __SSE2__
TYPED_TEST(ArithmeticTypedTest, AddTypedTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;
    constexpr size_t size = ls::simd_type< type, tag >::simd_size;

    simd a = ls::simd_type< type, tag >( 1 );
    simd b = ls::simd_type< type, tag >( 2 );
    simd c = ls::add< type, tag >( a, b );

    type* pCmp = reinterpret_cast<type*>( &c );
    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_EQ( 3, pCmp[ i ] );
    }

    simd d = a + b;

    pCmp = reinterpret_cast<type*>( &d );
    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_EQ( 3, pCmp[ i ] );
    }
}

TYPED_TEST(ArithmeticTypedTest, SubTypedTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;
    constexpr size_t size = ls::simd_type< type, tag >::simd_size;

    simd a = ls::simd_type< type, tag >( 8 );
    simd b = ls::simd_type< type, tag >( 3 );
    simd c = ls::sub< type, tag >( a, b );

    type* pCmp = reinterpret_cast<type*>( &c );
    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_EQ( 5, pCmp[ i ] );
    }

    simd d = a - b;

    pCmp = reinterpret_cast<type*>( &d );
    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_EQ( 5, pCmp[ i ] );
    }
}

TYPED_TEST(ArithmeticTaggedTest, MulLoHiTest)
{
    using tag = TypeParam;
    using simd16 = ls::simd_type< int16_t, tag >;
    using simd32 = ls::simd_type< int32_t, tag >;

    simd16 a = simd16( 0x2003 );
    simd16 b = simd16( 0x30 );
    simd16 c = ls::mullo( a, b );

    int16_t* pCmp = reinterpret_cast<int16_t*>( &c );
    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_EQ( 0x90, pCmp[ i ] );
    }

    c = ls::mulhi( a, b );
    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_EQ( 6, pCmp[ i ] );
    }

    simd32 d = simd32( 0x20000003 );
    simd32 e = simd32( 0x30 );
    simd32 f = ls::mullo( d, e );

    int32_t* pF = reinterpret_cast<int32_t*>( &f );
    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_EQ( 0x90, pF[ i ] );
    }
}

TYPED_TEST(ArithmeticTaggedTest, DivTest)
{
    using tag = TypeParam;
    using simdf = ls::simd_type< float, tag >;
    using simdd = ls::simd_type< double, tag >;

    simdf a = simdf( 20 );
    simdf b = simdf( 2 );
    simdf c = ls::div( a, b );

    float* pCmp = reinterpret_cast<float*>( &c );
    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_FLOAT_EQ( 10, pCmp[ i ] );
    }

    simdd d = simdd( 20 );
    simdd e = simdd( 2 );
    simdd f = ls::div( a, b );

    double* pF = reinterpret_cast<double*>( &f );
    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_DOUBLE_EQ( 10, pF[ i ] );
    }
}
#endif //__SSE2__
