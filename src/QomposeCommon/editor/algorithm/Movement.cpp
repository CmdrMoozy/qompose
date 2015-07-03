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

#include "Movement.h"

#include <algorithm>
#include <iterator>

namespace qompose
{
namespace editor
{
namespace algorithm
{
void home(QTextCursor &cursor, bool moveAnchor)
{
	// Find the last whitespace character in the text in front of the
	// block the cursor is currently positioned in.
	QTextCursor positionCursor(cursor);
	positionCursor.setPosition(positionCursor.position(),
	                           QTextCursor::MoveAnchor);
	positionCursor.movePosition(QTextCursor::StartOfBlock,
	                            QTextCursor::KeepAnchor);
	QString preCursorText = positionCursor.selectedText();
	auto whitespaceEnd =
	        std::find_if_not(preCursorText.begin(), preCursorText.end(),
	                         [](const QChar &c) -> bool
	                         {
		                         return c.isSpace();
		                 });

	// Move the cursor to the end of the whitespace if there are any
	// non-whitespace characters before it, or to the start of the block
	// otherwise.
	if(whitespaceEnd != preCursorText.end())
	{
		cursor.movePosition(QTextCursor::Left,
		                    moveAnchor ? QTextCursor::MoveAnchor
		                               : QTextCursor::KeepAnchor,
		                    preCursorText.length() -
		                            std::distance(preCursorText.begin(),
		                                          whitespaceEnd));
	}
	else
	{
		cursor.setPosition(QTextCursor::StartOfBlock,
		                   moveAnchor ? QTextCursor::MoveAnchor
		                              : QTextCursor::KeepAnchor);
	}
}

void goToBlock(QTextCursor &cursor, int block)
{
	block = std::max(block, 0);
	cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
	cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor,
	                    block);
}

void deselect(QTextCursor &cursor)
{
	cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
}
}
}
}
