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

#ifndef INCLUDE_QOMPOSE_NOTIFICATION_LABEL_H
#define INCLUDE_QOMPOSE_NOTIFICATION_LABEL_H

#include <QLabel>
#include <QColor>

/*!
 * \brief This class implements a Label suitable for status bar notifications.
 */
class QomposeNotificationLabel : public QLabel
{
	Q_OBJECT
	Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)

	public:
		QomposeNotificationLabel(QWidget *p = 0,
			Qt::WindowFlags f = 0);
		virtual ~QomposeNotificationLabel();

		QColor getTextColor() const;
		void setTextColor(const QColor &c);

		void displayNotification(const QString &n, bool c = false,
			int d = 0);

	private:
		QColor defaultColor;
};

#endif
