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

#include "QomposePreferencesListView.h"



#include "dialogs/preferences/QomposePreferencesListModel.h"
#include "dialogs/preferences/widgets/QomposePreferencesWidget.h"

/*!
 * This is our default constructor, which creates a new preferences list view.
 *
 * \param p The parent widget to use for this widget.
 */
QomposePreferencesListView::QomposePreferencesListView(QWidget *p)
	: QListView(p)
{
	setIconSize(QSize(30, 30));
	setSpacing(5);

	QFont f = font();
	f.setPointSize(10);
	setFont(f);
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
QomposePreferencesListView::~QomposePreferencesListView()
{
}

/*!
 * This function sets the preferences list model our view should be using.
 *
 * \param m The new model for our view to use.
 */
void QomposePreferencesListView::setModel(QomposePreferencesListModel *m)
{
	QAbstractItemModel *aim = dynamic_cast<QAbstractItemModel *>(m);

	setModel(aim);
}

/*!
 * We override our superclass's setModel() function to make it private,
 * so we can ensure any model we are given is a QomposePreferencesListModel.
 *
 * \param m The new model to use.
 */
void QomposePreferencesListView::setModel(QAbstractItemModel *m)
{
	QListView::setModel(m);
}
