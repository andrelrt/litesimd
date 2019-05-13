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

#include <cstring>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <array>
#include <algorithm>
#include <emmintrin.h>
#include <boost/timer/timer.hpp>
#include <boost/algorithm/searching/boyer_moore_horspool.hpp>

#include <litesimd/types.h>
#include <litesimd/compare.h>
#include <litesimd/shuffle.h>
#include <litesimd/bitwise.h>
#include <litesimd/algorithm.h>
#include <litesimd/helpers/iostream.h>

bool g_verbose = true;
void do_nothing( size_t );
namespace ls = litesimd;

struct std_searcher
{
    size_t operator()( const std::string& str, const std::string& find )
    {
        auto ret = str.find( find );
        if( ret == std::string::npos )
        {
            return str.size();
        }
        return ret;
    }
};

struct boost_searcher
{
    size_t operator()( const std::string& str, const std::string& find )
    {
        auto ret =
            boost::algorithm::boyer_moore_horspool_search( str.begin(), str.end(),
                                                           find.begin(), find.end() );

        // Boost changed the return value of boyer_moore_horspool_search in version 1.65.0
        #if BOOST_VERSION > 106500
        auto start = ret.first;
        #else
        auto start = ret;
        #endif

        if( start == str.end() )
        {
            return str.size();
        }
        return std::distance( str.begin(), start );
    }
};

template< typename Tag_T > int is_zero( ls::simd_type< int8_t, Tag_T > ){ return 0; }

template<> int is_zero< ls::sse_tag >( ls::simd_type< int8_t, ls::sse_tag > val )
{
    return _mm_testz_si128( val, ls::simd_type< int8_t, ls::sse_tag >::ones() );
}

#ifdef LITESIMD_HAS_AVX
template<> int is_zero< ls::avx_tag >( ls::simd_type< int8_t, ls::avx_tag > val )
{
    return _mm256_testz_si256( val, ls::simd_type< int8_t, ls::avx_tag >::ones() );
}
#endif // LITESIMD_HAS_AVX

template< typename Tag_T >
struct litesimd_boyer_moore_horspool
{
    size_t operator()( const std::string& str, const std::string& find )
    {
        using simd = ls::simd_type< int8_t, Tag_T >;
        constexpr size_t simd_size = ls::simd_type< int8_t, Tag_T >::simd_size;

        size_t find_size = find.size();

        simd find_end = *reinterpret_cast<const simd*>( find.data() + find_size - simd_size );

        std::array< size_t, 256 > index;
        index.fill( find_size );
        for( size_t i = 0; i < find_size; ++i )
        {
            index[ find[ i ] ] = find_size - i;
        }

        size_t str_size = str.size();
        for( size_t idx = find_size -1; idx < str_size; )
        {
            simd cur( str[ idx ] );

            auto mask = ls::equal_to< int8_t, Tag_T >( find_end, cur );

            if( !is_zero< Tag_T >( mask ) )
            {
                size_t found = std::numeric_limits<size_t>::max();
                uint32_t bitmask = ls::mask_to_bitmask< int8_t, Tag_T >( mask );
                ls::for_each_index<int8_t>( bitmask, [&]( int check_idx ) -> bool
                {
                    simd last = *reinterpret_cast<const simd*>( str.data() + idx - check_idx );
                    auto last_mask = ls::equal_to_bitmask< int8_t, Tag_T >( find_end, last );
                    if( ls::all_of<int8_t, Tag_T>( last_mask ) )
                    {
                        auto ret = memcmp( find.c_str(),
                                           str.data() + idx - check_idx - find_size,
                                           find_size );
                        if( ret == 0 )
                        {
                            found = check_idx;
                            return false;
                        }
                    }
                    return true;
                });
                if( found < str.size() )
                {
                    return found;
                }
            }
            idx += index[ str[ idx ] ];
        }
        return str.size();
    }
};

