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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_ALGORITHM_INDENTATION_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_ALGORITHM_INDENTATION_H

#include <cstddef>

#include <QTextCursor>

#include "core/Types.hpp"

namespace qompose
{
namespace editor
{
namespace algorithm
{
/*!
 * \param mode The indentation mode to generate a string for.
 * \param width The width of a single indentation, in spaces.
 * \return A string containing a "single indentation" for the given params.
 */
QString getIndentationString(qompose::core::IndentationMode mode,
                             std::size_t width);

/*!
 * This function increases the indent of all the lines in the given cursor's
 * selection. Partially selected lines are included. This function will insert
 * the indentation string at the beginning of each included line.
 *
 * The resulting selection will have an anchor at the beginning of the first
 * line, and a cursor position at the end of the last line.
 *
 * Also note that this operation is done in a single "edit block," for
 * undo/redo actions.
 *
 * \param cursor The cursor to operate with.
 * \param mode The indentation mode to use.
 * \param width The indentation width to use.
 */
void increaseSelectionIndent(QTextCursor &cursor,
                             qompose::core::IndentationMode mode,
                             std::size_t width);

/*!
 * This function decreases the indent of all the lines in the cursor's
 * selection. Partially selected lines are included.
 *
 * This function considers tabs and sets of spaces the same length as the
 * indentation width to be a single "indentation." This function will remove
 * one indentation from each line in the selection. If no indentations are
 * present, then we will instead look for any arbitrary leading whitespace to
 * remove.
 *
 * The resulting selection will have an anchor at the beginning of the first
 * line, and a cursor position at the end of the last line.
 *
 * Also note that this operation is done in a single "edit block," for
 * undo/redo actions.
 *
 * \param cursor The cursor to operate with.
 * \param mode The indentation mode to use.
 * \param width The indentation width to use.
 */
void decreaseSelectionIndent(QTextCursor &cursor,
                             qompose::core::IndentationMode mode,
                             std::size_t width);

/*!
 * This function provides a standard implementation of the "tab" key for an
 * editor. This function will either insert a "single indentation" string for
 * the given indentation mode and width, or it will increase the indentation
 * of all blocks in the given cursor's selection if it has one.
 *
 * \param cursor The cursor to operate with.
 * \param mode The indentation mode to use.
 * \param width The indentation width to use.
 */
void tab(QTextCursor &cursor, qompose::core::IndentationMode mode,
         std::size_t width);
}
}
}

#endif
