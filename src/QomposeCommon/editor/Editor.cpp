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
#include "QomposeCommon/editor/algorithm/Movement.h"
#include "QomposeCommon/util/Hotkey.h"

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
	editor::algorithm::applyAlgorithm(*this, editor::algorithm::backspace,
	                                  getIndentationMode(),
	                                  getIndentationWidth());
}

void Editor::doDelete()
{
	editor::algorithm::applyAlgorithm(*this,
	                                  editor::algorithm::deleteCharacter);
}

void Editor::doNewline()
{
	editor::algorithm::applyAlgorithm(*this, editor::algorithm::newline);
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

editor::search::FindResult Editor::doFind(bool f,
                                          editor::search::FindQuery const &q)
{
	return editor::search::applyAlgorithm(*this, editor::search::find, f,
	                                      q);
}

editor::search::FindResult
Editor::doBatchReplace(editor::search::ReplaceQuery const &q, int start,
                       int end)
{
	// If we weren't given a start position, use the current cursor.

	if(start < 0)
	{
		QTextCursor curs = textCursor();
		start = qMin(curs.anchor(), curs.position());
	}

	// Create a query with "replace in selection"-compatible properties.

	editor::search::ReplaceQuery query;
	query.expression = q.expression;
	query.replaceValue = q.replaceValue;
	query.wrap = false;
	query.wholeWords = q.wholeWords;
	query.caseSensitive = q.caseSensitive;
	query.reverse = false;
	query.isRegex = q.isRegex;

	// Move our cursor to our start position.

	QTextCursor curs = textCursor();
	curs.setPosition(start, QTextCursor::MoveAnchor);
	setTextCursor(curs);

	// Replace each match we find after our start position.

	curs = textCursor();
	editor::search::FindResult r = findNext(query);
	bool found = false;

	curs.beginEditBlock();

	while(r == editor::search::FindResult::Found)
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

		curs.insertText(query.replaceValue);

		curs.setPosition(anchor, QTextCursor::MoveAnchor);
		curs.movePosition(QTextCursor::NextCharacter,
		                  QTextCursor::KeepAnchor,
		                  query.replaceValue.length());

		// Find the next match!

		r = findNext(query);
	}

	curs.endEditBlock();

	setTextCursor(curs);

	// Return an appropriate find result.

	if(found)
		return editor::search::FindResult::Found;
	else
		return r;
}

void Editor::duplicateLine()
{
	editor::algorithm::applyAlgorithm(*this,
	                                  editor::algorithm::duplicateBlock);
}

void Editor::deselect()
{
	editor::algorithm::applyAlgorithm(*this, editor::algorithm::deselect);
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
	editor::algorithm::applyAlgorithm(*this, editor::algorithm::home,
	                                  moveAnchor);
}

editor::search::FindResult Editor::findNext(editor::search::FindQuery const &q)
{
	bool forward = true;

	if(q.reverse)
		forward = false;

	return doFind(forward, q);
}

editor::search::FindResult
Editor::findPrevious(editor::search::FindQuery const &q)
{
	bool forward = false;

	if(q.reverse)
		forward = true;

	return doFind(forward, q);
}

editor::search::FindResult
Editor::replace(editor::search::ReplaceQuery const &q)
{
	return editor::search::applyAlgorithm(*this, editor::search::replace,
	                                      q);
}

editor::search::FindResult
Editor::replaceSelection(editor::search::ReplaceQuery const &q)
{
	QTextCursor curs = textCursor();

	if(!curs.hasSelection())
		return editor::search::FindResult::NoMatches;

	int start = qMin(curs.anchor(), curs.position());
	int end = qMax(curs.anchor(), curs.position());

	return doBatchReplace(q, start, end);
}

editor::search::FindResult
Editor::replaceAll(editor::search::ReplaceQuery const &q)
{
	return doBatchReplace(q, 0);
}

void Editor::goToLine(int l)
{
	editor::algorithm::applyAlgorithm(*this, editor::algorithm::goToBlock,
	                                  l - 1);
}
}
