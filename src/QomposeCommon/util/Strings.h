/*
 * Qompose - A simple programmer's text editor.
 * Copyright (C) 2013 Axel Rasmussen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_STRINGS_H
#define INCLUDE_QOMPOSECOMMON_UTIL_STRINGS_H

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <limits>
#include <numeric>
#include <string>

namespace qompose
{
namespace string_utils
{
/*!
 * This function converts a std::wstring to a std::string using best-effort
 * character conversion for any unicode characters which may be present.
 *
 * \param s The string to convert.
 * \return The converted string.
 */
std::string wstringToString(const std::wstring &s);

/*!
 * This function returns the size of the smallest string in the given iterator
 * range. It is assumed that the Iterator type is such that the type it
 * dereferences to has std::string-style functions.
 *
 * \param s The starting iterator.
 * \param e The ending iterator.
 * \return The size of the smallest string in the iterator range.
 */
template <typename Iterator>
std::size_t getMaxCommonSize(Iterator s, Iterator e)
{
	if(s == e)
		return 0;
	std::size_t maxCommonSize = std::numeric_limits<std::size_t>::max();
	std::accumulate(s, e, maxCommonSize,
	                [](std::size_t max, const std::string &str)
	                {
		                return std::min(max, str.length());
		        });
	return maxCommonSize;
}
}
}

#endif
