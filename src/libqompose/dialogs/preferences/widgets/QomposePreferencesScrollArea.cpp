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

#include "QomposePreferencesScrollArea.h"

#include <QShowEvent>
#include <QScrollBar>
#include <QLayout>
#include <QEvent>

/*!
 * This is our default constructor, which creates a new scroll area with the
 * given widget.
 *
 * \param p The widget this scroll area will be displaying.
 */
QomposePreferencesScrollArea::QomposePreferencesScrollArea(QWidget *p)
	: QScrollArea(p)
{
	setWidgetResizable(true);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

/*!
 * This is our default destructor, which cleans up and destroys our scroll
 * area.
 */
QomposePreferencesScrollArea::~QomposePreferencesScrollArea()
{
}

/*!
 * This function sets the widget this scroll area will be displaying.
 *
 * \param w The new widget to display.
 */
void QomposePreferencesScrollArea::setWidget(QWidget *w)
{
	QScrollArea::setWidget(w);

	if(isVisible())
		resizeFixedHorizontal();
}

/*!
 * We filter events to catch resize events from the widget we're displaying,
 * to update the size of our scroll area to prevent any horizontal scrolling
 * from being neccessary.
 *
 * \param o The object whose event is being filtered.
 * \param e The event being handled.
 * \return True if the event was handled, or false otherwise.
 */
bool QomposePreferencesScrollArea::eventFilter(QObject *o, QEvent *e)
{
	if( (o == widget()) && (e->type() == QEvent::Resize) )
	{
		resizeFixedHorizontal();
		return true;
	}
	else
	{
		return QScrollArea::eventFilter(o, e);
	}
}

/*!
 * We handle our superclass's show event to resize our scroll area to prevent
 * horizontal scrolling from being neccessary. Since we do this on resize
 * events too, this is probably unneeded, but it doesn't hurt anything.
 *
 * \param e The event being handled.
 */
void QomposePreferencesScrollArea::showEvent(QShowEvent *e)
{
	QScrollArea::showEvent(e);

	resizeFixedHorizontal();
}

/*!
 * This is a utility function which updates our scroll area's minimum width
 * to be the sum of the width of our vertical scrollbar, and the minimum size
 * hint width of the widget we're displaying.
 *
 * This makes our scroll area always as wide as the widget we're displaying,
 * so no horizontal scrolling is necessary.
 */
void QomposePreferencesScrollArea::resizeFixedHorizontal()
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