template< typename Tag_T >
struct litesimd_boyer_moore_horspool2
{
    size_t operator()( const std::string& str, const std::string& find )
    {
        using simd = ls::simd_type< int8_t, Tag_T >;
        constexpr size_t simd_size = ls::simd_type< int8_t, Tag_T >::simd_size;

        int32_t find_size = (int32_t) find.size();

        size_t simd_str_size = str.size() / simd_size;
        size_t simd_find_size = find_size / simd_size -1;
        const simd* simd_str = reinterpret_cast<const simd*>( str.data() );
        _mm_prefetch( (char const*)(simd_str + simd_find_size), _MM_HINT_T0 );

        std::array< int8_t, 256 > index;
        index.fill( std::max<int8_t>( 1, find_size / simd_size ) );
        for( int32_t i = 0; i < find_size-1; ++i )
        {
            index[ find[ i ] ] = std::max<int8_t>( 1, (find_size - 1 - i)/simd_size );
        }

        const char* str_data = str.data();

        simd simd_last = simd( find.back() );

        for( size_t simd_idx = simd_find_size; simd_idx < simd_str_size; )
        {
            auto mask = ls::equal_to< int8_t, Tag_T >( simd_last, simd_str[ simd_idx ] );

            size_t base_end = (simd_idx+1) * simd_size;

            if( is_zero< Tag_T >( mask ) )
            {
                size_t zskip = index[ str_data[ base_end - 1 ] ];
                _mm_prefetch( (char const*)(simd_str + simd_idx + zskip), _MM_HINT_T0 );
                simd_idx += zskip;
            }
            else
            {
                uint32_t bitmask = ls::mask_to_bitmask< int8_t, Tag_T >( mask );
                size_t skip = 0;
                bool found = true;
                size_t ret = 0;

                ls::for_each_index<int8_t>( bitmask, [&]( int check_idx ) -> bool
                {
                    ++check_idx;

                    bool do_break = false;
                    size_t end_idx = base_end - (simd_size - check_idx);
                    size_t idx = end_idx -1;
                    for( size_t i = find_size-1; i > 0; --i, --idx )
                    {
                        if( str_data[ idx ] != find[ i ] )
                        {
                            found = false;
                            skip = index[ str_data[ idx ] ];
                            _mm_prefetch( (char const*)(simd_str + simd_idx + skip), _MM_HINT_T0 );
                            do_break = ( skip > simd_size ); // not found and it impossible for this simd has a hit
                            break;
                        }
                    }

                    if( do_break )
                        return false;

                    if( found )
                    {
                        ret = end_idx - find_size;
                        return false;
                    }

                    return true;
                } );

                if( found )
                    return ret;

                _mm_prefetch( (char const*)(simd_str + simd_idx + skip), _MM_HINT_T0 );
                simd_idx += skip;
            }
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
    std::string str( size, ' ' );
    std::generate( str.begin(), str.end(), []() -> char { return 32 + rand() % 96; } );
    std::string find = str.substr( size - seek );

    size_t pos;
    timer.start();
    for( size_t j = 0; j < loop; ++j )
    {
        pos = searcher( str, find );
        do_nothing( pos );
    }
    timer.stop();
    if( g_verbose )
        std::cout << "Search time " << name << ": " << timer.format();

    return timer.elapsed().wall;
}

int main(int argc, char* /*argv*/[])
{
    constexpr size_t runSize = 0x00100000;
    constexpr size_t seekSize = 0x200;
    constexpr size_t loop = 100000;
    if( argc > 1 )
    {
        g_verbose = false;
        std::cout << "base,sse,avx" << std::endl;
    }
    else
    {
        std::cout << std::hex << std::endl
                  << "str size.: 0x" << std::setw(8) << std::setfill('0') << runSize << std::endl
                  << "seek size: 0x" << std::setw(8) << std::setfill('0') << seekSize << std::endl
                  << std::endl;
    }
    while( 1 )
    {
        uint64_t sse = bench< litesimd_boyer_moore_horspool< ls::sse_tag > >( "SSE..", runSize, seekSize, loop );
        uint64_t sse2 = bench< litesimd_boyer_moore_horspool2< ls::sse_tag > >( "SSE2.", runSize, seekSize, loop );
#ifdef LITESIMD_HAS_AVX
        uint64_t avx = bench< litesimd_boyer_moore_horspool< ls::avx_tag > >( "AVX..", runSize, seekSize, loop );
        uint64_t avx2 = bench< litesimd_boyer_moore_horspool2< ls::avx_tag > >( "AVX2.", runSize, seekSize, loop );
#endif // LITESIMD_HAS_AVX
        uint64_t base = bench< boost_searcher >( "Boost", runSize, seekSize, loop );


        if( g_verbose )
        {
            bench< std_searcher >( "Std..", runSize, seekSize, loop );

            std::cout
                      << std::endl << "Index Speed up SSE.......: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(sse) << "x"

                      << std::endl << "Index Speed up SSE2......: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(sse2) << "x"

#ifdef LITESIMD_HAS_AVX
                      << std::endl << "Index Speed up AVX.......: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(avx) << "x"

                      << std::endl << "Index Speed up AVX2......: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(avx2) << "x"
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
