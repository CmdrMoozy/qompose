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

#include "QomposePreferencesWidget.h"

QomposePreferencesWidget::QomposePreferencesWidget(QWidget *p, Qt::WindowFlags f)
	: QWidget(p, f), icon(QIcon()), title(QString())
{
}

QomposePreferencesWidget::~QomposePreferencesWidget()
{
}

QIcon QomposePreferencesWidget::getPreferencesIcon() const
{
	return icon;
}

void QomposePreferencesWidget::setPreferencesIcon(const QIcon &i)
{
	icon = i;
}

QString QomposePreferencesWidget::getPreferencesTitle() const
{
	return title;
}

void QomposePreferencesWidget::setPreferencesTitle(const QString &t)
{
	title = t;
}
