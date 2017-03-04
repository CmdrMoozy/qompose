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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_WIDGETS_PREFERENCES_SCROLL_AREA_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_WIDGETS_PREFERENCES_SCROLL_AREA_H

#include <QScrollArea>

class QShowEvent;
class QEvent;

namespace qompose
{
/*!
 * \brief This class implements a scroll area for a preferences widget.
 */
class PreferencesScrollArea : public QScrollArea
{
public:
	/*!
	 * This is our default constructor, which creates a new scroll area
	 * with the given widget.
	 *
	 * \param p The widget this scroll area will be displaying.
	 */
	PreferencesScrollArea(QWidget *p = nullptr);

	virtual ~PreferencesScrollArea() = default;

	/*!
	 * This function sets the widget this scroll area will be displaying.
	 *
	 * \param w The new widget to display.
	 */
	virtual void setWidget(QWidget *w);

protected:
	/*!
	 * We filter events to catch resize events from the widget we're
	 * displaying, to update the size of our scroll area to prevent any
	 * horizontal scrolling from being neccessary.
	 *
	 * \param o The object whose event is being filtered.
	 * \param e The event being handled.
	 * \return True if the event was handled, or false otherwise.
	 */
	virtual bool eventFilter(QObject *o, QEvent *e);

	/*!
	 * We handle our superclass's show event to resize our scroll area to
	 * prevent horizontal scrolling from being neccessary. Since we do this
	 * on resize events too, this is probably unneeded, but it doesn't hurt
	 * anything.
	 *
	 * \param e The event being handled.
	 */
	virtual void showEvent(QShowEvent *e);

private:
	/*!
	 * This is a utility function which updates our scroll area's minimum
	 * width to be the sum of the width of our vertical scrollbar, and the
	 * minimum size hint width of the widget we're displaying.
	 *
	 * This makes our scroll area always as wide as the widget we're
	 * displaying, so no horizontal scrolling is necessary.
	 */
	void resizeFixedHorizontal();
};
}

#endif
