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
	PreferencesListView(QWidget *p = nullptr);
	virtual ~PreferencesListView();

	void setModel(PreferencesListModel *m);

private:
	virtual void setModel(QAbstractItemModel *m);
};
}

#endif
