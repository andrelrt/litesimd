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

/**
 * \defgroup algorithm Algorithm functions
 *
 * Algorithm defines a collection of functions especially designed to be used
 * on each value inside the packed SIMD register.
 *
 * All this functions are accessable at `<litesimd/algorithm.h>`
 */

/**
 * \ingroup algorithm
 * \brief Returns the smallest of each number in the SIMD register.
 *
 * \param vec SIMD register to compare
 * \return The lesser of the values in SIMD register
 *
 * **Example**
 * ```{.cpp}
 * // min example
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/algorithm.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *     std::cout << "min( zero ) == " << ls::min( ls::t_int32_simd::zero() ) << std::endl;
 *     std::cout << "min( iota ) == " << ls::min( ls::iota< int32_t >( 5 ) ) << std::endl;
 *     std::cout << "min( 4,3,2,1 ) == " << ls::min( ls::simd_type< int32_t, ls::sse_tag >( 4, 3, 2, 1 ) ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * min( zero ) == 0
 * min( iota ) == 5
 * min( 4,3,2,1 ) == 1
 * ```
 *
 * \see max
 */
template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
inline typename SimdType_T::simd_value_type min( SimdType_T vec )
{
    using type = typename SimdType_T::simd_value_type;
    using tag = typename SimdType_T::simd_tag;
    struct op{ inline SimdType_T operator()( SimdType_T lhs, SimdType_T rhs )
            { return min< type, tag >( lhs, rhs ); } };
    return intravector_op< type, tag >()( vec, op() );
}

/**
 * \ingroup algorithm
 * \brief Returns the largest the of each number in the SIMD register.
 *
 * \param vec SIMD register to compare
 * \return The greater of the values in SIMD register
 *
 * **Example**
 * ```{.cpp}
 * // max example
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/algorithm.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *     std::cout << "max( zero ) == " << ls::max( ls::t_int32_simd::zero() ) << std::endl;
 *     std::cout << "max( iota ) == " << ls::max( ls::iota< int32_t, ls::sse_tag >( 5 ) ) << std::endl;
 *     std::cout << "max( 4,3,2,1 ) == " << ls::max( ls::simd_type< int32_t, ls::sse_tag >( 4, 3, 2, 1 ) ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * max( zero ) == 0
 * max( iota ) == 8
 * max( 4,3,2,1 ) == 4
 * ```
 *
 * \see min
 */
template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
inline typename SimdType_T::simd_value_type max( SimdType_T vec )
{
    using type = typename SimdType_T::simd_value_type;
    using tag = typename SimdType_T::simd_tag;
    struct op{ inline SimdType_T operator()( SimdType_T lhs, SimdType_T rhs )
            { return max< type, tag >( lhs, rhs ); } };
    return intravector_op< type, tag >()( vec, op() );
}

} // namespace litesimd

#endif // LITESIMD_ALGORITHM_H
