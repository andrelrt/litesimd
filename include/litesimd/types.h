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
#include "traits.h"

namespace litesimd {

template< typename Type_T, typename Tag_T = default_tag >
class simd_type
{
    using simd_traits = traits< Type_T, Tag_T >;

public:
    using simd_value_type = Type_T;
    using simd_tag = Tag_T;

    using type = simd_type< simd_value_type, simd_tag >;
    using inner_type = typename simd_traits::simd_type;
    using bitmask_type = typename simd_traits::bitmask_type;
    constexpr static size_t simd_size = sizeof(inner_type) / sizeof(simd_value_type);

    simd_type(){}

    // Implicit transformation from/to bare SIMD type
    simd_type( inner_type v ) : v_(v) {}
    operator inner_type() const { return v_; }

    // Explicit transformation from value types
    explicit simd_type( simd_value_type v ) : v_( simd_traits::from_value( v ) ) {}

    template< typename... Value_T >
    simd_type( Value_T... v ) : v_( simd_traits::from_values( v... ) ) {}

    static inline simd_type zero() { return simd_type( simd_traits::zero() ); }
    static inline simd_type ones() { return simd_type( simd_traits::ones() ); }

private:
    inner_type v_;
};

template< typename Type_T, typename Tag_T = default_tag >
simd_type< Type_T, Tag_T > from_value( Type_T val )
{
    return simd_type< Type_T, Tag_T >( val );
}

using  t_int8_simd = simd_type<  int8_t, default_tag >;
using t_int16_simd = simd_type< int16_t, default_tag >;
using t_int32_simd = simd_type< int32_t, default_tag >;
using t_int64_simd = simd_type< int64_t, default_tag >;

constexpr static size_t  int8_simd_size = t_int8_simd::simd_size;
constexpr static size_t int16_simd_size = t_int16_simd::simd_size;
constexpr static size_t int32_simd_size = t_int32_simd::simd_size;
constexpr static size_t int64_simd_size = t_int64_simd::simd_size;


using t_float_simd  = simd_type<  float, default_tag >;
using t_double_simd = simd_type< double, default_tag >;

constexpr static size_t float_simd_size  = t_float_simd::simd_size;
constexpr static size_t double_simd_size = t_double_simd::simd_size;

using t_bitmask = size_t;

} // namespace litesimd

#endif //LITESIMD_TYPES_H
