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
class EllipsizedLabel;
class NotificationLabel;

/*!
 * \brief This class implements a status bar widget for our application.
 */
class StatusBar : public QStatusBar
{
	Q_OBJECT

public:
	/*!
	 * This is our default constructor, which creates a new instance of our
	 * status bar widget.
	 *
	 * \param p Our status bar's parent widget.
	 */
	StatusBar(QWidget *p = nullptr);

	StatusBar(const StatusBar &) = delete;
	virtual ~StatusBar() = default;

	StatusBar &operator=(const StatusBar &) = delete;

	/*!
	 * This function displays a notification in this status bar widget.
	 *
	 * \param n The notification to display.
	 * \param c Whether or not the notification is critical.
	 */
	void displayNotification(const QString &n, bool c = false);

	/*!
	 * This function sets the "current tab path" that we're displaying.
	 * This should be the absolute path to the file which is open in the
	 * current tab, or an empty string if no valid absolute path exists.
	 *
	 * \param p The current tab path to display.
	 */
	void setCurrentTabPath(const QString &p);

	/*!
	 * This function sets the line number we'll display in the status bar.
	 *
	 * \param l The current line number.
	 */
	void setLine(int l);

	/*!
	 * This function sets the column number we'll display in the status
	 * bar.
	 *
	 * \param c The current column number.
	 */
	void setColumn(int c);

private:
	QWidget *statusWidget;
	QGridLayout *statusLayout;

	NotificationLabel *notificationLabel;
	EllipsizedLabel *tabPathLabel;
	QLabel *lineLabel;
	QLabel *columnLabel;
};
}

#endif
