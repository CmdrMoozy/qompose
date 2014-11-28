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

#include "BufferWidget.h"

#include <QGridLayout>
#include <QSet>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QTabWidget>
#include <QPrinter>
#include <QFileInfo>

#include "QomposeCommon/dialogs/FileDialog.h"
#include "QomposeCommon/editor/Buffer.h"
#include "QomposeCommon/util/QomposeFindQuery.h"
#include "QomposeCommon/util/QomposeReplaceQuery.h"
#include "QomposeCommon/util/Settings.h"

namespace qompose
{

/*!
 * This is our default constructor, which creates a new instance of our buffers
 * tab widget.
 *
 * \param s The settings instance to give to our buffers.
 * \param p The parent widget to use for this widget.
 */
BufferWidget::BufferWidget(Settings *s, QWidget *p)
	: QWidget(p, nullptr), settings(s), layout(nullptr),
		tabWidget(nullptr), tabs(), closedTabs()
{
	layout = new QGridLayout(this);

	tabWidget = new QTabWidget(this);
	tabWidget->setMovable(true);
	tabWidget->setTabsClosable(true);
	tabWidget->setUsesScrollButtons(true);

	layout->addWidget(tabWidget, 0, 0, 1, 1, nullptr);
	setLayout(layout);

	QObject::connect(tabWidget, SIGNAL(currentChanged(int)),
		this, SLOT(doTabChanged(int)));
	QObject::connect(tabWidget, SIGNAL(tabCloseRequested(int)),
		this, SLOT(doTabCloseRequested(int)));
}

/*!
 * This is our default destructor, which cleans up & destroys our widget.
 */
BufferWidget::~BufferWidget()
{
}

/*!
 * This function returns the number of buffers our widget currently contains.
 *
 * \return The number of buffers we have.
 */
int BufferWidget::count() const
{
	return tabWidget->count();
}

/*!
 * This function returns the widget at the given index in our list of widgets.
 * If the given index is invalid, then this function will return NULL instead.
 *
 * \param i The index of the desired buffer.
 * \return The buffer at the given index.
 */
Buffer *BufferWidget::bufferAt(int i) const
{
	return dynamic_cast<Buffer *>(tabWidget->widget(i));
}

/*!
 * This function returns our current buffer (i.e., the visible one), or NULL
 * if we don't have any current buffer.
 *
 * \return Our widget's current buffer.
 */
Buffer *BufferWidget::currentBuffer() const
{
	return dynamic_cast<Buffer *>(tabWidget->currentWidget());
}

/*!
 * This function returns whether or not our buffers widget has a valid current
 * buffer. It will return false, for example, if all of our buffers have been
 * closed.
 *
 * \return Whether or not this widget has a valid "current buffer."
 */
bool BufferWidget::hasCurrentBuffer() const
{
	return (currentBuffer() != NULL);
}

/*!
 * This function sets our widget's visible buffer to the buffer at the given
 * index. If the given index is out of bounds, then no action is taken.
 *
 * \param i The index of the desired buffer.
 */
void BufferWidget::setCurrentBuffer(int i)
{
	tabWidget->setCurrentIndex(i);
}

/*!
 * This function prepares our buffers for a close event on our parent widget -
 * for example, if we are a component of a window, and that window is about to
 * be closed. We do this by ensuring each one of our buffers is saved (or
 * doesn't need to be saved).
 *
 * We return whether or not we are totally prepared for the impending close,
 * but our caller is in no way required to listen to us unless they are nice.
 *
 * \return True if we are ready to be closed, or false otherwise.
 */
bool BufferWidget::prepareCloseParent()
{
	for(int i = 0; i < count(); ++i)
	{
		Buffer *buf = bufferAt(i);

		if(buf == NULL)
			continue;

		setCurrentBuffer(i);

		if(buf->isModified())
		{
			QMessageBox::StandardButton b = QMessageBox::question(
				this, tr("Qompose - Unsaved Changes"),
				tr("Save changes before closing?"),
				QMessageBox::Yes | QMessageBox::No |
				QMessageBox::Cancel, QMessageBox::Yes);

			switch(b)
			{
				case QMessageBox::Yes:
					doSave();

					if(buf->isModified())
						return false;

					break;

				case QMessageBox::No:
					continue;

				case QMessageBox::Cancel:
				default:
					return false;
			};
		}
	}

	return true;
}

/*!
 * This function attempts to find a buffer whose path matches the given value.
 * If no such buffer can be found, then we will return -1 instead of a valid
 * tab index.
 *
 * \param p The path to search for.
 * \return The index of the (first) buffer which has the given path.
 */
int BufferWidget::findBufferWithPath(const QString &p)
{
	QFileInfo file(p);

	for(int i = 0; i < count(); ++i)
	{
		Buffer *buf = bufferAt(i);
		QFileInfo bufFile(buf->getPath());

		if(file == bufFile)
			return i;
	}

	return -1;
}

/*!
 * This is a utility function which creates a new buffer widget, connects
 * its signals and slots, adds it to our tab widget, and makes it the
 * active tab.
 *
 * \return The newly-created buffer widget.
 */
Buffer *BufferWidget::newBuffer()
{
	Buffer *b = new Buffer(settings, tabWidget);

	QObject::connect(b, SIGNAL(titleChanged(const QString &)),
		this, SLOT(doTabTitleChanged(const QString &)));
	QObject::connect(b, SIGNAL(pathChanged(const QString &)),
		this, SLOT(doTabPathChanged(const QString &)));
	QObject::connect(b, SIGNAL(cursorPositionChanged()),
		this, SLOT(doCursorPositionChanged()));
	QObject::connect(b, SIGNAL(searchWrapped()),
		this, SIGNAL(searchWrapped()));

	int i = tabWidget->addTab(b, b->getTitle());
	tabWidget->setCurrentIndex(i);

	tabs.insert(b);

	return b;
}

/*!
 * This function removes the current buffer from our tab widget, and deletes it.
 */
void BufferWidget::removeCurrentBuffer()
{
	int i = tabWidget->currentIndex();
	Buffer *b = currentBuffer();

	tabWidget->removeTab(i);

	tabs.remove(b);
	delete b;
}

/*!
 * This function moves the tab at the given index "f" to the given index "t".
 * That is, its new index after being moved will be exactly "t." If the given
 * from index is out-of-bounds, then no action is taken.
 *
 * \param f The index to move a tab from.
 * \param t The index to move the tab to.
 */
void BufferWidget::moveBuffer(int f, int t)
{
	Buffer *b = dynamic_cast<Buffer *>(tabWidget->widget(f));

	if(b == NULL)
		return;

	tabWidget->removeTab(f);
	tabWidget->insertTab(t, b, b->getTitle());
}

/*!
 * This function tries to compute a default directory for, e.g., open or save
 * actions, based upon our current buffer list.
 *
 * We will use the first valid path of the current buffer's path, the path of
 * the nearest buffer to the left, the path of the nearest buffer to the right,
 * or the user's home directory.
 *
 * \return A default directory to use for open/save actions.
 */
QString BufferWidget::getDefaultDirectory() const
{
	QString defaultDirectory = QDir::homePath();
	bool found = false;

	Buffer *buf = currentBuffer();
	int ci = tabWidget->indexOf(buf);

	// See if the current buffer has a valid default path.

	if(buf != NULL)
	{
		if(!buf->getDirectory().isNull())
		{
			defaultDirectory = buf->getDirectory();
			found = true;
		}
	}

	// See if any buffers to the left of this buffer have valid paths.

	if(!found)
	{
		for(int i = (ci - 1); i >= 0; --i)
		{
			buf = bufferAt(i);

			if(buf != NULL)
			{
				if(!buf->getDirectory().isNull())
				{
					defaultDirectory = buf->getDirectory();
					found = true;
					break;
				}
			}
		}
	}

	// See if any buffers to the right of this buffer have valid paths.

	if(!found)
	{
		for(int i = (ci + 1); i < tabWidget->count(); ++i)
		{
			buf = bufferAt(i);

			if(buf != NULL)
			{
				if(!buf->getDirectory().isNull())
				{
					defaultDirectory = buf->getDirectory();
					break;
				}
			}
		}
	}

	// Done!

	return defaultDirectory;
}

/*!
 * This slot executes a new action by creating a new buffer widget.
 */
void BufferWidget::doNew()
{ /* SLOT */

	newBuffer();

}

/*!
 * This slot executes an open action by showing an open dialog and then
 * opening the selected file. If we only had one "Untitled" tab before, then
 * it will be replaced by the newly opened file.
 */
void BufferWidget::doOpen()
{ /* SLOT */

	// Open the one or more selected files.

	QList<FileDescriptor> files = FileDialog::getOpenFileNames(this,
		tr("Open Files"), getDefaultDirectory());

	for(int i = 0; i < files.size(); ++i)
	{
		doOpenDescriptor(files.at(i));
	}

}

/*!
 * This is a utility function which will open the file denoted by the given
 * path directly, without displaying an "open file" dialog.
 *
 * \param p The path to open.
 */
void BufferWidget::doOpenPath(const QString &p)
{ /* SLOT */

	FileDescriptor desc = FileDialog::getPathDescriptor(p);

	if(desc.fileName.isNull())
		return;

	doOpenDescriptor(desc);

}

/*!
 * This slot executes a "reopen" action by opening the most recently closed
 * tab in a new tab. If the list of recently closed tabs is empty, then no
 * action is taken.
 */
void BufferWidget::doReopen()
{ /* SLOT */

	if(!closedTabs.empty())
		doReopenBuffer(closedTabs.pop());

}

/*!
 * This slot executes a revert action by instructing our current buffer to
 * revert its changes.
 */
void BufferWidget::doRevert()
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->revert();

}

