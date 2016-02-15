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

#include "Utf8String.hpp"

#include <iterator>

namespace
{
/*!
 * Find the "real" begin pointer for the given UTF8-encoded byte range.
 * This deals with skipping a BOM, if one is present.
 *
 * \param begin The "raw" begin pointer of the range.
 * \param end The "raw" end pointer of the range.
 */
uint8_t const *getRealUtf8BeginPointer(uint8_t const *begin, uint8_t const *end)
{
	if(begin + 3 <= end)
	{
		if(*begin == 0xEFU && *(begin + 1) == 0xBBU &&
		   *(begin + 2) == 0xBFU)
		{
			return begin + 3;
		}
	}

	return begin;
}
}

namespace qompose
{
namespace core
{
Utf8String::Utf8String() noexcept : bytes(), characterLength(0)
{
}

Utf8String::Utf8String(bdrck::string::StringRef const &str)
        : Utf8String(
                  reinterpret_cast<uint8_t const *>(str.data()),
                  reinterpret_cast<uint8_t const *>(str.data() + str.length()))
{
}

Utf8String::Utf8String(uint8_t const *begin, uint8_t const *end)
        : bytes(getRealUtf8BeginPointer(begin, end), end),
          characterLength(static_cast<std::size_t>(std::distance(
                  const_iterator(bytes.get().data(),
                                 bytes.get().data() + bytes.get().size()),
                  const_iterator(bytes.get().data(),
                                 bytes.get().data() + bytes.get().size(),
                                 bytes.get().data() + bytes.get().size()))))
{
}

bool Utf8String::empty() const
{
	return bytes.get().empty();
}

std::size_t Utf8String::length() const
{
	return characterLength;
}

std::size_t Utf8String::size() const
{
	return length();
}

Utf8String::const_iterator Utf8String::begin() const
{
	return const_iterator(bytes.get().data(),
	                      bytes.get().data() + bytes.get().size());
}

Utf8String::const_iterator Utf8String::end() const
{
	return const_iterator(bytes.get().data(),
	                      bytes.get().data() + bytes.get().size(),
	                      bytes.get().data() + bytes.get().size());
}

Utf8String::const_reverse_iterator Utf8String::rbegin() const
{
	return const_reverse_iterator(end());
}

Utf8String::const_reverse_iterator Utf8String::rend() const
{
	return const_reverse_iterator(begin());
}
}
}
