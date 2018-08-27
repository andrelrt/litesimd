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
#include <boost/timer/timer.hpp>

#include <litesimd/compare.h>
#include <litesimd/shuffle.h>
#include <litesimd/helpers/containers.h>

bool g_verbose = true;
namespace ls = litesimd;

template< class Cont_T, typename TAG_T >
class bubble
{
public:
	using container_type = Cont_T;
    using value_type     = typename container_type::value_type;
    using simd_type      = ls::simd_type< value_type, TAG_T >;

    void sort( container_type& cont )
    {
        bool sorted;
        size_t end = std::max<int64_t>( 0, static_cast<int64_t>(cont.size()) - array_size );
        do
        {
            sorted = true;
            for( size_t i = 0; i < end; ++i )
            {
                auto cmp = reinterpret_cast<simd_type*>( &cont[i+1] );
                uint32_t off = ls::greater_than_last_index< value_type, TAG_T >( cont[i], *cmp );

                if( off != 0 )
                {
                    std::swap( cont[i], cont[i + off] );
                    sorted = false;
                }
            }

            for( size_t i = end+1; i < cont.size(); ++i )
            {
                if( cont[i-1] > cont[i] )
                {
                    std::swap( cont[i-1], cont[i] );
                    sorted = false;
                }
            }

        } while( !sorted );
    }

private:
    constexpr static size_t array_size = ls::simd_type< value_type, TAG_T >::simd_size;
};


template< class Cont_T, typename TAG_T >
class bubble2
{
public:
	using container_type = Cont_T;
    using value_type     = typename container_type::value_type;
    using simd_type      = ls::simd_type< value_type, TAG_T >;

    void sort( container_type& cont )
    {
        bool sorted;
        size_t end = std::max<int64_t>( 0, static_cast<int64_t>(cont.size()) - array_size );
        do
        {
            sorted = true;
            auto cmp = *reinterpret_cast<simd_type*>( &cont[0] );
            for( size_t i = 0; i < end; ++i )
            {
                cmp = ls::high_insert< value_type, TAG_T >( cmp, cont[ i + array_size ] );
                uint32_t off = ls::greater_than_last_index< value_type, TAG_T >( cont[i], cmp );

                if( off != 0 )
                {
                    std::swap( cont[i], cont[i + off] );
                    sorted = false;
                }
            }

            for( size_t i = end; i < cont.size(); ++i )
            {
                if( cont[i-1] > cont[i] )
                {
                    std::swap( cont[i-1], cont[i] );
                    sorted = false;
                }
            }

        } while( !sorted );
    }

private:
    constexpr static size_t array_size = ls::simd_type< value_type, TAG_T >::simd_size;
};

template< class Cont_T, typename TAG_T >
struct stl_sort
{
	using container_type = Cont_T;

    void sort( container_type& cont )
    {
        std::sort( std::begin( cont ), std::end( cont ) );
    }
};

template< class Cont_T, typename TAG_T >
struct basic_bubble_sort
{
	using container_type = Cont_T;
    using iterator = typename container_type::iterator;

    void sort( container_type& cont )
    {
        bool sorted;
        do
        {
            sorted = true;
            auto it = std::begin( cont );
            auto last = it;

            for( ++it; it != std::end( cont ); ++it )
            {
                if( *last > *it )
                {
                    std::swap( *last, *it );
                    sorted = false;
                }
                last = it;
            }

        } while( !sorted );
    }
};

template< class Cont_T, template< typename...> class Sort_T, typename TAG_T >
uint64_t bench( const std::string& name, size_t size, size_t loop )
{
	using container_type = Cont_T;
    using sort_type = Sort_T< container_type, TAG_T >;

    boost::timer::cpu_timer timer;
    container_type org;

    srand(1);
    std::generate_n( std::back_inserter(org), size, &rand );

    sort_type sort;
    container_type warmup( org );
    sort.sort( warmup );

    timer.start();
    for( size_t j = 0; j < loop; ++j )
    {
        container_type temp( org );
        sort.sort( temp );

        auto it = std::begin( temp );
        auto last = it;
        for( ++it; it < std::end( temp ); ++it )
        {
            if( *it < *last )
            {
                std::cout << *last << " gt " << *it << " - ";
            }
            last = it;
        }
    }
    timer.stop();
    if( g_verbose )
        std::cout << "Sort all " << name << ": " << timer.format();

    return timer.elapsed().wall;
}

int main(int argc, char* /*argv*/[])
{
    constexpr size_t runSize = 0x00010000;
    constexpr size_t loop = 1;
    if( argc > 1 )
    {
        g_verbose = false;
        std::cout << "Bubble sort,SSE Bubble 1,SSE Bubble 2,AVX Bubble 1,AVX Bubble 2" << std::endl << std::endl << std::endl << std::endl;
    }
    else
    {
        std::cout << "\nsize: 0x" << std::hex << std::setw(8) << std::setfill( '0') << runSize << std::endl << std::endl;
    }
    while( 1 )
    {
        uint64_t bsort = bench< ls::aligned_vector< int32_t >, basic_bubble_sort,
                              ls::sse_tag >( "Bubble sort .....", runSize, loop );
        uint64_t ssesort = bench< ls::aligned_vector< int32_t >, bubble,
                                ls::sse_tag >( "SSE Bubble sort .", runSize, loop );
        uint64_t ssesort2 = bench< ls::aligned_vector< int32_t >, bubble2,
                                 ls::sse_tag >( "SSE Bubble2 sort ", runSize, loop );
#ifdef LITESIMD_HAS_AVX
        uint64_t avxsort = bench< ls::aligned_vector< int32_t >, bubble,
                                ls::avx_tag >( "AVX Bubble sort .", runSize, loop );
        uint64_t avxsort2 = bench< ls::aligned_vector< int32_t >, bubble2,
                                 ls::avx_tag >( "AVX Bubble2 sort ", runSize, loop );
#endif

        if( g_verbose )
        {
            bench< ls::aligned_vector< int32_t >, stl_sort, void >( "STL sort ........", runSize, loop );

            std::cout
                << std::endl << "SSE Speed up ......: " << std::fixed << std::setprecision(2)
                << static_cast<float>(bsort)/static_cast<float>(ssesort) << "x"
                << std::endl << "SSE2 Speed up .....: " << std::fixed << std::setprecision(2)
                << static_cast<float>(bsort)/static_cast<float>(ssesort2) << "x"
                << std::endl << "SSE2/SSE Speed up .: " << std::fixed << std::setprecision(2)
                << static_cast<float>(ssesort)/static_cast<float>(ssesort2) << "x"

#ifdef LITESIMD_HAS_AVX
                << std::endl << "AVX Speed up ......: " << std::fixed << std::setprecision(2)
                << static_cast<float>(bsort)/static_cast<float>(avxsort) << "x"
                << std::endl << "AVX2 Speed up .....: " << std::fixed << std::setprecision(2)
                << static_cast<float>(bsort)/static_cast<float>(avxsort2) << "x"
                << std::endl << "AVX2/AVX Speed up .: " << std::fixed << std::setprecision(2)
                << static_cast<float>(avxsort)/static_cast<float>(avxsort2) << "x"
#endif

                << std::endl << std::endl;
        }
        else
        {
            std::cout
                << bsort
                << "," << ssesort
                << "," << ssesort2
#ifdef LITESIMD_HAS_AVX
                << "," << avxsort
                << "," << avxsort2
#endif
                << std::endl;
        }
    }
    return 0;
}