/*!
 * This slot executes a revert action on all of the buffers in our widget, by
 * instructing each buffer to revert its changes. Note that this has no effect
 * on buffers which have never been saved.
 */
void BufferWidget::doRevertAll()
{ /* SLOT */

	for(int i = 0; i < tabWidget->count(); ++i)
	{
		Buffer *buf = bufferAt(i);

		if(buf != NULL)
			buf->revert();
	}

}

/*!
 * This slot executes a save action by instructing our current buffer to
 * save its changes, potentially showing a "save as" dialog if it hasn't ever
 * been saved before.
 */
void BufferWidget::doSave()
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	if(buf->hasBeenSaved())
		buf->save();
	else
		doSaveAs();

}

/*!
 * This slot executes a "save as" action by showing a save file dialog, and
 * then by instructing our current buffer to save to the selected path.
 */
void BufferWidget::doSaveAs()
{ /* SLOT */

	// Make sure the current buffer is valid.

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	// Open our save as dialog and, on accept, save the buffer.

	QString path = getDefaultDirectory();

	if(buf->hasBeenSaved())
	{
		if(!path.endsWith(QDir::separator()))
			path.append(QDir::separator());

		path.append(buf->getFile());
	}

	QString p = QFileDialog::getSaveFileName(this, tr("Save File"), path,
		QString(), nullptr, nullptr);

	if(p.length() <= 0)
		return;

	buf->save(p);

}

