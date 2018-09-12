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

#ifndef LITESIMD_ALGORITHM_FOREACH_H
#define LITESIMD_ALGORITHM_FOREACH_H

#include <utility>
#include <type_traits>
#include <litesimd/compare.h>
#include <litesimd/shuffle.h>
#include <litesimd/algorithm/detail/for_each_loop.h>

namespace litesimd {

/**
 * \ingroup algorithm
 * \brief Call function _func_ for each value inside the SIMD register from the lowest to the highest
 *
 * \param vec SIMD register
 * \param func Unary function that accepts two arguments. First argument is an index of the packed value, second argument is the value. It need to return a bool indicating `true` to continue to iterate; or `false` to break the loop.
 * \returns The _func_ parameter, as a `std::move( func )`.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/algorithm.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     auto vec = ls::iota< int32_t >( 4 );
 *     ls::for_each( vec, []( int index, int32_t val ) -> bool
 *     {
 *         std::cout << "vec[ " << index << " ] = " << val << std::endl;
 *         return true;
 *     } );
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * vec[ 0 ] = 4
 * vec[ 1 ] = 5
 * vec[ 2 ] = 6
 * vec[ 3 ] = 7
 * ```
 *
 * \see for_each_backward
 */
template< typename SimdType_T, typename Function_T,
          typename SimdType_T::simd_value_type* = nullptr >
inline Function_T for_each( SimdType_T vec, Function_T func )
{
    using st = SimdType_T;
    detail::for_each_loop< st::simd_size-1, st, Function_T >()( true, vec, func );
    return std::move( func );
}

/**
 * \ingroup algorithm
 * \brief Call function _func_ for each value inside the SIMD register from the highest to the lowest
 *
 * \param vec SIMD register
 * \param func Unary function that accepts two arguments. First argument is an index of the packed value, second argument is the value. It need to return a bool indicating `true` to continue to iterate; or `false` to break the loop.
 * \returns The _func_ parameter, as a `std::move( func )`.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/algorithm.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *
 *     auto vec = ls::iota< int32_t >( 4 );
 *     ls::for_each_backward( vec, []( int index, int32_t val ) -> bool
 *     {
 *         std::cout << "vec[ " << index << " ] = " << val << std::endl;
 *         return true;
 *     } );
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * vec[ 3 ] = 7
 * vec[ 2 ] = 6
 * vec[ 1 ] = 5
 * vec[ 0 ] = 4
 * ```
 *
 * \see for_each_backward
 */
template< typename SimdType_T, typename Function_T,
          typename SimdType_T::simd_value_type* = nullptr >
inline Function_T for_each_backward( SimdType_T vec, Function_T func )
{
    using st = SimdType_T;
    detail::for_each_loop< st::simd_size-1, st, Function_T >()( false, vec, func );
    return std::move( func );
}

/**
 * \ingroup algorithm
 * \brief Call function _func_ for each bit set on a bitmask from the lowest to the highest
 * \param bitmask Bitmask
 * \param func Unary function that accepts an index of the set bit as argument. It need to return a bool indicating `true` to continue to iterate; or `false` to break the loop.
 * \returns The _func_ parameter, as a `std::move( func )`.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/algorithm.h>
 * #include <litesimd/compare.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *     auto a = ls::simd_type< int32_t, ls::sse_tag >( 4, 3, 2, 1 );
 *     auto b = ls::simd_type< int32_t, ls::sse_tag >( 4, 0, 0, 1 );
 *     ls::for_each_index( ls::equal_to_bitmask( a, b ),
 *                         []( int index ) -> bool
 *     {
 *         std::cout << "Index " << index << " is equal" << std::endl;
 *         return true;
 *     } );
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * Index 0 is equal
 * Index 3 is equal
 * ```
 * \see for_each_index_backward
 * \see for_each
 */
template< typename ValueType_T, typename Function_T,
          typename std::enable_if_t<std::is_integral<ValueType_T>::value>* = nullptr >
inline Function_T for_each_index( uint32_t bitmask, Function_T func )
{
    constexpr uint32_t mask = (1 << sizeof(ValueType_T)) -1;
    while( bitmask != 0 )
    {
        int idx = bitmask_first_index< ValueType_T >( bitmask );
        if( !func( idx ) )
            break;
        bitmask &= ~(mask << (idx*sizeof(ValueType_T)));
    }
    return std::move( func );
}

template< typename ValueType_T, typename Function_T,
          typename std::enable_if_t<std::is_floating_point<ValueType_T>::value>* = nullptr >
inline Function_T for_each_index( uint32_t bitmask, Function_T func )
{
    while( bitmask != 0 )
    {
        int idx = bitmask_first_index< ValueType_T >( bitmask );
        if( !func( idx ) )
            break;
        bitmask &= ~(1 << idx);
    }
    return std::move( func );
}

/**
 * \ingroup algorithm
 * \brief Call function _func_ for each bit set on a bitmask from the highest bit to the lowest
 * \param bitmask Bitmask
 * \param func Unary function that accepts an index of the set bit as argument. It need to return a bool indicating `true` to continue to iterate; or `false` to break the loop.
 * \returns The _func_ parameter, as a `std::move( func )`.
 *
 * **Example**
 * ```{.cpp}
 * #include <iostream>
 * #include <litesimd/types.h>
 * #include <litesimd/algorithm.h>
 * #include <litesimd/compare.h>
 *
 * int main()
 * {
 *     namespace ls = litesimd;
 *     auto a = ls::simd_type< int32_t, ls::sse_tag >( 4, 3, 2, 1 );
 *     auto b = ls::simd_type< int32_t, ls::sse_tag >( 4, 0, 0, 1 );
 *     ls::for_each_index_backward( ls::equal_to_bitmask( a, b ),
 *                                  []( int index ) -> bool
 *     {
 *         std::cout << "Index " << index << " is equal" << std::endl;
 *         return true;
 *     } );
 *     return 0;
 * }
 * ```
 * Output on a SSE compilation
 * ```
 * Index 3 is equal
 * Index 0 is equal
 * ```
 * \see for_each_index
 * \see for_each_backward
 */
template< typename ValueType_T, typename Function_T,
          typename std::enable_if_t<std::is_integral<ValueType_T>::value>* = nullptr >
inline Function_T for_each_index_backward( uint32_t bitmask, Function_T func )
{
    constexpr uint32_t mask = (1 << sizeof(ValueType_T)) -1;
    while( bitmask != 0 )
    {
        int idx = bitmask_last_index< ValueType_T >( bitmask );
        if( !func( idx ) )
            break;
        bitmask &= ~(mask << (idx*sizeof(ValueType_T)));
    }
    return std::move( func );
}

template< typename ValueType_T, typename Function_T,
          typename std::enable_if_t<std::is_floating_point<ValueType_T>::value>* = nullptr >
inline Function_T for_each_index_backward( uint32_t bitmask, Function_T func )
{
    while( bitmask != 0 )
    {
        int idx = bitmask_last_index< ValueType_T >( bitmask );
        if( !func( idx ) )
            break;
        bitmask &= ~(1 << idx);
    }
    return std::move( func );
}

} // namespace litesimd

#endif // LITESIMD_ALGORITHM_FOREACH_H
