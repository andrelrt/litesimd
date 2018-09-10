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
// g++ -std=c++14 -O3 -avx2 -I<path/to/litesimd/include> greater.cpp -o greater

#include <iostream>
#include <litesimd/compare.h>
#include <litesimd/helpers/iostream.h>

int main()
{
    namespace ls = litesimd;

    // int32_t vector in default instruction set (SSE/AVX)
    ls::t_int32_simd cmp;

    // litesimd types are intrincs compatible
#ifdef LITESIMD_HAS_AVX
    cmp = _mm256_set_epi32( 80, 70, 60, 50, 40, 30, 20, 10 );
#else
    // No AVX, using SSE
    cmp = _mm_set_epi32( 40, 30, 20, 10 );
#endif // LITESIMD_HAS_AVX

    int32_t val = 5;

    // int32_simd_size is how many int32_t fits on t_int32_simd (4 - SSE, 8 - AVX)
    for( size_t i = 0; i <= ls::int32_simd_size; ++i )
    {
        // Compare 'val' against all 'cmp' values
        uint32_t mask = ls::greater_bitmask( val, cmp );

        // As 'cmp' is sorted, we can use the bitmask to find the
        // last item which 'val' is greater
        //
        // Returns values between [-1, ls::int32_simd_size)
        int index = ls::bitmask_last_index< int32_t >( mask );

        // greater_last_index could be called instead
        // greater_bitmask + bitmask_last_index
        //
        // int index = ls::greater_last_index( val, cmp );

        if( index < 0 )
        {
            std::cout << "The value " << val
                      << " is less than all values of " << cmp
                      << std::endl;
        }
        else if( index == ls::int32_simd_size -1 )
        {
            std::cout << "The value " << val
                      << " is greater than all values of " << cmp
                      << std::endl;
        }
        else
        {
            std::cout << "The value " << val
                      << " is between items " << index
                      << " and " << index + 1
                      << " of " << cmp
                      << std::endl;
        }

        val += 10;
    }
    return 0;
}

// The output on AVX will be
//
// $ ./greater
// The value 5 is less than all values of (80, 70, 60, 50, 40, 30, 20, 10)
// The value 15 is between items 0 and 1 of (80, 70, 60, 50, 40, 30, 20, 10)
// The value 25 is between items 1 and 2 of (80, 70, 60, 50, 40, 30, 20, 10)
// The value 35 is between items 2 and 3 of (80, 70, 60, 50, 40, 30, 20, 10)
// The value 45 is between items 3 and 4 of (80, 70, 60, 50, 40, 30, 20, 10)
// The value 55 is between items 4 and 5 of (80, 70, 60, 50, 40, 30, 20, 10)
// The value 65 is between items 5 and 6 of (80, 70, 60, 50, 40, 30, 20, 10)
// The value 75 is between items 6 and 7 of (80, 70, 60, 50, 40, 30, 20, 10)
// The value 85 is greater than all values of (80, 70, 60, 50, 40, 30, 20, 10)
