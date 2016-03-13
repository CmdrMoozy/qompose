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

#include <utility>

#include "core/document/Cursor.hpp"
#include "core/document/Piece.hpp"

namespace qompose
{
namespace core
{
namespace document
{
struct PieceTable
{
	typedef Cursor iterator;
	typedef Cursor const_iterator;
	typedef ReverseCursor reverse_iterator;
	typedef ReverseCursor const_reverse_iterator;

	PieceContainer pieces;

	template <typename TextResource> PieceTable(TextResource &&resource);

	PieceTable(PieceTable const &) = default;
	PieceTable(PieceTable &&) = default;
	PieceTable &operator=(PieceTable const &) = default;
	PieceTable &operator=(PieceTable &&) = default;

	Cursor begin() const;
	Cursor end() const;
	ReverseCursor rbegin() const;
	ReverseCursor rend() const;
};

template <typename TextResource>
PieceTable::PieceTable(TextResource &&resource)
        : pieces()
{
	auto b = resource.data();
	auto e = resource.data() + resource.size();
	pieces.emplace_back(std::move(resource),
	                    qompose::core::string::Utf8Iterator(b, e),
	                    qompose::core::string::Utf8Iterator(b, e, e));
}
}
}
}

#endif
