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

#include <cstddef>
#include <cstdint>
#include <iterator>

namespace qompose
{
namespace core
{
struct Utf8DecodeResult
{
	uint32_t value;            // Current character's processed value.
	uint8_t const *current;    // Current character's begin pointer.
	uint8_t const *currentEnd; // Current character's end pointer.
};

class Utf8Iterator
{
public:
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef uint32_t value_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type const *pointer;
	typedef value_type const &reference;

	/*!
	 * Construct an "end" iterator. End iterators are comparable with
	 * iterators from any real byte range.
	 */
	Utf8Iterator();

	/*!
	 * Construct a "begin" iterator which iterates over the given range
	 * of UTF8-encoded bytes.
	 *
	 * NOTE: This class does NOT deal with BOMs. It is up to the caller
	 * to provide a pointer to the first real byte, skipping the BOM
	 * if one is present.
	 *
	 * \param b The begin pointer for the range.
	 * \param e The end pointer for the range.
	 */
	Utf8Iterator(uint8_t const *b, uint8_t const *e);

	Utf8Iterator(Utf8Iterator const &) = default;
	Utf8Iterator(Utf8Iterator &&) = default;
	Utf8Iterator &operator=(Utf8Iterator const &) = default;
	Utf8Iterator &operator=(Utf8Iterator &&) = default;

	~Utf8Iterator() = default;

	bool operator==(Utf8Iterator const &o) const;
	bool operator!=(Utf8Iterator const &o) const;

	Utf8Iterator &operator++();
	Utf8Iterator operator++(int) const;
	Utf8Iterator &operator--();
	Utf8Iterator operator--(int) const;

	reference operator*() const;
	pointer operator->() const;

private:
	uint8_t const *begin;
	uint8_t const *end;
	Utf8DecodeResult value;
};
}
}
