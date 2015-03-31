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

#include "StatusBar.h"

#include <QGridLayout>
#include <QLabel>
#include <QFrame>

#include "QomposeCommon/gui/EllipsizedLabel.h"
#include "QomposeCommon/gui/NotificationLabel.h"

namespace qompose
{
StatusBar::StatusBar(QWidget *p)
        : QStatusBar(p),
          statusWidget(nullptr),
          statusLayout(nullptr),
          notificationLabel(nullptr),
          tabPathLabel(nullptr),
          lineLabel(nullptr),
          columnLabel(nullptr)
{
	statusWidget = new QWidget(this, nullptr);

	statusLayout = new QGridLayout(statusWidget);
	statusLayout->setContentsMargins(5, 0, 5, 0);
	statusLayout->setSpacing(5);

	notificationLabel = new NotificationLabel(statusWidget);

	tabPathLabel = new EllipsizedLabel(statusWidget, Qt::AlignRight);

	lineLabel = new QLabel(statusWidget, nullptr);
	lineLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	setLine(1);

	columnLabel = new QLabel(statusWidget, nullptr);
	columnLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	setColumn(1);

	statusLayout->addWidget(notificationLabel, 0, 0, 1, 1, nullptr);
	statusLayout->addWidget(tabPathLabel, 0, 2, 1, 1, nullptr);
	statusLayout->addWidget(lineLabel, 0, 3, 1, 1, nullptr);
	statusLayout->addWidget(columnLabel, 0, 4, 1, 1, nullptr);
	statusLayout->setColumnStretch(2, 1);
	statusWidget->setLayout(statusLayout);

	addPermanentWidget(statusWidget, 1);
}

void StatusBar::displayNotification(const QString &n, bool c)
{
	notificationLabel->displayNotification(n, c);
}

void StatusBar::setCurrentTabPath(const QString &p)
{
	int l = p.trimmed().length();

	if(l > 0)
	{
		tabPathLabel->setFrameStyle(QFrame::StyledPanel |
		                            QFrame::Plain);
	}
	else
	{
		tabPathLabel->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
	}

	tabPathLabel->setText(p.trimmed());
}

void StatusBar::setLine(int l)
{
	lineLabel->setText(QString("L %1").arg(l));
}

void StatusBar::setColumn(int c)
{
	columnLabel->setText(QString("C %1").arg(c));
}
}
