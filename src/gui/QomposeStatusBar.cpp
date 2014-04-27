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

#include "QomposeStatusBar.h"

#include <QGridLayout>
#include <QLabel>
#include <QFrame>

/*!
 * This is our default constructor, which creates a new instance of our status
 * bar widget.
 *
 * \param p Our status bar's parent widget.
 */
QomposeStatusBar::QomposeStatusBar(QWidget *p)
	: QStatusBar(p)
{
	statusWidget = new QWidget(this);

	statusLayout = new QGridLayout(statusWidget);
	statusLayout->setContentsMargins(5, 0, 5, 0);
	statusLayout->setSpacing(5);

	notificationLabel = new QLabel(statusWidget);

	tabPathLabel = new QLabel(statusWidget);

	lineLabel = new QLabel(statusWidget);
	lineLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	setLine(1);

	columnLabel = new QLabel(statusWidget);
	columnLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	setColumn(1);

	statusLayout->addWidget(notificationLabel, 0, 0, 1, 1);
	statusLayout->addWidget(tabPathLabel, 0, 2, 1, 1);
	statusLayout->addWidget(lineLabel, 0, 3, 1, 1);
	statusLayout->addWidget(columnLabel, 0, 4, 1, 1);
	statusLayout->setColumnStretch(1, 1);
	statusWidget->setLayout(statusLayout);

	addPermanentWidget(statusWidget, 1);
}

/*!
 * This is our default destructor, which cleans up and destroys our this status
 * bar widget instance.
 */
QomposeStatusBar::~QomposeStatusBar()
{
}

/*!
 * This function displays a notification in this status bar widget.
 *
 * \param n The notification to display.
 */
void QomposeStatusBar::displayNotification(const QString &n)
{
	int l = n.trimmed().length();

	if(l > 0)
		notificationLabel->setFrameStyle(QFrame::StyledPanel |
			QFrame::Plain);
	else
		notificationLabel->setFrameStyle(QFrame::NoFrame |
			QFrame::Plain);

	notificationLabel->setText(n.trimmed());
}

/*!
 * This function sets the "current tab path" that we're displaying. This should
 * be the absolute path to the file which is open in the current tab, or an
 * empty string if no valid absolute path exists.
 *
 * \param p The current tab path to display.
 */
void QomposeStatusBar::setCurrentTabPath(const QString &p)
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
void QomposeStatusBar::setLine(int l)
{
	lineLabel->setText(QString("L %1").arg(l));
}

/*!
 * This function sets the column number we'll display in the status bar.
 *
 * \param c The current column number.
 */
void QomposeStatusBar::setColumn(int c)
{
	columnLabel->setText(QString("C %1").arg(c));
}
