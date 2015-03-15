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

#include "NotificationLabel.h"

#include <cmath>

#include <QPalette>
#include <QPropertyAnimation>

namespace qompose
{
NotificationLabel::NotificationLabel(QWidget *p, Qt::WindowFlags f)
        : QLabel(p, f), defaultColor(QColor())
{
	defaultColor = getTextColor();
}

QColor NotificationLabel::getTextColor() const
{
	QPalette p = palette();

	return p.color(foregroundRole());
}

void NotificationLabel::setTextColor(const QColor &c)
{
	QPalette p = palette();

	p.setColor(foregroundRole(), c);

	setPalette(p);
}

void NotificationLabel::displayNotification(const QString &n, bool c, int d)
{
	setText(n.trimmed());

	if(d <= 0)
	{
		double duration = log(static_cast<double>(n.length()));
		duration /= log(10);

		duration = qMax(duration, 1.0) * 2000.0;

		d = qRound(duration);
	}

	QPropertyAnimation *anim =
	        new QPropertyAnimation(this, "textColor", this);

	QColor transparent(defaultColor);
	transparent.setAlpha(0);

	if(c)
	{
		QColor critical(255, 0, 0);

		anim->setKeyValueAt(0, critical);
		anim->setKeyValueAt(1, transparent);

		anim->setDuration(d);
	}
	else
	{
		anim->setKeyValueAt(0, defaultColor);
		anim->setKeyValueAt(1, transparent);

		anim->setDuration(d);
	}

	anim->start(QAbstractAnimation::DeleteWhenStopped);
}
}