/*!
 * This slot executes a close action by possibly prompting to save any unsaved
 * changes, and ultimately by removing the current buffer from our tab widget.
 */
void BufferWidget::doClose()
{ /* SLOT */

	bool remove = false;
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	if(buf->isModified())
	{
		QMessageBox::StandardButton b = QMessageBox::question(
			this, tr("Qompose - Unsaved Changes"),
			tr("Save changes to this buffer before closing?"),
			QMessageBox::Yes | QMessageBox::No |
			QMessageBox::Cancel, QMessageBox::Yes);

		switch(b)
		{
			case QMessageBox::Yes:
				doSave();

				if(!buf->isModified())
					remove = true;

				break;

			case QMessageBox::No:
				remove = true;
				break;

			case QMessageBox::Cancel:
			default:
				return;
				break;
		};
	}
	else
	{
		remove = true;
	}

	if(remove)
	{
		doTabClosing(tabWidget->indexOf(buf));

		removeCurrentBuffer();
	}

}

/*!
 * This slot executes an undo action by instructing our current buffer to
 * undo the most recent edit block.
 */
void BufferWidget::doUndo()
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->undo();

}

/*!
 * This slot executes a redo action by instructing our current buffer to
 * redo the most recent edit block.
 */
void BufferWidget::doRedo()
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->redo();

}

/*!
 * This slot executes a cut action by instructing our current buffer to cut
 * any selected text.
 */
void BufferWidget::doCut()
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->cut();

}

/*!
 * This slot executes a copy action by instructing our current buffer to copy
 * any selected text.
 */
void BufferWidget::doCopy()
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->copy();

}

/*!
 * This slot executes a paste action by instructing our current buffer to paste
 * any text in the clipboard.
 */
void BufferWidget::doPaste()
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->paste();

}

/*!
 * This slot executes a "duplicate line" action by instructing our current
 * buffer to duplicate its current line.
 */
void BufferWidget::doDuplicateLine()
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->duplicateLine();

}

/*!
 * This slot executes a "select all" action by instructing our current buffer
 * to select all of its contents.
 */
void BufferWidget::doSelectAll()
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->selectAll();

}

/*!
 * This slot executes a deslect action by instructing our current buffer to
 * clear any selection it currently has.
 */
void BufferWidget::doDeselect()
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->deselect();

}

/*!
 * This slot executes an "increase indent" action by instructing our current
 * buffer to increase the indent of any selection it might have.
 */
void BufferWidget::doIncreaseIndent()
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->increaseSelectionIndent();

}

