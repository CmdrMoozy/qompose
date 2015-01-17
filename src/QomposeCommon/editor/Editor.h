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
	 * \brief This enum denotes the possible results of a find operation.
	 */
	enum FindResult
	{
		NoDocument,
		BadRegularExpression,
		NoMatches,
		Found
	};

	Editor(QWidget *p = nullptr);
	virtual ~Editor();

protected:
	virtual void keyPressEvent(QKeyEvent *e);

private:
	typedef void (Editor::*HotkeyFunction)();
	HotkeyMap<HotkeyFunction> hotkeys;

	void initializeHotkeys();

	void doNoop();

	void doBackspace();
	void doDelete();
	void doNewline();
	void doTab();
	void doMoveHome();
	void doSelectHome();

	FindResult doFind(bool forward, const FindQuery *q);
	FindResult doBatchReplace(const ReplaceQuery *q, int start = -1,
	                          int end = -1);

public Q_SLOTS:
	virtual void undo();
	virtual void redo();
	void duplicateLine();
	void deselect();
	void increaseSelectionIndent();
	void decreaseSelectionIndent();
	void doHome(bool moveAnchor);
	FindResult findNext(const FindQuery *q);
	FindResult findPrevious(const FindQuery *q);
	FindResult replace(const ReplaceQuery *q);
	FindResult replaceSelection(const ReplaceQuery *q);
	FindResult replaceAll(const ReplaceQuery *q);
	void goToLine(int l);

Q_SIGNALS:
	void searchWrapped();
};
}

#endif