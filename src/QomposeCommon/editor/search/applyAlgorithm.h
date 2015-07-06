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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_SEARCH_APPLY_ALGORITHM_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_SEARCH_APPLY_ALGORITHM_H

#include <utility>

#include <QTextCursor>
#include <QTextDocument>

#include "QomposeCommon/editor/search/Query.h"

namespace qompose
{
namespace editor
{
namespace search
{
/*!
 * This is a utility function which will apply an arbitrary search algorithm
 * to the given generic "editor". The editor type must be a type which has the
 * "textCursor()", "setTextCursor()", and "document()" functions.
 *
 * The algorithm will be passed the editor's current text cursor and document,
 * and the editor's text cursor will be updated using the post-algorithm text
 * cursor.
 *
 * \param editor The editor to apply the algorithm to.
 * \param algorithm The algorithm to apply to the editor.
 * \param arg Any additional arguments to pass to the editor.
 * \return The result of the search algorithm.
 */
template <typename E, typename A, typename... Arg>
FindResult applyAlgorithm(E &editor, A algorithm, Arg... arg)
{
	QTextCursor cursor = editor.textCursor();
	FindResult result = algorithm(cursor, *editor.document(),
	                              std::forward<Arg>(arg)...);
	editor.setTextCursor(cursor);
	return result;
}
}
}
}

#endif
