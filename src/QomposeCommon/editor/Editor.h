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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_EDITOR_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_EDITOR_H

#include "QomposeCommon/editor/DecoratedTextEdit.h"
#include "QomposeCommon/editor/search/Find.h"
#include "QomposeCommon/util/HotkeyMap.h"

#include <QSize>

class QFocusEvent;
class QResizeEvent;
class QKeyEvent;
class QWheelEvent;
class QMouseEvent;

namespace qompose
{
class FindQuery;
class ReplaceQuery;

/*!
 * \brief This class implements our main text editor widget.
 *
 * We depend on our decorated text edit for the various editor decorations we
 * support, while we add advanced editor functionality like improved line
 * indentation semantics, find and replace, and etc.
 */
class Editor : public DecoratedTextEdit
{
	Q_OBJECT

public:
	/*!
	 * This function initializes a new editor widget, using the given
	 * parent widget.
	 *
	 * \param p Our parent widget.
	 */
	Editor(QWidget *p = nullptr);

	Editor(const Editor &) = delete;
	virtual ~Editor() = default;

	Editor &operator=(const Editor &) = delete;

protected:
	/*!
	 * We implement the following hotkeys:
	 *
	 * Custom hotkeys implemented by this class:
	 *      Backspace        Deletes the char to the left of the cursor.
	 *      Delete           Deletes the char to the right of the cursor.
	 * 	Return           Move to a new line, maintaining indent.
	 * 	Enter            Move to a new line, maintaining indent.
	 * 	Tab              Increase indent on selection.
	 * 	Shift+Backtab    Decrease indent on selection.
	 *      Shift+Tab        Decrease indent on selection.
	 * 	Home             Move to non-indent start, or to line start.
	 *      Shift+Home       Select to non-indent start, or to line start.
	 *      Ctrl+D           Duplicate the current line.
	 *      Shift+Return     The same as Return.
	 *      Shift+Ender      The same as Enter.
	 *      Ctrl+(Zero)      Reset text zoom to default level.
	 *
	 * Inherited from QPlainTextEdit:
	 * 	Ctrl+C           Copy selected text to clipboard.
	 * 	Ctrl+V           Paste clipboard into text edit.
	 * 	Ctrl+X           Deletes selected text + copies to clipboard.
	 * 	Ctrl+Z           Undo.
	 * 	Ctrl+Y           Redo.
	 * 	Left             Moves the cursor one character left.
	 * 	Ctrl+Left        Moves the cursor one word left.
	 * 	Right            Moves the cursor one character right.
	 * 	Ctrl+Right       Moves the cursor one word right.
	 * 	Up               Moves the cursor one line up.
	 * 	Down             Moves the cursor one line down.
	 * 	PageUp           Moves the cursor one page up.
	 * 	PageDown         Moves the cursor one page down.
	 * 	Ctrl+Home        Moves the cursor to the document start.
	 * 	End              Moves the cursor to the end of the line.
	 * 	Ctrl+End         Moves the cursor to the end of the document.
	 * 	Alt+Wheel        Scrolls the page horizontally.
	 *
	 * Inherited, but ignored by this class:
	 *      Ctrl+Shift+Left  Select to beginning of line.
	 *      Ctrl+Shift+Right Select to end of line.
	 * 	Ctrl+Insert      Copy selected text to clipboard.
	 * 	Ctrl+K           Deletes to the end of the line.
	 * 	Shift+Insert     Paste clipboard into text edit.
	 * 	Shift+Delete     Deletes selected text + copies to clipboard.
	 *
	 * \param e The event being handled.
	 */
	virtual void keyPressEvent(QKeyEvent *e);

private:
	HotkeyMap hotkeys;

	/*!
	 * This function initializes our hotkeys map, which is used by our
	 * keyPressEvent handler to decide what action to take when a given
	 * hotkey is pressed.
	 */
	void initializeHotkeys();

	/*!
	 * This function is a "no-op". It is used for hotkeys we recognize, but
	 * which we do not take any action for (e.g., if we are capturing one
	 * of our superclass's hotkeys in order to intentionally ignore it).
	 */
	void doNoop();

	/*!
	 * This function implements the standard "backspace" key action.
	 */
	void doBackspace();

	/*!
	 * This function implements the standard "delete" key action.
	 */
	void doDelete();

	/*!
	 * This function inserts a new line at the current text cursor
	 * position, optionally preserving the current line's leading
	 * whitespace (i.e., indent).
	 */
	void doNewline();

	/*!
	 * This function performs a "tab" action, by either increasing the
	 * indentation of the current selection (if applicable), or by simply
	 * inserting a single tab character otherwise.
	 */
	void doTab();

	/*!
	 * This is a simple utility function which moves the cursor to the
	 * beginning of the current line. This is equivalent to calling
	 * doHome(true).
	 */
	void doMoveHome();

	/*!
	 * This is a simple utility function which moves the cursor to the
	 * beginning of the current line, without moving the anchor - i.e.,
	 * selecting. This is equivalent to calling doHome(false).
	 */
	void doSelectHome();

	/*!
	 * This function executes a given find query, using the given
	 * predefined direction. Note that we do not inspect the direction
	 * properties of the given query - it is assumed that our caller has
	 * resolved these into a single direction, based upon whether this is a
	 * "find next" or a "find previous" operation, and whether or not the
	 * query is reversed.
	 *
	 * \param f True means we should go forward, false means backward.
	 * \param q The query to execute.
	 * \return The results of executing this find query.
	 */
	editor::search::FindResult doFind(bool f, const FindQuery *q);

