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
#include <type_traits>
#include <litesimd/types.h>
#include <litesimd/shuffle.h>
#include <litesimd/algorithm.h>

namespace litesimd {

/**
 * \ingroup helpers
 * \brief Compatibility version to `operator<<` to output a `simd_type` on `ostream`.
 *
 * \param out Output stream
 * \param vec simd_type to print to stream
 * \return The output stream
 */
template< typename SimdType_T,
          typename std::enable_if<std::is_integral<typename SimdType_T::simd_value_type>::value>::type* = nullptr >
inline std::ostream& operator<<( std::ostream& out, SimdType_T vec )
{
    std::ios_base::fmtflags f( out.flags() );

    out << "(";
    for_each_backward( vec, [&out]( int index, typename SimdType_T::simd_value_type val ) -> bool
    {
        constexpr size_t mask = (1 << (2*sizeof(typename SimdType_T::simd_value_type)))-1;
        out << (+val & mask);
        if( index > 0 )
            out << ", ";
        return true;
    } );
    out << ")";

    out.flags( f );

    return out;
}

template< typename SimdType_T,
          typename std::enable_if<std::is_floating_point<typename SimdType_T::simd_value_type>::value>::type* = nullptr >
inline std::ostream& operator<<( std::ostream& out, SimdType_T vec )
{
    std::ios_base::fmtflags f( out.flags() );

    out << "(";
    for_each_backward( vec, [&out]( int index, typename SimdType_T::simd_value_type val ) -> bool
    {
        out << val;
        if( index > 0 )
            out << ", ";
        return true;
    } );
    out << ")";

    out.flags( f );

    return out;
}

} // namespace litesimd

#endif // LITESIMD_HELPERS_IOSTREAM_H
