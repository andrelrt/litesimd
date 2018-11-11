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

#ifndef LITESIMD_ALGORITHM_SORT_H
#define LITESIMD_ALGORITHM_SORT_H

#include <litesimd/detail/arch/sse/sort.h>
#include <litesimd/detail/arch/avx/sort.h>

namespace litesimd {

template< typename ValueType_T, typename Tag_T = default_tag >
inline simd_type< ValueType_T, Tag_T >
sort( simd_type< ValueType_T, Tag_T > vec )
{
    return intravector_sort_op< ValueType_T, Tag_T >()( vec );
}

template< typename ValueType_T, typename Tag_T = default_tag >
inline std::pair< simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > >
merge_sorted( simd_type< ValueType_T, Tag_T > lhs, simd_type< ValueType_T, Tag_T > rhs )
{
    return merge_sorted_op< ValueType_T, Tag_T >()( lhs, rhs );
}

template< typename ValueType_T, typename Tag_T = default_tag >
inline std::pair< simd_type< ValueType_T, Tag_T >, simd_type< ValueType_T, Tag_T > >
sort( simd_type< ValueType_T, Tag_T > lhs, simd_type< ValueType_T, Tag_T > rhs )
{
    return merge_sorted< ValueType_T, Tag_T >(
                sort< ValueType_T, Tag_T >( lhs ),
                sort< ValueType_T, Tag_T >( rhs ) );
}

} // namespace litesimd

#endif // LITESIMD_ALGORITHM_SORT_H
