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
#include <boost/timer/timer.hpp>

#include <litesimd/types.h>
#include <litesimd/compare.h>
#include <litesimd/shuffle.h>
#include <litesimd/helpers/containers.h>

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
class index_nocache
{
public:
	using container_type = Cont_T;
    using value_type     = typename container_type::value_type;
    using const_iterator = typename container_type::const_iterator;

    index_nocache( const container_type& ref )
        : ref_( ref ){}

    void build_index()
    {
        size_t step = ref_.size() / (array_size + 1);

        const_iterator end = ref_.begin();
        for( size_t i = 0; i < array_size; ++i )
        {
            std::advance( end, step );
            cmp_ = ls::high_insert( cmp_, *end );
        }
    }

    const_iterator find( const value_type& key ) const
    {
        size_t i = ls::greater_last_index< value_type, TAG_T >( key, cmp_ ) + 1;
        size_t step = ref_.size() / (array_size + 1);

        const_iterator beg = ref_.begin();
        std::advance( beg, i * step );
        const_iterator end;
        if( i == array_size )
        {
            end = ref_.end();
        }
        else
        {
            end = beg;
            std::advance( end, step + 1 );
        }

        auto first = std::lower_bound( beg, end, key );
        return (first!=end && !(key<*first)) ? first : ref_.end();
    }

private:
    constexpr static size_t array_size = ls::simd_type< value_type, TAG_T >::simd_size;

    const container_type& ref_;
    ls::simd_type< value_type, TAG_T > cmp_;
};

//any container smart_step
template< class Cont_T, typename TAG_T >
class index_cache
{
public:
	using container_type = Cont_T;
    using value_type     = typename container_type::value_type;
    using const_iterator = typename container_type::const_iterator;

    index_cache( const container_type& ref )
        : ref_( ref ){}

    void build_index()
    {
        size_t step = ref_.size() / (array_size + 1);

        const_iterator it = ref_.begin();
        ranges_[ 0 ] = it;
        for( size_t i = 1; i <= array_size; ++i )
        {
            std::advance( it, step );
            ranges_[ i ] = it;
            cmp_ = ls::high_insert< value_type, TAG_T >( cmp_, *it );
        }
        ranges_[ array_size+1 ] = std::prev(ref_.end());
    }

    const_iterator find( const value_type& key ) const
    {
        int i = 1 + ls::greater_last_index< value_type, TAG_T >( key, cmp_ );
        auto end = std::next( ranges_[ i + 1 ] );
        auto first = std::lower_bound( ranges_[ i ], end, key );
        return (first!=end && !(key<*first)) ? first : ref_.end();
    }

private:
    constexpr static size_t array_size = ls::simd_type< value_type, TAG_T >::simd_size;

    const container_type& ref_;
    std::array< const_iterator, array_size + 2 > ranges_;
    ls::simd_type< value_type, TAG_T > cmp_;
};

template <class ForwardIterator, class T, typename TAG_T >
ForwardIterator lower_bound( ForwardIterator beg, ForwardIterator end, const T& key )
{
    using value_type     = typename std::iterator_traits< ForwardIterator >::value_type;
    using const_iterator = ForwardIterator;
    using simd_type      = ls::simd_type< value_type, TAG_T >;

    constexpr static size_t array_size = ls::simd_type< value_type, TAG_T >::simd_size;

    size_t size = std::distance( beg, end );
    size_t step = size / (array_size + 1);

    // Create SIMD search key
    simd_type cmp;
    const_iterator it = beg;
    for( size_t i = 0; i < array_size; ++i )
    {
        std::advance( it, step );
        cmp = ls::high_insert( cmp, *it );
    }

    // N-Way search
    size_t i = ls::greater_last_index< value_type, TAG_T >( key, cmp ) + 1;

    // Recalculate iterators
    it = beg;
    std::advance( it, i * step );
    const_iterator itEnd;
    if( i == array_size )
    {
        itEnd = end;
    }
    else
    {
        itEnd = it;
        std::advance( itEnd, step + 1 );
    }

    // Standard lower_bound on 1/(n+1) of container size
    return std::lower_bound( it, itEnd, key );
}

