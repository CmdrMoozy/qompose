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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_PREFERENCES_LIST_VIEW_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_PREFERENCES_LIST_VIEW_H

#include <QListView>
#include <QSize>

class QAbstractItemModel;

namespace qompose
{
class PreferencesListModel;

/*!
 * \brief This class implements a view for a list of preferences widgets.
 */
class PreferencesListView : public QListView
{
public:
	/*!
	 * This is our default constructor, which creates a new preferences
	 * list view.
	 *
	 * \param p The parent widget to use for this widget.
	 */
	PreferencesListView(QWidget *p = nullptr);

	virtual ~PreferencesListView() = default;

	/*!
	 * This function sets the preferences list model our view should be
	 * using.
	 *
	 * \param m The new model for our view to use.
	 */
	void setModel(PreferencesListModel *m);

private:
	/*!
	 * We override our superclass's setModel() function to make it private,
	 * so we can ensure any model we are given is a PreferencesListModel.
	 *
	 * \param m The new model to use.
	 */
	virtual void setModel(QAbstractItemModel *m);
};
}

#endif
