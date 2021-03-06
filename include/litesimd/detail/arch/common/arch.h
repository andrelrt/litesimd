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

#ifndef LITESIMD_ARCH_COMMON_ARCH_H
#define LITESIMD_ARCH_COMMON_ARCH_H

// Check windows
#if defined(_WIN32)
#define LITESIMD_HAS_SSE

#if defined(__AVX2__)
#define LITESIMD_HAS_AVX
#endif //__AVX2__

#endif //_WIN64

// Check GCC
#if defined(__GNUC__)
#if defined(__x86_64__)
#define LITESIMD_HAS_SSE

#if defined(__AVX2__)
#define LITESIMD_HAS_AVX
#endif //__AVX2__

#endif //__x86_64__

#if defined(__ARM_ARCH_7__)
#define LITESIMD_HAS_NEON
#endif //__ARM_ARCH_7__

#endif //__GNUC__


#endif // LITESIMD_ARCH_COMMON_ARCH_H
