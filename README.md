Litesimd is a no overhead, header only, C++ library for SIMD processing. This library goal is to provide tools for developers to incorporate SIMD processing in all kinds of algorithms not only for calculations. To achieve this goal, some design principles are observed.

[WIP]

## Design principles

#### SIMD for all kind of algorithms

Typically SIMD is used for mathematical algorithms, such as linear algebra, FFT or imaging processing. However, the processor has several SIMD instructions for all purposes. Litesimd library has an emphasis on SIMD comparison and data shuffle instead of math operations.

#### Not conceal the SIMD complexity

By design, the library does not attempt to hide the complexity of using SIMD. When using any SIMD library, the developer needs to understand what is happening anyway. So why try to hide this complexity from him? Some C++ operators are (or will be) overloaded for convenience, but this is not the library focus.

#### Intrincs interoperability

Any SIMD library typically covers a smaller scope than the total set of processor SIMD instructions. Litesimd library must be transparently interoperable with SIMD intrincs, allowing the developer to perform more complex operations than originally anticipated by the library.

## Example

```cpp
// Compiled with
//
// g++ -std=c++11 -O3 -mavx2 -I<path/to/litesimd/include> greater.cpp -o greater

#include <iostream>
#include <litesimd/compare.h>
#include <litesimd/helpers/iostream.h>

int main()
{
    namespace ls = litesimd;

    // int32_t vector in default instruction set (AVX)
    ls::t_int32_simd cmp;

    // litesimd types are intrincs compatible
    cmp = _mm256_set_epi32( 80, 70, 60, 50, 40, 30, 20, 10 );

    int32_t val = 5;

    // int32_simd_size is how many int32_t fits on t_int32_simd (8)
    for( size_t i = 0; i <= ls::int32_simd_size; ++i )
    {
        // Compare 'val' against all 'cmp' values
        uint32_t bitmask = ls::greater_bitmask( val, cmp );

        // As 'cmp' is sorted, we can use the bitmask to find the
        // last item which 'val' is greater
        //
        // Returns values between [-1, ls::int32_simd_size)
        int index = ls::bitmask_last_index< int32_t >( bitmask );

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
```
This will produce the follow output:

```
$ ./greater
The value 5 is less than all values of (80, 70, 60, 50, 40, 30, 20, 10)
The value 15 is between items 0 and 1 of (80, 70, 60, 50, 40, 30, 20, 10)
The value 25 is between items 1 and 2 of (80, 70, 60, 50, 40, 30, 20, 10)
The value 35 is between items 2 and 3 of (80, 70, 60, 50, 40, 30, 20, 10)
The value 45 is between items 3 and 4 of (80, 70, 60, 50, 40, 30, 20, 10)
The value 55 is between items 4 and 5 of (80, 70, 60, 50, 40, 30, 20, 10)
The value 65 is between items 5 and 6 of (80, 70, 60, 50, 40, 30, 20, 10)
The value 75 is between items 6 and 7 of (80, 70, 60, 50, 40, 30, 20, 10)
The value 85 is greater than all values of (80, 70, 60, 50, 40, 30, 20, 10)
```

## Directory structure

```
litesimd/
    doc/                        ; Doxygen project
    include/litesimd/
        algorithm/
            for_each.h          ; for_each item of simd_type, also for_each index of bitmask
            iota.h              ; Fill vetor with [0, simd_size), eg. (3, 2, 1, 0)
        arch/                   ; Architecture dependent code, should not included directly
        detail/                 ; Internal helper functions/classes, should not included directly
        helpers/
            containers.h        ; Aligned std containers, depends on boost::align
            iostream.h          ; operator<< overload for litesimd types
        algorithm.h             ; min/max, for_each, iota and all other algorithms
        arithmetic.h            ; add, sub, mul, mullo, mulhi, div functions
        bitwise.h               ; bit_and, bit_or, bit_xor and bit_not functions
        compare.h               ; greater, equal_to, mask_to_bitmask, bitmask_to_high/low_index
        intravector.h           ; horizontal reduction
        shuffle.h               ; high/low_insert, blend, get/set<>
        traits.h                ; Internal traits, should not included directly
        types.h                 ; simd_type
    samples/
        binary_search/          ; Benchmark lower_bound implementations
        boyer_moore_horspool/   ; Substring search using SIMD
        bubble_sort/            ; Classic bubble sort in SIMD style
        greater/                ; Simple greater than sample (the same of above)
        nway_tree/              ; Another approach for same lower_bound search, using trees
        to_lower/               ; ASCII to_lower benchmark
    test/                       ; Unit tests
```

## Building samples and tests

## Support

## Requirements


