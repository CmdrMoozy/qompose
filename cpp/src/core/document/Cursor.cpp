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

#include "Cursor.hpp"

#include <cassert>
#include <type_traits>

namespace qompose
{
namespace core
{
namespace document
{
static_assert(
        std::is_same<
                PieceContainer::const_iterator::difference_type,
                qompose::core::string::Utf8Iterator::difference_type>::value,
        "Iterators must have the same difference_type.");

Cursor::Cursor() : pieceBegin(), piece(), pieceEnd(), position()
{
}

Cursor::Cursor(PieceIterator piBegin, PieceIterator pi, PieceIterator piEnd,
               PositionIterator po)
        : pieceBegin(piBegin), piece(pi), pieceEnd(piEnd), position(po)
{
}

bool Cursor::operator==(Cursor const &o) const
{
	return (piece == pieceEnd && o.piece == o.pieceEnd) ||
	       (piece == o.piece && position == o.position);
}

bool Cursor::operator!=(Cursor const &o) const
{
	return !(*this == o);
}

Cursor &Cursor::operator++()
{
	// Either this is the end cursor, or the current position is valid.
	assert(piece == pieceEnd || position != piece->end);

	// If we are already the end cursor, don't move.
	if(piece == pieceEnd)
		return *this;

	// Increment the position. If we've reached the end of the piece,
	// move to the next piece and reset the position instead. If there
	// are no more piece, just reset to being the end cursor instead.
	++position;
	if(position == piece->end)
	{
		++piece;
		if(piece == pieceEnd)
			position = PositionIterator();
		else
			position = piece->begin;
	}

	return *this;
}

Cursor &Cursor::operator--()
{
	// If we are already at the first position, don't move.
	if(piece == pieceBegin && position == piece->begin)
		return *this;

	if((piece == pieceEnd) || (position == piece->begin))
	{
		// If this is the end cursor, or the position is already
		// the first position in the piece, move to the last valid
		// position in the previous piece instead.

		--piece;
		position = piece->end;
		--position;
	}
	else
	{
		// Otherwise, just move to the position before the current
		// position.
		--position;
	}

	return *this;
}

Cursor Cursor::operator++(int)
{
	Cursor temp = *this;
	++(*this);
	return temp;
}

Cursor Cursor::operator--(int)
{
	Cursor temp = *this;
	--(*this);
	return temp;
}

Cursor::reference Cursor::operator*() const
{
	return *position;
}

Cursor::pointer Cursor::operator->() const
{
	return position.operator->();
}

ReverseCursor::ReverseCursor() : preBegin(true), cursor()
{
}

ReverseCursor::ReverseCursor(Cursor const &o) : preBegin(false), cursor(o)
{
	++(*this);
}

bool ReverseCursor::operator==(ReverseCursor const &o) const
{
	return cursor == o.cursor && preBegin == o.preBegin;
}

bool ReverseCursor::operator!=(ReverseCursor const &o) const
{
	return !(*this == o);
}

ReverseCursor &ReverseCursor::operator++()
{
	Cursor temp = cursor;
	--cursor;
	if(cursor == temp)
		preBegin = true;
	return *this;
}

ReverseCursor &ReverseCursor::operator--()
{
	++cursor;
	return *this;
}

ReverseCursor ReverseCursor::operator++(int)
{
	ReverseCursor temp = *this;
	++(*this);
	return temp;
}

ReverseCursor ReverseCursor::operator--(int)
{
	ReverseCursor temp = *this;
	--(*this);
	return temp;
}

ReverseCursor::reference ReverseCursor::operator*() const
{
	return *cursor;
}

ReverseCursor::pointer ReverseCursor::operator->() const
{
	return cursor.operator->();
}
}
}
}
