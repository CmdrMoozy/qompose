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

#include <cstdint>
#include <list>
#include <memory>
#include <utility>

#include "core/string/Utf8Iterator.hpp"

namespace qompose
{
namespace core
{
namespace document
{
struct Cursor;

class Piece
{
private:
	std::shared_ptr<void> resource;

public:
	qompose::core::string::Utf8Iterator begin;
	qompose::core::string::Utf8Iterator end;

	template <typename TextResource>
	Piece(TextResource &&r, qompose::core::string::Utf8Iterator b,
	      qompose::core::string::Utf8Iterator e);

	Piece(Piece const &) = default;
	Piece(Piece &&) = default;
	Piece &operator=(Piece const &) = default;
	Piece &operator=(Piece &&) = default;

	~Piece() = default;
};

struct PieceTable
{
	typedef std::list<Piece> PieceContainer;

	PieceContainer pieces;

	template <typename TextResource> PieceTable(TextResource &&resource);

	PieceTable(PieceTable const &) = default;
	PieceTable(PieceTable &&) = default;
	PieceTable &operator=(PieceTable const &) = default;
	PieceTable &operator=(PieceTable &&) = default;
};

PieceTable remove(PieceTable const &pieces, Cursor const &begin,
                  Cursor const &end);

template <typename TextResource>
Piece::Piece(TextResource &&r, qompose::core::string::Utf8Iterator b,
             qompose::core::string::Utf8Iterator e)
        : resource(std::make_shared<TextResource>(std::move(r))),
          begin(b),
          end(e)
{
}

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
