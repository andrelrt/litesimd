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

#ifndef LITESIMD_ALGORITHM_H
#define LITESIMD_ALGORITHM_H

#include "arch/sse/algorithm.h"
#include "arch/avx/algorithm.h"
#include "algorithm/for_each.h"
#include "algorithm/iota.h"
#include "intravector.h"

namespace litesimd {

// Min max
// ---------------------------------------------------------------------------------------
template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
typename SimdType_T::simd_value_type min( SimdType_T vec )
{
    using type = typename SimdType_T::simd_value_type;
    using tag = typename SimdType_T::simd_tag;
    struct op{ inline SimdType_T operator()( SimdType_T lhs, SimdType_T rhs )
            { return min< type, tag >( lhs, rhs ); } };
    return intravector_op< type, tag >()( vec, op() );
}

template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
typename SimdType_T::simd_value_type max( SimdType_T vec )
{
    using type = typename SimdType_T::simd_value_type;
    using tag = typename SimdType_T::simd_tag;
    struct op{ inline SimdType_T operator()( SimdType_T lhs, SimdType_T rhs )
            { return max< type, tag >( lhs, rhs ); } };
    return intravector_op< type, tag >()( vec, op() );
}

} // namespace litesimd

#endif // LITESIMD_ALGORITHM_H
