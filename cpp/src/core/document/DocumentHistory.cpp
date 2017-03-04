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

#include "DocumentHistory.hpp"

namespace qompose
{
namespace core
{
namespace document
{
DocumentHistory::DocumentHistory() : past(), future()
{
}

boost::optional<Document> present(DocumentHistory &history)
{
	if(history.past.empty())
		return boost::none;
	return history.past.top();
}

void undo(DocumentHistory &history)
{
	if(history.past.empty())
		return;
	history.future.push(history.past.top());
	history.past.pop();
}

void redo(DocumentHistory &history)
{
	if(history.future.empty())
		return;
	history.past.push(history.future.top());
	history.future.pop();
}

void push(DocumentHistory &history, Document const &document)
{
	while(!history.future.empty())
		history.future.pop();
	history.past.push(document);
}
}
}
}
