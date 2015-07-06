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
#include <experimental/optional>

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
#include "QomposeCommon/editor/search/applyAlgorithm.h"
#include "QomposeCommon/editor/search/Find.h"
#include "QomposeCommon/editor/search/Replace.h"
#include "QomposeCommon/hotkey/Hotkey.h"

namespace qompose
{
Editor::Editor(QWidget *p) : hotkey::HotkeyedWidget<DecoratedTextEdit>(p)
{
	initializeHotkeys();
}

void Editor::initializeHotkeys()
{
	// Backspace

	addHotkey(Hotkey(Qt::Key_Backspace, nullptr,
	                 ~Qt::KeyboardModifiers(nullptr)),
	          std::bind(&Editor::doBackspace, this));

	// Delete

	addHotkey(Hotkey(Qt::Key_Delete, nullptr,
	                 ~Qt::KeyboardModifiers(nullptr)),
	          std::bind(&Editor::doDelete, this));

	// Enter

	addHotkey(Hotkey(Qt::Key_Return, nullptr,
	                 ~Qt::KeyboardModifiers(nullptr)),
	          std::bind(&Editor::doNewline, this));

	addHotkey(
	        Hotkey(Qt::Key_Enter, nullptr, ~Qt::KeyboardModifiers(nullptr)),
	        std::bind(&Editor::doNewline, this));

	// Tab

	addHotkey(Hotkey(Qt::Key_Tab), std::bind(&Editor::doTab, this));

	// Shift + Tab

	addHotkey(Hotkey(Qt::Key_Tab, Qt::ShiftModifier),
	          std::bind(&Editor::decreaseSelectionIndent, this));

	addHotkey(Hotkey(Qt::Key_Backtab, Qt::ShiftModifier),
	          std::bind(&Editor::decreaseSelectionIndent, this));

	// Home

	addHotkey(Hotkey(Qt::Key_Home), std::bind(&Editor::doMoveHome, this));

	// Shift + Home

	addHotkey(Hotkey(Qt::Key_Home, Qt::ShiftModifier),
	          std::bind(&Editor::doSelectHome, this));

	// Ctrl+D

	addHotkey(Hotkey(Qt::Key_D, Qt::ControlModifier),
	          std::bind(&Editor::duplicateLine, this));

	// Ctrl+(Zero)

	addHotkey(Hotkey(Qt::Key_0, Qt::ControlModifier),
	          std::bind(&Editor::resetFontZoom, this));

	// Ctrl+Shift+Left

	addHotkey(Hotkey(Qt::Key_Left, Qt::ControlModifier | Qt::ShiftModifier),
	          std::bind(&Editor::doNoop, this));

	// Ctrl+Shift+Right

	addHotkey(
	        Hotkey(Qt::Key_Right, Qt::ControlModifier | Qt::ShiftModifier),
	        std::bind(&Editor::doNoop, this));

	// Ctrl+Insert

	addHotkey(Hotkey(Qt::Key_Insert, Qt::ControlModifier),
	          std::bind(&Editor::doNoop, this));

	// Ctrl+K

	addHotkey(Hotkey(Qt::Key_K, Qt::ControlModifier),
	          std::bind(&Editor::doNoop, this));

	// Shift+Insert

	addHotkey(Hotkey(Qt::Key_Insert, Qt::ShiftModifier),
	          std::bind(&Editor::doNoop, this));

	// Shift+Delete

	addHotkey(Hotkey(Qt::Key_Delete, Qt::ShiftModifier),
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
	return doFind(true, q);
}

editor::search::FindResult
Editor::findPrevious(editor::search::FindQuery const &q)
{
	return doFind(false, q);
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
	QTextCursor c = textCursor();
	return editor::search::applyAlgorithm(
	        *this, editor::search::batchReplace, q,
	        std::min(c.anchor(), c.position()),
	        std::max(c.anchor(), c.position()));
}

editor::search::FindResult
Editor::replaceAll(editor::search::ReplaceQuery const &q)
{
	return editor::search::applyAlgorithm(*this,
	                                      editor::search::batchReplace, q,
	                                      0, std::experimental::nullopt);
}

void Editor::goToLine(int l)
{
	editor::algorithm::applyAlgorithm(*this, editor::algorithm::goToBlock,
	                                  l - 1);
}
}
