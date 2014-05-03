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

#ifndef INCLUDE_QOMPOSE_BUFFER_WIDGET_H
#define INCLUDE_QOMPOSE_BUFFER_WIDGET_H

#include <QWidget>
#include <QSet>
#include <QStack>

#include "editor/QomposeEditor.h"

class QGridLayout;
class QTabWidget;
class QPrinter;

class QomposeBuffer;
class QomposeSettings;
class QomposeFindQuery;
class QomposeReplaceQuery;
class QomposeFileDescriptor;

/*!
 * \brief This class provides high-level management of a set of buffers.
 */
class QomposeBufferWidget : public QWidget
{
	Q_OBJECT

	public:
		QomposeBufferWidget(QomposeSettings *s, QWidget *p = 0);
		virtual ~QomposeBufferWidget();

		int count() const;
		QomposeBuffer *bufferAt(int i) const;
		QomposeBuffer *currentBuffer() const;
		bool hasCurrentBuffer() const;
		void setCurrentBuffer(int i);

		bool prepareCloseParent();

		int findBufferWithPath(const QString &p);

	private:
		QomposeSettings *settings;

		QGridLayout *layout;
		QTabWidget *tabWidget;

		QSet<QomposeBuffer *> tabs;
		QStack<QomposeFileDescriptor> closedTabs;

		QomposeBuffer *newBuffer();
		void removeCurrentBuffer();
		void moveBuffer(int f, int t);

		QString getDefaultDirectory() const;

	public Q_SLOTS:
		void doNew();
		void doOpen();
		void doOpenPath(const QString &p);
		void doReopen();
		void doRevert();
		void doRevertAll();
		void doSave();
		void doSaveAs();
		void doClose();
		void doUndo();
		void doRedo();
		void doCut();
		void doCopy();
		void doPaste();
		void doDuplicateLine();
		void doSelectAll();
		void doDeselect();
		void doIncreaseIndent();
		void doDecreaseIndent();
		QomposeEditor::FindResult doFindNext(const QomposeFindQuery *q);
		QomposeEditor::FindResult doFindPrevious(const QomposeFindQuery *q);
		QomposeEditor::FindResult doReplace(const QomposeReplaceQuery *q);
		QomposeEditor::FindResult doReplaceSelection(const QomposeReplaceQuery *q);
		QomposeEditor::FindResult doReplaceAll(const QomposeReplaceQuery *q);
		void doGoTo(int l);
		void doPreviousBuffer();
		void doNextBuffer();
		void doMoveBufferLeft();
		void doMoveBufferRight();

		void doPrint(QPrinter *p);

	private Q_SLOTS:
		void doTabChanged(int i);
		void doTabCloseRequested(int i);
		void doTabClosing(int i);

		void doTabTitleChanged(const QString &t);
		void doTabPathChanged(const QString &p);

		void doCursorPositionChanged();

		void doOpenDescriptor(const QomposeFileDescriptor &d);

	Q_SIGNALS:
		void pathChanged(const QString &);
		void pathOpened(const QString &);
		void cursorPositionChanged(int, int);
		void searchWrapped();
};

#endif
