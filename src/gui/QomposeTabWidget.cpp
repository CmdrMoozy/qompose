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

#include "QomposeTabWidget.h"

#include <QGridLayout>
#include <QTabWidget>
#include <QSet>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

#include "dialogs/QomposeFileDialog.h"
#include "editor/QomposeBuffer.h"

QomposeTabWidget::QomposeTabWidget(QWidget *p)
	: QWidget(p)
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

QomposeTabWidget::~QomposeTabWidget()
{
	QList<QomposeBuffer *> t = tabs.values();
	
	while(!t.empty())
		delete t.takeLast();
	
	tabs.empty();
}

QomposeBuffer *QomposeTabWidget::currentBuffer() const
{
	return dynamic_cast<QomposeBuffer *>(tabWidget->currentWidget());
}

QomposeBuffer *QomposeTabWidget::newBuffer()
{
	QomposeBuffer *b = new QomposeBuffer(tabWidget);
	
	QObject::connect(b, SIGNAL(titleChanged(const QString &)),
		this, SLOT(doTabTitleChanged(const QString &)));
	QObject::connect(b, SIGNAL(pathChanged(const QString &)),
		this, SLOT(doTabPathChanged(const QString &)));
	
	int i = tabWidget->addTab(b, b->getTitle());
	tabWidget->setCurrentIndex(i);
	
	tabs.insert(b);
	
	return b;
}

void QomposeTabWidget::removeCurrentBuffer()
{
	int i = tabWidget->currentIndex();
	QomposeBuffer *b = currentBuffer();
	
	tabWidget->removeTab(i);
	
	tabs.remove(b);
	delete b;
}

void QomposeTabWidget::moveBuffer(int f, int t)
{
	QomposeBuffer *b = dynamic_cast<QomposeBuffer *>(tabWidget->widget(f));
	
	if(b == NULL)
		return;
	
	tabWidget->removeTab(f);
	tabWidget->insertTab(t, b, b->getTitle());
}

void QomposeTabWidget::doNew()
{ /* SLOT */
	
	newBuffer();
	
}

void QomposeTabWidget::doOpen()
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

void QomposeTabWidget::doRevert()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	buf->revert();
	
}

void QomposeTabWidget::doSave()
{ /* SLOT */
	
	QomposeBuffer *buf = currentBuffer();
	
	if(buf == NULL)
		return;
	
	if(buf->hasBeenSaved())
		buf->save();
	else
		doSaveAs();
	
}

void QomposeTabWidget::doSaveAs()
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

void QomposeTabWidget::doClose()
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

void QomposeTabWidget::doPreviousBuffer()
{ /* SLOT */
	
	int i = tabWidget->currentIndex() - 1;
	
	if(i < 0)
		i = tabWidget->count() - 1;
	
	tabWidget->setCurrentIndex(i);
	
}

void QomposeTabWidget::doNextBuffer()
{ /* SLOT */
	
	int i = tabWidget->currentIndex() + 1;
	
	if(i >= tabWidget->count())
		i = 0;
	
	tabWidget->setCurrentIndex(i);
	
}

void QomposeTabWidget::doMoveBufferLeft()
{ /* SLOT */
	
	int f = tabWidget->currentIndex();
	int t = f - 1;
	
	if(t < 0)
		return;
	
	moveBuffer(f, t);
	tabWidget->setCurrentIndex(t);
	
}

void QomposeTabWidget::doMoveBufferRight()
{ /* SLOT */
	
	int f = tabWidget->currentIndex();
	int t = f + 1;
	
	if(t >= tabWidget->count())
		return;
	
	moveBuffer(f, t);
	tabWidget->setCurrentIndex(t);
	
}

void QomposeTabWidget::doTabChanged(int i)
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
void QomposeTabWidget::doTabCloseRequested(int i)
{ /* SLOT */
	
	tabWidget->setCurrentIndex(i);
	doClose();
	
}

void QomposeTabWidget::doTabTitleChanged(const QString &t)
{ /* SLOT */
	
	QomposeBuffer *b = dynamic_cast<QomposeBuffer *>(sender());
	
	if(b != NULL)
	{
		int i = tabWidget->indexOf(b);
		
		if(i != -1)
			tabWidget->setTabText(i, t);
	}
	
}

void QomposeTabWidget::doTabPathChanged(const QString &p)
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
