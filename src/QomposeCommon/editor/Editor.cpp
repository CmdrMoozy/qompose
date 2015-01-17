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

#include "Editor.h"

#include <cstddef>
#include <utility>

#include <QGridLayout>
#include <QPainter>
#include <QTextBlock>
#include <QColor>
#include <QRect>
#include <QFont>
#include <QList>
#include <QWheelEvent>
#include <QFocusEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QTextCursor>
#include <QRegExp>
#include <QMouseEvent>
#include <QFontMetrics>
#include <QRegExp>

#include "QomposeCommon/util/FindQuery.h"
#include "QomposeCommon/util/Hotkey.h"
#include "QomposeCommon/util/ReplaceQuery.h"

#define CALL_HOTKEY_FUNCTION(obj, fn) ((obj).*(fn))

namespace qompose
{
/*!
 * This function initializes a new editor widget, using the given parent
 * widget.
 *
 * \param p Our parent widget.
 */
Editor::Editor(QWidget *p) : DecoratedTextEdit(p), hotkeys()
{
	initializeHotkeys();
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
Editor::~Editor()
{
}

/*!
 * We implement the following hotkeys:
 *
 * Custom hotkeys implemented by this class:
 *      Backspace        Deletes the character to the left of the cursor.
 *      Delete           Deletes the character to the right of the cursor.
 * 	Return           Move to a new line, maintaining indent.
 * 	Enter            Move to a new line, maintaining indent.
 * 	Tab              Increase indent on selection.
 * 	Shift+Backtab    Decrease indent on selection.
 *      Shift+Tab        Decrease indent on selection.
 * 	Home             Move to start of non-indent, then to start of line.
 *      Shift+Home       Select to start of non-indent, then to start of line.
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
 * 	Ctrl+Home        Moves the cursor to the beginning of the document.
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
void Editor::keyPressEvent(QKeyEvent *e)
{
	const HotkeyFunction *handler = hotkeys.getHotkeyHandler(e);

	if(handler != nullptr)
		CALL_HOTKEY_FUNCTION (*this, *handler)();
	else
		DecoratedTextEdit::keyPressEvent(e);
}

/*!
 * This function initializes our hotkeys map, which is used by our
 * keyPressEvent handler to decide what action to take when a given hotkey is
 * pressed.
 */
void Editor::initializeHotkeys()
{
	// Backspace

	hotkeys.addHotkey(Hotkey(Qt::Key_Backspace, nullptr,
	                         ~Qt::KeyboardModifiers(nullptr)),
	                  &Editor::doBackspace);

	// Delete

	hotkeys.addHotkey(Hotkey(Qt::Key_Delete, nullptr,
	                         ~Qt::KeyboardModifiers(nullptr)),
	                  &Editor::doDelete);

	// Enter

	hotkeys.addHotkey(Hotkey(Qt::Key_Return, nullptr,
	                         ~Qt::KeyboardModifiers(nullptr)),
	                  &Editor::doNewline);

	hotkeys.addHotkey(
	        Hotkey(Qt::Key_Enter, nullptr, ~Qt::KeyboardModifiers(nullptr)),
	        &Editor::doNewline);

	// Tab

	hotkeys.addHotkey(Hotkey(Qt::Key_Tab), &Editor::doTab);

	// Shift + Tab

	hotkeys.addHotkey(Hotkey(Qt::Key_Tab, Qt::ShiftModifier),
	                  &Editor::decreaseSelectionIndent);

	hotkeys.addHotkey(Hotkey(Qt::Key_Backtab, Qt::ShiftModifier),
	                  &Editor::decreaseSelectionIndent);

	// Home

	hotkeys.addHotkey(Hotkey(Qt::Key_Home), &Editor::doMoveHome);

	// Shift + Home

	hotkeys.addHotkey(Hotkey(Qt::Key_Home, Qt::ShiftModifier),
	                  &Editor::doSelectHome);

	// Ctrl+D

	hotkeys.addHotkey(Hotkey(Qt::Key_D, Qt::ControlModifier),
	                  &Editor::duplicateLine);

	// Ctrl+(Zero)

	hotkeys.addHotkey(Hotkey(Qt::Key_0, Qt::ControlModifier),
	                  &Editor::resetFontZoom);

	// Ctrl+Shift+Left

	hotkeys.addHotkey(
	        Hotkey(Qt::Key_Left, Qt::ControlModifier | Qt::ShiftModifier),
	        &Editor::doNoop);

	// Ctrl+Shift+Right

	hotkeys.addHotkey(
	        Hotkey(Qt::Key_Right, Qt::ControlModifier | Qt::ShiftModifier),
	        &Editor::doNoop);

	// Ctrl+Insert

	hotkeys.addHotkey(Hotkey(Qt::Key_Insert, Qt::ControlModifier),
	                  &Editor::doNoop);

	// Ctrl+K

	hotkeys.addHotkey(Hotkey(Qt::Key_K, Qt::ControlModifier),
	                  &Editor::doNoop);

	// Shift+Insert

	hotkeys.addHotkey(Hotkey(Qt::Key_Insert, Qt::ShiftModifier),
	                  &Editor::doNoop);

	// Shift+Delete

	hotkeys.addHotkey(Hotkey(Qt::Key_Delete, Qt::ShiftModifier),
	                  &Editor::doNoop);
}

/*!
 * This function is a "no-op". It is used for hotkeys we recognize, but which
 * we do not take any action for (e.g., if we are capturing one of our
 * superclass's hotkeys in order to intentionally ignore it).
 */
void Editor::doNoop()
{
}

/*!
 * This function implements the standard "backspace" key action.
 */
void Editor::doBackspace()
{
	QTextCursor curs = textCursor();
	int originalPosition = curs.position();

	if((getIndentationMode() == IndentationMode::Spaces) &&
	   (!curs.hasSelection()))
	{
		curs.movePosition(QTextCursor::StartOfBlock,
		                  QTextCursor::KeepAnchor, 1);
		QString selectedText = curs.selectedText();
		curs.setPosition(originalPosition, QTextCursor::MoveAnchor);

		if(selectedText.length() % getIndentationWidth() == 0)
		{
			bool onlySpaces = true;

			for(int i = 0; i < selectedText.length(); ++i)
			{
				if(selectedText[i] != ' ')
				{
					onlySpaces = false;
					break;
				}
			}

			if(onlySpaces)
			{
				curs.movePosition(QTextCursor::Left,
				                  QTextCursor::KeepAnchor,
				                  getIndentationWidth() - 1);
				curs.removeSelectedText();
			}
		}
	}

	curs.deletePreviousChar();

	setTextCursor(curs);
}

/*!
 * This function implements the standard "delete" key action.
 */
void Editor::doDelete()
{
	QTextCursor curs = textCursor();
	curs.deleteChar();
	setTextCursor(curs);
}

/*!
 * This function inserts a new line at the current text cursor position,
 * optionally preserving the current line's leading whitespace (i.e., indent).
 */
void Editor::doNewline()
{
	// Compute the text to insert to preserve the previous line's indent.

	QString insert = "\n";

	QTextCursor l = textCursor();

	if(l.hasSelection())
		l.setPosition(l.selectionStart(), QTextCursor::MoveAnchor);

	l.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);

	QString line = l.selectedText();

	line.replace(QRegExp("^([ \\t]*)\\S.*$", Qt::CaseSensitive,
	                     QRegExp::RegExp2),
	             "\\1");

	insert.append(line);

	// Insert that text, and set our text cursor.

	QTextCursor curs = textCursor();
	curs.insertText(insert);
	setTextCursor(curs);
}

/*!
 * This function performs a "tab" action, by either increasing the indentation
 * of the current selection (if applicable), or by simply inserting a single
 * tab character otherwise.
 */
void Editor::doTab()
{
	QTextCursor curs = textCursor();

	if(!curs.hasSelection())
	{
		curs.insertText(getIndentString());
		setTextCursor(curs);
	}
	else
	{
		increaseSelectionIndent();
	}
}

/*!
 * This is a simple utility function which moves the cursor to the beginning of
 * the current line. This is equivalent to calling doHome(true).
 */
void Editor::doMoveHome()
{
	doHome(true);
}

/*!
 * This is a simple utility function which moves the cursor to the beginning of
 * the current line, without moving the anchor - i.e., selecting. This is
 * equivalent to calling doHome(false).
 */
void Editor::doSelectHome()
{
	doHome(false);
}

/*!
 * This function executes a given find query, using the given predefined
 * direction. Note that we do not inspect the direction properties of the given
 * query - it is assumed that our caller has resolved these into a single
 * direction, based upon whether this is a "find next" or a "find previous"
 * operation, and whether or not the query is reversed.
 *
 * \param f True means we should go forward, false means backward.
 * \param q The query to execute.
 * \return The results of executing this find query.
 */
Editor::FindResult Editor::doFind(bool f, const FindQuery *q)
{
	// Prepare our find flags.

	QTextDocument::FindFlags flags = q->getFindFlags();

	if(!f)
		flags |= QTextDocument::FindBackward;

	// Prepare our cursors.

	QTextCursor current = textCursor();

	QTextCursor restart = textCursor();

	if(f)
		restart.movePosition(QTextCursor::Start,
		                     QTextCursor::MoveAnchor);
	else
		restart.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);

