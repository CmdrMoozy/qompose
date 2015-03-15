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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_NOTIFICATION_LABEL_H
#define INCLUDE_QOMPOSECOMMON_GUI_NOTIFICATION_LABEL_H

#include <QLabel>
#include <QColor>

namespace qompose
{
/*!
 * \brief This class implements a Label suitable for status bar notifications.
 */
class NotificationLabel : public QLabel
{
	Q_OBJECT
	Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)

public:
	/*!
	 * This is our default constructor, which creates a new instance of our
	 * notification label.
	 *
	 * \param p The parent widget for this label.
	 * \param f The window flags for this label.
	 */
	NotificationLabel(QWidget *p = nullptr, Qt::WindowFlags f = nullptr);

	NotificationLabel(const NotificationLabel &) = delete;
	virtual ~NotificationLabel() = default;

	NotificationLabel &operator=(const NotificationLabel &) = delete;

	/*!
	 * This function returns the color this label's text is currently being
	 * displayed in.
	 *
	 * \return This label's current text color.
	 */
	QColor getTextColor() const;

	/*!
	 * This function sets the color this label's text will be displayed in.
	 *
	 * \param c This label's new text color.
	 */
	void setTextColor(const QColor &c);

	/*!
	 * This function displays a new notification string in this label.
	 *
	 * \param n The notification to display.
	 * \param c Whether or not the notification is critical.
	 * \param d The duration to display the notification for.
	 */
	void displayNotification(const QString &n, bool c = false, int d = 0);

private:
	QColor defaultColor;
};
}

#endif
