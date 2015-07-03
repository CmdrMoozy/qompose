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

#include "Indentation.h"

#include <cassert>
#include <utility>

#include <QTextBlock>

#include "QomposeCommon/editor/algorithm/General.h"

namespace
{
struct SelectionIndentationState
{
	int startPosition;
	int endPosition;
	int startBlock;
	int endBlock;
	int blockCount;
};

/*!
 * This function computes the selection indentation state for the given
 * cursor. If the given cursor doesn't have a selection, then this function's
 * behavior is undefined.
 *
 * After this function is finished, the cursor will be positioned at the
 * start of the selection block, and it will no longer have a selection.
 *
 * \param cursor The cursor to inspect and move.
 * \return The state of the given cursor.
 */
SelectionIndentationState getSelectionState(QTextCursor &cursor)
{
	assert(cursor.hasSelection());

	SelectionIndentationState state;

	state.startPosition = cursor.anchor();
	state.endPosition = cursor.position();
	if(state.startPosition > state.endPosition)
		std::swap(state.startPosition, state.endPosition);

	cursor.setPosition(state.startPosition, QTextCursor::MoveAnchor);
	state.startBlock = cursor.block().blockNumber();
	cursor.setPosition(state.endPosition, QTextCursor::MoveAnchor);
	state.endBlock = cursor.block().blockNumber();
	state.blockCount = state.endBlock - state.startBlock + 1;

	cursor.setPosition(state.startPosition, QTextCursor::MoveAnchor);

	return state;
}

/*!
 * This function performs the first step of decreaseSelectionIndent. Namely,
 * we try to remove at most one full indentation string from the beginning of
 * each block in the selection, and we return true if at least one block was
 * modified in this way.
 *
 * \param cursor The cursor to operate on.
 * \param state The cursor's original selection state.
 * \param mode The indentation mode to use.
 * \param width The indentation width to use.
 * \return True if at least one block was successfully de-indented.
 */
bool deindentSelection(QTextCursor &cursor,
                       SelectionIndentationState const &state,
                       qompose::IndentationMode mode, std::size_t width)
{
	bool foundIndent = false;
	QString indentStr =
	        qompose::editor::algorithm::getIndentationString(mode, width);

	for(int i = 0; i <= state.blockCount; ++i)
	{
		cursor.movePosition(QTextCursor::StartOfBlock,
		                    QTextCursor::MoveAnchor);

		// If this block starts with our indentation string, we want
		// to remove that one instance of it from the block.
		if(static_cast<std::size_t>(cursor.block().length()) >= width)
		{
			cursor.movePosition(QTextCursor::NextCharacter,
			                    QTextCursor::KeepAnchor, width);
		}

		if(cursor.hasSelection() &&
		   (cursor.selectedText() == indentStr))
		{
			cursor.removeSelectedText();
			foundIndent = true;
		}

		cursor.movePosition(QTextCursor::NextBlock,
		                    QTextCursor::MoveAnchor);
	}
	return foundIndent;
}

/*!
 * This function performs the optional second step of decreaseSelectionIndent.
 * If no blocks were successfully de-indented, this function will attempt to
 * remove any leading whitespace from any of the lines. See the documentation
 * for decreaseSelectionIndentation for more information on exactly what is
 * done.
 *
 * \param cursor The cursor to operate on.
 * \param state The cursor's original selection state.
 * \param width The indentation width to use.
 */
void dewhitespaceSelection(QTextCursor &cursor,
                           SelectionIndentationState const &state,
                           std::size_t width)
{
	// Try to remove up to the indentation width's number of whitespace
	// characters.
	cursor.setPosition(state.startPosition, QTextCursor::MoveAnchor);
	for(int i = 0; i <= state.blockCount; ++i)
	{
		cursor.movePosition(QTextCursor::StartOfBlock,
		                    QTextCursor::MoveAnchor);

		for(int j = 0; j < static_cast<int>(width); ++j)
		{
			QChar c = cursor.block().text().at(0);

			// Stop at the first non-whitespace char.
			if(!c.isSpace())
				break;

			// If this isn't the first char, and it's a
			// tab instead of a normal space, stop here.
			// We'll remove tabs on the next de-indent.
			if((j > 0) && (c == '\t'))
				break;

			cursor.deleteChar();
		}

		cursor.movePosition(QTextCursor::NextBlock,
		                    QTextCursor::MoveAnchor);
	}
}
}

namespace qompose
{
namespace editor
{
namespace algorithm
{
QString getIndentationString(IndentationMode mode, std::size_t width)
{
	switch(mode)
	{
	case IndentationMode::Spaces:
		return QString(" ").repeated(width);

	case IndentationMode::Tabs:
	default:
		return QString("\t");
	}
}

void increaseSelectionIndent(QTextCursor &cursor, IndentationMode mode,
                             std::size_t width)
{
	if(!cursor.hasSelection())
		return;
	SelectionIndentationState state = getSelectionState(cursor);

	// Do the indent.
	cursor.beginEditBlock();
	foreachBlock(cursor, state.startPosition,
	             static_cast<std::size_t>(state.blockCount),
	             [&mode, &width](QTextCursor &c)
	             {
		             c.insertText(getIndentationString(mode, width));
		     });
	cursor.endEditBlock();

	// Set the cursor's selection to span all of the involved lines.
	cursor.setPosition(state.startPosition, QTextCursor::MoveAnchor);
	cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
	while(cursor.block().blockNumber() < state.endBlock)
	{
		cursor.movePosition(QTextCursor::NextBlock,
		                    QTextCursor::KeepAnchor);
	}
	cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
}

void decreaseSelectionIndent(QTextCursor &cursor, IndentationMode mode,
                             std::size_t width)
{
	if(!cursor.hasSelection())
		return;
	SelectionIndentationState state = getSelectionState(cursor);

	cursor.beginEditBlock();
	if(!deindentSelection(cursor, state, mode, width))
		dewhitespaceSelection(cursor, state, width);
	cursor.endEditBlock();

	// Set the cursor's selection to span all of the involved lines.
	cursor.setPosition(state.startPosition, QTextCursor::MoveAnchor);
	cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
	while(cursor.block().blockNumber() < state.endBlock)
	{
		cursor.movePosition(QTextCursor::NextBlock,
		                    QTextCursor::KeepAnchor);
	}
	cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
}

void tab(QTextCursor &cursor, IndentationMode mode, std::size_t width)
{
	if(!cursor.hasSelection())
		cursor.insertText(getIndentationString(mode, width));
	else
		increaseSelectionIndent(cursor, mode, width);
}
}
}
}