/*!
 * This slot executes a "decrease indent" action by instructing our current
 * buffer to decrease the indent of any selection it might have.
 */
void BufferWidget::doDecreaseIndent()
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->decreaseSelectionIndent();

}

/*!
 * This slot executes a "find next" action by instructing our current buffer to
 * execute the given find query.
 *
 * \param q The find query to execute.
 * \return The result of this find action.
 */
Editor::FindResult BufferWidget::doFindNext(const QomposeFindQuery *q)
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return Editor::NoDocument;

	return buf->findNext(q);

}

/*!
 * This slot executes a "find previous" action by instructing our current
 * buffer to execute the given find query.
 *
 * \param q The find query to execute.
 * \return The result of this find action.
 */
Editor::FindResult BufferWidget::doFindPrevious(const QomposeFindQuery *q)
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return Editor::NoDocument;

	return buf->findPrevious(q);

}

/*!
 * This slot executes a "replace" action by instructing our current buffer to
 * execute the given replace query.
 *
 * \param q The replace query to execute.
 * \return The result of this replace action.
 */
Editor::FindResult BufferWidget::doReplace(const QomposeReplaceQuery *q)
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return Editor::NoDocument;

	return buf->replace(q);

}

/*!
 * This slot executes a "replace in selection" action by instructing our
 * current buffer to execute the given replace query.
 *
 * \param q The replace query to execute.
 * \return The result of this "replace in selection" action.
 */
Editor::FindResult BufferWidget::doReplaceSelection(
	const QomposeReplaceQuery *q)
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return Editor::NoDocument;

	return buf->replaceSelection(q);

}

/*!
 * This slot executes a "replace all" action by instructing our current buffer
 * to execute the given replace query.
 *
 * \param q The replace query to execute.
 * \return The result of this "replace all" action.
 */
Editor::FindResult BufferWidget::doReplaceAll(const QomposeReplaceQuery *q)
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return Editor::NoDocument;

	return buf->replaceAll(q);

}

/*!
 * This slot executes a "go to" action by instructing our current buffer to
 * move its cursor to the given line.
 *
 * \param l The line to move our current cursor to.
 */
void BufferWidget::doGoTo(int l)
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->goToLine(l);

}

/*!
 * This slot executes a "previous buffer" action by activating the buffer to
 * the left of our current buffer (or the rightmost buffer, if the leftmost
 * one is currently selected).
 */
void BufferWidget::doPreviousBuffer()
{ /* SLOT */

	int i = tabWidget->currentIndex() - 1;

	if(i < 0)
		i = tabWidget->count() - 1;

	tabWidget->setCurrentIndex(i);

}

/*!
 * This slot executes a "next buffer" action by activating the buffer to the
 * right of our current buffer (or the leftmost buffer, if the rightmost one
 * is currently selected).
 */
void BufferWidget::doNextBuffer()
{ /* SLOT */

	int i = tabWidget->currentIndex() + 1;

	if(i >= tabWidget->count())
		i = 0;

	tabWidget->setCurrentIndex(i);

}

/*!
 * This slot executes a "move buffer left" action by moving the currently
 * selected buffer to the left by one position. If the current buffer is
 * already the leftmost one, then no action is taken.
 */
void BufferWidget::doMoveBufferLeft()
{ /* SLOT */

	int f = tabWidget->currentIndex();
	int t = f - 1;

	if(t < 0)
		return;

	moveBuffer(f, t);
	tabWidget->setCurrentIndex(t);

}

/*!
 * This slot executes a "move buffer right" action by moving the currently
 * selected buffer to the right by one position. If the current buffer is
 * already the rightmost one, then no action is taken.
 */
void BufferWidget::doMoveBufferRight()
{ /* SLOT */

	int f = tabWidget->currentIndex();
	int t = f + 1;

	if(t >= tabWidget->count())
		return;

	moveBuffer(f, t);
	tabWidget->setCurrentIndex(t);

}

/*!
 * This slot executes a "print" action by instructing our current buffer
 * to print its contents to the given printer. If we have no current buffer,
 * then no action is taken.
 *
 * \param p The printer to write the current buffer's contents to.
 */
void BufferWidget::doPrint(QPrinter *p)
{ /* SLOT */

	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->print(p);

}

/*!
 * This function handles our current tab being changed by notifying various
 * listeners that our path has changed, and by focusing that tab's editor
 * widget.
 *
 * \param i The index of the newly-active tab.
 */
