Litesimd is a no overhead, header only, C++ library for SIMD processing. This library goal is to provide tools for developers to incorporate SIMD processing in all kinds of algorithms not only for calculations. To achieve this goal, some design principles are observed.

## Design principles

#### SIMD for all kind of algorithms

Typically SIMD is used for mathematical algorithms, such as linear algebra, FFT or imaging processing. However, the processor has several SIMD instructions for all purposes. Litesimd library has an emphasis on SIMD comparison and data shuffle instead of math operations.

#### Not conceal the SIMD complexity

By design, the library does not attempt to hide the complexity of using SIMD. When using any SIMD library, the developer needs to understand what is happening anyway. So why try to hide this complexity from him? Some C++ operators are (or will be) overloaded for convenience, but this is not the library focus.

#### Intrincs interoperability

Any SIMD library typically covers a smaller scope than the total set of processor SIMD instructions. Litesimd library must be transparently interoperable with SIMD intrincs, allowing the developer to perform more complex operations than originally anticipated by the library.

[WIP]

## Example

```cpp
// Compiled with
//
// g++ -O3 -mavx2 -I<path/to/litesimd/include> compare.cpp -o compare_avx

#include <iostream>
#include <litesimd/compare.h>
#include <litesimd/helpers/iostream.h>

int main()
{
    namespace ls = litesimd;

    // int32_t vector in default instruction set (AVX2)
    ls::t_int32_simd cmp;

    // litesimd types are intrincs compatible
    cmp = _mm256_set_epi32( 80, 70, 60, 50, 40, 30, 20, 10 );

    // With SSE looks similar
    // cmp = _mm_set_epi32( 40, 30, 20, 10 );

    int32_t val = 5;

    // int32_simd_size is how many int32_t fits on t_int32_simd (8)
    for( size_t i = 0; i <= ls::int32_simd_size; ++i )
    {
        // Compare 'val' against all cmp values
        uint32_t mask = ls::greater_than_bitmask( val, cmp );

        // Get the return bitmask and find the first item which val is greater
        uint32_t index = ls::bitmask_high_index( mask );

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
    }
    return 0;
}
```


## Building samples and tests

## Support

## Requirements


