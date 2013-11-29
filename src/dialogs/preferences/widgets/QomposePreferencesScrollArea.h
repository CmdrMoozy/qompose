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

#ifndef INCLUDE_QOMPOSE_PREFERENCES_SCROLL_AREA_H
#define INCLUDE_QOMPOSE_PREFERENCES_SCROLL_AREA_H

#include <QScrollArea>

class QShowEvent;
class QEvent;

/*!
 * \brief This class implements a scroll area for a preferences widget.
 */
class QomposePreferencesScrollArea : public QScrollArea
{
	public:
		QomposePreferencesScrollArea(QWidget *p = 0);
		virtual ~QomposePreferencesScrollArea();
		
		virtual void setWidget(QWidget *w);
		
	protected:
		virtual bool eventFilter(QObject *o, QEvent *e);
		virtual void showEvent(QShowEvent *e);
		
	private:
		void resizeFixedHorizontal();
};

#endif
