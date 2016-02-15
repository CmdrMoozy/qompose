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

#ifndef qompose_core_Utf8String_HPP
#define qompose_core_Utf8String_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

#include <boost/flyweight.hpp>

#include <bdrck/string/StringRef.hpp>

#include "core/Utf8Iterator.hpp"

namespace qompose
{
namespace core
{
class Utf8String
{
public:
	typedef Utf8Iterator iterator;
	typedef Utf8Iterator const_iterator;
	typedef Utf8ReverseIterator reverse_iterator;
	typedef Utf8ReverseIterator const_reverse_iterator;

	Utf8String() noexcept;

	Utf8String(bdrck::string::StringRef const &str);

	/*!
	 * Construct a UTF-8 string from the given bytes. Throws an
	 * exception if the given bytes are not a valid UTF-8 sequence.
	 */
	Utf8String(uint8_t const *begin, uint8_t const *end);

	Utf8String(Utf8String const &) = default;
	Utf8String(Utf8String &&) = default;
	Utf8String &operator=(Utf8String const &) = default;
	Utf8String &operator=(Utf8String &&) = default;

	~Utf8String() = default;

	/*!
	 * \return Whether or not this string is empty, in O(1) time.
	 */
	bool empty() const;

	std::size_t length() const;
	std::size_t size() const;

	const_iterator begin() const;
	const_iterator end() const;
	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;

private:
	const boost::flyweights::flyweight<std::vector<uint8_t>> bytes;
	const std::size_t characterLength;
};
}
}

#endif
