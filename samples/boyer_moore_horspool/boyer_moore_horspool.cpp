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
#include <boost/algorithm/boyer_moore_horspool.hpp>

#include <litesimd/types.h>
#include <litesimd/compare.h>

bool g_verbose = true;
void do_nothing( size_t );
namespace ls = litesimd;

struct boost_searcher
{
    size_t operator()( const std::string& str, const std::string& find )
    {
        auto ret = boyer_moore_horspool_search( str.begin(), str.end(),
                                                find.begin(), find.end() );
        if( ret.first == str.end() )
        {
            return str.size();
        }
        return std::distance( str.begin(), ret.first );
    }
};

template< typename Tag_T >
struct litesimd_boyer_moore_horspool
{
    size_t operator()( const std::string& str, const std::string& find )
    {
        using stype = ls::traits< int8_t, Tag_T >::simd_type;
        constexpr size_t ssize = ls::traits< int8_t, Tag_T >::simd_size;
        size_t sz = find.size();
        std::array< size_t, 256 > index;
        index.fill( sz );
        for( size_t i = 0; i < sz-1; ++i )
        {
            index[ find[ i ] ] = sz - 1 - i;
        }

        size_t simd_find_size = find.size() & ~(ssize-1);
        size_t simd_find_rem = find.size() & (ssize-1);
        size_t bitmask_all = (1 << (sizeof(ssize) >> 3)) -1;

        const stype* simd_find = reinterpret_cast<const stype*>( find.data() + simd_find_rem );
        size_t str_end = sz - find.size();

        for( size_t i = 0; i < str_end; )
        {
            const stype* sstr = reinterpret_cast<const stype*>( str.data() + i );
            const stype* sfind = simd_find;
            bool found = true;
            for( size_t j = simd_find_size -1; j >=0; --j )
            {
                auto bitmask = ls::equals_bitmask< int8_t, Tag_T >( sfind[ j ], sstr[ j ] );
                if( bitmask != bitmask_all )
                {
                    const char* ss = reinterpret_cast< const char* >( sstr );
                    found = false;

                    auto idx = ls::bitmask_first_index( bitmask_all & ~bitmask );

                    i += index[ ss[ idx ] ];

                    break;
                }
            }

            if( found )
            {
                size_t rem = simd_find_rem -1;
                for( size_t j = i + rem; j >= i; --j, --rem )
                {
                    if( str[ j ] != find[ rem ] )
                    {
                        found = false;
                        i += index[ str[ j ] ];
                    }
                }
            }

            if( found )
                return i;

            sstr = *reinterpret_cast<const stype*>( str.data() + i );
        }

        return str.size();
    }
};


template< typename SEARCH_T >
uint64_t bench( const std::string& name, size_t size, size_t seek, size_t loop )
{
	using functor = SEARCH_T;

    boost::timer::cpu_timer timer;
    functor searcher;

    srand( 1 );
    std::string str( size );
    std::generate( str.begin(), str.end(), [](){ return 32 + rand() % 224; } );
    std::string find = str.substr( size - seek );

    timer.start();
    for( size_t j = 0; j < loop; ++j )
    {
        size_t pos = searcher( str, find );
        do_nothing( pos );
    }
    timer.stop();
    if( g_verbose )
        std::cout << "Search time " << name << ": " << timer.format();

    return timer.elapsed().wall;
}

int main(int argc, char* /*argv*/[])
{
    constexpr size_t runSize = 0x00100001;
    constexpr size_t seekSize = 0x200;
    constexpr size_t loop = 10000;
    if( argc > 1 )
    {
        g_verbose = false;
        std::cout << "base,sse,avx" << std::endl;
    }
    else
    {
        std::cout << std::hex << std::endl
                  << "size: 0x" << std::setw(8) << std::setfill('0') << runSize << std::endl
                  << "seek size: 0x" << std::setw(8) << std::setfill('0') << seekSize << std::endl
                  << std::endl;
    }
    while( 1 )
    {
        uint64_t base = bench< cachesize_to_lower >( "Scalar ", runSize, seekSize, loop );
        uint64_t sse = bench< to_lower< ls::sse_tag > >( "SSE ...", runSize, seekSize, loop );
        uint64_t avx = bench< to_lower< ls::avx_tag > >( "AVX ...", runSize, seekSize, loop );


        if( g_verbose )
        {
            bench< std_to_lower >( "STD ...", runSize, seekSize, loop );
            bench< autovec_to_lower >( "Autovec", runSize, seekSize, loop );
            bench< default_simd_to_lower >( "Default", runSize, seekSize, loop );
            std::cout
                      << std::endl << "Index Speed up SSE.......: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(sse) << "x"

                      << std::endl << "Index Speed up AVX.......: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(avx) << "x"

                      << std::endl << std::endl;
        }
        else
        {
            std::cout
                << base << ","
                << sse << ","
                << avx
                << std::endl;
        }
    }
    return 0;
}