	// Execute the find query, based on type.

	if(q->isRegularExpression())
	{
		QRegExp regex(q->getExpression(), Qt::CaseSensitive,
		              QRegExp::RegExp2);

		if(!regex.isValid())
			return BadRegularExpression;

		QTextCursor found = document()->find(regex, current, flags);

		if(found.isNull())
		{
			if(q->isWrapping())
			{
				found = document()->find(regex, restart, flags);

				if(!found.isNull())
					Q_EMIT searchWrapped();
			}
		}

		if(!found.isNull())
		{
			setTextCursor(found);
			return Found;
		}
	}
	else
	{
		QString expression = q->getExpression();

		QTextCursor found =
		        document()->find(expression, current, flags);

		if(found.isNull())
		{
			if(q->isWrapping())
			{
				found = document()->find(expression, restart,
				                         flags);

				if(!found.isNull())
					Q_EMIT searchWrapped();
			}
		}

		if(!found.isNull())
		{
			setTextCursor(found);
			return Found;
		}
	}

	return NoMatches;
}

/*!
 * This is a utility function which will replace ever match of the given
 * replace query from the given start position until the given end position.
 *
 * If the start position is omitted, then we will simply use the starting
 * position from the current cursor (the smaller of its anchor and position).
 *
 * If the end position is omitted, then we will simply continue until the end
 * of the document.
 *
 * \param q The replace query to execute.
 * \param start The cursor position to start searching from.
 * \param end The cursor position to stop searching at.
 * \return The result of this replacement's find operation.
 */
