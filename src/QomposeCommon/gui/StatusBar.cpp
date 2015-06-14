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

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QVariant>

#include "QomposeCommon/gui/EllipsizedLabel.h"
#include "QomposeCommon/gui/NotificationLabel.h"
#include "QomposeCommon/util/Settings.h"

namespace qompose
{
StatusBar::StatusBar(QWidget *p)
        : QStatusBar(p),
          statusWidget(nullptr),
          statusLayout(nullptr),
          notificationLabel(nullptr),
          filePathLabel(nullptr),
          lineLabel(nullptr),
          columnLabel(nullptr)
{
	statusWidget = new QWidget(this, nullptr);

	statusLayout = new QGridLayout(statusWidget);
	statusLayout->setContentsMargins(5, 0, 5, 0);
	statusLayout->setSpacing(5);

	notificationLabel = new NotificationLabel(statusWidget);

	filePathLabel = new EllipsizedLabel(Qt::AlignRight, statusWidget);

	lineLabel = new QLabel(statusWidget, nullptr);
	lineLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	columnLabel = new QLabel(statusWidget, nullptr);
	columnLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	setCursorPosition(1, 1);

	statusLayout->addWidget(notificationLabel, 0, 0, 1, 1, nullptr);
	statusLayout->addWidget(filePathLabel, 0, 2, 1, 1, nullptr);
	statusLayout->addWidget(lineLabel, 0, 3, 1, 1, nullptr);
	statusLayout->addWidget(columnLabel, 0, 4, 1, 1, nullptr);
	statusLayout->setColumnStretch(2, 1);
	statusWidget->setLayout(statusLayout);

	addPermanentWidget(statusWidget, 1);

	setVisible(Settings::instance().getSetting("show-status-bar").toBool());

	QObject::connect(&Settings::instance(), &Settings::settingChanged, this,
	                 &StatusBar::doSettingChanged);
}

void StatusBar::displayNotification(const QString &n, bool c)
{
	notificationLabel->displayNotification(n, c);
}

void StatusBar::setFilePath(const QString &p)
{
	int l = p.trimmed().length();

	if(l > 0)
	{
		filePathLabel->setFrameStyle(QFrame::StyledPanel |
		                             QFrame::Plain);
	}
	else
	{
		filePathLabel->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
	}

	filePathLabel->setText(p.trimmed());
}

void StatusBar::setCursorPosition(int l, int c)
{
	lineLabel->setText(QString("L %1").arg(l));
	columnLabel->setText(QString("C %1").arg(c));
}

void StatusBar::doSettingChanged(const QString &k, const QVariant &v)
{
	if(k == "show-status-bar")
		setVisible(v.toBool());
}
}
