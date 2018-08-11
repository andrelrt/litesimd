Litesimd is a no overhead, header only, C++ library for SIMD processing. This library goal is to provide tools for developers to incorporate SIMD processing in all kinds of algorithms not only for calculations. To achieve this goal, some design principles are observed.

## Design principles

#### SIMD for all kind of algorithms

Typically SIMD is used for mathematical algorithms, such as linear algebra, FFT or imaging processing. However, the processor has several SIMD instructions for all purposes. Litesimd library has an emphasis on SIMD comparison and data shuffle instead of math operations.

#### Not conceal the SIMD complexity

By design, the library does not attempt to hide the complexity of using SIMD. When using any SIMD library, the developer needs to understand what is happening anyway. So why try to hide this complexity from him? Some C++ operators are (or will be) overloaded for convenience, but this is not the library focus.

#### Intrincs interoperability

Any SIMD library typically covers a smaller scope than the total set of processor SIMD instructions. Litesimd library must be transparently interoperable with SIMD intrincs, allowing the developer to perform more complex operations than originally anticipated by the library.

[WIP]

## Examples

## Building samples and tests

## Support

## Requirements


