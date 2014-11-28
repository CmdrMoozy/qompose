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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_STATUS_BAR_H
#define INCLUDE_QOMPOSECOMMON_GUI_STATUS_BAR_H

#include <QStatusBar>

class QGridLayout;
class QLabel;

namespace qompose
{

class NotificationLabel;

/*!
 * \brief This class implements a status bar widget for our application.
 */
class StatusBar : public QStatusBar
{
	Q_OBJECT

public:
	StatusBar(QWidget * = nullptr);
	virtual ~StatusBar();

	void displayNotification(const QString &, bool = false);

	void setCurrentTabPath(const QString &);

	void setLine(int);
	void setColumn(int);

private:
	QWidget *statusWidget;
	QGridLayout *statusLayout;

	NotificationLabel *notificationLabel;
	QLabel *tabPathLabel;
	QLabel *lineLabel;
	QLabel *columnLabel;

	StatusBar(const StatusBar &);
	StatusBar &operator=(const StatusBar &);
};

}

#endif
