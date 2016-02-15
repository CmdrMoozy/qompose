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

class Utf8Iterator : public std::iterator<std::bidirectional_iterator_tag,
                                          uint32_t, std::ptrdiff_t,
                                          uint32_t const *, uint32_t const &>
{
public:
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
	 * \param c The current pointer for the range.
	 */
	Utf8Iterator(uint8_t const *b, uint8_t const *e,
	             uint8_t const *c = nullptr);

	/*!
	 * Construct an "end" iterator over an empty string.
	 */
	Utf8Iterator();

	Utf8Iterator(Utf8Iterator const &) = default;
	Utf8Iterator(Utf8Iterator &&) = default;
	Utf8Iterator &operator=(Utf8Iterator const &) = default;
	Utf8Iterator &operator=(Utf8Iterator &&) = default;

	~Utf8Iterator() = default;

	bool operator==(Utf8Iterator const &o) const;
	bool operator!=(Utf8Iterator const &o) const;

	Utf8Iterator &operator++();
	Utf8Iterator operator++(int);
	Utf8Iterator &operator--();
	Utf8Iterator operator--(int);

	reference operator*() const;
	pointer operator->() const;

private:
	uint8_t const *begin;
	uint8_t const *end;
	Utf8DecodeResult value;
};

class Utf8ReverseIterator
        : public std::iterator<std::bidirectional_iterator_tag, uint32_t,
                               std::ptrdiff_t, uint32_t const *,
                               uint32_t const &>
{
public:
	Utf8ReverseIterator();
	Utf8ReverseIterator(Utf8Iterator const &o);

	Utf8ReverseIterator(Utf8ReverseIterator const &) = default;
	Utf8ReverseIterator(Utf8ReverseIterator &&) = default;
	Utf8ReverseIterator &operator=(Utf8ReverseIterator const &) = default;
	Utf8ReverseIterator &operator=(Utf8ReverseIterator &&) = default;

	~Utf8ReverseIterator() = default;

	bool operator==(Utf8ReverseIterator const &o) const;
	bool operator!=(Utf8ReverseIterator const &o) const;

	Utf8ReverseIterator &operator++();
	Utf8ReverseIterator &operator++(int);
	Utf8ReverseIterator &operator--();
	Utf8ReverseIterator &operator--(int);

	reference operator*() const;
	pointer operator->() const;

private:
	Utf8Iterator iterator;
};
}
}
