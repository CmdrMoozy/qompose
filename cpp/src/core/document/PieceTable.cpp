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

#include "PieceTable.hpp"

namespace qompose
{
namespace core
{
namespace document
{
Cursor PieceTable::begin() const
{
	auto beginPiece = pieces.begin();
	auto endPiece = pieces.end();
	return Cursor(beginPiece, beginPiece, endPiece,
	              beginPiece == endPiece ? Cursor::PositionIterator()
	                                     : beginPiece->begin);
}

Cursor PieceTable::end() const
{
	return Cursor(pieces.begin(), pieces.end(), pieces.end(),
	              Cursor::PositionIterator());
}

ReverseCursor PieceTable::rbegin() const
{
	return ReverseCursor(end());
}

ReverseCursor PieceTable::rend() const
{
	return ReverseCursor(begin());
}
}
}
}
