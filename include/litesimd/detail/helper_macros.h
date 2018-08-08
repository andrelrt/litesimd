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

#ifndef LITESIMD_DETAIL_HELPER_MACROS_H
#define LITESIMD_DETAIL_HELPER_MACROS_H

// SIMD Functions
// ---------------------------------------------------------------------------------------
#define DEFINE_UNARY_SIMD_FUNCTION( TYPE_T, TAG_T, RET_T, PARAM_T, NAME, CMD ) \
template<> inline typename traits< TYPE_T, TAG_T >::RET_T \
NAME< TYPE_T, TAG_T >( typename traits< TYPE_T, TAG_T>::PARAM_T parm0 ) { \
    return CMD( parm0 ); }

#define DEFINE_BINARY_SIMD_FUNCTION( TYPE_T, TAG_T, RET_T, PARAM_T, NAME, CMD ) \
template<> inline typename traits< TYPE_T, TAG_T >::RET_T \
NAME< TYPE_T, TAG_T >( typename traits< TYPE_T, TAG_T>::PARAM_T parm0, \
                       typename traits< TYPE_T, TAG_T>::PARAM_T parm1 ) { \
    return CMD( parm0, parm1 ); }

// Binary function adaptors
// ---------------------------------------------------------------------------------------
#define DEFINE_BINARY_FUNCTION_SIMD_TYPE_ADAPTOR( NAME, RET_T ) \
template< typename SimdType_T, typename Tag_T = default_tag, \
          typename SimdType_T::simd_value_type* = nullptr > \
inline typename traits< typename SimdType_T::simd_value_type, Tag_T >::RET_T \
NAME( SimdType_T lhs, SimdType_T rhs ) { \
    return NAME< typename SimdType_T::simd_value_type, Tag_T >( lhs, rhs ); }

#define DEFINE_BINARY_FUNCTION_LEFT_VALUE_ADAPTOR( NAME, RET_T ) \
template< typename ValueType_T, typename Tag_T = default_tag > \
inline typename traits< ValueType_T, Tag_T >::RET_T \
NAME( ValueType_T lhs, typename traits< ValueType_T, Tag_T >::simd_type rhs ) { \
    return NAME< ValueType_T, Tag_T >( from_value< ValueType_T, Tag_T >( lhs ), rhs ); }

#define DEFINE_BINARY_FUNCTION_RIGHT_VALUE_ADAPTOR( NAME, RET_T ) \
template< typename ValueType_T, typename Tag_T = default_tag > \
inline typename traits< ValueType_T, Tag_T >::RET_T \
NAME( typename traits< ValueType_T, Tag_T >::simd_type lhs, ValueType_T rhs ) { \
    return NAME< ValueType_T, Tag_T >( lhs, from_value< ValueType_T, Tag_T >( rhs ) ); }

#define DEFINE_BINARY_FUNCTION_ADAPTORS( NAME, RET_T ) \
DEFINE_BINARY_FUNCTION_SIMD_TYPE_ADAPTOR( NAME, RET_T ) \
DEFINE_BINARY_FUNCTION_LEFT_VALUE_ADAPTOR( NAME, RET_T ) \
DEFINE_BINARY_FUNCTION_RIGHT_VALUE_ADAPTOR( NAME, RET_T ) 






#endif // LITESIMD_DETAIL_HELPER_MACROS_H
