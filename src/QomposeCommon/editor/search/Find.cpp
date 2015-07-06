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

#include "Find.h"

#include <QRegExp>

namespace
{
/*!
 * Find a string in a document.
 *
 * \param cursor The cursor to start searching from.
 * \param wrapCursor The cursor to restart at if wrapping is enabled.
 * \param document The document to search.
 * \param query The find query to execute.
 * \param flags The find flags to use to perform the search.
 * \return The result of the find operation.
 */
qompose::editor::search::FindResult
findString(QTextCursor &cursor, QTextCursor &wrapCursor,
           QTextDocument const &document,
           qompose::editor::search::FindQuery const &query,
           QTextDocument::FindFlags flags)
{
	QTextCursor found = document.find(query.expression, cursor, flags);
	if(found.isNull() && query.wrap)
		found = document.find(query.expression, wrapCursor, flags);

	if(!found.isNull())
	{
		cursor = found;
		return qompose::editor::search::FindResult::Found;
	}

	return qompose::editor::search::FindResult::NoMatches;
}

/*!
 * Find a regular expression in a document.
 *
 * \param cursor The cursor to start searching from.
 * \param wrapCursor The cursor to restart at if wrapping is enabled.
 * \param document The document to search.
 * \param query The find query to execute.
 * \param flags The find flags to use to perform the search.
 * \return The result of the find operation.
 */
qompose::editor::search::FindResult
findRegularExpression(QTextCursor &cursor, QTextCursor &wrapCursor,
                      QTextDocument const &document,
                      qompose::editor::search::FindQuery const &query,
                      QTextDocument::FindFlags flags)
{
	QRegExp regex(query.expression, Qt::CaseSensitive, QRegExp::RegExp2);
	if(!regex.isValid())
	{
		return qompose::editor::search::FindResult::
		        BadRegularExpression;
	}

	QTextCursor found = document.find(regex, cursor, flags);
	if(found.isNull() && query.wrap)
		found = document.find(regex, wrapCursor, flags);

	if(!found.isNull())
	{
		cursor = found;
		return qompose::editor::search::FindResult::Found;
	}

	return qompose::editor::search::FindResult::NoMatches;
}
}

namespace qompose
{
namespace editor
{
namespace search
{
FindResult find(QTextCursor &cursor, QTextDocument const &document,
                bool forward, FindQuery const &query)
{
	QTextDocument::FindFlags flags = query.getFindFlags(forward);

	QTextCursor wrapCursor(cursor);
	if(flags & QTextDocument::FindBackward)
	{
		wrapCursor.movePosition(QTextCursor::End,
		                        QTextCursor::MoveAnchor);
	}
	else
	{
		wrapCursor.movePosition(QTextCursor::Start,
		                        QTextCursor::MoveAnchor);
	}

	if(query.isRegex)
	{
		return findRegularExpression(cursor, wrapCursor, document,
		                             query, flags);
	}
	else
	{
		return findString(cursor, wrapCursor, document, query, flags);
	}
}
}
}
}
