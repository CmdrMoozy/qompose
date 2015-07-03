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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_ALGORITHM_MOVEMENT_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_ALGORITHM_MOVEMENT_H

#include <QTextCursor>

namespace qompose
{
namespace editor
{
namespace algorithm
{
/*!
 * This function performs an indentation-sensitive "home" action with the given
 * cursor. We'll move the cursor to the end of its current block's leading
 * whitespace, or to the beginning of the block if only whitespace appears
 * before the cursor already.
 *
 * \param cursor The cursor to move.
 * \param moveAnchor Whether or not to move the cursor's current anchor.
 */
void home(QTextCursor &cursor, bool moveAnchor);

/*!
 * This function will move the given cursor to the beginning of the specified
 * block. Note that text blocks are zero-indexed. If the given block number is
 * out of bounds, the cursor will simply be moved to the first or last block.
 *
 * \param cursor The cursor to move.
 * \param block The number of the block to move to.
 */
void goToBlock(QTextCursor &cursor, int block);

/*!
 * This function clears the given cursor's current selection (if any) by moving
 * its anchor to the same position as the cursor.
 *
 * \param cursor The cursor whose selection should be cleared.
 */
void deselect(QTextCursor &cursor);
}
}
}

#endif
