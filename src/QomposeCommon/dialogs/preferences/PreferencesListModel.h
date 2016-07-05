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

#ifndef INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_PREFERENCES_LIST_MODEL_H
#define INCLUDE_QOMPOSECOMMON_DIALOGS_PREFERENCES_PREFERENCES_LIST_MODEL_H

#include <QAbstractListModel>

#include <QList>
#include <QModelIndex>
#include <QVariant>

namespace qompose
{
class PreferencesWidget;
class PreferencesScrollArea;

/*!
 * \brief This class implements a model to store a list of preferences widgets.
 */
class PreferencesListModel : public QAbstractListModel
{
public:
	/*!
	 * This is our default constructor, which creates a new, empty
	 * preferences list model.
	 *
	 * \param p The parent object for this new model.
	 */
	PreferencesListModel(QObject *p = nullptr);

	PreferencesListModel(const PreferencesListModel &) = delete;

	/*!
	 * This is our default destructor, which cleans up and destroys our
	 * list.
	 */
	virtual ~PreferencesListModel();

	PreferencesListModel &operator=(const PreferencesListModel &) = delete;

	/*!
	 * This function returns the number of rows currently in our model.
	 *
	 * \param p The parent model index - this is ignored.
	 * \return The number of rows currently in our model.
	 */
	virtual int rowCount(const QModelIndex &p = QModelIndex()) const;

	/*!
	 * This function returns the data for the given model index and role.
	 * We will return valid data for a display role (the widget title) as
	 * well as the decoration role (the widget icon).
	 *
	 * \param i The model index (i.e., the row) to get data for.
	 * \param r The role to get data for.
	 * \return The data matching the given row and role.
	 */
	virtual QVariant data(const QModelIndex &i,
	                      int r = Qt::DisplayRole) const;

	/*!
	 * This function returns an invalid QVariant, since out model doesn't
	 * keep track of any "header data."
	 *
	 * \param s The section number of the desired data (ignored).
	 * \param o The orientation the data will be displayed in (ignored).
	 * \param r The role of the desired data (ignored).
	 * \return An invalid QVariant, since we do not store any header data.
	 */
	virtual QVariant headerData(int s, Qt::Orientation o,
	                            int r = Qt::DisplayRole) const;

	/*!
	 * This is a utility function which returns the preferences widget on
	 * the given row of our model. If the given row number is
	 * out-of-bounds, then this function will return nullptr instead.
	 *
	 * \param i The row of the desired widget.
	 * \return The desired preferences widget.
	 */
	PreferencesWidget *widgetAt(int i) const;

	/*!
	 * This is a utility function which returns the preferences widget on
	 * the given row of our model, encapsulated in a scroll area. If the
	 * given row number is out-of-bounds, then this function will return
	 * nullptr instead.
	 *
	 * \param i The row of the desired widget.
	 * \return The desired preferences widget, encapsulated in a scroll
	 *area.
	 */
	PreferencesScrollArea *scrollWidgetAt(int i) const;

	/*!
	 * This function appends the given preferences widget to our model's
	 * list of widgets.
	 *
	 * \param w The widget to add to our model.
	 */
	void addPreferencesWidget(PreferencesWidget *w);

private:
	QList<PreferencesWidget *> widgets;
	QList<PreferencesScrollArea *> scrollWidgets;
};
}

#endif
