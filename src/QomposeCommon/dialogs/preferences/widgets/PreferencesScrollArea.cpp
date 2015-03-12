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

#include "PreferencesScrollArea.h"

#include <QShowEvent>
#include <QScrollBar>
#include <QLayout>
#include <QEvent>

namespace qompose
{
PreferencesScrollArea::PreferencesScrollArea(QWidget *p) : QScrollArea(p)
{
	setWidgetResizable(true);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void PreferencesScrollArea::setWidget(QWidget *w)
{
	QScrollArea::setWidget(w);

	if(isVisible())
		resizeFixedHorizontal();
}

bool PreferencesScrollArea::eventFilter(QObject *o, QEvent *e)
{
	if((o == widget()) && (e->type() == QEvent::Resize))
	{
		resizeFixedHorizontal();
		return true;
	}
	else
	{
		return QScrollArea::eventFilter(o, e);
	}
}

void PreferencesScrollArea::showEvent(QShowEvent *e)
{
	QScrollArea::showEvent(e);

	resizeFixedHorizontal();
}

void PreferencesScrollArea::resizeFixedHorizontal()
{
	// Get our widget's size.

	QWidget *w = widget();

	if(w == nullptr)
		return;

	int ww = w->minimumSizeHint().width();

	// Get our scroll bar's size.

	QScrollBar *vs = verticalScrollBar();

	int sw = vs->width();

	// Set our fixed width.

	setMinimumWidth(ww + sw);
}
}
