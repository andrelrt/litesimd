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

#ifndef LITESIMD_HELPERS_IOSTREAM_H
#define LITESIMD_HELPERS_IOSTREAM_H

#include <iostream>
#include <iomanip>
#include "../types.h"

namespace litesimd {

// Stream Operators
// -----------------------------------------------------------------------------
template< typename SimdType_T, typename SimdType_T::simd_value_type* = nullptr >
std::ostream& operator<<( std::ostream& out, SimdType_T val )
{
    auto pval = reinterpret_cast<typename SimdType_T::simd_value_type const*>( &val );
    constexpr size_t sz = sizeof(typename SimdType_T::simd_value_type);
    constexpr size_t len = sizeof(SimdType_T) / sz;

    std::ios_base::fmtflags f( out.flags() );

    out << "(" << pval[0];

    for( size_t i = 1; i < len; ++i )
    {
        out << ", " << pval[i];
    }

    out << ")";

    out.flags( f );

    return out;
}

} // namespace litesimd

#endif // LITESIMD_HELPERS_IOSTREAM_H
