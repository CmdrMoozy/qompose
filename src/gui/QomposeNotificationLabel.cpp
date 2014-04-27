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

#include "QomposeNotificationLabel.h"

#include <QPalette>

/*!
 * This is our default constructor, which creates a new instance of our
 * notification label.
 *
 * \param p The parent widget for this label.
 * \param f The window flags for this label.
 */
QomposeNotificationLabel::QomposeNotificationLabel(
	QWidget *p, Qt::WindowFlags f)
	: QLabel(p, f)
{
}

/*!
 * This is our default destructor, which cleans up and destroys this
 * notification label instance.
 */
QomposeNotificationLabel::~QomposeNotificationLabel()
{
}

/*!
 * This function returns the color this label's text is currently being
 * displayed in.
 *
 * \return This label's current text color.
 */
QColor QomposeNotificationLabel::getTextColor() const
{
	QPalette p = palette();

	return p.color(QPalette::Normal, QPalette::Window);
}

/*!
 * This function sets the color this label's text will be displayed in.
 *
 * \param c This label's new text color.
 */
void QomposeNotificationLabel::setTextColor(const QColor &c)
{
	QPalette p = palette();

	p.setBrush(QPalette::WindowText, c);

	setPalette(p);
}

void QomposeNotificationLabel::displayNotification(const QString &n)
{
	setText(n.trimmed());
}
