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

    simd a = ls::iota< type, tag >( 1 );
    type sum = 0;
    for( size_t i = 0; i < simd::simd_size; ++i )
    {
        sum += i + 1;
    }
    EXPECT_EQ( sum, ls::horizontal( a, ls::add< type, tag > ) );
}

#endif // LITESIMD_HAS_SSE
