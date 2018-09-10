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

#ifndef LITESIMD_ARITHMETIC_H
#define LITESIMD_ARITHMETIC_H

#include "types.h"
#include "arch/arithmetic.h"
#include "detail/helper_macros.h"

namespace litesimd {

/**
 * \defgroup arithmetic Arithmetic operations
 *
 * In litesimd, arithmetic group has binary functions to execute the basic arithmetic operations
 * (addition, subtraction, multiplication and division).
 */

// Basic operations
// ---------------------------------------------------------------------------------------
DEFINE_BINARY_FUNCTION_ADAPTORS( add, type )
DEFINE_BINARY_FUNCTION_ADAPTORS( sub, type )
DEFINE_BINARY_FUNCTION_ADAPTORS( mullo, type )
DEFINE_BINARY_FUNCTION_ADAPTORS( mulhi, type )
DEFINE_BINARY_FUNCTION_ADAPTORS( div, type )

/**
 * \ingroup arithmetic
 * \brief Returns the sum of two values.
 *
 * \param lhs, rhs Values to be added.
 * \returns Sum of the two values.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/arithmetic.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 1 ), b( 10 );
 *     std::cout << "a + b:  " << a + b << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * a + b: (11, 11, 11, 11)
 * ```
 */
template< typename LHS, typename RHS >
inline auto operator+( LHS lhs, RHS rhs )
{
    return add( lhs, rhs );
}

/**
 * \ingroup arithmetic
 * \brief Returns the subtraction of two values.
 *
 * \param lhs, rhs Values to be subtracted.
 * \returns Difference of the two values.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/arithmetic.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 10 ), b( 1 );
 *     std::cout << "a - b:  " << a - b << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * a - b: (9, 9, 9, 9)
 * ```
 */
template< typename LHS, typename RHS >
inline auto operator-( LHS lhs, RHS rhs )
{
    return sub( lhs, rhs );
}

} // namespace litesimd

#endif // LITESIMD_ARITHMETIC_H

