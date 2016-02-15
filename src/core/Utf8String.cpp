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
#include <stdexcept>

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
Utf8String::Utf8String() noexcept
        : bytes(),
          bytesBegin(bytes.get().data()),
          bytesEnd(bytes.get().data() + bytes.get().size()),
          characterLength(0)
{
}

Utf8String::Utf8String(uint8_t const *begin, uint8_t const *end)
        : bytes(getRealUtf8BeginPointer(begin, end), end),
          bytesBegin(bytes.get().data()),
          bytesEnd(bytes.get().data() + bytes.get().size()),
          characterLength(static_cast<std::size_t>(std::distance(
                  const_iterator(bytesBegin, bytesEnd),
                  const_iterator(bytesBegin, bytesEnd, bytesEnd))))
{
}

Utf8String::Utf8String(bdrck::string::StringRef const &str)
        : Utf8String(
                  reinterpret_cast<uint8_t const *>(str.data()),
                  reinterpret_cast<uint8_t const *>(str.data() + str.length()))
{
}

bool Utf8String::operator==(Utf8String const &o) const
{
	if((bytesEnd - bytesBegin) != (o.bytesEnd - o.bytesBegin))
		return false;
	return std::equal(bytesBegin, bytesEnd, o.bytesBegin);
}

bool Utf8String::operator!=(Utf8String const &o) const
{
	return !(*this == o);
}

uint8_t const *Utf8String::data() const
{
	return bytesBegin;
}

Utf8String::size_type Utf8String::dataLength() const
{
	return static_cast<size_type>(bytesEnd - bytesBegin);
}

Utf8String::size_type Utf8String::dataSize() const
{
	return dataLength();
}

bool Utf8String::empty() const
{
	return bytes.get().empty();
}

Utf8String::size_type Utf8String::length() const
{
	return characterLength;
}

Utf8String::size_type Utf8String::size() const
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

Utf8String::iterator::reference Utf8String::at(size_type pos) const
{
	if(pos >= length())
	{
		throw std::out_of_range(
		        "The specified character position is out of range.");
	}
	return (*this)[pos];
}

Utf8String::iterator::reference Utf8String::operator[](size_type pos) const
{
	auto it = begin();
	std::advance(it, pos);
	return *it;
}

Utf8String::iterator::reference Utf8String::front() const
{
	return *begin();
}

Utf8String::iterator::reference Utf8String::back() const
{
	return *rbegin();
}
}
}
