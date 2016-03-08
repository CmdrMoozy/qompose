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

#include "Utf8StringRef.hpp"

#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace qompose
{
namespace core
{
namespace string
{
Utf8StringRef::Utf8StringRef() : beginPtr(nullptr), endPtr(nullptr)
{
}

Utf8StringRef::Utf8StringRef(uint8_t const *begin, uint8_t const *end)
        : beginPtr(begin), endPtr(end)
{
}

Utf8StringRef::Utf8StringRef(bdrck::string::StringRef const &str)
        : beginPtr(reinterpret_cast<uint8_t const *>(str.data())),
          endPtr(reinterpret_cast<uint8_t const *>(str.data() + str.size()))
{
}

bool Utf8StringRef::operator==(Utf8StringRef const &o) const
{
	if((endPtr - beginPtr) != (o.endPtr - o.beginPtr))
		return false;
	return std::equal(beginPtr, endPtr, o.beginPtr);
}

bool Utf8StringRef::operator!=(Utf8StringRef const &o) const
{
	return !(*this == o);
}

Utf8StringRef::size_type Utf8StringRef::dataSize() const
{
	return static_cast<size_type>(endPtr - beginPtr);
}

Utf8StringRef::size_type Utf8StringRef::dataLength() const
{
	return dataSize();
}

Utf8StringRef::size_type Utf8StringRef::size() const
{
	return static_cast<size_type>(std::distance(begin(), end()));
}

Utf8StringRef::size_type Utf8StringRef::length() const
{
	return size();
}

Utf8StringRef::size_type Utf8StringRef::max_size() const
{
	return size();
}

bool Utf8StringRef::empty() const
{
	return beginPtr == endPtr;
}

Utf8StringRef::const_iterator Utf8StringRef::begin() const
{
	return const_iterator(beginPtr, endPtr);
}

Utf8StringRef::const_iterator Utf8StringRef::cbegin() const
{
	return const_iterator(beginPtr, endPtr);
}

Utf8StringRef::const_iterator Utf8StringRef::end() const
{
	return const_iterator(beginPtr, endPtr, endPtr);
}

Utf8StringRef::const_iterator Utf8StringRef::cend() const
{
	return const_iterator(beginPtr, endPtr, endPtr);
}

Utf8StringRef::const_reverse_iterator Utf8StringRef::rbegin() const
{
	return const_reverse_iterator(begin());
}

Utf8StringRef::const_reverse_iterator Utf8StringRef::crbegin() const
{
	return const_reverse_iterator(cbegin());
}

Utf8StringRef::const_reverse_iterator Utf8StringRef::rend() const
{
	return const_reverse_iterator(end());
}

Utf8StringRef::const_reverse_iterator Utf8StringRef::crend() const
{
	return const_reverse_iterator(cend());
}

Utf8StringRef::const_iterator::value_type Utf8StringRef::
operator[](size_type pos) const
{
	auto it = begin();
	std::advance(it, pos);
	return *it;
}

Utf8StringRef::const_iterator::value_type Utf8StringRef::at(size_type pos) const
{
	auto it = begin();
	std::advance(it, pos);
	if(it == end())
		throw std::out_of_range("Index out of bounds.");
	return *it;
}

Utf8StringRef::const_iterator::value_type Utf8StringRef::front() const
{
	return *cbegin();
}

Utf8StringRef::const_iterator::value_type Utf8StringRef::back() const
{
	return *crbegin();
}

Utf8StringRef::const_pointer Utf8StringRef::data() const
{
	return beginPtr;
}

void Utf8StringRef::clear()
{
	beginPtr = nullptr;
	endPtr = nullptr;
}
}
}
}
