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

#include "QomposeCommon/gui/NotificationLabel.h"

namespace qompose
{
/*!
 * This is our default constructor, which creates a new instance of our status
 * bar widget.
 *
 * \param p Our status bar's parent widget.
 */
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

	tabPathLabel = new QLabel(statusWidget, nullptr);

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
	statusLayout->setColumnStretch(1, 1);
	statusWidget->setLayout(statusLayout);

	addPermanentWidget(statusWidget, 1);
}

/*!
 * This is our default destructor, which cleans up and destroys our this status
 * bar widget instance.
 */
StatusBar::~StatusBar()
{
}

/*!
 * This function displays a notification in this status bar widget.
 *
 * \param n The notification to display.
 * \param c Whether or not the notification is critical.
 */
void StatusBar::displayNotification(const QString &n, bool c)
{
	notificationLabel->displayNotification(n, c);
}

/*!
 * This function sets the "current tab path" that we're displaying. This should
 * be the absolute path to the file which is open in the current tab, or an
 * empty string if no valid absolute path exists.
 *
 * \param p The current tab path to display.
 */
void StatusBar::setCurrentTabPath(const QString &p)
{
	int l = p.trimmed().length();

	if(l > 0)
		tabPathLabel->setFrameStyle(QFrame::StyledPanel |
		                            QFrame::Plain);
	else
		tabPathLabel->setFrameStyle(QFrame::NoFrame | QFrame::Plain);

	tabPathLabel->setText(p.trimmed());
}

/*!
 * This function sets the line number we'll display in the status bar.
 *
 * \param l The current line number.
 */
void StatusBar::setLine(int l)
{
	lineLabel->setText(QString("L %1").arg(l));
}

/*!
 * This function sets the column number we'll display in the status bar.
 *
 * \param c The current column number.
 */
void StatusBar::setColumn(int c)
{
	columnLabel->setText(QString("C %1").arg(c));
}
}
