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

#include "QomposeTabWidget.h"

#include <QKeyEvent>

/*!
 * This is our default constructor, which initializes a new tab widget with
 * the given parent widget.
 *
 * \param p Our new widget's parent.
 */
QomposeTabWidget::QomposeTabWidget(QWidget *p)
	: QTabWidget(p)
{
}

/*!
 * This is our default destructor, which cleans up & destroys our widget.
 */
QomposeTabWidget::~QomposeTabWidget()
{
}

/*!
 * This function handles a key press event by deciding if it's one of the
 * keystrokes we consider a hotkey, and proceeds accordingly.
 *
 * \param e The event being handled.
 */
void QomposeTabWidget::keyPressEvent(QKeyEvent *e)
{
	bool processed = false;
	
	if(e->modifiers() == (Qt::ControlModifier | Qt::AltModifier))
	{
		if( (e->key() == Qt::Key_Left) || (e->key() == Qt::Key_Right) )
		{
			qDebug("Got CTRL+SHIFT+(L or R)");
			processed = true;
			e->ignore();
		}
	}
	
	if(!processed)
		QTabWidget::keyPressEvent(e);
}