Editor::FindResult Editor::doBatchReplace(const ReplaceQuery *q, int start,
                                          int end)
{
	// If we weren't given a start position, use the current cursor.

	if(start < 0)
	{
		QTextCursor curs = textCursor();
		start = qMin(curs.anchor(), curs.position());
	}

	// Create a query with "replace in selection"-compatible properties.

	ReplaceQuery query;

	query.setExpression(q->getExpression());
	query.setReplaceValue(q->getReplaceValue());
	query.setWrapping(false);
	query.setWholeWords(q->isWholeWords());
	query.setCaseSensitive(q->isCaseSensitive());
	query.setReversed(false);
	query.setRegularExpression(q->isRegularExpression());

	q = nullptr;

	// Move our cursor to our start position.

	QTextCursor curs = textCursor();
	curs.setPosition(start, QTextCursor::MoveAnchor);
	setTextCursor(curs);

	// Replace each match we find after our start position.

	curs = textCursor();
	Editor::FindResult r = findNext(&query);
	bool found = false;

	curs.beginEditBlock();

	while(r == Editor::Found)
	{
		// Make sure this match is good to go.

		int finda = textCursor().anchor(),
		    findp = textCursor().position();

		if(end >= 0)
			if(qMax(finda, findp) > end)
				break;

		curs.setPosition(finda, QTextCursor::MoveAnchor);
		curs.setPosition(findp, QTextCursor::KeepAnchor);

		found = true;

		// Replace this match.

		int anchor = qMin(curs.anchor(), curs.position());

		curs.insertText(query.getReplaceValue());

		curs.setPosition(anchor, QTextCursor::MoveAnchor);
		curs.movePosition(QTextCursor::NextCharacter,
		                  QTextCursor::KeepAnchor,
		                  query.getReplaceValue().length());

		// Find the next match!

		r = findNext(&query);
	}

	curs.endEditBlock();

	setTextCursor(curs);

	// Return an appropriate find result.

	if(found)
		return Editor::Found;
	else
		return r;
}

