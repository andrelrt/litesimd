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

#include <functional>
#include <litesimd/types.h>
#include <litesimd/algorithm.h>
#include <litesimd/helpers/iostream.h>
#include "gtest/gtest.h"

namespace ls = litesimd;

template <typename T> class AlgorithmTypedTest: public ::testing::Test {};

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
TYPED_TEST_CASE(AlgorithmTypedTest, TestTypes);

#ifdef __SSE2__
TYPED_TEST(AlgorithmTypedTest, ForEachTypedTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;

    ls::for_each( simd( 3 ), []( int index, type val )
    {
        EXPECT_EQ( 3, val ) << "Error on index " << index;
        return true;
    } );

    ls::for_each( ls::iota< type, tag >( 0 ), []( int index, type val )
    {
        EXPECT_EQ( static_cast<type>( index ), val ) << "Error on index " << index;
        return true;
    } );

    int expected = simd::simd_size;
    ls::for_each_backward( ls::iota< type, tag >( 1 ), [&expected]( int index, type val )
    {
        EXPECT_EQ( static_cast<type>( expected ), val ) << "Error on index " << index;
        --expected;
        return true;
    } );
}

TYPED_TEST(AlgorithmTypedTest, IotaTypedTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;

    ls::for_each( ls::iota< type, tag >( 0 ), []( int index, type val )
    {
        EXPECT_EQ( static_cast<type>( index ), val ) << "Error on index " << index;
        return true;
    } );

    ls::for_each( ls::iota< type, tag >( 10 ), []( int index, type val )
    {
        EXPECT_EQ( static_cast<type>( index + 10 ), val ) << "Error on index " << index;
        return true;
    } );
}

TYPED_TEST(AlgorithmTypedTest, MinMaxTypedTest)
{
    using type = typename TypeParam::first_type;
    using tag = typename TypeParam::second_type;
    using simd = ls::simd_type< type, tag >;

    simd a = ls::iota< type, tag >( 0 );
    simd b = simd( 1 );

    EXPECT_EQ( static_cast<type>(0), (ls::min<simd>( a )) ) << "Simd: " << a;
    EXPECT_EQ( static_cast<type>(simd::simd_size-1), (ls::max<simd>( a )) ) << "Simd: " << a;

    auto min = ls::min< type, tag >( a, b );
    ls::for_each( min, [&min]( int index, type val )
    {
        if( index > 1 )
        {
            EXPECT_EQ( static_cast<type>( 1 ), val ) << "Error on index " << index << " Simd: " << min;
        }
        else
        {
            EXPECT_EQ( static_cast<type>( index ), val ) << "Error on index " << index << " Simd: " << min;
        }
        return true;
    } );

    auto max = ls::max< type, tag >( a, b );
    ls::for_each( max, [&max]( int index, type val )
    {
        if( index < 1 )
        {
            EXPECT_EQ( static_cast<type>( 1 ), val ) << "Error on index " << index << " Simd: " << max;
        }
        else
        {
            EXPECT_EQ( static_cast<type>( index ), val ) << "Error on index " << index << " Simd: " << max;
        }
        return true;
    } );
}


#endif //__SSE2__

