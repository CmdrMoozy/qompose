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

#include "QomposeBufferWidget.h"

#include <QGridLayout>
#include <QSet>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QTabWidget>

#include "dialogs/QomposeFileDialog.h"
#include "editor/QomposeBuffer.h"
#include "util/QomposeFindQuery.h"
#include "util/QomposeReplaceQuery.h"
#include "util/QomposeSettings.h"

/*!
 * This is our default constructor, which creates a new instance of our buffers
 * tab widget.
 *
 * \param s The settings instance to give to our buffers.
 * \param p The parent widget to use for this widget.
 */
QomposeBufferWidget::QomposeBufferWidget(QomposeSettings *s, QWidget *p)
	: QWidget(p), settings(s)
{
	layout = new QGridLayout(this);
	
	tabWidget = new QTabWidget(this);
	tabWidget->setMovable(true);
	tabWidget->setTabsClosable(true);
	tabWidget->setUsesScrollButtons(true);
	
	layout->addWidget(tabWidget, 0, 0, 1, 1);
	setLayout(layout);
	
	QObject::connect( tabWidget, SIGNAL( currentChanged(int)    ), this, SLOT( doTabChanged(int)        ) );
	QObject::connect( tabWidget, SIGNAL( tabCloseRequested(int) ), this, SLOT( doTabCloseRequested(int) ) );
}

/*!
 * This is our default destructor, which cleans up & destroys our widget.
 */
QomposeBufferWidget::~QomposeBufferWidget()
{
}

/*!
 * This function returns the number of buffers our widget currently contains.
 *
 * \return The number of buffers we have.
 */
int QomposeBufferWidget::count() const
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
QomposeBuffer *QomposeBufferWidget::bufferAt(int i) const
{
	return dynamic_cast<QomposeBuffer *>(tabWidget->widget(i));
}

/*!
 * This function returns our current buffer (i.e., the visible one), or NULL
 * if we don't have any current buffer.
 *
 * \return Our widget's current buffer.
 */
QomposeBuffer *QomposeBufferWidget::currentBuffer() const
{
	return dynamic_cast<QomposeBuffer *>(tabWidget->currentWidget());
}

/*!
 * This function sets our widget's visible buffer to the buffer at the given
 * index. If the given index is out of bounds, then no action is taken.
 *
 * \param i The index of the desired buffer.
 */
