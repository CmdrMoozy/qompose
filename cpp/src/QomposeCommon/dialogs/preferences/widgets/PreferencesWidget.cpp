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

#include "PreferencesWidget.h"

namespace qompose
{
PreferencesWidget::PreferencesWidget(QWidget *p, Qt::WindowFlags f)
        : QWidget(p, f), icon(QIcon()), title(QString())
{
}

PreferencesWidget::~PreferencesWidget()
{
}

QIcon PreferencesWidget::getPreferencesIcon() const
{
	return icon;
}

void PreferencesWidget::setPreferencesIcon(const QIcon &i)
{
	icon = i;
}

QString PreferencesWidget::getPreferencesTitle() const
{
	return title;
}

void PreferencesWidget::setPreferencesTitle(const QString &t)
{
	title = t;
}
}
