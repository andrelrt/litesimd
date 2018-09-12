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

#ifndef LITESIMD_BITWISE_H
#define LITESIMD_BITWISE_H

#include <litesimd/types.h>
#include <litesimd/intravector.h>
#include <litesimd/arch/bitwise.h>
#include <litesimd/detail/helper_macros.h>

namespace litesimd {

/**
 * \defgroup bitwise Bitwise operations
 *
 * In litesimd, bitwise group has functions to execute the basic binary operations
 * (and, or, xor, not).
 *
 * All this functions are accessable at `<litesimd/bitwise.h>`
 */

// Basic bitwise operations adaptors
// ---------------------------------------------------------------------------------------
DEFINE_BINARY_FUNCTION_ADAPTORS( bit_and, type )
DEFINE_BINARY_FUNCTION_ADAPTORS( bit_or, type )
DEFINE_BINARY_FUNCTION_ADAPTORS( bit_xor, type )

/**
 * \ingroup bitwise
 * \brief Returns the result of `AND` operation among all numbers in the SIMD register.
 *
 * \param vec SIMD register to apply `AND` operator
 * \returns Result of `AND` operation among all values
 *
 * **Example**
 * ```{.cpp}
 * // min example
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/bitwise.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 0x71, 0x3c, 0x1e, 0x0f );
 *     std::cout << "bit_and( a ): " << ls::bit_and( a ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * bit_and( a ): 8
 * ```
 */
template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
inline typename SimdType_T::simd_value_type bit_and( SimdType_T vec )
{
    using type = typename SimdType_T::simd_value_type;
    using tag = typename SimdType_T::simd_tag;
    struct op{ inline SimdType_T operator()( SimdType_T lhs, SimdType_T rhs )
            { return bit_and< type, tag >( lhs, rhs ); } };
    return intravector_op< type, tag >()( vec, op() );
}

/**
 * \ingroup bitwise
 * \brief Returns the result of `OR` operation among all numbers in the SIMD register.
 *
 * \param vec SIMD register to apply `OR` operator
 * \returns Result of `OR` operation among all values
 *
 * **Example**
 * ```{.cpp}
 * // min example
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/bitwise.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 9, 2, 4, 8 );
 *     std::cout << "bit_or( a ): " << ls::bit_or( a ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * bit_or( a ): 15
 * ```
 */
template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
inline typename SimdType_T::simd_value_type bit_or( SimdType_T vec )
{
    using type = typename SimdType_T::simd_value_type;
    using tag = typename SimdType_T::simd_tag;
    struct op{ inline SimdType_T operator()( SimdType_T lhs, SimdType_T rhs )
            { return bit_or< type, tag >( lhs, rhs ); } };
    return intravector_op< type, tag >()( vec, op() );
}

/**
 * \ingroup bitwise
 * \brief Returns the result of `XOR` operation among all numbers in the SIMD register.
 *
 * \param vec SIMD register to apply `XOR` operator
 * \returns Result of `XOR` operation among all values
 *
 * **Example**
 * ```{.cpp}
 * // min example
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/bitwise.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 9, 2, 4, 8 );
 *     std::cout << "bit_xor( a ): " << ls::bit_xor( a ) << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * bit_xor( a ): 7
 * ```
 */
template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
inline typename SimdType_T::simd_value_type bit_xor( SimdType_T vec )
{
    using type = typename SimdType_T::simd_value_type;
    using tag = typename SimdType_T::simd_tag;
    struct op{ inline SimdType_T operator()( SimdType_T lhs, SimdType_T rhs )
            { return bit_xor< type, tag >( lhs, rhs ); } };
    return intravector_op< type, tag >()( vec, op() );
}

/**
 * \ingroup bitwise
 * \brief Returns the bitwise AND operation between passed parameters.
 *
 * \param lhs, rhs SIMD registers to apply bitwise AND operation.
 * \returns SIMD register with the result of the operation.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/bitwise.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 3 ), b( 6 );
 *     std::cout << "a & b:  " << a & b << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * a & b: (2, 2, 2, 2)
 * ```
 */
template< typename LHS, typename RHS >
inline auto operator&( LHS lhs, RHS rhs ) -> decltype( bit_and( lhs, rhs ) )
{
    return bit_and( lhs, rhs );
}

/**
 * \ingroup bitwise
 * \brief Returns the bitwise OR operation between passed parameters.
 *
 * \param lhs, rhs SIMD registers to apply bitwise OR operation.
 * \returns SIMD register with the result of the operation.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/bitwise.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 3 ), b( 6 );
 *     std::cout << "a | b:  " << a | b << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * a | b: (7, 7, 7, 7)
 * ```
 */
template< typename LHS, typename RHS >
inline auto operator|( LHS lhs, RHS rhs ) -> decltype( bit_or( lhs, rhs ) )
{
    return bit_or( lhs, rhs );
}

/**
 * \ingroup bitwise
 * \brief Returns the bitwise XOR operation between passed parameters.
 *
 * \param lhs, rhs SIMD registers to apply bitwise XOR operation.
 * \returns SIMD register with the result of the operation.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/bitwise.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 3 ), b( 6 );
 *     std::cout << "a ^ b:  " << a ^ b << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * a ^ b: (5, 5, 5, 5)
 * ```
 */
template< typename LHS, typename RHS >
inline auto operator^( LHS lhs, RHS rhs ) -> decltype( bit_xor( lhs, rhs ) )
{
    return bit_xor( lhs, rhs );
}

/**
 * \ingroup bitwise
 * \brief Inverts all bits on a SIMD register.
 *
 * \param rhs SIMD register to negate.
 * \returns SIMD register with all bit inverted.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostreams>
 * #include <litesimd/types.h>
 * #include <litesimd/bitwise.h>
 * #include <litesimd/helpers/iostream.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     ls::t_int32_simd a( 0xfffffff0 );
 *     std::cout << "~a: " << ~a << std::endl;
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * ~a: (15, 15, 15, 15)
 * ```
 */
template< typename RHS >
inline auto operator~( RHS rhs ) -> decltype( bit_not( rhs ) )
{
    return bit_not( rhs );
}

} // namespace litesimd

#endif // LITESIMD_BITWISE_H
