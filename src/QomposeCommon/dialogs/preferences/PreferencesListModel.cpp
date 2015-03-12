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

#include "PreferencesListModel.h"

#include "QomposeCommon/Defines.h"
#include "QomposeCommon/dialogs/preferences/widgets/PreferencesScrollArea.h"
#include "QomposeCommon/dialogs/preferences/widgets/PreferencesWidget.h"

namespace qompose
{
PreferencesListModel::PreferencesListModel(QObject *p)
        : QAbstractListModel(p), widgets(), scrollWidgets()
{
}

PreferencesListModel::~PreferencesListModel()
{
	while(!scrollWidgets.isEmpty())
	{
		PreferencesScrollArea *w = scrollWidgets.takeLast();

		delete w;
	}
}

int PreferencesListModel::rowCount(const QModelIndex &QUNUSED(p)) const
{
	return widgets.count();
}

QVariant PreferencesListModel::data(const QModelIndex &i, int r) const
{
	int idx = i.row();

	if((idx < 0) || (idx >= widgets.count()))
		return QVariant(QVariant::Invalid);

	PreferencesWidget *widget = widgets.at(idx);

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

QVariant PreferencesListModel::headerData(int QUNUSED(s),
                                          Qt::Orientation QUNUSED(o),
                                          int QUNUSED(r)) const
{
	return QVariant(QVariant::Invalid);
}

PreferencesWidget *PreferencesListModel::widgetAt(int i) const
{
	if((i < 0) || (i >= rowCount()))
		return nullptr;

	return widgets.at(i);
}

PreferencesScrollArea *PreferencesListModel::scrollWidgetAt(int i) const
{
	if((i < 0) || (i >= rowCount()))
		return nullptr;

	return scrollWidgets.at(i);
}

void PreferencesListModel::addPreferencesWidget(PreferencesWidget *w)
{
	// Add the widget to our list.

	widgets.append(w);

	// Add a scroll area containing this widget to our list.

	PreferencesScrollArea *sw = new PreferencesScrollArea();

	sw->setWidget(w);

	scrollWidgets.append(sw);
}
}
