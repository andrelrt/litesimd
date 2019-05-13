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
#include <litesimd/helpers/containers.h>
#include <litesimd/helpers/iostream.h>

bool g_verbose = true;
namespace ls = litesimd;

template< class Cont_T, typename TAG_T >
struct container_only
{
	using container_type = Cont_T;
    using value_type     = typename container_type::value_type;
    using const_iterator = typename container_type::const_iterator;

    container_only( const container_type& ref ) : ref_( ref ){}

    void build_index(){}

    const_iterator find( const value_type& key )
    {
        auto first = std::lower_bound( ref_.begin(), ref_.end(), key );
        return (first!=ref_.end() && !(key<*first)) ? first : ref_.end();
    }
private:
    const container_type& ref_;
};

template< class Cont_T, typename TAG_T >
struct map_index
{
	using container_type = Cont_T;
    using value_type     = typename container_type::value_type;
    using const_iterator = typename container_type::const_iterator;

    map_index( const container_type& ref ) : ref_( ref ){}

    void build_index()
    {
        for( auto it = ref_.begin(); it != ref_.end(); ++it )
        {
            index_[ *it ] = it;
        }
    }

    const_iterator find( const value_type& key )
    {
        auto it = index_.find( key );
        return (it != index_.end()) ? it->second : ref_.end() ;
    }

private:
    const container_type& ref_;
    std::map< value_type, const_iterator > index_;
};

template< class Cont_T, typename TAG_T >
class nway_tree
{
public:
	using container_type = Cont_T;
    using value_type     = typename container_type::value_type;
    using const_iterator = typename container_type::const_iterator;

    nway_tree( const container_type& ref )
        : ref_( ref ){}

    void build_index()
    {
        build_index( ref_ );
    }

    const_iterator find( const value_type& key ) const
    {
        size_t idx = 0;
        for( auto&& level : tree_ )
        {
            int li = ls::greater_last_index< value_type, TAG_T >( key, *level.get_simd( idx ) );
            idx = idx * array_size + li + 1;
        }

        const simd_type* cmp = reinterpret_cast< const simd_type* >( &ref_[ idx * array_size ] );
        int off = ls::equal_to_last_index< value_type, TAG_T >( key, *cmp );

        if( off < 0 )
        {
            return ref_.end();
        }
        auto it = ref_.begin();
        std::advance( it, idx * array_size + off );
        return it;
    }

private:
    constexpr static size_t array_size = ls::simd_type< value_type, TAG_T >::simd_size;
    using simd_type = ls::simd_type< value_type, TAG_T >;

    struct tree_level
    {
        ls::vector< value_type > keys_;

        const simd_type* get_simd( size_t idx ) const
        {
            return reinterpret_cast< const simd_type* >( &keys_[ idx * array_size ] );
        }

        void adjust()
        {
            size_t size = keys_.size() / array_size;
            if( keys_.size() > size * array_size )
                ++size;
            size *= array_size;
            keys_.resize( size, std::numeric_limits< value_type >::max() );
        }
    };

    ls::vector< tree_level > tree_;
    const container_type& ref_;

    void build_index( const container_type& cont )
    {
        if( cont.size() <= array_size )
            return;

        tree_level level;
        for( size_t i = array_size-1; i < cont.size(); i += array_size )
        {
            level.keys_.push_back( cont[ i ] );
        }

        build_index( level.keys_ );

        level.adjust();
        tree_.emplace_back( std::move( level ) );
        return;
    }
};

void do_nothing( int32_t );

template< class Cont_T, template < typename... > class Index_T, typename TAG_T >
uint64_t bench( const std::string& name, size_t size, size_t loop )
{
	using container_type = Cont_T;
    using index_type = Index_T< container_type, TAG_T >;

    boost::timer::cpu_timer timer;
    container_type org;

    srand( 1 );
    std::generate_n( std::back_inserter(org), size, &rand );
    container_type sorted( org );
    std::sort( sorted.begin(), sorted.end() );
    index_type index( sorted );

    index.build_index();

    timer.start();
    for( size_t j = 0; j < loop; ++j )
    {
        for( auto i : org )
        {
            auto ret = index.find( i );
            do_nothing( *ret );
        }
    }
    timer.stop();
    if( g_verbose )
        std::cout << "Find all " << name << ": " << timer.format();

    return timer.elapsed().wall;
}

int main(int argc, char* /*argv*/[])
{
    constexpr size_t runSize = 0x00400000;
    constexpr size_t loop = 10;
    if( argc > 1 )
    {
        g_verbose = false;
        std::cout << "base,index_sse,index_avx" << std::endl;
    }
    else
    {
        std::cout << "\nsize: 0x" << std::hex << std::setw(8) << std::setfill( '0') << runSize << std::endl << std::endl;
    }
    while( 1 )
    {
        uint64_t base = bench< ls::vector< int32_t >, container_only, void >( "lower_bound .", runSize, loop );
        uint64_t index1 = bench< ls::vector< int32_t >, nway_tree, ls::sse_tag >( "index SSE ...", runSize, loop );
#ifdef LITESIMD_HAS_AVX
        uint64_t index2 = bench< ls::vector< int32_t >, nway_tree, ls::avx_tag >( "index AVX ...", runSize, loop );
#endif // LITESIMD_HAS_AVX

        if( g_verbose )
        {
            bench< ls::vector< int32_t >, map_index, void >( "std::map ....", runSize, loop );

            std::cout
                      << std::endl << "Index Speed up SSE.......: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(index1) << "x"

#ifdef LITESIMD_HAS_AVX
                      << std::endl << "Index Speed up AVX.......: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(index2) << "x"
#endif // LITESIMD_HAS_AVX

                      << std::endl << std::endl;
        }
        else
        {
            std::cout
                << base << ","
                << index1 << ","
#ifdef LITESIMD_HAS_AVX
                << index2
#endif // LITESIMD_HAS_AVX
                << std::endl;
        }
    }
    return 0;
}