void Editor::undo()
{
	DecoratedTextEdit::undo();
}

void Editor::redo()
{
	DecoratedTextEdit::redo();
}

/*!
 * This slot duplicates the current line. The new line is inserted below the
 * current line, without altering our cursor position. Note that this entire
 * operation is a single "edit block," for undo/redo operations.
 */
void Editor::duplicateLine()
{
	// Save our cursor's initial state.

	QTextCursor curs = textCursor();

	int cpos = curs.position();
	int apos = curs.anchor();

	curs.beginEditBlock();

	// Get into position, and copy this block (line).

	curs.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
	QString line = curs.block().text();

	// Duplicate the line.

	curs.insertText("\n");
	curs.insertText(line);

	// End our edit block, and reset our cursor position.

	curs.endEditBlock();

	curs.setPosition(apos, QTextCursor::MoveAnchor);
	curs.setPosition(cpos, QTextCursor::KeepAnchor);

	// Done.

	setTextCursor(curs);
}

/*!
 * This slot clears any selection our editor may have. This is done by simply
 * discarding the "anchor" portion of the current cursor, leaving its actual
 * "position" where it was.
 */
void Editor::deselect()
{
	QTextCursor curs = textCursor();
	curs.setPosition(curs.position(), QTextCursor::MoveAnchor);
	setTextCursor(curs);
}

/*!
 * This slot increases the indent of all the lines in the current selection.
 * Partially selected lines are included. This function will insert a single
 * tab character at the beginning of each included line.
 *
 * The resulting selection will have an anchor at the beginning of the first
 * line, and a cursor position at the end of the last line.
 *
 * Also note that this operation is done in a single "edit block," for
 * undo/redo actions.
 */
void Editor::increaseSelectionIndent()
{
	QTextCursor curs = textCursor();

	if(!curs.hasSelection())
		return;

	// Get the first and count of lines to indent.

	int spos = curs.anchor();
	int epos = curs.position();

	if(spos > epos)
		std::swap(spos, epos);

	curs.setPosition(spos, QTextCursor::MoveAnchor);
	int sblock = curs.block().blockNumber();

	curs.setPosition(epos, QTextCursor::MoveAnchor);
	int eblock = curs.block().blockNumber();

	int blockdiff = eblock - sblock;

	// Do the indent.

	curs.setPosition(spos, QTextCursor::MoveAnchor);

	curs.beginEditBlock();

	for(int i = 0; i <= blockdiff; ++i)
	{
		curs.movePosition(QTextCursor::StartOfBlock,
		                  QTextCursor::MoveAnchor);

		curs.insertText(getIndentString());

		curs.movePosition(QTextCursor::NextBlock,
		                  QTextCursor::MoveAnchor);
	}

	curs.endEditBlock();

	// Set our cursor's selection to span all of the involved lines.

	curs.setPosition(spos, QTextCursor::MoveAnchor);
	curs.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);

	while(curs.block().blockNumber() < eblock)
	{
		curs.movePosition(QTextCursor::NextBlock,
		                  QTextCursor::KeepAnchor);
	}

	curs.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

	setTextCursor(curs);
}

/*!
 * This slot decreases the indent of all the lines in the current selection.
 * Partially selected lines are included.
 *
 * This slot considers tabs and sets of spaces the same length as the tab width
 * to be a single "indentation." This function will remove one indentation from
 * each line in the selection. If no indentations are present, then we will
 * instead look for any arbitrary leading spaces to remove.
 *
 * The resulting selection will have an anchor at the beginning of the first
 * line, and a cursor position at the end of the last line.
 *
 * Also note that this operation is done in a single "edit block," for
 * undo/redo actions.
 */
