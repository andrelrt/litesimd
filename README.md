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
// g++ -std=c++14 -O3 -msse4.2 -I<path/to/litesimd/include> greater_than.cpp -o greater_than

#include <iostream>
#include <litesimd/compare.h>
#include <litesimd/helpers/iostream.h>

int main()
{
    namespace ls = litesimd;

    // int32_t vector in default instruction set (SSE)
    ls::t_int32_simd cmp;

    // litesimd types are intrincs compatible
    cmp = _mm_set_epi32( 40, 30, 20, 10 );

    int32_t val = 5;

    // int32_simd_size is how many int32_t fits on t_int32_simd (4)
    for( size_t i = 0; i <= ls::int32_simd_size; ++i )
    {
        // Compare 'val' against all 'cmp' values
        uint32_t bitmask = ls::greater_than_bitmask( val, cmp );

        // As 'cmp' is sorted, we can use the bitmask to find the
        // last item which 'val' is greater
        //
        // Returns values between [-1, ls::int32_simd_size)
        uint32_t index = ls::bitmask_last_index< int32_t >( bitmask );

        // greater_than_last_index could be called instead
        // greater_than_bitmask + bitmask_last_index
        //
        // uint32_t index = ls::greater_than_last_index( val, cmp );

        if( index == -1 )
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
```
This will produce the follow output:

```
$ ./greater_than
The value 5 is less than all values of (40, 30, 20, 10)
The value 15 is between items 0 and 1 of (40, 30, 20, 10)
The value 25 is between items 1 and 2 of (40, 30, 20, 10)
The value 35 is between items 2 and 3 of (40, 30, 20, 10)
The value 45 is greater than all values of (40, 30, 20, 10)
```

## Building samples and tests

## Support

## Requirements


