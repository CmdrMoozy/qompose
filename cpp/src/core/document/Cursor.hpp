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

#ifndef qompose_core_document_Cursor_HPP
#define qompose_core_document_Cursor_HPP

#include <iterator>

#include "core/document/Piece.hpp"
#include "core/string/Utf8Iterator.hpp"

namespace qompose
{
namespace core
{
namespace document
{
namespace detail
{
typedef std::iterator<std::bidirectional_iterator_tag,
                      qompose::core::string::Utf8Iterator::value_type,
                      qompose::core::string::Utf8Iterator::difference_type,
                      qompose::core::string::Utf8Iterator::pointer,
                      qompose::core::string::Utf8Iterator::reference>
        CursorTraits;
}

class Cursor : public detail::CursorTraits
{
public:
	typedef PieceContainer::const_iterator PieceIterator;
	typedef qompose::core::string::Utf8Iterator PositionIterator;

	Cursor();
	Cursor(PieceIterator piBegin, PieceIterator pi, PieceIterator piEnd,
	       PositionIterator po);

	Cursor(Cursor const &) = default;
	Cursor(Cursor &&) = default;
	Cursor &operator=(Cursor const &) = default;
	Cursor &operator=(Cursor &&) = default;

	~Cursor() = default;

	bool operator==(Cursor const &o) const;
	bool operator!=(Cursor const &o) const;

	Cursor &operator++();
	Cursor &operator--();
	Cursor operator++(int);
	Cursor operator--(int);

	reference operator*() const;
	pointer operator->() const;

private:
	PieceIterator pieceBegin;
	PieceIterator piece;
	PieceIterator pieceEnd;
	PositionIterator position;
};

class ReverseCursor : public detail::CursorTraits
{
public:
	ReverseCursor();
	ReverseCursor(Cursor const &o);

	ReverseCursor(ReverseCursor const &) = default;
	ReverseCursor(ReverseCursor &&) = default;
	ReverseCursor &operator=(ReverseCursor const &) = default;
	ReverseCursor &operator=(ReverseCursor &&) = default;

	~ReverseCursor() = default;

	bool operator==(ReverseCursor const &o) const;
	bool operator!=(ReverseCursor const &o) const;

	ReverseCursor &operator++();
	ReverseCursor &operator--();
	ReverseCursor operator++(int);
	ReverseCursor operator--(int);

	reference operator*() const;
	pointer operator->() const;

private:
	bool preBegin;
	Cursor cursor;
};
}
}
}

#endif