template< class Cont_T, typename TAG_T >
struct container_simd_lb
{
	using container_type = Cont_T;
    using value_type     = typename container_type::value_type;
    using const_iterator = typename container_type::const_iterator;

    container_simd_lb( const container_type& ref ) : ref_( ref ){}

    void build_index(){}

    const_iterator find( const value_type& key )
    {
        auto first = lower_bound< const_iterator,
                                  value_type,
                                  TAG_T>( ref_.begin(), ref_.end(), key );

        return (first!=ref_.end() && !(key<*first)) ? first : ref_.end();
    }
private:
    const container_type& ref_;
};

#ifdef LITESIMD_HAS_AVX
template< typename Tag_T > ls::simd_type< int32_t, Tag_T >
i32Gather( const int*, ls::simd_type< int32_t, Tag_T > ){}

template<> ls::simd_type< int32_t, ls::avx_tag >
i32Gather< ls::avx_tag >( const int* ptr, ls::simd_type< int32_t, ls::avx_tag > idx )
{
    return _mm256_i32gather_epi32( ptr, idx, 4 );
}

template< typename Tag_T > int is_zero( ls::simd_type< int32_t, Tag_T > ){ return 0; }

template<> int is_zero< ls::avx_tag >( ls::simd_type< int32_t, ls::avx_tag > val )
{
    return _mm256_testz_si256( val, ls::simd_type< int32_t, ls::avx_tag >::ones() );
}

template <class ForwardIterator, class T, typename TAG_T >
ForwardIterator lower_bound2( ForwardIterator beg, ForwardIterator end, const T& key )
{
    using value_type = typename std::iterator_traits< ForwardIterator >::value_type;
    using simd_type  = ls::simd_type< value_type, TAG_T >;

    constexpr static size_t array_size = ls::simd_type< value_type, TAG_T >::simd_size;

    size_t size = std::distance( beg, end );
    if( size < 0x20 )
    {
        // Standard lower_bound on small sizes
        return std::lower_bound( beg, end, key );
    }

    size_t step = size / (array_size + 1);

    const value_type* start = &(*beg);
    // Create SIMD index key
    ls::simd_type< int32_t, TAG_T > indexes;
    for( size_t i = 1; i <= array_size; ++i )
    {
        // Preload SIMD search key values into processor cache
        _mm_prefetch( start + (step*i), _MM_HINT_T0 );
        indexes = ls::high_insert( indexes, (int32_t)(step*i) );
    }

    // Create SIMD search key
    simd_type cmp = i32Gather< TAG_T >( start, indexes );

    auto eq = ls::equal_to< value_type, TAG_T >( key, cmp );

    if( !is_zero< TAG_T >( eq ) )
    {
        auto it = beg;
        std::advance( it, step * ( 1 + ls::bitmask_last_index< value_type >(
                                     ls::mask_to_bitmask< value_type, TAG_T >( eq ) ) ) );
        return it;
    }

    // N-Way search
    size_t i = ls::greater_last_index< value_type, TAG_T >( key, cmp ) + 1;

    // Recalculate iterators
    auto it = beg;
    std::advance( it, i * step );
    decltype(it) itEnd;
    if( i == array_size )
    {
        itEnd = end;
    }
    else
    {
        itEnd = it;
        std::advance( itEnd, step + 1 );
    }

    return lower_bound2< ForwardIterator, T, TAG_T >( it, itEnd, key );
}

template< class Cont_T, typename TAG_T >
struct container_simd_lb2
{
	using container_type = Cont_T;
    using value_type     = typename container_type::value_type;
    using const_iterator = typename container_type::const_iterator;

