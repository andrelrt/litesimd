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

#ifndef LITESIMD_ALGORITHM_IOTA_H
#define LITESIMD_ALGORITHM_IOTA_H

#include <litesimd/shuffle.h>

namespace litesimd {

/**
 * \ingroup algorithm
 * \brief Store increasing sequence on a SIMD register
 *
 * Assigns to every element in SIMD register successive values of _val_, as if incremented with `++val` after each element is written.
 *
 * \param val Start value (default 0)
 * \returns SIMD register with increasing values
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/algorithm.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     std::cout << "iota< int32_t >(): " << ls::iota< int32_t >() << std::endl;
 *     std::cout << "iota< int16_t >( 4 ): " << ls::iota< int16_t >( 4 ) << std::endl;
 *     std::cout << "iota< double >( 1.2 ): " << ls::iota< double >( 1.2 ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * iota< int32_t >(): (3, 2, 1, 0)
 * iota< int16_t >( 4 ): (11, 10, 9, 8, 7, 6, 5, 4)
 * iota< double >( 1.2 ): (2.2, 1.2)
 * ```
 */
template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
iota( ValueType_T val = 0 )
{
    using simd_type = simd_type< ValueType_T, Tag_T >;
    simd_type ret;
    for( size_t i = 0; i < simd_type::simd_size; ++i )
    {
        ret = high_insert( ret, static_cast< ValueType_T >( val + i ) );
    }
    return ret;
}

} // namespace litesimd

#endif // LITESIMD_ALGORITHM_IOTA_H
