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
    using simd = typename ls::traits< type, tag >::simd_type;
    constexpr size_t size = ls::traits< type, tag >::simd_size;

    simd a = ls::from_value< type, tag >( 3 );
    simd b = ls::from_value< type, tag >( 2 );
    simd c = ls::bit_and< type, tag >( a, b );

    type* pCmp = reinterpret_cast<type*>( &c );
    for( size_t i = 0; i < size; ++i )
    {
        EXPECT_EQ( 2, pCmp[ i ] );
    }
}
#endif //__SSE2__

