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
#include <functional>
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

#include "QomposeCommon/editor/algorithm/General.h"
#include "QomposeCommon/editor/algorithm/Indentation.h"
#include "QomposeCommon/util/FindQuery.h"
#include "QomposeCommon/util/Hotkey.h"
#include "QomposeCommon/util/ReplaceQuery.h"

namespace qompose
{
Editor::Editor(QWidget *p) : DecoratedTextEdit(p), hotkeys()
{
	initializeHotkeys();
}

void Editor::keyPressEvent(QKeyEvent *e)
{
	const std::function<void()> *handler = hotkeys.getHotkeyHandler(e);

	if(handler != nullptr)
		(*handler)();
	else
		DecoratedTextEdit::keyPressEvent(e);
}

void Editor::initializeHotkeys()
{
	// Backspace

	hotkeys.addHotkey(Hotkey(Qt::Key_Backspace, nullptr,
	                         ~Qt::KeyboardModifiers(nullptr)),
	                  std::bind(&Editor::doBackspace, this));

	// Delete

	hotkeys.addHotkey(Hotkey(Qt::Key_Delete, nullptr,
	                         ~Qt::KeyboardModifiers(nullptr)),
	                  std::bind(&Editor::doDelete, this));

	// Enter

	hotkeys.addHotkey(Hotkey(Qt::Key_Return, nullptr,
	                         ~Qt::KeyboardModifiers(nullptr)),
	                  std::bind(&Editor::doNewline, this));

	hotkeys.addHotkey(
	        Hotkey(Qt::Key_Enter, nullptr, ~Qt::KeyboardModifiers(nullptr)),
	        std::bind(&Editor::doNewline, this));

	// Tab

	hotkeys.addHotkey(Hotkey(Qt::Key_Tab), std::bind(&Editor::doTab, this));

	// Shift + Tab

	hotkeys.addHotkey(Hotkey(Qt::Key_Tab, Qt::ShiftModifier),
	                  std::bind(&Editor::decreaseSelectionIndent, this));

	hotkeys.addHotkey(Hotkey(Qt::Key_Backtab, Qt::ShiftModifier),
	                  std::bind(&Editor::decreaseSelectionIndent, this));

	// Home

	hotkeys.addHotkey(Hotkey(Qt::Key_Home),
	                  std::bind(&Editor::doMoveHome, this));

	// Shift + Home

	hotkeys.addHotkey(Hotkey(Qt::Key_Home, Qt::ShiftModifier),
	                  std::bind(&Editor::doSelectHome, this));

	// Ctrl+D

	hotkeys.addHotkey(Hotkey(Qt::Key_D, Qt::ControlModifier),
	                  std::bind(&Editor::duplicateLine, this));

	// Ctrl+(Zero)

	hotkeys.addHotkey(Hotkey(Qt::Key_0, Qt::ControlModifier),
	                  std::bind(&Editor::resetFontZoom, this));

	// Ctrl+Shift+Left

	hotkeys.addHotkey(
	        Hotkey(Qt::Key_Left, Qt::ControlModifier | Qt::ShiftModifier),
	        std::bind(&Editor::doNoop, this));

	// Ctrl+Shift+Right

	hotkeys.addHotkey(
	        Hotkey(Qt::Key_Right, Qt::ControlModifier | Qt::ShiftModifier),
	        std::bind(&Editor::doNoop, this));

	// Ctrl+Insert

	hotkeys.addHotkey(Hotkey(Qt::Key_Insert, Qt::ControlModifier),
	                  std::bind(&Editor::doNoop, this));

	// Ctrl+K

	hotkeys.addHotkey(Hotkey(Qt::Key_K, Qt::ControlModifier),
	                  std::bind(&Editor::doNoop, this));

	// Shift+Insert

	hotkeys.addHotkey(Hotkey(Qt::Key_Insert, Qt::ShiftModifier),
	                  std::bind(&Editor::doNoop, this));

	// Shift+Delete

	hotkeys.addHotkey(Hotkey(Qt::Key_Delete, Qt::ShiftModifier),
	                  std::bind(&Editor::doNoop, this));
}

void Editor::doNoop()
{
}

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

		int indentWidth = static_cast<int>(getIndentationWidth());
		if((selectedText.length() % indentWidth == 0) &&
		   (selectedText.length() >= indentWidth))
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

void Editor::doDelete()
{
	QTextCursor curs = textCursor();
	curs.deleteChar();
	setTextCursor(curs);
}

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

void Editor::doTab()
{
	editor::algorithm::applyAlgorithm(*this, editor::algorithm::tab,
	                                  getIndentationMode(),
	                                  getIndentationWidth());
}

void Editor::doMoveHome()
{
	doHome(true);
}

void Editor::doSelectHome()
{
	doHome(false);
}

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

void Editor::deselect()
{
	QTextCursor curs = textCursor();
	curs.setPosition(curs.position(), QTextCursor::MoveAnchor);
	setTextCursor(curs);
}

void Editor::increaseSelectionIndent()
{
	editor::algorithm::applyAlgorithm(
	        *this, editor::algorithm::increaseSelectionIndent,
	        getIndentationMode(), getIndentationWidth());
}

void Editor::decreaseSelectionIndent()
{
	editor::algorithm::applyAlgorithm(
	        *this, editor::algorithm::decreaseSelectionIndent,
	        getIndentationMode(), getIndentationWidth());
}

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

Editor::FindResult Editor::findNext(const FindQuery *q)
{
	bool forward = true;

	if(q->isReversed())
		forward = false;

	return doFind(forward, q);
}

Editor::FindResult Editor::findPrevious(const FindQuery *q)
{
	bool forward = false;

	if(q->isReversed())
		forward = true;

	return doFind(forward, q);
}

Editor::FindResult Editor::replace(const ReplaceQuery *q)
{
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

Editor::FindResult Editor::replaceSelection(const ReplaceQuery *q)
{
	QTextCursor curs = textCursor();

	if(!curs.hasSelection())
		return Editor::NoMatches;

	int start = qMin(curs.anchor(), curs.position());
	int end = qMax(curs.anchor(), curs.position());

	return doBatchReplace(q, start, end);
}

Editor::FindResult Editor::replaceAll(const ReplaceQuery *q)
{
	return doBatchReplace(q, 0);
}

void Editor::goToLine(int l)
{
	l = qMax(l, 1);

	QTextCursor curs = textCursor();

	curs.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
	curs.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor,
	                  qMax(l - 1, 0));

	setTextCursor(curs);
}
}
