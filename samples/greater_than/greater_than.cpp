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

// Compiled with
//
// g++ -std=c++14 -O3 -msse4.2 -I<path/to/litesimd/include> greater_than.cpp -o greater_than

#include <iostream>
#include <litesimd/compare.h>
#include <litesimd/helpers/iostream.h>

int main()
{
    namespace ls = litesimd;

    // int32_t vector in default instruction set (AVX2)
    ls::t_int32_simd cmp;

    // litesimd types are intrincs compatible
    //cmp = _mm_set_epi32( 40, 30, 20, 10 );

    // With AVX looks similar
    cmp = _mm256_set_epi32( 80, 70, 60, 50, 40, 30, 20, 10 );

    int32_t val = 5;

    // int32_simd_size is how many int32_t fits on t_int32_simd (8)
    for( size_t i = 0; i <= ls::int32_simd_size; ++i )
    {
        // Compare 'val' against all cmp values
        uint32_t mask = ls::greater_than_bitmask( val, cmp );

        // Get the return bitmask and find the first item which val is greater
        uint32_t index = ls::bitmask_last_index< int32_t >( mask );

        if( index == 0 )
        {
            std::cout << "The value " << val
                      << " is less than all values of " << cmp
                      << std::endl;
        }
        else if( index == ls::int32_simd_size )
        {
            std::cout << "The value " << val
                      << " is greater than all values of " << cmp
                      << std::endl;
        }
        else
        {
            std::cout << "The value " << val
                      << " is between itens " << index -1
                      << " and " << index
                      << " of " << cmp
                      << std::endl;
        }

        val += 10;
    }
    return 0;
}
