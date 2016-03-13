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

#include "Replace.h"

#include <algorithm>

#include "QomposeCommon/editor/search/Find.h"

namespace qompose
{
namespace editor
{
namespace search
{
FindResult replace(QTextCursor &cursor, QTextDocument const &document,
                   ReplaceQuery const &query)
{
	cursor.setPosition(cursor.selectionStart(), QTextCursor::MoveAnchor);
	FindResult result = find(cursor, document, true, query);

	if((result == FindResult::Found) && cursor.hasSelection())
	{
		cursor.beginEditBlock();
		int start = cursor.selectionStart();
		int length = query.replaceValue.length();

		cursor.insertText(query.replaceValue);
		cursor.setPosition(start, QTextCursor::MoveAnchor);
		cursor.movePosition(QTextCursor::NextCharacter,
		                    QTextCursor::KeepAnchor, length);
		cursor.endEditBlock();
	}

	return result;
}

FindResult batchReplace(QTextCursor &cursor, QTextDocument const &document,
                        ReplaceQuery const &query, boost::optional<int> start,
                        boost::optional<int> end)
{
	// If we weren't given a start position, use the current cursor.
	if(!start)
		start = std::min(cursor.anchor(), cursor.position());

	// We can't enable wrapping, when replacing inside a specific range.
	ReplaceQuery rangeQuery = query;
	rangeQuery.wrap = false;

	// Replace each match we find after our start position.

	cursor.setPosition(*start, QTextCursor::MoveAnchor);
	FindResult result = find(cursor, document, true, rangeQuery);
	bool foundAny = false;
	cursor.beginEditBlock();

	while(result == FindResult::Found)
	{
		// Make sure this match is in range.
		int foundAnchor = cursor.anchor();
		int foundPosition = cursor.position();
		if(!!end && (std::max(foundAnchor, foundPosition) > *end))
			break;

		foundAny = true;

		// Replace this match, and move the cursor after it. We also
		// need to update the "end" value, since we may have changed
		// the length of the document.
		if(!!end)
		{
			int selectionLength = cursor.selectedText().length();
			end = *end + (rangeQuery.replaceValue.length() -
			              selectionLength);
		}
		cursor.insertText(rangeQuery.replaceValue);
		cursor.setPosition(std::min(foundAnchor, foundPosition),
		                   QTextCursor::MoveAnchor);
		cursor.movePosition(QTextCursor::NextCharacter,
		                    QTextCursor::KeepAnchor,
		                    rangeQuery.replaceValue.length());

		// Find the next match (if any).
		result = find(cursor, document, true, rangeQuery);
	};

	cursor.endEditBlock();

	return foundAny ? FindResult::Found : result;
}
}
}
}