void QomposeBufferWidget::setCurrentBuffer(int i)
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
bool QomposeBufferWidget::prepareCloseParent()
{
	for(int i = 0; i < count(); ++i)
	{
		QomposeBuffer *buf = bufferAt(i);
		
		if(buf == NULL)
			continue;
		
		setCurrentBuffer(i);
		
		if(buf->isModified())
		{
			QMessageBox::StandardButton b = QMessageBox::question(
				this, tr("Qompose - Unsaved Changes"),
				tr("Save changes to this buffer before closing?"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
				QMessageBox::Yes);
			
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
 * This is a utility function which creates a new buffer widget, connects
 * its signals and slots, adds it to our tab widget, and makes it the
 * active tab.
 *
 * \return The newly-created buffer widget.
 */
QomposeBuffer *QomposeBufferWidget::newBuffer()
{
	QomposeBuffer *b = new QomposeBuffer(settings, tabWidget);
	
	QObject::connect(b, SIGNAL(titleChanged(const QString &)),
		this, SLOT(doTabTitleChanged(const QString &)));
	QObject::connect(b, SIGNAL(pathChanged(const QString &)),
		this, SLOT(doTabPathChanged(const QString &)));
	
	int i = tabWidget->addTab(b, b->getTitle());
	tabWidget->setCurrentIndex(i);
	
	tabs.insert(b);
	
	return b;
}

/*!
 * This function removes the current buffer from our tab widget, and deletes it.
 */
void QomposeBufferWidget::removeCurrentBuffer()
{
	int i = tabWidget->currentIndex();
	QomposeBuffer *b = currentBuffer();
	
	tabWidget->removeTab(i);
	
	tabs.remove(b);
	delete b;
}

/*!
 * This function moves the tab at the given index "f" to the given index "t". That is,
 * its new index after being moved will be exactly "t." If the given from index is
 * out-of-bounds, then no action is taken.
 *
 * \param f The index to move a tab from.
 * \param t The index to move the tab to.
 */
void QomposeBufferWidget::moveBuffer(int f, int t)
{
	QomposeBuffer *b = dynamic_cast<QomposeBuffer *>(tabWidget->widget(f));
	
	if(b == NULL)
		return;
	
	tabWidget->removeTab(f);
	tabWidget->insertTab(t, b, b->getTitle());
}

/*!
 * This slot executes a new action by creating a new buffer widget.
 */
void QomposeBufferWidget::doNew()
{ /* SLOT */
	
	newBuffer();
	
}

/*!
 * This slot executes an open action by showing an open dialog and then
 * opening the selected file. If we only had one "Untitled" tab before, then
 * it will be replaced by the newly opened file.
 */
void QomposeBufferWidget::doOpen()
{ /* SLOT */
	
	QList<QomposeFileDescriptor> files = QomposeFileDialog::getOpenFileNames(
		this, tr("Open Files"), QDir::homePath());
	
	if(files.length() > 0)
	{
		// If we only have one "Untitled" buffer, we'll replace it.
		
		if(tabs.count() == 1)
		{
			QSet<QomposeBuffer *>::iterator i = tabs.begin();
			QomposeBuffer *b = *i;
			int idx = tabWidget->indexOf(b);
			
			if( (!b->hasBeenSaved()) && (!b->isModified()) )
			{
				tabWidget->removeTab(idx);
				delete b;
				tabs.empty();
			}
		}
		
		// Open each of the files.
		
		for(int i = 0; i < files.size(); ++i)
		{
			QomposeBuffer *b = newBuffer();
			b->open(files.at(i));
		}
	}
	
}

/*!
 * This slot executes a revert action by instructing our current buffer to
 * revert its changes.
 */
void QomposeBufferWidget::doRevert()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	buf->revert();
	
}

/*!
 * This slot executes a save action by instructing our current buffer to
 * save its changes, potentially showing a "save as" dialog if it hasn't ever
 * been saved before.
 */
void QomposeBufferWidget::doSave()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	if(buf->hasBeenSaved())
		buf->save();
	else
		doSaveAs();
	
}

/*!
 * This slot executes a "save as" action by showing a save file dialog, and then
 * by instructing our current buffer to save to the selected path.
 */
void QomposeBufferWidget::doSaveAs()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	QString p = QFileDialog::getSaveFileName(this, tr("Save File"),
		QDir::homePath());
	
	if(p.length() <= 0)
		return;
	
	buf->save(p);
	
}

/*!
 * This slot executes a close action by possibly prompting to save any unsaved
 * changes, and ultimately by removing the current buffer from our tab widget.
 */
void QomposeBufferWidget::doClose()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	if(buf->isModified())
	{
		QMessageBox::StandardButton b = QMessageBox::question(
			this, tr("Qompose - Unsaved Changes"),
			tr("Save changes to this buffer before closing?"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
			QMessageBox::Yes);
		
		switch(b)
		{
			case QMessageBox::Yes:
				doSave();
				
				if(!buf->isModified())
					removeCurrentBuffer();
				
				break;
				
			case QMessageBox::No:
				removeCurrentBuffer();
				break;
				
			case QMessageBox::Cancel:
			default:
				return;
				break;
		};
	}
	else
	{
		removeCurrentBuffer();
	}
	
}

/*!
 * This slot executes an undo action by instructing our current buffer to
 * undo the most recent edit block.
 */
void QomposeBufferWidget::doUndo()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	buf->undo();
	
}

/*!
 * This slot executes a redo action by instructing our current buffer to
 * redo the most recent edit block.
 */
void QomposeBufferWidget::doRedo()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	buf->redo();
	
}

/*!
 * This slot executes a cut action by instructing our current buffer to cut
 * any selected text.
 */
void QomposeBufferWidget::doCut()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	buf->cut();
	
}

/*!
 * This slot executes a copy action by instructing our current buffer to copy
 * any selected text.
 */
void QomposeBufferWidget::doCopy()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	buf->copy();
	
}

/*!
 * This slot executes a paste action by instructing our current buffer to paste
 * any text in the clipboard.
 */
void QomposeBufferWidget::doPaste()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	buf->paste();
	
}

/*!
 * This slot executes a "duplicate line" action by instructing our current buffer
 * to duplicate its current line.
 */
void QomposeBufferWidget::doDuplicateLine()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	buf->duplicateLine();
	
}

/*!
 * This slot executes a "select all" action by instructing our current buffer to
 * select all of its contents.
 */
void QomposeBufferWidget::doSelectAll()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	buf->selectAll();
	
}

/*!
 * This slot executes a deslect action by instructing our current buffer to clear
 * any selection it currently has.
 */
void QomposeBufferWidget::doDeselect()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	buf->deselect();
	
}

/*!
 * This slot executes an "increase indent" action by instructing our current buffer
 * to increase the indent of any selection it might have.
 */
