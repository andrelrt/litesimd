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

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <map>
#include <boost/timer/timer.hpp>

#include <litesimd/types.h>
#include <litesimd/compare.h>
#include <litesimd/bitwise.h>
#include <litesimd/shuffle.h>
#include <litesimd/arithmetic.h>
#include <litesimd/helpers/containers.h>

bool g_verbose = true;
namespace ls = litesimd;

struct default_simd_to_lower
{
    void operator()( ls::string& str )
    {
        ls::t_int8_simd* data = (ls::t_int8_simd*) str.data();

        size_t sz = str.size() & ~(ls::int8_simd_size-1);
        for( size_t i = 0; i < sz; i += ls::int8_simd_size)
        {
            *data = ls::blend< int8_t >(
                        ls::bit_and< int8_t >(
                            ls::greater( *data, int8_t('A'-1) ),
                            ls::greater( int8_t('Z'+1), *data ) ),
                        ls::add( *data, int8_t(0x20) ),
                        *data );
            ++data;
        }

        size_t end = str.size();
        for( size_t i = sz; i < end; ++i )
        {
            str[i] = ( 'A' <= str[i] && str[i] <= 'Z' ) ? str[i] + 0x20 : str[i];
        }
    }
};

template< typename TAG_T >
struct to_lower
{
    void operator()( ls::string& str )
    {
        constexpr static size_t array_size = ls::simd_type< int8_t, TAG_T >::simd_size;
        using simd_type = ls::simd_type< int8_t, TAG_T >;

        simd_type* data = (simd_type*) str.data();

        size_t sz = str.size() & ~(array_size-1);
        for( size_t i = 0; i < sz; i += array_size )
        {
            *data = ls::blend< int8_t, TAG_T >(
                        ls::bit_and< int8_t, TAG_T >(
                            ls::greater< int8_t, TAG_T >( *data, 'A'-1 ),
                            ls::greater< int8_t, TAG_T >( 'Z'+1, *data ) ),
                        ls::add< int8_t, TAG_T >( *data, 0x20 ),
                        *data );
            ++data;
        }

        size_t end = str.size();
        for( size_t i = sz; i < end; ++i )
        {
            str[i] = ( 'A' <= str[i] && str[i] <= 'Z' ) ? str[i] + 0x20 : str[i];
        }
    }
};

template< typename TAG_T >
void maskstore( ls::simd_type< int8_t, TAG_T >*,
                ls::simd_type< int8_t, TAG_T >,
                ls::simd_type< int8_t, TAG_T > ){}

template<> void
maskstore< ls::sse_tag >( ls::simd_type< int8_t, ls::sse_tag >* ptr,
                          ls::simd_type< int8_t, ls::sse_tag > val,
                          ls::simd_type< int8_t, ls::sse_tag > mask )
{
    _mm_maskmoveu_si128( val, mask, (char*)ptr );
}

#ifdef LITESIMD_HAS_AVX
template<> void
maskstore< ls::avx_tag >( ls::simd_type< int8_t, ls::avx_tag >* ptr,
                          ls::simd_type< int8_t, ls::avx_tag > val,
                          ls::simd_type< int8_t, ls::avx_tag > mask )
{
    __m128i* ssePtr = reinterpret_cast<__m128i*>( ptr );
    _mm_maskmoveu_si128( _mm256_extracti128_si256( val, 0 ),
                         _mm256_extracti128_si256( mask, 0 ),
                         (char*)ssePtr );
    _mm_maskmoveu_si128( _mm256_extracti128_si256( val, 1 ),
                         _mm256_extracti128_si256( mask, 1 ),
                         (char*)(ssePtr + 1) );
}
#endif // LITESIMD_HAS_AVX

