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
}
}
}