void QomposeBufferWidget::doIncreaseIndent()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	buf->increaseSelectionIndent();
	
}

/*!
 * This slot executes a "decrease indent" action by instructing our current buffer
 * to decrease the indent of any selection it might have.
 */
void QomposeBufferWidget::doDecreaseIndent()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
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
QomposeEditor::FindResult QomposeBufferWidget::doFindNext(const QomposeFindQuery *q)
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return QomposeEditor::NoDocument;
	
	return buf->findNext(q);
	
}

/*!
 * This slot executes a "find previous" action by instructing our current buffer
 * to execute the given find query.
 *
 * \param q The find query to execute.
 * \return The result of this find action.
 */
QomposeEditor::FindResult QomposeBufferWidget::doFindPrevious(const QomposeFindQuery *q)
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return QomposeEditor::NoDocument;
	
	return buf->findPrevious(q);
	
}

/*!
 * This slot executes a "replace" action by instructing our current buffer to execute
 * the given replace query.
 *
 * \param q The replace query to execute.
 * \return The result of this replace action.
 */
QomposeEditor::FindResult QomposeBufferWidget::doReplace(const QomposeReplaceQuery *q)
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return QomposeEditor::NoDocument;
	
	return buf->replace(q);
	
}

/*!
 * This slot executes a "replace in selection" action by instructing our current buffer
 * to execute the given replace query.
 *
 * \param q The replace query to execute.
 * \return The result of this "replace in selection" action.
 */
QomposeEditor::FindResult QomposeBufferWidget::doReplaceSelection(const QomposeReplaceQuery *q)
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return QomposeEditor::NoDocument;
	
	return buf->replaceSelection(q);
	
}

/*!
 * This slot executes a "replace all" action by instructing our current buffer to
 * execute the given replace query.
 *
 * \param q The replace query to execute.
 * \return The result of this "replace all" action.
 */
QomposeEditor::FindResult QomposeBufferWidget::doReplaceAll(const QomposeReplaceQuery *q)
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return QomposeEditor::NoDocument;
	
	return buf->replaceAll(q);
	
}

/*!
 * This slot executes a "go to" action by instructing our current buffer to
 * move its cursor to the given line.
 *
 * \param l The line to move our current cursor to.
 */
void QomposeBufferWidget::doGoTo(int l)
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	buf->goToLine(l);
	
}

/*!
 * This slot executes a "previous buffer" action by activating the buffer to
 * the left of our current buffer (or the rightmost buffer, if the leftmost
 * one is currently selected).
 */
void QomposeBufferWidget::doPreviousBuffer()
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
void QomposeBufferWidget::doNextBuffer()
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
void QomposeBufferWidget::doMoveBufferLeft()
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
void QomposeBufferWidget::doMoveBufferRight()
{ /* SLOT */
	
	int f = tabWidget->currentIndex();
	int t = f + 1;
	
	if(t >= tabWidget->count())
		return;
	
	moveBuffer(f, t);
	tabWidget->setCurrentIndex(t);
	
}

/*!
 * This function handles our current tab being changed by notifying various
 * listeners that our path has changed, and by focusing that tab's editor
 * widget.
 *
 * \param i The index of the newly-active tab.
 */
void QomposeBufferWidget::doTabChanged(int i)
{ /* SLOT */
	
	QomposeBuffer *b = dynamic_cast<QomposeBuffer *>(tabWidget->widget(i));
	
	if(b != NULL)
	{
		b->setFocus(Qt::OtherFocusReason);
		emit pathChanged(b->getPath());
	}
	
}

/*!
 * This function handles a tab close request by clicking the "close" button
 * on our tab bar.
 *
 * \param i The index of the tab being closed.
 */
void QomposeBufferWidget::doTabCloseRequested(int i)
{ /* SLOT */
	
	tabWidget->setCurrentIndex(i);
	doClose();
	
}

/*!
 * This slot handles a buffer's title being changed by updating the text on
 * its corresponding tab. The buffer whose title changed is determined via the
 * sender() function.
 *
 * \param t The new title for this buffer.
 */
void QomposeBufferWidget::doTabTitleChanged(const QString &t)
{ /* SLOT */
	
	QomposeBuffer *b = dynamic_cast<QomposeBuffer *>(sender());
	
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
void QomposeBufferWidget::doTabPathChanged(const QString &p)
{ /* SLOT */
	
	QomposeBuffer *b = dynamic_cast<QomposeBuffer *>(sender());
	
	if(b != NULL)
	{
		if(b == currentBuffer())
		{
			emit pathChanged(p);
		}
	}
	
}