void BufferWidget::doTabChanged(int i)
{ /* SLOT */

	Buffer *b = dynamic_cast<Buffer *>(tabWidget->widget(i));

	if(b != NULL)
	{
		QTextCursor curs = b->textCursor();

		b->setFocus(Qt::OtherFocusReason);

		Q_EMIT pathChanged(b->getPath());
		Q_EMIT cursorPositionChanged(curs.blockNumber() + 1,
			curs.positionInBlock() + 1);
	}
	else
	{
		Q_EMIT pathChanged("");
		Q_EMIT cursorPositionChanged(1, 1);
	}

}

/*!
 * This function handles a tab close request by clicking the "close" button
 * on our tab bar.
 *
 * \param i The index of the tab being closed.
 */
void BufferWidget::doTabCloseRequested(int i)
{ /* SLOT */

	tabWidget->setCurrentIndex(i);
	doClose();

}

/*!
 * This function handles a tab closing event. More specifically, this function
 * is called just BEFORE any tab is removed from this widget.
 *
 * \param i The index of the tab which is about to be closed.
 */
void BufferWidget::doTabClosing(int i)
{ /* SLOT */

	QWidget *t = tabWidget->widget(i);
	Buffer *buf = dynamic_cast<Buffer *>(t);

	if(buf == NULL)
		return;

	if(buf->hasBeenSaved())
	{
		ClosedBufferDescriptor desc = {
			buf->getFileDescriptor(),
			buf->textCursor().position()
		};

		closedTabs.push(desc);

		while(closedTabs.count() > 20)
			closedTabs.remove(0);
	}

}

/*!
 * This slot handles a buffer's title being changed by updating the text on
 * its corresponding tab. The buffer whose title changed is determined via the
 * sender() function.
 *
 * \param t The new title for this buffer.
 */
void BufferWidget::doTabTitleChanged(const QString &t)
{ /* SLOT */

	Buffer *b = dynamic_cast<Buffer *>(sender());

	if(b != NULL)
	{
		int i = tabWidget->indexOf(b);

		if(i != -1)
			tabWidget->setTabText(i, t);
	}

}

/*!
 * This slot handles a buffer's path being changed by, if it is the current
 * buffer, notifying our listeners that our path has changed. The buffer
 * whose path changed is determined via the sender() function.
 *
 * \param p The new path for this buffer.
 */
void BufferWidget::doTabPathChanged(const QString &p)
{ /* SLOT */

	Buffer *b = dynamic_cast<Buffer *>(sender());

	if(b != NULL)
	{
		if(b == currentBuffer())
		{
			Q_EMIT pathChanged(p);
		}
	}

}

/*!
 * This slot handles a buffer's cursor position changing by, if it is the
 * current buffer, notifying our listeners that our cursor position has
 * changed. The buffer whose cursor position changed is determined via the
 * sender() function.
 */
void BufferWidget::doCursorPositionChanged()
{ /* SLOT */

	Buffer *b = dynamic_cast<Buffer *>(sender());

	if(b != NULL)
	{
		if(b == currentBuffer())
		{
			Q_EMIT cursorPositionChanged(b->getCurrentLine(),
				b->getCurrentColumn());
		}
	}

}

/*!
 * This is a utility function which will create a new buffer containing the
 * contents of the file denoted by the given file descriptor.
 *
 * \param d The file descriptor to open.
 * \return A pointer to the newly-opened buffer, or nullptr if none.
 */
Buffer *BufferWidget::doOpenDescriptor(const FileDescriptor &d)
{ /* SLOT */

	// If we only have one "Untitled" buffer, we'll replace it.

	if(tabs.count() == 1)
	{
		QSet<Buffer *>::iterator i = tabs.begin();
		Buffer *b = *i;
		int idx = tabWidget->indexOf(b);

		if( (!b->hasBeenSaved()) && (!b->isModified()) )
		{
			tabWidget->removeTab(idx);
			delete b;
			tabs.clear();
		}
	}

	// Open each of the files.

	int existing = findBufferWithPath(d.fileName);

	if(existing != -1)
	{
		tabWidget->setCurrentIndex(existing);
		return nullptr;
	}

	Buffer *b = newBuffer();
	b->open(d);

	Q_EMIT pathOpened(d.fileName);

	return b;

}

/*!
 * This is a utility function which attempts to reopen a closed buffer, using
 * the given closed buffer descriptor.
 *
 * \param d The closed buffer descriptor to reopen.
 */
void BufferWidget::doReopenBuffer(
	const ClosedBufferDescriptor &d)
{ /* SLOT */

	Buffer *b = doOpenDescriptor(d.file);

	if(b)
	{
		QTextCursor curs = b->textCursor();

		curs.setPosition(d.cursorPosition);

		b->setTextCursor(curs);
	}

}

}
