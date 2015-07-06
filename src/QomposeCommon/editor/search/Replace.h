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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_SEARCH_REPLACE_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_SEARCH_REPLACE_H

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
 * This function performs a typical "revere" action using the given cursor and
 * the given document.
 *
 * \param cursor The initial cursor to start searching from.
 * \param document The text document to search.
 * \param query The replace query to execute.
 * \return The result of this replace operation.
 */
FindResult replace(QTextCursor &cursor, QTextDocument const &document,
                   ReplaceQuery const &query);
}
}
}

#endif
