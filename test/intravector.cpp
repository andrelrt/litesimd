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
#include <litesimd/intravector.h>
#include <litesimd/arithmetic.h>
#include <litesimd/helpers/iostream.h>
#include "gtest/gtest.h"

namespace ls = litesimd;

template <typename T> class IntravectorTypedTest: public ::testing::Test {};

using TestTypes = ::testing::Types<
#ifdef LITESIMD_HAS_SSE
    std::pair<int8_t, ls::sse_tag>, std::pair<int16_t, ls::sse_tag>,
    std::pair<int32_t, ls::sse_tag>, std::pair<int64_t, ls::sse_tag>,
    std::pair<float, ls::sse_tag>, std::pair<double, ls::sse_tag>
#ifdef LITESIMD_HAS_AVX
    , std::pair<int8_t, ls::avx_tag>, std::pair<int16_t, ls::avx_tag>,
    std::pair<int32_t, ls::avx_tag>, std::pair<int64_t, ls::avx_tag>,
    std::pair<float, ls::avx_tag>, std::pair<double, ls::avx_tag>
#endif // LITESIMD_HAS_AVX
#endif // LITESIMD_HAS_SSE
>;
TYPED_TEST_CASE(IntravectorTypedTest, TestTypes);

#ifdef LITESIMD_HAS_SSE
TYPED_TEST(IntravectorTypedTest, HorizontalAritmeticTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;
    using func_t = simd(*)(simd, simd);

    simd a = ls::iota< type, tag >( 1 );
    type sum = 0;
    for( size_t i = 0; i < simd::simd_size; ++i )
    {
        sum += i + 1;
    }
    EXPECT_EQ( sum, ls::horizontal( a, static_cast< func_t >(ls::add< type, tag >) ) );
}

TYPED_TEST(IntravectorTypedTest, HorizontalLambdaTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;

    simd a = ls::iota< type, tag >( 1 );
    type sum = 0;
    for( size_t i = 0; i < simd::simd_size; ++i )
    {
        sum += i + 2;
    }
    --sum;
    auto func = []( simd lhs, simd rhs ) -> simd
    {
        return ls::add( static_cast< type >( 1 ), ls::add( lhs, rhs ) );
    };
    EXPECT_EQ( sum, ls::horizontal( a, func ) );
}

TEST(BaseTest, HorizontalIntrincsTest)
{
    // Sum ones, because iota will give us an overflow
    auto  i8 = ls::simd_type<  int8_t, ls::sse_tag >( 1 );
    auto i16 = ls::iota< int16_t, ls::sse_tag >( 1 );
    auto i32 = ls::iota< int32_t, ls::sse_tag >( 1 );
    auto i64 = ls::iota< int64_t, ls::sse_tag >( 1 );
    auto f32 = ls::iota<   float, ls::sse_tag >( 1 );
    auto f64 = ls::iota<  double, ls::sse_tag >( 1 );

    EXPECT_EQ( 16, ls::horizontal(  i8, _mm_add_epi8 ) );
    EXPECT_EQ( 36, ls::horizontal( i16, _mm_add_epi16 ) );
    EXPECT_EQ( 10, ls::horizontal( i32, _mm_add_epi32 ) );
    EXPECT_EQ(  3, ls::horizontal( i64, _mm_add_epi64 ) );
    EXPECT_FLOAT_EQ(  10, ls::horizontal( f32, _mm_add_ps ) );
    EXPECT_DOUBLE_EQ(  3, ls::horizontal( f64, _mm_add_pd ) );

#ifdef LITESIMD_HAS_AVX
    // Sum ones, because iota will give us an overflow
    auto  i8a = ls::simd_type<  int8_t, ls::avx_tag >( 1 );
    auto i16a = ls::iota< int16_t, ls::avx_tag >( 1 );
    auto i32a = ls::iota< int32_t, ls::avx_tag >( 1 );
    auto i64a = ls::iota< int64_t, ls::avx_tag >( 1 );
    auto f32a = ls::iota<   float, ls::avx_tag >( 1 );
    auto f64a = ls::iota<  double, ls::avx_tag >( 1 );

    EXPECT_EQ(  32, ls::horizontal(  i8a, _mm256_add_epi8 ) );
    EXPECT_EQ( 136, ls::horizontal( i16a, _mm256_add_epi16 ) );
    EXPECT_EQ(  36, ls::horizontal( i32a, _mm256_add_epi32 ) );
    EXPECT_EQ(  10, ls::horizontal( i64a, _mm256_add_epi64 ) );
    EXPECT_FLOAT_EQ(  36, ls::horizontal( f32a, _mm256_add_ps ) );
    EXPECT_DOUBLE_EQ( 10, ls::horizontal( f64a, _mm256_add_pd ) );
#endif // LITESIMD_HAS_AVX
}

#endif // LITESIMD_HAS_SSE