template< typename TAG_T >
struct maskmove_to_lower
{
    void operator()( ls::string& str )
    {
        constexpr static size_t array_size = ls::simd_type< int8_t, TAG_T >::simd_size;
        using simd_type = ls::simd_type< int8_t, TAG_T >;

        simd_type* data = (simd_type*) str.data();

        size_t sz = str.size() & ~(array_size-1);
        for( size_t i = 0; i < sz; i += array_size )
        {
            maskstore< TAG_T >( data,
                                ls::add< int8_t, TAG_T >( *data, 0x20 ),
                                ls::bit_and< int8_t, TAG_T >(
                                    ls::greater< int8_t, TAG_T >( *data, 'A'-1 ),
                                    ls::greater< int8_t, TAG_T >( 'Z'+1, *data ) ) );
            ++data;
        }

        size_t end = str.size();
        for( size_t i = sz; i < end; ++i )
        {
            str[i] = ( 'A' <= str[i] && str[i] <= 'Z' ) ? str[i] + 0x20 : str[i];
        }
    }
};

struct std_to_lower
{
    void operator()( ls::string& str )
    {
        for( size_t i = 0; i < str.size(); ++i )
        {
            str[i] = ( 'A' <= str[i] && str[i] <= 'Z' ) ? str[i] + 0x20 : str[i];
        }
    }
};

struct cachesize_to_lower
{
    void operator()( ls::string& str )
    {
        size_t sz = str.size();
        for( size_t i = 0; i < sz; ++i )
        {
            str[i] = ( 'A' <= str[i] && str[i] <= 'Z' ) ? str[i] + 0x20 : str[i];
        }
    }
};

struct autovec_to_lower
{
    void operator()( ls::string& str )
    {
        size_t sz = str.size();
        char* s = (char*) str.data();
        for( size_t i = 0; i < sz; ++i )
        {
            s[i] = ( 'A' <= s[i] && s[i] <= 'Z' ) ? s[i] + 0x20 : s[i];
        }
    }
};

void do_nothing( const ls::string& );

template< typename TO_LOWER_T >
uint64_t bench( const ls::string& name, size_t size, size_t loop )
{
	using functor = TO_LOWER_T;

    boost::timer::cpu_timer timer;
    functor toLower;

    ls::string str( size, 'C' );

    timer.start();
    for( size_t j = 0; j < loop; ++j )
    {
        toLower( str );
        do_nothing( str );
    }
    timer.stop();
    if( g_verbose )
        std::cout << "To lower " << name << ": " << timer.format();

    return timer.elapsed().wall;
}

int main(int argc, char* /*argv*/[])
{
    constexpr size_t runSize = 0x00100001;
    constexpr size_t loop = 10000;
    if( argc > 1 )
    {
        g_verbose = false;
        std::cout << "base,sse,avx" << std::endl;
    }
    else
    {
        std::cout << "\nsize: 0x" << std::hex << std::setw(8) << std::setfill( '0') << runSize << std::endl << std::endl;
    }
    while( 1 )
    {
        uint64_t base = bench< cachesize_to_lower >( "Scalar ", runSize, loop );
        uint64_t sse = bench< to_lower< ls::sse_tag > >( "SSE ...", runSize, loop );
#ifdef LITESIMD_HAS_AVX
        uint64_t avx = bench< to_lower< ls::avx_tag > >( "AVX ...", runSize, loop );
#endif // LITESIMD_HAS_AVX

        if( g_verbose )
        {
            bench< maskmove_to_lower< ls::sse_tag > >( "MM SSE ", runSize, loop );
#ifdef LITESIMD_HAS_AVX
            bench< maskmove_to_lower< ls::avx_tag > >( "MM AVX ", runSize, loop );
#endif // LITESIMD_HAS_AVX
            bench< std_to_lower >( "STD ...", runSize, loop );
            bench< autovec_to_lower >( "Autovec", runSize, loop );
            bench< default_simd_to_lower >( "Default", runSize, loop );
            std::cout
                      << std::endl << "Index Speed up SSE.......: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(sse) << "x"

#ifdef LITESIMD_HAS_AVX
                      << std::endl << "Index Speed up AVX.......: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(avx) << "x"
#endif // LITESIMD_HAS_AVX

                      << std::endl << std::endl;
        }
        else
        {
            std::cout
                << base << ","
                << sse << ","
#ifdef LITESIMD_HAS_AVX
                << avx
#endif // LITESIMD_HAS_AVX
                << std::endl;
        }
    }
    return 0;
}
