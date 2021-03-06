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

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QPrinter>
#include <QSet>
#include <QTabWidget>

#include <bdrck/fs/Util.hpp>

#include "QomposeCommon/dialogs/FileDialog.h"
#include "QomposeCommon/editor/Buffer.h"
#include "QomposeCommon/editor/pane/Pane.h"

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

editor::Buffer *BufferWidget::bufferAt(int i) const
{
	Pane *pane = dynamic_cast<Pane *>(tabWidget->widget(i));
	if(pane == nullptr)
		return nullptr;
	return pane->getBuffer();
}

editor::Buffer *BufferWidget::currentBuffer() const
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
		editor::Buffer *buf = bufferAt(i);
		if(buf == nullptr)
			continue;
		setCurrentBuffer(i);
		if(!buf->prepareToClose())
			return false;
	}

	return true;
}

int BufferWidget::findBufferWithPath(const QString &p)
{
	QFileInfo file(p);

	for(int i = 0; i < count(); ++i)
	{
		editor::Buffer *buf = bufferAt(i);
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
		editor::Buffer *buf = bufferAt(i);
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

	editor::Buffer *buf = currentBuffer();
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
	return bdrck::fs::commonParentPath(getOpenPaths());
}

Pane *BufferWidget::newPane()
{
	Pane *p = new Pane(this);

	QObject::connect(p->getBuffer(), &editor::Buffer::titleChanged, this,
	                 &BufferWidget::doTabTitleChanged);
	QObject::connect(p->getBuffer(), &editor::Buffer::pathChanged, this,
	                 &BufferWidget::doTabPathChanged);
	QObject::connect(p->getBuffer(), &editor::Buffer::encodingChanged, this,
	                 &BufferWidget::doBufferEncodingChanged);
	QObject::connect(p->getBuffer(), &editor::Buffer::searchWrapped, this,
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
	editor::Buffer *buf = currentBuffer();
	if(buf == nullptr)
		return;
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
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->revert();
}

void BufferWidget::doRevertAll()
{
	for(int i = 0; i < tabWidget->count(); ++i)
	{
		editor::Buffer *buf = bufferAt(i);

		if(buf != NULL)
			buf->revert();
	}
}

void BufferWidget::doSave()
{
	editor::Buffer *buf = currentBuffer();
	if(buf == nullptr)
		return;
	buf->save();
}

void BufferWidget::doSaveAs()
{
	editor::Buffer *buf = currentBuffer();
	if(buf == nullptr)
		return;
	buf->saveAs();
}

void BufferWidget::doClose()
{
	editor::Buffer *buf = currentBuffer();
	if(buf == nullptr)
		return;

	if(buf->prepareToClose())
	{
		doTabClosing(tabWidget->indexOf(buf));
		removeCurrentBuffer();
	}
}

void BufferWidget::doUndo()
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->undo();
}

void BufferWidget::doRedo()
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->redo();
}

void BufferWidget::doCut()
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->cut();
}

void BufferWidget::doCopy()
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->copy();
}

void BufferWidget::doPaste()
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->paste();
}

void BufferWidget::doDuplicateLine()
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->duplicateLine();
}

void BufferWidget::doSelectAll()
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->selectAll();
}

void BufferWidget::doDeselect()
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->deselect();
}

void BufferWidget::doIncreaseIndent()
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->increaseSelectionIndent();
}

void BufferWidget::doDecreaseIndent()
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->decreaseSelectionIndent();
}

void BufferWidget::doLineWrapping(bool enabled)
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return;

	buf->setLineWrapping(enabled);
}

editor::search::FindResult
BufferWidget::doFindNext(editor::search::FindQuery const &q)
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return editor::search::FindResult::NoDocument;

	return buf->findNext(q);
}

editor::search::FindResult
BufferWidget::doFindPrevious(editor::search::FindQuery const &q)
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return editor::search::FindResult::NoDocument;

	return buf->findPrevious(q);
}

editor::search::FindResult
BufferWidget::doReplace(editor::search::ReplaceQuery const &q)
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return editor::search::FindResult::NoDocument;

	return buf->replace(q);
}

editor::search::FindResult
BufferWidget::doReplaceSelection(editor::search::ReplaceQuery const &q)
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return editor::search::FindResult::NoDocument;

	return buf->replaceSelection(q);
}

editor::search::FindResult
BufferWidget::doReplaceAll(editor::search::ReplaceQuery const &q)
{
	editor::Buffer *buf = currentBuffer();

	if(buf == NULL)
		return editor::search::FindResult::NoDocument;

	return buf->replaceAll(q);
}

void BufferWidget::doGoTo(int l)
{
	editor::Buffer *buf = currentBuffer();

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
	editor::Buffer *buf = currentBuffer();

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
	}
	else
	{
		Q_EMIT pathChanged("");
		Q_EMIT encodingChanged("UTF-8");
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
	editor::Buffer *b = dynamic_cast<editor::Buffer *>(sender());
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
	editor::Buffer *b = dynamic_cast<editor::Buffer *>(sender());

	if(b != NULL)
	{
		if(b == currentBuffer())
		{
			Q_EMIT pathChanged(p);
		}
	}
}

editor::Buffer *BufferWidget::doOpenDescriptor(const FileDescriptor &d)
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
	editor::Buffer *b = doOpenDescriptor(d.file);

	if(b)
	{
		QTextCursor curs = b->textCursor();

		curs.setPosition(d.cursorPosition);

		b->setTextCursor(curs);
	}
}

void BufferWidget::doBufferEncodingChanged(const QByteArray &e)
{
	const editor::Buffer *b =
	        dynamic_cast<const editor::Buffer *>(sender());
	if(b == nullptr)
		return;

	const editor::Buffer *current = currentBuffer();

	if(b == current)
		Q_EMIT(encodingChanged(e));
}
}
