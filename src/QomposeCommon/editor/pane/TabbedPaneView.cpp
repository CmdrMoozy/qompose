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

#include "TabbedPaneView.h"

#include <cstddef>

#include <QGridLayout>
#include <QStackedLayout>
#include <QTabBar>

#include "QomposeCommon/editor/Buffer.h"
#include "QomposeCommon/editor/pane/Pane.h"
#include "QomposeCommon/editor/pane/PaneModel.h"

namespace qompose
{
TabbedPaneView::TabbedPaneView(PaneModel *m, QWidget *p, Qt::WindowFlags f)
        : PaneView(m, p, f),
          model(m),
          layout(nullptr),
          tabBar(nullptr),
          currentTabLayout(nullptr)
{
	layout = new QGridLayout(this);

	tabBar = new QTabBar(this);
	tabBar->setMovable(true);
	tabBar->setTabsClosable(true);

	currentTabLayout = new QStackedLayout(layout);

	layout->addWidget(tabBar, 0, 0, 1, 1);
	layout->addLayout(currentTabLayout, 1, 0, 1, 1);
	layout->setColumnStretch(0, 1);
	layout->setRowStretch(1, 1);
	setLayout(layout);
}

void TabbedPaneView::connectPane(Pane *p) const
{
	QObject::connect(p->getBuffer(), &Buffer::titleChanged, this,
	                 &TabbedPaneView::doTabTitleChanged);
	QObject::connect(p->getBuffer(), &Buffer::pathChanged, this,
	                 &TabbedPaneView::doTabPathChanged);
	QObject::connect(p->getBuffer(), &Buffer::encodingChanged, this,
	                 &TabbedPaneView::doBufferEncodingChanged);
	QObject::connect(p->getBuffer(), &Buffer::searchWrapped, this,
	                 &TabbedPaneView::searchWrapped);
}

void TabbedPaneView::doPaneAdded(Pane *p, std::size_t i)
{
	connectPane(p);
	tabBar->insertTab(i, p->getBuffer()->getTitle());
	setCurrentPane(i);
}

void TabbedPaneView::doPaneRemoved(std::size_t i)
{
	tabBar->removeTab(i);
}

void TabbedPaneView::doCurrentChanged(int index)
{
	setCurrentPane(static_cast<std::size_t>(index));
	currentTabLayout->setCurrentWidget(currentPane());
}

void TabbedPaneView::doTabMoved(int from, int to)
{
	model->movePane(static_cast<std::size_t>(from),
	                static_cast<std::size_t>(to));
}

void TabbedPaneView::doCloseRequested(int)
{
}

void TabbedPaneView::doTabTitleChanged(const QString &)
{
}

void TabbedPaneView::doTabPathChanged(const QString &)
{
}

void TabbedPaneView::doBufferEncodingChanged(const QByteArray &)
{
}
}