void Editor::decreaseSelectionIndent()
{
	QTextCursor curs = textCursor();

	if(!curs.hasSelection())
		return;

	// Get the first and count of lines to de-indent.

	int spos = curs.anchor();
	int epos = curs.position();

	if(spos > epos)
		std::swap(spos, epos);

	curs.setPosition(spos, QTextCursor::MoveAnchor);
	int sblock = curs.block().blockNumber();
	int sblockpos = curs.block().position();

	curs.setPosition(epos, QTextCursor::MoveAnchor);
	int eblock = curs.block().blockNumber();

	int blockdiff = eblock - sblock;

	// Do the de-indent.

	curs.setPosition(spos, QTextCursor::MoveAnchor);

	curs.beginEditBlock();

	bool foundIndent = false;

	for(int i = 0; i <= blockdiff; ++i)
	{
		curs.movePosition(QTextCursor::StartOfBlock,
		                  QTextCursor::MoveAnchor);

		QString text = curs.block().text();

		if(text.startsWith("\t"))
		{
			curs.deleteChar();
			foundIndent = true;
		}
		else if(text.startsWith(QString(getIndentationWidth(), ' ')))
		{
			for(int j = 0; j < getIndentationWidth(); ++j)
			{
				curs.deleteChar();
				foundIndent = true;
			}
		}

		curs.movePosition(QTextCursor::NextBlock,
		                  QTextCursor::MoveAnchor);
	}

	if(!foundIndent)
	{
		// If no lines were indented, try to remove any whitespace.

		curs.setPosition(spos, QTextCursor::MoveAnchor);

		for(int i = 0; i <= (eblock - sblock); ++i)
		{
			curs.movePosition(QTextCursor::StartOfBlock,
			                  QTextCursor::MoveAnchor);

			for(int j = 0; j < getIndentationWidth(); ++j)
			{
				QChar c = curs.block().text().at(0);

				if(!c.isSpace())
					break;

				if((j > 0) && (c == '\t'))
					break;

				curs.deleteChar();
			}

			curs.movePosition(QTextCursor::NextBlock,
			                  QTextCursor::MoveAnchor);
		}
	}

	curs.endEditBlock();

	// Set our cursor's selection to span all of the involved lines.

	curs.setPosition(sblockpos, QTextCursor::MoveAnchor);

	while(curs.block().blockNumber() < eblock)
	{
		curs.movePosition(QTextCursor::NextBlock,
		                  QTextCursor::KeepAnchor);
	}

	curs.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

	setTextCursor(curs);
}

/*!
 * This function moves the cursor to the beginning of the current line,
 * following some special rules:
 *
 *     - By default, the cursor is simply moved to the beginning of the line's
 *       indentation - that is, just before the first non-whitespace character
 *       on the line.
 *
 *     - If there is only whitespace before the position the cursor already
 *       occupies, then the cursor is moved to the very beginning of the line
 *       instead - that is, just before the first character on the line.
 *
 * Additionally, moving the cursor's anchor is optional. This means that this
 * function will either move the cursor, or select using the cursor, following
 * these rules.
 *
 * \param moveAnchor Whether or not the cursor's anchor should be moved.
 */
void Editor::doHome(bool moveAnchor)
{
	// Get the characters between the current cursor and the start-of-line.

	QString line, trimmed;

	{
		QTextCursor l = textCursor();

		if(l.hasSelection())
			l.setPosition(l.selectionStart(),
			              QTextCursor::MoveAnchor);

		l.movePosition(QTextCursor::StartOfLine,
		               QTextCursor::KeepAnchor);

		line = l.selectedText();
	}

	// Get the leading whitespace from the line.

	{
		int idx = 0;

		for(int i = 0; i < line.length(); ++i)
		{
			if(!line.at(i).isSpace())
			{
				idx = i;
				break;
			}
		}

		trimmed = line.left(idx);
	}

	// Move the cursor to its appropriate location.

	if(trimmed == line)
	{
		// There is only whitespace in front of us - move to the
		// start-of-line.

		QTextCursor curs = textCursor();

		if(moveAnchor)
		{
			curs.movePosition(QTextCursor::StartOfLine,
			                  QTextCursor::MoveAnchor);
		}
		else
		{
			curs.movePosition(QTextCursor::StartOfLine,
			                  QTextCursor::KeepAnchor);
		}

		setTextCursor(curs);
	}
	else
	{
		// There is non-whitespace in front - move to the end of the
		// whitespace.

		QTextCursor curs = textCursor();

		int eos = qMax(curs.selectionStart(), curs.selectionEnd());

		curs.movePosition(QTextCursor::StartOfLine,
		                  QTextCursor::MoveAnchor);

		int sol = curs.position();

		if(moveAnchor)
		{
			curs.setPosition(sol + trimmed.length(),
			                 QTextCursor::MoveAnchor);
		}
		else
		{
			curs.setPosition(eos, QTextCursor::MoveAnchor);

			curs.setPosition(sol + trimmed.length(),
			                 QTextCursor::KeepAnchor);
		}

		setTextCursor(curs);
	}
}

