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
#include <array>
#include <algorithm>
#include <emmintrin.h>
#include <boost/timer/timer.hpp>
#include <boost/algorithm/searching/boyer_moore_horspool.hpp>

#include <litesimd/types.h>
#include <litesimd/compare.h>
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
        if( ret.first == str.end() )
        {
            return str.size();
        }
        return std::distance( str.begin(), ret.first );
    }
};

//template< typename Tag_T >
//struct litesimd_boyer_moore_horspool
//{
//    size_t operator()( const std::string& str, const std::string& find )
//    {
//        using stype = typename ls::traits< int8_t, Tag_T >::simd_type;
//        constexpr size_t ssize = ls::traits< int8_t, Tag_T >::simd_size;
//        size_t find_size = find.size();
//        std::array< size_t, 256 > index;
//        index.fill( find_size );
//        for( size_t i = 0; i < find_size-1; ++i )
//        {
//            index[ (+find[ i ] & 0xff) ] = find_size - 1 - i;
//        }
//
//        size_t simd_find_size = find_size / ssize;
//        size_t simd_find_rem = find_size & (ssize-1);
//        size_t bitmask_all = (1ul << sizeof(stype)) -1;
//
////        std::cout << "simd_find_size, simd_find_rem, bitmask_all: " << std::hex
////                  << simd_find_size << ", " << simd_find_rem << ", " << bitmask_all << std::endl;
//
//        const stype* simd_find = reinterpret_cast<const stype*>( find.data() + simd_find_rem );
//        size_t str_end = str.size() - find_size;
////        std::cout << "std_end: " << str_end << std::endl;
////        for( size_t i = 0; i < simd_find_size; ++i )
////        {
////            std::cout << "i, simd_find[ i ]: " << i << ", " << simd_find[ i ] << std::endl;
////        }
////
////        std::cout << "i --------------------: 0" << std::endl;
//
//        for( size_t i = 0; i <= str_end; )
//        {
//            const stype* sstr = reinterpret_cast<const stype*>( str.data() + i );
//            bool found = true;
//            for( size_t j = simd_find_size -1; j > 0; --j )
//            {
//                auto bitmask = ls::equals_bitmask< int8_t, Tag_T >( simd_find[ j ], _mm_lddqu_si128( (__m128i*) sstr + j ) );
////                std::cout << "bitmask: " << bitmask << std::endl;
//                if( bitmask != bitmask_all )
//                {
//                    found = false;
////                    std::cout << "j, simd_find != sstr: " << j << std::endl
////                              << stype( _mm_loadu_si128( (__m128i*) simd_find + j ) ) << " != " << std::endl
////                              << stype( _mm_loadu_si128( (__m128i*) sstr + j ) ) << std::endl;
//
//                    const char* ss = reinterpret_cast< const char* >( sstr + j );
//
//                    auto idx = ssize - ls::bitmask_first_index< int8_t >( bitmask_all & ~bitmask );
////                    if( idx != 0x1ff )
////                    std::cout << "idx, bitmask2: " << idx << ", " << (bitmask_all & ~bitmask) << std::endl;
//
//                    i += index[ (+ss[ idx ] & 0xff) ];
////                    std::cout << "ss[ idx ], index: "
////                              << (+ss[ idx ] & 0xff) << ", " << index[ (+ss[ idx ] & 0xff) ]  << std::endl;
//                    break;
//                }
//            }
//
//            if( found )
//            {
//                size_t rem = simd_find_rem -1;
//                for( size_t j = i + rem; j >= i; --j, --rem )
//                {
//                    if( str[ j ] != find[ rem ] )
//                    {
//                        found = false;
//                        i += index[ str[ j ] ];
//                    }
//                }
//            }
//
//            if( found )
//                return i;
//
////            std::cout << "i --------------------: " << i << std::endl;
//        }
//
//        return str.size();
//    }
//};

template< typename ValueType_T >
inline void for_each_index( size_t bitmask, std::function< bool(size_t) > func )
{
    while( bitmask != 0 )
    {
        size_t idx = ls::bitmask_first_index< ValueType_T >( bitmask );
        if( !func( idx ) )
            break;
        bitmask &= ~(size_t(1) << (idx-1));
    }
}

template< typename Tag_T > int is_zero( typename ls::traits< int8_t, Tag_T >::simd_type ){ return false; }

template<> int is_zero< ls::sse_tag >( typename ls::traits< int8_t, ls::sse_tag >::simd_type val )
{
    auto zero = _mm_setzero_si128();
    auto ones = _mm_cmpeq_epi8( zero, zero );
    return _mm_testz_si128( val, ones );
}

