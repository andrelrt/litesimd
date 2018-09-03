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

#include "types.h"
#include "intravector.h"
#include "arch/bitwise.h"
#include "detail/helper_macros.h"

namespace litesimd {

// Basic bitwise operations adaptors
// ---------------------------------------------------------------------------------------
DEFINE_BINARY_FUNCTION_ADAPTORS( bit_and, type )
DEFINE_BINARY_FUNCTION_ADAPTORS( bit_or, type )
DEFINE_BINARY_FUNCTION_ADAPTORS( bit_xor, type )

template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
typename SimdType_T::simd_value_type bit_and( SimdType_T vec )
{
    using type = SimdType_T::simd_value_type;
    using tag = SimdType_T::simd_tag;
    return intravector_op< type, tag >()( vec, &bit_and< type, tag > );
}

template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
typename SimdType_T::simd_value_type bit_or( SimdType_T vec )
{
    using type = SimdType_T::simd_value_type;
    using tag = SimdType_T::simd_tag;
    return intravector_op< type, tag >()( vec, &bit_or< type, tag > );
}

template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
typename SimdType_T::simd_value_type bit_xor( SimdType_T vec )
{
    using type = SimdType_T::simd_value_type;
    using tag = SimdType_T::simd_tag;
    return intravector_op< type, tag >()( vec, &bit_xor< type, tag > );
}

template< typename LHS, typename RHS >
auto inline operator&( LHS lhs, RHS rhs )
{
    return bit_and( lhs, rhs );
}

template< typename LHS, typename RHS >
auto inline operator|( LHS lhs, RHS rhs )
{
    return bit_or( lhs, rhs );
}

template< typename LHS, typename RHS >
auto inline operator^( LHS lhs, RHS rhs )
{
    return bit_xor( lhs, rhs );
}

template< typename LHS >
auto inline operator~( LHS lhs )
{
    return bit_not( lhs );
}

} // namespace litesimd

#endif // LITESIMD_BITWISE_H
