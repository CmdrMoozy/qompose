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

#ifndef qompose_core_string_Utf8StringRef_HPP
#define qompose_core_string_Utf8StringRef_HPP

#include <cstddef>
#include <cstdint>

#include <bdrck/string/StringRef.hpp>

#include "core/string/Utf8Iterator.hpp"

namespace qompose
{
namespace core
{
namespace string
{
class Utf8StringRef
{
public:
	typedef uint8_t value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type &reference;
	typedef value_type const &const_reference;
	typedef value_type *pointer;
	typedef value_type const *const_pointer;
	typedef Utf8Iterator iterator;
	typedef Utf8Iterator const_iterator;
	typedef Utf8ReverseIterator reverse_iterator;
	typedef Utf8ReverseIterator const_reverse_iterator;

	Utf8StringRef();
	Utf8StringRef(uint8_t const *begin, uint8_t const *end);
	Utf8StringRef(bdrck::string::StringRef const &str);

	Utf8StringRef(Utf8StringRef const &) = default;
	Utf8StringRef(Utf8StringRef &&) = default;
	Utf8StringRef &operator=(Utf8StringRef const &) = default;
	Utf8StringRef &operator=(Utf8StringRef &&) = default;

	~Utf8StringRef() = default;

	bool operator==(Utf8StringRef const &o) const;
	bool operator!=(Utf8StringRef const &o) const;

	size_type dataSize() const;
	size_type dataLength() const;

	size_type size() const;
	size_type length() const;
	size_type max_size() const;
	bool empty() const;

	const_iterator begin() const;
	const_iterator cbegin() const;
	const_iterator end() const;
	const_iterator cend() const;
	const_reverse_iterator rbegin() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator rend() const;
	const_reverse_iterator crend() const;

	const_iterator::value_type operator[](size_type pos) const;
	const_iterator::value_type at(size_type pos) const;
	const_iterator::value_type front() const;
	const_iterator::value_type back() const;
	const_pointer data() const;

	void clear();

private:
	const_pointer beginPtr;
	const_pointer endPtr;
};
}
}
}

#endif