	/*!
	 * This is a utility function which will replace ever match of the
	 * given replace query from the given start position until the given
	 * end position.
	 *
	 * If the start position is omitted, then we will simply use the
	 * starting position from the current cursor (the smaller of its anchor
	 * and position).
	 *
	 * If the end position is omitted, then we will simply continue until
	 * the end of the document.
	 *
	 * \param q The replace query to execute.
	 * \param start The cursor position to start searching from.
	 * \param end The cursor position to stop searching at.
	 * \return The result of this replacement's find operation.
	 */
	editor::search::FindResult doBatchReplace(const ReplaceQuery *q,
	                                          int start = -1, int end = -1);

public Q_SLOTS:
	/*!
	 * This slot duplicates the current line. The new line is inserted
	 * below the current line, without altering our cursor position. Note
	 * that this entire operation is a single "edit block," for undo/redo
	 * operations.
	 */
	void duplicateLine();

	/*!
	 * This slot clears any selection our editor may have. This is done by
	 * simply discarding the "anchor" portion of the current cursor,
	 * leaving its actual "position" where it was.
	 */
	void deselect();

	/*!
	 * This slot increases the indent of all the lines in the current
	 * selection. Partially selected lines are included. This function will
	 * insert a single tab character at the beginning of each included
	 * line.
	 *
	 * The resulting selection will have an anchor at the beginning of the
	 * first line, and a cursor position at the end of the last line.
	 *
	 * Also note that this operation is done in a single "edit block," for
	 * undo/redo actions.
	 */
	void increaseSelectionIndent();

	/*!
	 * This slot decreases the indent of all the lines in the current
	 * selection. Partially selected lines are included.
	 *
	 * This slot considers tabs and sets of spaces the same length as the
	 * tab width to be a single "indentation." This function will remove
	 * one indentation from each line in the selection. If no indentations
	 * are present, then we will instead look for any arbitrary leading
	 * spaces to remove.
	 *
	 * The resulting selection will have an anchor at the beginning of the
	 * first line, and a cursor position at the end of the last line.
	 *
	 * Also note that this operation is done in a single "edit block," for
	 * undo/redo actions.
	 */
	void decreaseSelectionIndent();

	/*!
	 * This function moves the cursor to the beginning of the current line,
	 * following some special rules:
	 *
	 *     - By default, the cursor is simply moved to the beginning of the
	 *       line's indentation - that is, just before the first
	 *       non-whitespace character on the line.
	 *
	 *     - If there is only whitespace before the position the cursor
	 *       already occupies, then the cursor is moved to the very
	 *       beginning of the line instead - that is, just before the first
	 *       character on the line.
	 *
	 * Additionally, moving the cursor's anchor is optional. This means
	 * that this function will either move the cursor, or select using the
	 * cursor, following these rules.
	 *
	 * \param moveAnchor Whether or not the cursor's anchor should be moved.
	 */
	void doHome(bool moveAnchor);

	/*!
	 * This slot moves to the next match based upon the given find query
	 * and the current cursor location.
	 *
	 * \param q The query to execute.
	 * \return The result of the find query's execution.
	 */
	editor::search::FindResult findNext(const FindQuery *q);

	/*!
	 * This slot moves to the previous find match based upon the given find
	 * query and the current cursor location.
	 *
	 * \param q The query to execute.
	 * \return The result of the find query's execution.
	 */
	editor::search::FindResult findPrevious(const FindQuery *q);

	/*!
	 * This slot performs a single replace operation. We will replace the
	 * very next match of the given query with the query's replace value.
	 * Note that we will begin searching at the start of the current
	 * cursor's selection (or its position, if it has no selection).
	 *
	 * \param q The replace query to execute.
	 * \return The result of this replacement's find operation.
	 */
	editor::search::FindResult replace(const ReplaceQuery *q);

	/*!
	 * This slot performs a "replace in selection" operation. We will
	 * replace every single match of the given query in our editor's
	 * current selection. If we do not have any selection, then we will
	 * return NoMatches instead.
	 *
	 * Note that we will return Found if at least one match is replaced, or
	 * NoMatches otherwise.
	 *
	 * \param q The replace query to execute.
	 * \return The result of this replacement's find operation.
	 */
	editor::search::FindResult replaceSelection(const ReplaceQuery *q);

	/*!
	 * This slot performs a "replace all" operation. We will replace every
	 * single match of the given query in our editor's document.
	 *
	 * Note that we will return Found if at least one match is replaced, or
	 * NoMatches otherwise.
	 *
	 * \param q The replace query to execute.
	 * \return The result of this replacement's find operation.
	 */
	editor::search::FindResult replaceAll(const ReplaceQuery *q);

	/*!
	 * This function will move our cursor to the very beginning of the
	 * given line number. Note that the resulting cursor will be at the
	 * very beginning of the block (i.e., line), and it will have no
	 * selection.
	 *
	 * Line numbers less than 1 will result in the cursor being positioned
	 * at the very beginning of the document, and line numbers larger than
	 * our document's last line will result in the cursor being positioned
	 * at the beginning of the very last line in the document.
	 *
	 * \param l The destination line number.
	 */
	void goToLine(int l);

Q_SIGNALS:
	void searchWrapped();
};
}

#endif