template<> int is_zero< ls::avx_tag >( typename ls::traits< int8_t, ls::avx_tag >::simd_type val )
{
    auto zero = _mm256_setzero_si256();
    auto ones = _mm256_cmpeq_epi8( zero, zero );
    return _mm256_testz_si256( val, ones );
}


template< typename Tag_T >
struct litesimd_boyer_moore_horspool_tupinamba
{
    size_t operator()( const std::string& str, const std::string& find )
    {
        using stype = typename ls::traits< int8_t, Tag_T >::simd_type;
        constexpr size_t ssize = ls::traits< int8_t, Tag_T >::simd_size;
        size_t find_size = find.size();
        std::array< size_t, 256 > index;
        index.fill( find_size );
        for( size_t i = 0; i < find_size-1; ++i )
        {
            index[ (+find[ i ] & 0xff) ] = find_size - 1 - i;
        }

        size_t simd_find_size = find_size / ssize;
        size_t simd_str_size = str.size() / ssize;

        stype simd_last = ls::from_value< int8_t, Tag_T >( find.back() );

        const stype* simd_str = reinterpret_cast<const stype*>( str.data() );

        size_t base_end = simd_find_size * ssize;
        for( size_t simd_idx = simd_find_size -1; simd_idx < simd_str_size; )
        {
//            std::cout << "simd_idx --------------------: " << simd_idx << std::endl;

            auto mask = ls::equals< int8_t, Tag_T >( simd_last, simd_str[ simd_idx ] );
            //size_t bitmask = ls::equals_bitmask< int8_t, Tag_T >( simd_last, simd_str[ simd_idx ] );
//            std::cout << "bitmask, simd_last, simd_str: "
//                      << bitmask << ", " << simd_last << ", " << simd_str[ simd_idx ] << std::endl;
//            if( bitmask == 0 )
            if( is_zero< Tag_T >( mask ) )
            {
//                std::cout << "str, index, simd_idx, next: "
//                          << (+str[ simd_idx * ssize + ssize - 1 ] & 0xff) << ", "
//                          << index[ +str[ simd_idx * ssize + ssize - 1 ] & 0xff ] << ", "
//                          << simd_idx << ", "
//                          << simd_idx + (1 + index[ +str[ simd_idx * ssize + ssize - 1 ] & 0xff ] / ssize)
//                          << std::endl;
                simd_idx += 1 + index[ +str[ base_end - 1 ] & 0xff ] / ssize;
            }
            else
            {
                size_t bitmask = ls::mask_to_bitmask< int8_t, Tag_T >( mask );
                size_t skip = 0;
                do
                {
                    size_t check_idx = ls::bitmask_first_index< int8_t >( bitmask );

                    bool found = true;
                    bool do_break = false;
                    size_t end_idx = base_end - check_idx;
                    size_t idx = end_idx;
                    for( size_t i = find_size-1; i > 0; --i, --idx )
                    {
                        if( str[ idx ] != find[ i ] )
                        {
                            found = false;
                            skip = index[ +str[ idx ] & 0xff ];
                            do_break = ( skip > ssize ); // not found and it impossible for this simd has a hit
                            break;
                        }
                    }

                    if( do_break )
                        break;

                    if( found )
                        return end_idx - find_size;

                    bitmask &= ~(((size_t)1) << (check_idx-1));
                } while( bitmask != 0 );

                simd_idx += 1 + skip / ssize;
            }
            base_end += ssize;
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
    std::generate( str.begin(), str.end(), [](){ return 32 + rand() % 96; } );
    std::string find = str.substr( size - seek );

    timer.start();
    for( size_t j = 0; j < loop; ++j )
    {
        size_t pos = searcher( str, find );
//        std::cout << "Pos: " << pos << std::endl;
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
    constexpr size_t loop = 10000; //10000;
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
        //bench< litesimd_boyer_moore_horspool< ls::sse_tag > >( "SSE..", runSize, seekSize, loop );
        uint64_t sse = bench< litesimd_boyer_moore_horspool_tupinamba< ls::sse_tag > >( "SSE..", runSize, seekSize, loop );
        uint64_t avx = bench< litesimd_boyer_moore_horspool_tupinamba< ls::avx_tag > >( "AVX..", runSize, seekSize, loop );
        uint64_t base = bench< boost_searcher >( "Boost", runSize, seekSize, loop );
        bench< std_searcher >( "Std..", runSize, seekSize, loop );


        if( g_verbose )
        {
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
