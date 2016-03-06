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

#ifndef qompose_core_document_PieceTable_HPP
#define qompose_core_document_PieceTable_HPP

#include <list>
#include <utility>

#include "core/Utf8String.hpp"

namespace qompose
{
namespace core
{
namespace document
{
struct Cursor;

typedef std::pair<Utf8String::const_iterator, Utf8String::const_iterator> Piece;
typedef std::list<Piece> PieceTable;

PieceTable remove(PieceTable const &pieces, Cursor const &begin,
                  Cursor const &end);
}
}
}

#endif
