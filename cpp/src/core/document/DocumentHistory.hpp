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

#ifndef qompose_core_document_DocumentHistory_HPP
#define qompose_core_document_DocumentHistory_HPP

#include <stack>

#include <boost/optional/optional.hpp>

#include "core/document/Document.hpp"

namespace qompose
{
namespace core
{
namespace document
{
struct DocumentHistory
{
	std::stack<Document> past;
	std::stack<Document> future;

	DocumentHistory();

	DocumentHistory(DocumentHistory const &) = delete;
	DocumentHistory(DocumentHistory &&) = default;
	DocumentHistory &operator=(DocumentHistory const &) = delete;
	DocumentHistory &operator=(DocumentHistory &&) = default;

	~DocumentHistory() = default;
};

boost::optional<Document> present(DocumentHistory &history);

void undo(DocumentHistory &history);
void redo(DocumentHistory &history);
void push(DocumentHistory &history, Document const &document);
}
}
}

#endif
