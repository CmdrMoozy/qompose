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

#ifndef INCLUDE_QOMPOSE_EDITOR_H
#define INCLUDE_QOMPOSE_EDITOR_H

#include "editor/QomposeDecoratedTextEdit.h"

#include <QSize>

class QFocusEvent;
class QResizeEvent;
class QKeyEvent;
class QWheelEvent;
class QMouseEvent;

class QomposeFindQuery;
class QomposeReplaceQuery;

/*!
 * \brief This class implements our main text editor widget.
 *
 * We depend on our decorated text edit for the various editor decorations we
 * support, while we add advanced editor functionality like improved line indentation
 * semantics, find and replace, and etc.
 */
class QomposeEditor : public QomposeDecoratedTextEdit
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
		
		QomposeEditor(QWidget *p = NULL);
		virtual ~QomposeEditor();
		
	protected:
		virtual void keyPressEvent(QKeyEvent *e);
		
	private:
		void doNewline(bool preserveIndent = false);
		void doMoveHome(bool moveAnchor = true);
		
		FindResult doFind(bool forward, const QomposeFindQuery *q);
		FindResult doBatchReplace(const QomposeReplaceQuery *q,
			int start = -1, int end = -1);
	
	public Q_SLOTS:
		void duplicateLine();
		void deselect();
		void increaseSelectionIndent();
		void decreaseSelectionIndent();
		FindResult findNext(const QomposeFindQuery *q);
		FindResult findPrevious(const QomposeFindQuery *q);
		FindResult replace(const QomposeReplaceQuery *q);
		FindResult replaceSelection(const QomposeReplaceQuery *q);
		FindResult replaceAll(const QomposeReplaceQuery *q);
		void goToLine(int l);	
};

#endif