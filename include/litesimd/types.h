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
#include "arch/types.h"

namespace litesimd {

using  t_int8_simd = typename traits<  int8_t, default_tag >::simd_type;
using t_int16_simd = typename traits< int16_t, default_tag >::simd_type;
using t_int32_simd = typename traits< int32_t, default_tag >::simd_type;
using t_int64_simd = typename traits< int64_t, default_tag >::simd_type;

constexpr static size_t  int8_simd_size = traits<  int8_t, default_tag >::simd_size;
constexpr static size_t int16_simd_size = traits< int16_t, default_tag >::simd_size;
constexpr static size_t int32_simd_size = traits< int32_t, default_tag >::simd_size;
constexpr static size_t int64_simd_size = traits< int64_t, default_tag >::simd_size;


using t_float_simd  = typename traits<  float, default_tag >::simd_type;
using t_double_simd = typename traits< double, default_tag >::simd_type;

constexpr static size_t float_size  = traits<  float, default_tag >::simd_size;
constexpr static size_t double_size = traits< double, default_tag >::simd_size;

using t_mask_simd = typename traits< int8_t, default_tag >::mask_type;
using t_bitmask = typename traits< int8_t, default_tag >::bitmask_type;

} // namespace litesimd

#endif //LITESIMD_TYPES_H