    explicit container_simd_lb2( const container_type& ref ) : ref_( ref ){}

    void build_index(){}

    const_iterator find( const value_type& key )
    {
        auto first = lower_bound2< const_iterator,
                                  value_type,
                                  TAG_T>( ref_.begin(), ref_.end(), key );

        return (first!=ref_.end() && !(key<*first)) ? first : ref_.end();
    }
private:
    const container_type& ref_;
};
#endif // LITESIMD_HAS_AVX

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
//        size_t cnt = 0;
        for( auto i : org )
        {
            auto ret = index.find( i );
            do_nothing( *ret );

//            // Test if the retun value is correct
//            if( ret == sorted.end() )
//            {
//                std::cout << "end: 0x" << std::hex << i << ", i: 0x" << cnt << std::endl;
//                break;
//            }
//            else
//            {
//                if( *ret != i )
//                {
//                    std::cout << *ret << "," << i << "-";
//                }
//            }
//            ++cnt;
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
        std::cout << "std::lower_bound,First step SSE,First step AVX" << std::endl << std::endl << std::endl << std::endl;
    }
    else
    {
        std::cout << "\nsize: 0x" << std::hex << std::setw(8) << std::setfill( '0') << runSize << std::endl << std::endl;
    }
    while( 1 )
    {
        uint64_t base =   bench< ls::vector< int32_t >, container_only, void >    ( "lower_bound ..........", runSize, loop );
        uint64_t cache =  bench< ls::vector< int32_t >, index_cache, ls::sse_tag >( "index_cache SSE.......", runSize, loop );
#ifdef LITESIMD_HAS_AVX
        uint64_t cache2 = bench< ls::vector< int32_t >, index_cache, ls::avx_tag >( "index_cache AVX.......", runSize, loop );
#endif

        if( g_verbose )
        {
            uint64_t nocache =  bench< ls::vector< int32_t >, index_nocache, ls::sse_tag >( "index_nocache SSE ....", runSize, loop );
            uint64_t simdlb =  bench< ls::vector< int32_t >, container_simd_lb, ls::sse_tag >( "SIMD lower_bound SSE .", runSize, loop );

#ifdef LITESIMD_HAS_AVX
            uint64_t nocache2 = bench< ls::vector< int32_t >, index_nocache, ls::avx_tag >( "index_nocache AVX ....", runSize, loop );
            uint64_t simdlb2 = bench< ls::vector< int32_t >, container_simd_lb, ls::avx_tag >( "SIMD lower_bound AVX .", runSize, loop );
            uint64_t simdlb2v2 = bench< ls::vector< int32_t >, container_simd_lb2, ls::avx_tag >( "SIMD lower_boundv2 AVX", runSize, loop );
#endif

            std::cout
                      << std::endl << "Index Nocahe Speed up SSE.......: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(nocache) << "x"

                      << std::endl << "Index Cache Speed up SSE........: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(cache) << "x"

                      << std::endl << "SIMD lower_bound Speed up SSE...: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(simdlb) << "x"

                      << std::endl << "Index Cache/Nocache Speed up SSE: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(nocache)/static_cast<float>(cache) << "x"

#ifdef LITESIMD_HAS_AVX
                      << std::endl << "Index Nocahe Speed up AVX.......: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(nocache2) << "x"

                      << std::endl << "Index Cache Speed up AVX........: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(cache2) << "x"

                      << std::endl << "SIMD lower_bound Speed up AVX...: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(simdlb2) << "x"

                      << std::endl << "SIMD lower_boundv2 Speed up AVX.: " << std::fixed << std::setprecision(2)
                      << static_cast<float>(base)/static_cast<float>(simdlb2v2) << "x"
#endif

                      << std::endl << std::endl;
        }
        else
        {
            std::cout
                << base
                << "," << cache
#ifdef LITESIMD_HAS_AVX
                << "," << cache2
#endif
                << std::endl;
        }
    }
    return 0;
}
