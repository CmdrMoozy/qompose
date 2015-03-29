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
#include "QomposeCommon/util/FindQuery.h"
#include "QomposeCommon/util/ReplaceQuery.h"
#include "QomposeCommon/util/Settings.h"

namespace qompose
{
BufferWidget::BufferWidget(Settings *s, QWidget *p)
        : QWidget(p, nullptr),
          settings(s),
          layout(nullptr),
          tabWidget(nullptr),
          tabs(),
          closedTabs()
{
	layout = new QGridLayout(this);

	tabWidget = new QTabWidget(this);
	tabWidget->setMovable(true);
	tabWidget->setTabsClosable(true);
	tabWidget->setUsesScrollButtons(true);

	layout->addWidget(tabWidget, 0, 0, 1, 1, nullptr);
	setLayout(layout);

	QObject::connect(tabWidget, SIGNAL(currentChanged(int)), this,
	                 SLOT(doTabChanged(int)));
	QObject::connect(tabWidget, SIGNAL(tabCloseRequested(int)), this,
	                 SLOT(doTabCloseRequested(int)));
}

int BufferWidget::count() const
{
	return tabWidget->count();
}

Buffer *BufferWidget::bufferAt(int i) const
{
	return dynamic_cast<Buffer *>(tabWidget->widget(i));
}

Buffer *BufferWidget::currentBuffer() const
{
	return dynamic_cast<Buffer *>(tabWidget->currentWidget());
}

bool BufferWidget::hasCurrentBuffer() const
{
	return (currentBuffer() != NULL);
}

void BufferWidget::setCurrentBuffer(int i)
{
	tabWidget->setCurrentIndex(i);
}

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
			                QMessageBox::Cancel,
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

Buffer *BufferWidget::newBuffer()
{
	Buffer *b = new Buffer(settings, tabWidget);

	QObject::connect(b, SIGNAL(titleChanged(const QString &)), this,
	                 SLOT(doTabTitleChanged(const QString &)));
	QObject::connect(b, SIGNAL(pathChanged(const QString &)), this,
	                 SLOT(doTabPathChanged(const QString &)));
	QObject::connect(b, SIGNAL(cursorPositionChanged()), this,
	                 SLOT(doCursorPositionChanged()));
	QObject::connect(b, SIGNAL(searchWrapped()), this,
	                 SIGNAL(searchWrapped()));

	int i = tabWidget->addTab(b, b->getTitle());
	tabWidget->setCurrentIndex(i);

	tabs.insert(b);

	return b;
}

void BufferWidget::removeCurrentBuffer()
{
	int i = tabWidget->currentIndex();
	Buffer *b = currentBuffer();

	tabWidget->removeTab(i);

	tabs.remove(b);
	delete b;
}

void BufferWidget::moveBuffer(int f, int t)
{
	Buffer *b = dynamic_cast<Buffer *>(tabWidget->widget(f));

	if(b == NULL)
		return;

	tabWidget->removeTab(f);
	tabWidget->insertTab(t, b, b->getTitle());
}

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

void BufferWidget::doNew()
{
	newBuffer();
}

void BufferWidget::doOpen()
{
	// Open the one or more selected files.

	QList<FileDescriptor> files = FileDialog::getOpenFileNames(
	        this, tr("Open Files"), getDefaultDirectory());

	for(int i = 0; i < files.size(); ++i)
	{
		doOpenDescriptor(files.at(i));
	}
}

void BufferWidget::doOpenPath(const QString &p)
{
	FileDescriptor desc = FileDialog::getPathDescriptor(p);

	if(desc.fileName.isNull())
		return;

	doOpenDescriptor(desc);
}

void BufferWidget::doReopen()
{
	if(!closedTabs.empty())
		doReopenBuffer(closedTabs.pop());
}

void BufferWidget::doRevert()
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->revert();
}

void BufferWidget::doRevertAll()
{
	for(int i = 0; i < tabWidget->count(); ++i)
	{
		Buffer *buf = bufferAt(i);

		if(buf != NULL)
			buf->revert();
	}
}

void BufferWidget::doSave()
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	if(buf->hasBeenSaved())
		buf->save();
	else
		doSaveAs();
}

void BufferWidget::doSaveAs()
{
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

void BufferWidget::doClose()
{
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
		                QMessageBox::Cancel,
		        QMessageBox::Yes);

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

void BufferWidget::doUndo()
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->undo();
}

void BufferWidget::doRedo()
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->redo();
}

void BufferWidget::doCut()
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->cut();
}

