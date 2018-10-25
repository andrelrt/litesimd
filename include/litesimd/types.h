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

#ifndef LITESIMD_TYPES_H
#define LITESIMD_TYPES_H

#include <cstdint>
#include <litesimd/arch/traits.h>

namespace litesimd {

/**
 * \defgroup type SIMD type
 *
 * This defines the basic litesimd type.
 *
 * All this functions are accessable at `<litesimd/types.h>`
 */

/**
 * \ingroup type
 * \class simd_type
 * \brief Basic litesimd type
 *
 * The simd_type is transparently interoperable with SIMD intrincs to
 * allow the developer to perform more complex operations than originally
 * anticipated by the library.
 *
 * \tparam ValueType_T Base type of original SIMD register
 */
template< typename ValueType_T, typename Tag_T = default_tag >
class simd_type
{
    using simd_traits = traits< ValueType_T, Tag_T >;

public:
    /// Base type of original SIMD register
    using simd_value_type = ValueType_T;
    /// SIMD instruction TAG selector
    using simd_tag = Tag_T;

    /// Type of this class
    using type = simd_type< simd_value_type, simd_tag >;
    /// Intrincs original type
    using inner_type = typename simd_traits::simd_type;
    /// Type of bitmasks
    using bitmask_type = typename simd_traits::bitmask_type;
    /// Type of index
    using index_type = int;
    /// How many values fit on SIMD register
    constexpr static size_t simd_size = sizeof(inner_type) / sizeof(simd_value_type);

    simd_type(){}

    /// Implicit contructor from intrincs SIMD type
    simd_type( inner_type v ) : v_(v) {}

    /// Implicit cast to intrincs SIMD type
    operator inner_type() const { return v_; }

    /**
     * \brief Construct a simd_type with all values equal
     *
     * \param v Value to be set
     *
     * **Example**
     * ```{.cpp}
     * #include <iostream>
     * #include <litesimd/types.h>
     * #include <litesimd/helpers/iostream.h>
     *
     * int main()
     * {
     *     namespace ls = litesimd;
     *
     *     ls::t_int32_simd x( 3 );
     *     std::cout << "x: " << x << std::endl;
     *     return 0;
     * }
     * ```
     * Output on a SSE compilation
     * ```
     * x: (3, 3, 3, 3)
     * ```
     */
    explicit simd_type( simd_value_type v ) : v_( simd_traits::from_value( v ) ) {}

    /**
     * \brief Construct a simd_type setting all values
     *
     * \param i, j, v Values to be set
     *
     * This contructor have different parameter count to match with how
     * many values fits on simd_type.
     *
     * **Example**
     * ```{.cpp}
     * #include <iostream>
     * #include <litesimd/types.h>
     * #include <litesimd/helpers/iostream.h>
     *
     * int main()
     * {
     *     namespace ls = litesimd;
     *
     *     ls::t_int32_simd x( 3, 2, 1, 0 );
     *     std::cout << "x: " << x << std::endl;
     *     return 0;
     * }
     * ```
     * Output on a SSE compilation
     * ```
     * x: (3, 2, 1, 0)
     * ```
     */
    template< typename... Value_T >
    simd_type( simd_value_type i, simd_value_type j, Value_T... v ) :
        v_( simd_traits::from_values( i, j, v... ) ) {}

    /// Returns a simd_type with all values zero
    static inline simd_type zero() { return simd_type( simd_traits::zero() ); }

    /// Returns a simd_type with all bits 1
    static inline simd_type ones() { return simd_type( simd_traits::ones() ); }

    /// Returns a simd_type with increasing values from 0 to simd_size -1, eg. (3, 2, 1, 0).
    static inline simd_type iota() { return simd_type( simd_traits::iota() ); }

private:
    inner_type v_;
};

/// Shortcut for int8_t simd_type on default instruction set
using  t_int8_simd = simd_type<  int8_t, default_tag >;
/// Shortcut for int16_t simd_type on default instruction set
using t_int16_simd = simd_type< int16_t, default_tag >;
/// Shortcut for int32_t simd_type on default instruction set
using t_int32_simd = simd_type< int32_t, default_tag >;
/// Shortcut for int64_t simd_type on default instruction set
using t_int64_simd = simd_type< int64_t, default_tag >;

/// Shortcut for float simd_type on default instruction set
using t_float_simd  = simd_type<  float, default_tag >;
/// Shortcut for double simd_type on default instruction set
using t_double_simd = simd_type< double, default_tag >;

} // namespace litesimd

#endif // LITESIMD_TYPES_H
