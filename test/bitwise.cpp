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
#include <litesimd/bitwise.h>
#include <litesimd/shuffle.h>
#include <litesimd/algorithm.h>
#include <litesimd/helpers/iostream.h>
#include "gtest/gtest.h"

namespace ls = litesimd;

template <typename T> class BitwiseTypedTest: public ::testing::Test {};

using TestTypes = ::testing::Types<
#ifdef __SSE2__
    std::pair<int8_t, ls::sse_tag>, std::pair<int16_t, ls::sse_tag>,
    std::pair<int32_t, ls::sse_tag>, std::pair<int64_t, ls::sse_tag>
#ifdef __AVX2__
    , std::pair<int8_t, ls::avx_tag>, std::pair<int16_t, ls::avx_tag>,
    std::pair<int32_t, ls::avx_tag>, std::pair<int64_t, ls::avx_tag>
#endif //__AVX2__
#endif //__SSE2__
>;
TYPED_TEST_CASE(BitwiseTypedTest, TestTypes);

#ifdef __SSE2__
TYPED_TEST(BitwiseTypedTest, AndTypedTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;

    simd a = simd( 3 );
    simd b = simd( 6 );

    ls::for_each( ls::bit_and< type, tag >( a, b ), []( int index, type val )
    {
        EXPECT_EQ( 2, val ) << "Error on index " << index;
        return true;
    } );

    ls::for_each( a & b, []( int index, type val )
    {
        EXPECT_EQ( 2, val ) << "Error on index " << index;
        return true;
    } );

    a = ls::set<0, type, tag>( a, 6 );
    auto ret = ls::bit_and(a);
    EXPECT_EQ( static_cast<type>(6), (ls::get<0, type, tag>(a)) ) << "Simd: " << a;
    EXPECT_EQ( static_cast<type>(2), ret ) << "ret, Simd: " << +ret << ", " << a;
}

TYPED_TEST(BitwiseTypedTest, OrTypedTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;

    simd a = simd( 3 );
    simd b = simd( 6 );

    ls::for_each( ls::bit_or< type, tag >( a, b ), []( int index, type val )
    {
        EXPECT_EQ( 7, val ) << "Error on index " << index;
        return true;
    } );

    ls::for_each( a | b, []( int index, type val )
    {
        EXPECT_EQ( 7, val ) << "Error on index " << index;
        return true;
    } );

    a = ls::set<0, type, tag>( a, 6 );
    EXPECT_EQ( static_cast<type>(7), ls::bit_or( a ) );
}

TYPED_TEST(BitwiseTypedTest, XorTypedTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;

    simd a = simd( 3 );
    simd b = simd( 6 );

    ls::for_each( ls::bit_xor< type, tag >( a, b ), []( int index, type val )
    {
        EXPECT_EQ( 5, val ) << "Error on index " << index;
        return true;
    } );

    ls::for_each( a ^ b, []( int index, type val )
    {
        EXPECT_EQ( 5, val ) << "Error on index " << index;
        return true;
    } );

    a = ls::set<0, type, tag>( a, 6 );
    EXPECT_EQ( static_cast<type>(5), ls::bit_xor( a ) );
}

TYPED_TEST(BitwiseTypedTest, NotTypedTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;

    simd a = simd( 3 );

    ls::for_each( ls::bit_not< type, tag >( a ), []( int index, type val )
    {
        EXPECT_EQ( static_cast<type>( ~3 ), val ) << "Error on index " << index;
        return true;
    } );

    ls::for_each( ~a, []( int index, type val )
    {
        EXPECT_EQ( static_cast<type>( ~3 ), val ) << "Error on index " << index;
        return true;
    } );
}

#endif //__SSE2__