void BufferWidget::doCopy()
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->copy();
}

void BufferWidget::doPaste()
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->paste();
}

void BufferWidget::doDuplicateLine()
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->duplicateLine();
}

void BufferWidget::doSelectAll()
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->selectAll();
}

void BufferWidget::doDeselect()
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->deselect();
}

void BufferWidget::doIncreaseIndent()
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->increaseSelectionIndent();
}

void BufferWidget::doDecreaseIndent()
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->decreaseSelectionIndent();
}

void BufferWidget::doLineWrapping(bool enabled)
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->setLineWrapping(enabled);
}

Editor::FindResult BufferWidget::doFindNext(const FindQuery *q)
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return Editor::NoDocument;

	return buf->findNext(q);
}

Editor::FindResult BufferWidget::doFindPrevious(const FindQuery *q)
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return Editor::NoDocument;

	return buf->findPrevious(q);
}

Editor::FindResult BufferWidget::doReplace(const ReplaceQuery *q)
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return Editor::NoDocument;

	return buf->replace(q);
}

Editor::FindResult BufferWidget::doReplaceSelection(const ReplaceQuery *q)
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return Editor::NoDocument;

	return buf->replaceSelection(q);
}

Editor::FindResult BufferWidget::doReplaceAll(const ReplaceQuery *q)
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return Editor::NoDocument;

	return buf->replaceAll(q);
}

void BufferWidget::doGoTo(int l)
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->goToLine(l);
}

void BufferWidget::doPreviousBuffer()
{
	int i = tabWidget->currentIndex() - 1;

	if(i < 0)
		i = tabWidget->count() - 1;

	tabWidget->setCurrentIndex(i);
}

void BufferWidget::doNextBuffer()
{
	int i = tabWidget->currentIndex() + 1;

	if(i >= tabWidget->count())
		i = 0;

	tabWidget->setCurrentIndex(i);
}

void BufferWidget::doMoveBufferLeft()
{
	int f = tabWidget->currentIndex();
	int t = f - 1;

	if(t < 0)
		return;

	moveBuffer(f, t);
	tabWidget->setCurrentIndex(t);
}

void BufferWidget::doMoveBufferRight()
{
	int f = tabWidget->currentIndex();
	int t = f + 1;

	if(t >= tabWidget->count())
		return;

	moveBuffer(f, t);
	tabWidget->setCurrentIndex(t);
}

void BufferWidget::doPrint(QPrinter *p)
{
	Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->print(p);
}

void BufferWidget::doTabChanged(int i)
{
	Q_EMIT bufferChanged();

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

void BufferWidget::doTabCloseRequested(int i)
{
	tabWidget->setCurrentIndex(i);
	doClose();
}

void BufferWidget::doTabClosing(int i)
{
	QWidget *t = tabWidget->widget(i);
	Buffer *buf = dynamic_cast<Buffer *>(t);

	if(buf == NULL)
		return;

	if(buf->hasBeenSaved())
	{
		ClosedBufferDescriptor desc = {buf->getFileDescriptor(),
		                               buf->textCursor().position()};

		closedTabs.push(desc);

		while(closedTabs.count() > 20)
			closedTabs.remove(0);
	}
}

void BufferWidget::doTabTitleChanged(const QString &t)
{
	Buffer *b = dynamic_cast<Buffer *>(sender());

	if(b != nullptr)
	{
		int i = tabWidget->indexOf(b);

		if(i != -1)
			tabWidget->setTabText(i, t);
	}
}

void BufferWidget::doTabPathChanged(const QString &p)
{
	Buffer *b = dynamic_cast<Buffer *>(sender());

	if(b != NULL)
	{
		if(b == currentBuffer())
		{
			Q_EMIT pathChanged(p);
		}
	}
}

void BufferWidget::doCursorPositionChanged()
{
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

Buffer *BufferWidget::doOpenDescriptor(const FileDescriptor &d)
{
	// If we only have one "Untitled" buffer, we'll replace it.

	if(tabs.count() == 1)
	{
		QSet<Buffer *>::iterator i = tabs.begin();
		Buffer *b = *i;
		int idx = tabWidget->indexOf(b);

		if((!b->hasBeenSaved()) && (!b->isModified()))
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

void BufferWidget::doReopenBuffer(const ClosedBufferDescriptor &d)
{
	Buffer *b = doOpenDescriptor(d.file);

	if(b)
	{
		QTextCursor curs = b->textCursor();

		curs.setPosition(d.cursorPosition);

		b->setTextCursor(curs);
	}
}
}
