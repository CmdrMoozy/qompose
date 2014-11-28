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

#include "QomposePreferencesListModel.h"

#include "QomposeCommon/Defines.h"
#include "QomposeCommon/dialogs/preferences/widgets/QomposePreferencesScrollArea.h"
#include "QomposeCommon/dialogs/preferences/widgets/QomposePreferencesWidget.h"

/*!
 * This is our default constructor, which creates a new, empty preferences
 * list model.
 *
 * \param p The parent object for this new model.
 */
QomposePreferencesListModel::QomposePreferencesListModel(QObject *p)
	: QAbstractListModel(p),
		widgets(QList<QomposePreferencesWidget *>()),
		scrollWidgets(QList<QomposePreferencesScrollArea *>())
{
}

/*!
 * This is our default destructor, which cleans up and destroys our list.
 */
QomposePreferencesListModel::~QomposePreferencesListModel()
{
	while(!scrollWidgets.isEmpty())
	{
		QomposePreferencesScrollArea *w = scrollWidgets.takeLast();

		delete w;
	}
}

/*!
 * This function returns the number of rows currently in our model.
 *
 * \param p The parent model index - this is ignored.
 * \return The number of rows currently in our model.
 */
int QomposePreferencesListModel::rowCount(const QModelIndex &QUNUSED(p)) const
{
	return widgets.count();
}

/*!
 * This function returns the data for the given model index and role. We will
 * return valid data for a display role (the widget title) as well as the
 * decoration role (the widget icon).
 *
 * \param i The model index (i.e., the row - the column is ignored) to get data for.
 * \param r The role to get data for.
 * \return The data matching the given row and role.
 */
QVariant QomposePreferencesListModel::data(const QModelIndex &i, int r) const
{
	int idx = i.row();

	if( (idx < 0) || (idx >= widgets.count()) )
		return QVariant(QVariant::Invalid);

	QomposePreferencesWidget *widget = widgets.at(idx);

	switch(r)
	{
		case Qt::DisplayRole:
			return widget->getPreferencesTitle();
			break;

		case Qt::DecorationRole:
			return widget->getPreferencesIcon();
			break;

		default:
			return QVariant(QVariant::Invalid);
			break;
	};
}

/*!
 * This function returns an invalid QVariant, since out model doesn't keep track
 * of any "header data."
 *
 * \param s The section number of the desired data - this is ignored.
 * \param o The orientation the data will be displayed in - this is ignored.
 * \param r The role of the desired data - this is ignored.
 * \return An invalid QVariant, since we do not store any header data.
 */
QVariant QomposePreferencesListModel::headerData(int QUNUSED(s),
	Qt::Orientation QUNUSED(o), int QUNUSED(r)) const
{
	return QVariant(QVariant::Invalid);
}

/*!
 * This is a utility function which returns the preferences widget on the given
 * row of our model. If the given row number is out-of-bounds, then this function
 * will return NULL instead.
 *
 * \param i The row of the desired widget.
 * \return The desired preferences widget.
 */
QomposePreferencesWidget *QomposePreferencesListModel::widgetAt(int i) const
{
	if( (i < 0) || (i >= rowCount()) )
		return NULL;

	return widgets.at(i);
}

/*!
 * This is a utility function which returns the preferences widget on the given row
 * of our model, encapsulated in a scroll area. If the given row number is out-of-bounds,
 * then this function will return NULL instead.
 *
 * \param i The row of the desired widget.
 * \return The desired preferences widget, encapsulated in a scroll area.
 */
QomposePreferencesScrollArea *QomposePreferencesListModel::scrollWidgetAt(int i) const
{
	if( (i < 0) || (i >= rowCount()) )
		return NULL;

	return scrollWidgets.at(i);
}

/*!
 * This function appends the given preferences widget to our model's list of widgets.
 *
 * \param w The widget to add to our model.
 */
void QomposePreferencesListModel::addPreferencesWidget(QomposePreferencesWidget *w)
{
	// Add the widget to our list.

	widgets.append(w);

	// Add a scroll area containing this widget to our list.

	QomposePreferencesScrollArea *sw = new QomposePreferencesScrollArea();

	sw->setWidget(w);

	scrollWidgets.append(sw);
}
