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

// Basic operations
// ---------------------------------------------------------------------------------------
DEFINE_BINARY_FUNCTION_ADAPTORS( add, type )
DEFINE_BINARY_FUNCTION_ADAPTORS( sub, type )
DEFINE_BINARY_FUNCTION_ADAPTORS( mullo, type )
DEFINE_BINARY_FUNCTION_ADAPTORS( mulhi, type )
DEFINE_BINARY_FUNCTION_ADAPTORS( div, type )

template< typename LHS, typename RHS >
auto inline operator+( LHS lhs, RHS rhs )
{
    return add( lhs, rhs );
}

template< typename LHS, typename RHS >
auto inline operator-( LHS lhs, RHS rhs )
{
    return sub( lhs, rhs );
}

} // namespace litesimd

#endif // LITESIMD_ARITHMETIC_H

