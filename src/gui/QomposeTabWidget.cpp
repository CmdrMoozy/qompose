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
#include <QTabBar>
#include <QList>
#include <QStackedLayout>

#include "dialogs/QomposeFileDialog.h"
#include "editor/QomposeBuffer.h"

QomposeTabWidget::QomposeTabWidget(QWidget *p)
	: QWidget(p)
{
	layout = new QGridLayout(this);
	
	tabBar = new QTabBar(this);
	tabBar->setMovable(true);
	tabBar->setTabsClosable(true);
	tabBar->setUsesScrollButtons(true);
	
	tabDisplayWidget = new QWidget(this);
	tabDisplayLayout = new QStackedLayout(tabDisplayWidget);
	
	layout->addWidget(tabBar, 0, 0, 1, 1);
	layout->addWidget(tabDisplayWidget, 1, 0, 1, 1);
	layout->setRowStretch(1, 1);
	layout->setColumnStretch(0, 1);
	setLayout(layout);
	
	QObject::connect( tabBar, SIGNAL( currentChanged(int)    ), this, SLOT( doTabChanged(int) ) );
	QObject::connect( tabBar, SIGNAL( tabCloseRequested(int) ), this, SLOT( doCloseTab(int)   ) );
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
	return tabs.value(tabBar->currentIndex(), NULL);
}

QomposeBuffer *QomposeTabWidget::newBuffer()
{
	QomposeBuffer *b = new QomposeBuffer(this);
	
	QObject::connect(b, SIGNAL(titleChanged(const QString &)),
		this, SLOT(doTabTitleChanged(const QString &)));
	QObject::connect(b, SIGNAL(pathChanged(const QString &)),
		this, SLOT(doTabPathChanged(const QString &)));
	
	int i = tabBar->addTab(b->getTitle());
	tabBar->setCurrentIndex(i);
	
	tabs.insert(i, b);
	
	tabDisplayLayout->addWidget(b);
	
	return b;
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
			QMap<int, QomposeBuffer *>::iterator i = tabs.begin();
			
			if( (!i.value()->hasBeenSaved()) && (!i.value()->isModified()) )
			{
				tabBar->removeTab(i.key());
				delete i.value();
				
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

void QomposeTabWidget::doTabChanged(int i)
{ /* SLOT */
	
	QomposeBuffer *b = tabs.value(i, NULL);
	
	if(b != NULL)
	{
		tabDisplayLayout->setCurrentWidget(b);
		
		emit pathChanged(b->getPath());
	}
	
}

void QomposeTabWidget::doCloseTab(int i)
{ /* SLOT */
	
	
	
}

void QomposeTabWidget::doTabTitleChanged(const QString &t)
{ /* SLOT */
	
	QomposeBuffer *b = dynamic_cast<QomposeBuffer *>(sender());
	
	if(b != NULL)
	{
		int i = tabs.key(b, -1);
		
		if(i != -1)
			tabBar->setTabText(i, t);
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