TEST(AlgorithmTest, ForEachIndexTest)
{
    int expected = 1;
    ls::for_each_index< int8_t >( 0xaaaaaaaa, [&expected]( size_t index )
    {
        EXPECT_EQ( (size_t) expected, index );
        expected += 2;
        if( expected > 33 )
        {
            ADD_FAILURE() << "Expected bigger than 33";
            return false;
        }
        return true;
    } );
    EXPECT_EQ( 33, expected );

    expected = 31;
    ls::for_each_index_backward< int8_t >( 0xaaaaaaaa, [&expected]( size_t index )
    {
        EXPECT_EQ( (size_t) expected, index );
        expected -= 2;
        if( expected < -1 )
        {
            ADD_FAILURE() << "Expected less than -1";
            return false;
        }
        return true;
    } );
    EXPECT_EQ( -1, expected );

    expected = 1;
    ls::for_each_index< int16_t >( 0xcccccccc, [&expected]( size_t index )
    {
        EXPECT_EQ( (size_t) expected, index );
        expected += 2;
        if( expected > 17 )
        {
            ADD_FAILURE() << "Expected bigger than 17";
            return false;
        }
        return true;
    } );
    EXPECT_EQ( 17, expected );

    expected = 15;
    ls::for_each_index_backward< int16_t >( 0xcccccccc, [&expected]( size_t index )
    {
        EXPECT_EQ( (size_t) expected, index );
        expected -= 2;
        if( expected < -1 )
        {
            ADD_FAILURE() << "Expected less than -1";
            return false;
        }
        return true;
    } );
    EXPECT_EQ( -1, expected );

    expected = 1;
    ls::for_each_index< int32_t >( 0xf0f0f0f0, [&expected]( size_t index )
    {
        EXPECT_EQ( (size_t) expected, index );
        expected += 2;
        if( expected > 9 )
        {
            ADD_FAILURE() << "Expected bigger than 9";
            return false;
        }
        return true;
    } );
    EXPECT_EQ( 9, expected );

    expected = 7;
    ls::for_each_index_backward< int32_t >( 0xf0f0f0f0, [&expected]( size_t index )
    {
        EXPECT_EQ( (size_t) expected, index );
        expected -= 2;
        if( expected < -1 )
        {
            ADD_FAILURE() << "Expected less than -1";
            return false;
        }
        return true;
    } );
    EXPECT_EQ( -1, expected );

    expected = 1;
    ls::for_each_index< int64_t >( 0xff00ff00, [&expected]( size_t index )
    {
        EXPECT_EQ( (size_t) expected, index );
        expected += 2;
        if( expected > 5 )
        {
            ADD_FAILURE() << "Expected bigger than 5";
            return false;
        }
        return true;
    } );
    EXPECT_EQ( 5, expected );

    expected = 3;
    ls::for_each_index_backward< int64_t >( 0xff00ff00, [&expected]( size_t index )
    {
        EXPECT_EQ( (size_t) expected, index );
        expected -= 2;
        if( expected < -1 )
        {
            ADD_FAILURE() << "Expected less than -1";
            return false;
        }
        return true;
    } );
    EXPECT_EQ( -1, expected );

    expected = 1;
    ls::for_each_index< float >( 0x000000aa, [&expected]( size_t index )
    {
        EXPECT_EQ( (size_t) expected, index );
        expected += 2;
        if( expected > 9 )
        {
            ADD_FAILURE() << "Expected bigger than 9";
            return false;
        }
        return true;
    } );
    EXPECT_EQ( 9, expected );

    expected = 7;
    ls::for_each_index_backward< float >( 0x000000aa, [&expected]( size_t index )
    {
        EXPECT_EQ( (size_t) expected, index );
        expected -= 2;
        if( expected < -1 )
        {
            ADD_FAILURE() << "Expected less than -1";
            return false;
        }
        return true;
    } );
    EXPECT_EQ( -1, expected );

    expected = 1;
    ls::for_each_index< double >( 0x0000000a, [&expected]( size_t index )
    {
        EXPECT_EQ( (size_t) expected, index );
        expected += 2;
        if( expected > 5 )
        {
            ADD_FAILURE() << "Expected bigger than 5";
            return false;
        }
        return true;
    } );
    EXPECT_EQ( 5, expected );

    expected = 3;
    ls::for_each_index_backward< double >( 0x0000000a, [&expected]( size_t index )
    {
        EXPECT_EQ( (size_t) expected, index );
        expected -= 2;
        if( expected < -1 )
        {
            ADD_FAILURE() << "Expected less than -1";
            return false;
        }
        return true;
    } );
    EXPECT_EQ( -1, expected );
}
