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

#ifndef LITESIMD_HELPERS_CONTAINERS_H
#define LITESIMD_HELPERS_CONTAINERS_H

#include <vector>
#include <string>
#include <boost/align/aligned_allocator.hpp>

namespace litesimd {

/**
 * \defgroup helpers Helper classes and compatibility
 *
 * Helpers defines a group of type substitution and `std` compatibility functions
 * and classes.
 *
 * Each file must be included separately from the path `<litesimd/helpers/[file].h>`
 */

/**
 * \ingroup helpers
 * \brief Aligned `std::vector`
 *
 * This type is a drop-in substitute to `std::vector` with aligned allocator.
 *
 * Depends on `boost::align`.
 */
template< typename Val_T >
using vector = std::vector< Val_T, boost::alignment::aligned_allocator<Val_T, 64> >;

/**
 * \ingroup helpers
 * \brief Aligned `std::string`
 *
 * This type is a drop-in substitute to `std::string` with aligned allocator.
 *
 * Depends on `boost::align`.
 */
using string = std::basic_string< char,
                                  std::char_traits<char>,
                                  boost::alignment::aligned_allocator<char, 64> >;

} // namespace litesimd

#endif // LITESIMD_HELPERS_CONTAINERS_H
