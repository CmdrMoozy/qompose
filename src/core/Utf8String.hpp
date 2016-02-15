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
	typedef std::size_t size_type;

	typedef Utf8Iterator iterator;
	typedef Utf8Iterator const_iterator;
	typedef Utf8ReverseIterator reverse_iterator;
	typedef Utf8ReverseIterator const_reverse_iterator;

	Utf8String() noexcept;

	/*!
	 * Construct a UTF-8 string from the given bytes. Throws an
	 * exception if the given bytes are not a valid UTF-8 sequence.
	 */
	Utf8String(uint8_t const *begin, uint8_t const *end);

	Utf8String(bdrck::string::StringRef const &str);

	Utf8String(Utf8String const &) = default;
	Utf8String(Utf8String &&) = default;
	Utf8String &operator=(Utf8String const &) = default;
	Utf8String &operator=(Utf8String &&) = default;

	~Utf8String() = default;

	bool operator==(Utf8String const &o) const;
	bool operator!=(Utf8String const &o) const;

	/*!
	 * Return a pointer to the beginning of the string's encoded data.
	 */
	uint8_t const *data() const;

	/*!
	 * \return The length of the string's encoded data, in bytes.
	 */
	size_type dataLength() const;

	/*!
	 * A synonym for dataLength().
	 */
	size_type dataSize() const;

	/*!
	 * \return Whether or not this string is empty, in O(1) time.
	 */
	bool empty() const;

	/*!
	 * \return The length of the string, in decoded characters.
	 */
	size_type length() const;

	/*!
	 * A synonym for length().
	 */
	size_type size() const;

	const_iterator begin() const;
	const_iterator end() const;
	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;

	/*!
	 * Random character access function, with bounds checking. Beware
	 * that each call to this function is O(n), so using iterators
	 * instead is much more efficient.
	 *
	 * \param pos The position of the desired character.
	 * \return The character at the given position in the string.
	 */
	iterator::reference at(size_type pos) const;

	/*!
	 * A version of at() without bounds checking.
	 *
	 * \param pos The position of the desired character.
	 * \return The character at the given position in the string.
	 */
	iterator::reference operator[](size_type pos) const;

	iterator::reference front() const;
	iterator::reference back() const;

private:
	const boost::flyweights::flyweight<std::vector<uint8_t>> bytes;
	uint8_t const *const bytesBegin;
	uint8_t const *const bytesEnd;
	const std::size_t characterLength;
};
}
}

#endif
