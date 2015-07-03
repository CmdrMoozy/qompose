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

#include "General.h"

#include <cassert>
#include <utility>

namespace qompose
{
namespace editor
{
namespace algorithm
{
CursorSelectionState::CursorSelectionState(QTextCursor &cursor)
        : startPosition(cursor.anchor()),
          endPosition(cursor.position()),
          startBlock(0),
          endBlock(0),
          blockCount(0)
{
	if(startPosition > endPosition)
		std::swap(startPosition, endPosition);

	int originalAnchor = cursor.anchor();
	int originalPosition = cursor.position();

	cursor.setPosition(startPosition, QTextCursor::MoveAnchor);
	startBlock = cursor.block().blockNumber();
	cursor.setPosition(endPosition, QTextCursor::MoveAnchor);
	endBlock = cursor.block().blockNumber();
	assert(endBlock >= startBlock);
	blockCount = static_cast<std::size_t>(endBlock - startBlock) + 1;

	cursor.setPosition(originalAnchor, QTextCursor::MoveAnchor);
	cursor.setPosition(originalPosition, QTextCursor::KeepAnchor);
}

void setNormalizedSelection(QTextCursor &cursor,
                            CursorSelectionState const &state)
{
	cursor.setPosition(state.startPosition, QTextCursor::MoveAnchor);
	cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
	cursor.setPosition(state.endPosition, QTextCursor::KeepAnchor);
	cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
}

void backspace(QTextCursor &cursor, IndentationMode mode, std::size_t width)
{
	if((mode == IndentationMode::Spaces) && !cursor.hasSelection())
	{
		// If we're using space indentation, and we don't have a
		// selection, then we may want to remove "one indentation"
		// worth of spaces (if the text before our cursor is a series
		// of space indentations).

		int originalPosition = cursor.position();
		cursor.movePosition(QTextCursor::StartOfBlock,
		                    QTextCursor::KeepAnchor);
		QString selectedText = cursor.selectedText();
		cursor.setPosition(originalPosition, QTextCursor::MoveAnchor);
		std::size_t selectedLength =
		        static_cast<std::size_t>(selectedText.length());

		if((selectedLength % width == 0) && (selectedLength >= width))
		{
			bool onlySpaces = true;
			for(std::size_t i = 0; i < selectedLength; ++i)
			{
				if(selectedText[static_cast<int>(i)] != ' ')
				{
					onlySpaces = false;
					break;
				}
			}

			if(onlySpaces)
			{
				// Select one indentation's spaces, so the
				// following deletePreviousChar() call will
				// remove the one full indentation.
				cursor.movePosition(QTextCursor::Left,
				                    QTextCursor::KeepAnchor,
				                    width);
			}
		}
	}

	cursor.deletePreviousChar();
}
}
}
}
