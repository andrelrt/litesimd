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

#ifndef LITESIMD_INTRAVECTOR_H
#define LITESIMD_INTRAVECTOR_H

#include <litesimd/arch/intravector.h>

namespace litesimd {

/**
 * \defgroup intravector Intravector operations
 *
 * In litesimd, the intravector group has functions which operates between
 * the values of one SIMD register.
 *
 * All this functions are accessable at `<litesimd/intravector.h>`
 */

/**
 * \ingroup intravector
 * \brief Apply a generic SIMD binary function to reduce all SIMD values to a single one.
 *
 * The SIMD binary function should receive 2 simd_type and return the same type.
 * Lambda functions could be used as well.
 *
 * ```{.cpp}
 * ls::t_int32_simd vec( 1 );
 * ls::horizontal( vec, []( ls::t_int32_simd x, ls::t_int32_simd y )
 * {
 *     return (x ^ y) | 1;
 * } );
 * ```
 *
 * \param vec SIMD register to be reduced
 * \param func SIMD binary function
 * \tparam ValueType_T Base type of original SIMD register
 * \tparam Function_T Binary function type
 * \returns The result of reduction
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/arithmetic.h>
 * #include <litesimd/intravector.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd x( 1, 2, 3, 4 );
 *     std::cout << "horizontal( x, add ): "
 *               << ls::horizontal( x, ls::add< int32_t > ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * horizontal( x, add ): 10
 * ```
 */
template< typename ValueType_T, typename Function_T, typename Tag_T = default_tag >
inline ValueType_T horizontal( simd_type< ValueType_T, Tag_T > vec, Function_T func )
{
    return intravector_op< ValueType_T, Tag_T >()( vec, func );
}

} // namespace litesimd

#endif // LITESIMD_INTRAVECTOR_H
