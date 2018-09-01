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
#include <litesimd/algorithm.h>
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

    simd a = simd( 1 );
    simd b = simd( 2 );

    ls::for_each( ls::bit_add< type, tag >( a, b ), []( int index, type val )
    {
        EXPECT_EQ( 3, val ) << "Error on index " << index;
        return true;
    } );

    ls::for_each( a + b, []( int index, type val )
    {
        EXPECT_EQ( 3, val ) << "Error on index " << index;
        return true;
    } );
}

TYPED_TEST(ArithmeticTypedTest, SubTypedTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;

    simd a = simd( 8 );
    simd b = simd( 3 );

    ls::for_each( ls::bit_sub< type, tag >( a, b ), []( int index, type val )
    {
        EXPECT_EQ( 5, val ) << "Error on index " << index;
        return true;
    } );

    ls::for_each( a - b, []( int index, type val )
    {
        EXPECT_EQ( 5, val ) << "Error on index " << index;
        return true;
    } );
}

TYPED_TEST(ArithmeticTaggedTest, MulLoHiTest)
{
    using tag = TypeParam;
    using simd16 = ls::simd_type< int16_t, tag >;
    using simd32 = ls::simd_type< int32_t, tag >;

    simd16 a = simd16( 0x2003 );
    simd16 b = simd16( 0x30 );

    ls::for_each( ls::mullo( a, b ), []( int index, type val )
    {
        EXPECT_EQ( 0x90, val ) << "Error on index " << index;
        return true;
    } );

    ls::for_each( ls::mulhi( a, b ), []( int index, type val )
    {
        EXPECT_EQ( 6, val ) << "Error on index " << index;
        return true;
    } );

    simd32 d = simd32( 0x20000003 );
    simd32 e = simd32( 0x30 );

    ls::for_each( ls::mullo( d, e ), []( int index, type val )
    {
        EXPECT_EQ( 0x90, val ) << "Error on index " << index;
        return true;
    } );
}

TYPED_TEST(ArithmeticTaggedTest, DivTest)
{
    using tag = TypeParam;
    using simdf = ls::simd_type< float, tag >;
    using simdd = ls::simd_type< double, tag >;

    simdf a = simdf( 20 );
    simdf b = simdf( 2 );

    ls::for_each( ls::div( a, b ), []( int index, type val )
    {
        EXPECT_FLOAT_EQ( 10, val ) << "Error on index " << index;
        return true;
    } );

    simdd d = simdd( 20 );
    simdd e = simdd( 2 );

    ls::for_each( ls::div( d, e ), []( int index, type val )
    {
        EXPECT_DOUBLE_EQ( 10, val ) << "Error on index " << index;
        return true;
    } );
}
#endif //__SSE2__
