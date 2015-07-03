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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_ALGORITHM_GENERAL_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_ALGORITHM_GENERAL_H

#include <cstddef>
#include <utility>

#include <QTextBlock>
#include <QTextCursor>

#include "QomposeCommon/Types.h"

namespace qompose
{
namespace editor
{
namespace algorithm
{
/*!
 * \brief This structure encapsulates information about a cursor's selection.
 */
struct CursorSelectionState
{
	int startPosition;
	int endPosition;
	int startBlock;
	int endBlock;
	std::size_t blockCount;

	/*!
	 * Construct a new CursorSelectionState from the given cursor. The
	 * given cursor's position and anchor will not be modified by this
	 * constructor.
	 *
	 * \param cursor The cursor to inspect.
	 */
	CursorSelectionState(QTextCursor &cursor);
};

/*!
 * This function will set the given cursor's selection to a noramlized version
 * of the given selection state. In particular, the anchor will be moved to
 * the start of the block containing the state's start position, and then the
 * cursor will be positioned at the end of the block containing the state's
 * end position.
 *
 * \param cursor The cursor to modify.
 * \param state The selection state to normalize and apply to the cursor.
 */
void setNormalizedSelection(QTextCursor &cursor,
                            CursorSelectionState const &state);

/*!
 * This is a utility function which applies the given other function to the
 * given number of blocks, starting with the block containing the given
 * absolute cursor position.
 *
 * The function given will be passed the text cursor, positioned at the start
 * of the block, as well as any other arguments given to this function.
 *
 * \param cursor The cursor to pass to the iteration function.
 * \param position The absolute position of the block to start at.
 * \param blocks The number of blocks to iterate over.
 * \param fn The function to call on each block.
 * \param arg The arguments to pass to the iteration function.
 */
template <typename F, typename... Arg>
void foreachBlock(QTextCursor &cursor, int position, std::size_t blocks, F fn,
                  Arg... arg)
{
	cursor.setPosition(position, QTextCursor::MoveAnchor);
	for(std::size_t i = 0; i < blocks; ++i)
	{
		cursor.movePosition(QTextCursor::StartOfBlock,
		                    QTextCursor::MoveAnchor);
		fn(cursor, std::forward<Arg>(arg)...);
		cursor.movePosition(QTextCursor::NextBlock,
		                    QTextCursor::MoveAnchor);
	}
}

/*!
 * This is a utility function which applies a QTextCursor algorithm to the
 * given arbitrary type of "editor". The editor type must be a type which has
 * the "textCursor()" and "setTextCursor()" functions.
 *
 * The algorithm will be passed the editor's current text cursor, and the
 * editor's text cursor will be updated using the post-algorithm text cursor.
 *
 * \param editor The editor to apply the algorithm to.
 * \param algorithm The algorithm to apply to the editor.
 * \param arg The additional arguments to pass to the editor.
 */
template <typename E, typename A, typename... Arg>
void applyAlgorithm(E &editor, A algorithm, Arg... arg)
{
	QTextCursor cursor = editor.textCursor();
	algorithm(cursor, std::forward<Arg>(arg)...);
	editor.setTextCursor(cursor);
}

/*!
 * This function performs an indentation-sensitive "backspace" action. This is
 * equivalent to the normal "delete one character before the cursor" behavior,
 * except in the case of space indentation. In that case, if the text before
 * the cursor is just a series of space-based indentations, one full indentaion
 * will be removed (so, width spaces will be removed).
 *
 * \param cursor The cursor to operate on.
 * \param mode The indentation mode to use.
 * \param width The indentation width to use.
 */
void backspace(QTextCursor &cursor, IndentationMode mode, std::size_t width);
}
}
}

#endif
