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
#include "QomposeCommon/editor/pane/Pane.h"
#include "QomposeCommon/util/FindQuery.h"
#include "QomposeCommon/util/ReplaceQuery.h"

namespace qompose
{
BufferWidget::BufferWidget(QWidget *p)
        : QWidget(p, nullptr),
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
	layout->setContentsMargins(0, 0, 0, 0);
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
	Pane *pane = dynamic_cast<Pane *>(tabWidget->widget(i));
	if(pane == nullptr)
		return nullptr;
	return pane->getBuffer();
}

Buffer *BufferWidget::currentBuffer() const
{
	Pane *pane = dynamic_cast<Pane *>(tabWidget->currentWidget());
	if(pane == nullptr)
		return nullptr;
	return pane->getBuffer();
}

bool BufferWidget::hasCurrentBuffer() const
{
	return (currentBuffer() != nullptr);
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

std::vector<std::string> BufferWidget::getOpenPaths() const
{
	std::vector<std::string> paths;
	for(int i = 0; i < count(); ++i)
	{
		Buffer *buf = bufferAt(i);
		QString path = buf->getPath();
		if(!path.isEmpty())
			continue;
		paths.push_back(path.toStdString());
	}
	return paths;
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

std::string BufferWidget::getCommonParentPath() const
{
	return path_utils::getCommonParentPath(getOpenPaths());
}

Pane *BufferWidget::newPane()
{
	Pane *p = new Pane(this);

	QObject::connect(p->getBuffer(), &Buffer::titleChanged, this,
	                 &BufferWidget::doTabTitleChanged);
	QObject::connect(p->getBuffer(), &Buffer::pathChanged, this,
	                 &BufferWidget::doTabPathChanged);
	QObject::connect(p->getBuffer(), &Buffer::encodingChanged, this,
	                 &BufferWidget::doBufferEncodingChanged);
	QObject::connect(p->getBuffer(), &Buffer::cursorPositionChanged, this,
	                 &BufferWidget::doCursorPositionChanged);
	QObject::connect(p->getBuffer(), &Buffer::searchWrapped, this,
	                 &BufferWidget::searchWrapped);

	int i = tabWidget->addTab(p, p->getBuffer()->getTitle());
	tabWidget->setCurrentIndex(i);

	tabs.insert(p);

	return p;
}

void BufferWidget::removeCurrentBuffer()
{
	int i = tabWidget->currentIndex();
	Pane *pane = dynamic_cast<Pane *>(tabWidget->currentWidget());
	if(pane == nullptr)
		return;

	tabWidget->removeTab(i);
	tabs.remove(pane);
	delete pane;
}

void BufferWidget::moveBuffer(int f, int t)
{
	Pane *p = dynamic_cast<Pane *>(tabWidget->widget(f));
	if(p == nullptr)
		return;

	tabWidget->removeTab(f);
	tabWidget->insertTab(t, p, p->getBuffer()->getTitle());
}

void BufferWidget::doSetEncoding(const QByteArray &e)
{
	Buffer *buf = currentBuffer();

	if(buf == nullptr)
		return;

	if(buf->hasBeenSaved() && buf->isModified())
	{
		QMessageBox::StandardButton b = QMessageBox::question(
		        this, tr("Qompose - Unsaved Changes"),
		        tr("Really discard buffer changes to change encoding?"),
		        QMessageBox::Yes | QMessageBox::Cancel,
		        QMessageBox::Yes);

		switch(b)
		{
		case QMessageBox::Cancel:
			return;

		default:
			break;
		};
	}

	buf->setEncoding(e);
}

void BufferWidget::doNew()
{
	newPane();
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
	if(buf == nullptr)
		return;
	buf->save();
}

void BufferWidget::doSaveAs()
{
	Buffer *buf = currentBuffer();
	if(buf == nullptr)
		return;
	buf->saveAs();
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

	Pane *p = dynamic_cast<Pane *>(tabWidget->widget(i));

	if(p != nullptr)
	{
		QTextCursor curs = p->getBuffer()->textCursor();

		p->getBuffer()->setFocus(Qt::OtherFocusReason);

		Q_EMIT pathChanged(p->getBuffer()->getPath());
		Q_EMIT encodingChanged(p->getBuffer()->getEncoding());
		Q_EMIT cursorPositionChanged(curs.blockNumber() + 1,
		                             curs.positionInBlock() + 1);
	}
	else
	{
		Q_EMIT pathChanged("");
		Q_EMIT encodingChanged("UTF-8");
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
	Pane *p = dynamic_cast<Pane *>(tabWidget->widget(i));
	if(p == nullptr)
		return;

	if(p->getBuffer()->hasBeenSaved())
	{
		ClosedBufferDescriptor desc = {
		        p->getBuffer()->getFileDescriptor(),
		        p->getBuffer()->textCursor().position()};

		closedTabs.push(desc);

		while(closedTabs.count() > 20)
			closedTabs.remove(0);
	}
}

void BufferWidget::doTabTitleChanged(const QString &t)
{
	Buffer *b = dynamic_cast<Buffer *>(sender());
	if(b == nullptr)
		return;

	for(int i = 0; i < count(); ++i)
	{
		Pane *p = dynamic_cast<Pane *>(tabWidget->widget(i));
		if(p == nullptr)
			continue;

		if(p->getBuffer() == b)
		{
			tabWidget->setTabText(i, t);
			break;
		}
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
		QSet<Pane *>::iterator i = tabs.begin();
		Pane *p = *i;
		int idx = tabWidget->indexOf(p);

		if((!p->getBuffer()->hasBeenSaved()) &&
		   (!p->getBuffer()->isModified()))
		{
			tabWidget->removeTab(idx);
			delete p;
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

	Pane *p = newPane();
	p->getBuffer()->open(d);

	Q_EMIT pathOpened(d.fileName);

	return p->getBuffer();
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

void BufferWidget::doBufferEncodingChanged(const QByteArray &e)
{
	const Buffer *b = dynamic_cast<const Buffer *>(sender());
	if(b == nullptr)
		return;

	const Buffer *current = currentBuffer();

	if(b == current)
		Q_EMIT(encodingChanged(e));
}
}