/*!
 * This slot moves to the next match based upon the given find query and the
 * current cursor location.
 *
 * \param q The query to execute.
 * \return The result of the find query's execution.
 */
Editor::FindResult Editor::findNext(const FindQuery *q)
{ /* SLOT */

	bool forward = true;

	if(q->isReversed())
		forward = false;

	return doFind(forward, q);
}

/*!
 * This slot moves to the previous find match based upon the given find query
 * and the current cursor location.
 *
 * \param q The query to execute.
 * \return The result of the find query's execution.
 */
Editor::FindResult Editor::findPrevious(const FindQuery *q)
{ /* SLOT */

	bool forward = false;

	if(q->isReversed())
		forward = true;

	return doFind(forward, q);
}

/*!
 * This slot performs a single replace operation. We will replace the very next
 * match of the given query with the query's replace value. Note that we will
 * begin searching at the start of the current cursor's selection (or its
 * position, if it has no selection).
 *
 * \param q The replace query to execute.
 * \return The result of this replacement's find operation.
 */
Editor::FindResult Editor::replace(const ReplaceQuery *q)
{ /* SLOT */

	// Reset our cursor's position.

	QTextCursor curs = textCursor();

	curs.setPosition(qMin(curs.anchor(), curs.position()),
	                 QTextCursor::MoveAnchor);

	setTextCursor(curs);

	// Try to find the next match.

	Editor::FindResult r = findNext(q);

	// If we got a match, replace it with our replace value.

	if(r == Editor::Found)
	{
		curs = textCursor();

		if(curs.hasSelection())
		{
			curs.beginEditBlock();

			int anchor = qMin(curs.anchor(), curs.position());
			int length = qMax(q->getReplaceValue().length(), 0);

			curs.insertText(q->getReplaceValue());

			curs.setPosition(anchor, QTextCursor::MoveAnchor);
			curs.movePosition(QTextCursor::NextCharacter,
			                  QTextCursor::KeepAnchor, length);

			curs.endEditBlock();

			setTextCursor(curs);
		}
	}

	// Done!

	return r;
}

/*!
 * This slot performs a "replace in selection" operation. We will replace every
 * single match of the given query in our editor's current selection. If we do
 * not have any selection, then we will return NoMatches instead.
 *
 * Note that we will return Found if at least one match is replaced, or
 * NoMatches otherwise.
 *
 * \param q The replace query to execute.
 * \return The result of this replacement's find operation.
 */
Editor::FindResult Editor::replaceSelection(const ReplaceQuery *q)
{ /* SLOT */

	QTextCursor curs = textCursor();

	if(!curs.hasSelection())
		return Editor::NoMatches;

	int start = qMin(curs.anchor(), curs.position());
	int end = qMax(curs.anchor(), curs.position());

	return doBatchReplace(q, start, end);
}

/*!
 * This slot performs a "replace all" operation. We will replace every single
 * match of the given query in our editor's document.
 *
 * Note that we will return Found if at least one match is replaced, or
 * NoMatches otherwise.
 *
 * \param q The replace query to execute.
 * \return The result of this replacement's find operation.
 */
Editor::FindResult Editor::replaceAll(const ReplaceQuery *q)
{ /* SLOT */

	return doBatchReplace(q, 0);
}

/*!
 * This function will move our cursor to the very beginning of the given line
 * number. Note that the resulting cursor will be at the very beginning of the
 * block (i.e., line), and it will have no selection.
 *
 * Line numbers less than 1 will result in the cursor being positioned at the
 * very beginning of the document, and line numbers larger than our document's
 * last line will result in the cursor being positioned at the beginning of the
 * very last line in the document.
 *
 * \param l The destination line number.
 */
void Editor::goToLine(int l)
{ /* SLOT */

	l = qMax(l, 1);

	QTextCursor curs = textCursor();

	curs.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
	curs.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor,
	                  qMax(l - 1, 0));

	setTextCursor(curs);
}
}
