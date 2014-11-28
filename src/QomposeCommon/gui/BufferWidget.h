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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_BUFFER_WIDGET_H
#define INCLUDE_QOMPOSECOMMON_GUI_BUFFER_WIDGET_H

#include <QWidget>
#include <QSet>
#include <QStack>

#include "QomposeCommon/Types.h"
#include "QomposeCommon/editor/QomposeEditor.h"

class QGridLayout;
class QTabWidget;
class QPrinter;

class QomposeSettings;
class QomposeFindQuery;
class QomposeReplaceQuery;

namespace qompose
{

class Buffer;

/*!
 * \brief This class provides high-level management of a set of buffers.
 */
class BufferWidget : public QWidget
{
	Q_OBJECT

public:
	BufferWidget(QomposeSettings *, QWidget * = nullptr);
	virtual ~BufferWidget();

	int count() const;
	Buffer *bufferAt(int) const;
	Buffer *currentBuffer() const;
	bool hasCurrentBuffer() const;
	void setCurrentBuffer(int);

	bool prepareCloseParent();

	int findBufferWithPath(const QString &);

private:
	QomposeSettings *settings;

	QGridLayout *layout;
	QTabWidget *tabWidget;

	QSet<Buffer *> tabs;
	QStack<ClosedBufferDescriptor> closedTabs;

	BufferWidget(const BufferWidget &);
	BufferWidget &operator=(const BufferWidget &);

	Buffer *newBuffer();
	void removeCurrentBuffer();
	void moveBuffer(int, int);

	QString getDefaultDirectory() const;

public Q_SLOTS:
	void doNew();
	void doOpen();
	void doOpenPath(const QString &);
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
	QomposeEditor::FindResult doFindNext(
		const QomposeFindQuery *);
	QomposeEditor::FindResult doFindPrevious(
		const QomposeFindQuery *);
	QomposeEditor::FindResult doReplace(
		const QomposeReplaceQuery *);
	QomposeEditor::FindResult doReplaceSelection(
		const QomposeReplaceQuery *);
	QomposeEditor::FindResult doReplaceAll(
		const QomposeReplaceQuery *);
	void doGoTo(int);
	void doPreviousBuffer();
	void doNextBuffer();
	void doMoveBufferLeft();
	void doMoveBufferRight();

	void doPrint(QPrinter *);

private Q_SLOTS:
	void doTabChanged(int);
	void doTabCloseRequested(int);
	void doTabClosing(int);

	void doTabTitleChanged(const QString &);
	void doTabPathChanged(const QString &);

	void doCursorPositionChanged();

	Buffer *doOpenDescriptor(const FileDescriptor &);
	void doReopenBuffer(const ClosedBufferDescriptor &);

Q_SIGNALS:
	void pathChanged(const QString &);
	void pathOpened(const QString &);
	void cursorPositionChanged(int, int);
	void searchWrapped();
};

}

#endif
