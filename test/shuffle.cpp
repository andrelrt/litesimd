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
#include <litesimd/shuffle.h>
#include <litesimd/algorithm.h>
#include <litesimd/helpers/iostream.h>
#include "gtest/gtest.h"

namespace ls = litesimd;

template <typename T> class ShuffleTypedTest: public ::testing::Test {};

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
TYPED_TEST_CASE(ShuffleTypedTest, TestTypes);

#ifdef __SSE2__
TEST(BaseTest, Set1FloatTest)
{
    __m128 a = _mm_set_ps( 4.0f, 3.0f, 2.0f, 1.0f );
    EXPECT_FLOAT_EQ( 1.0f, (ls::get<0, float, ls::sse_tag >( a )) );

    __m128d c = _mm_set_pd( 2.0, 1.0 );
    EXPECT_DOUBLE_EQ( 1.0, (ls::get<0, double, ls::sse_tag >( c )) );

#ifdef __AVX2__
    __m256 b = _mm256_set_ps( 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f );
    EXPECT_FLOAT_EQ( 1.0f, (ls::get<0, float, ls::avx_tag >( b )) );

    __m256d d = _mm256_set_pd( 4.0, 3.0, 2.0, 1.0 );
    EXPECT_DOUBLE_EQ( 1.0, (ls::get<0, double, ls::avx_tag >( d )) );

#endif //__AVX2__
}

TYPED_TEST(ShuffleTypedTest, GetSetTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;

    simd a = simd::zero();

    EXPECT_EQ( static_cast<type>(0), (ls::get<0, type, tag >( a )) );

    a = ls::set< 0, type, tag >( a, (type)1 );

    EXPECT_EQ( static_cast<type>(1), (ls::get<0, type, tag >( a )) );
}

TYPED_TEST(ShuffleTypedTest, HighInsertTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;

    simd a = simd::zero();
    a = ls::high_insert( a, 1 );
    EXPECT_EQ( 1, (ls::get< simd::simd_size -1, type, tag> ( a )) ) << "Simd: " << a;

    a = ls::high_insert( ls::iota< type, tag >( 0 ), simd::simd_size );
    EXPECT_EQ( simd::simd_size, (ls::get< simd::simd_size -1, type, tag> ( a )) ) << "Simd: " << a;

    ls::for_each( a, [&a]( int index, type val )
    {
        EXPECT_EQ( static_cast<type>( index + 1 ), val ) << "Error on index " << index << ", Simd: " << a;
        return true;
    } );
}

TYPED_TEST(ShuffleTypedTest, LowInsertTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;

    simd a = ls::iota< type, tag >( 1 );
    a = ls::low_insert( a, 0 );
    EXPECT_EQ( 0, (ls::get< 0, type, tag> ( a )) );
    ls::for_each( a, []( int index, type val )
    {
        EXPECT_EQ( static_cast<type>( index ), val ) << "Error on index " << index;
        return true;
    } );
}
#endif // __SSE2__
