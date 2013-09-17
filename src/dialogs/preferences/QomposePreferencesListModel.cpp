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

#include "QomposeDefines.h"
#include "dialogs/preferences/widgets/QomposePreferencesWidget.h"

QomposePreferencesListModel::QomposePreferencesListModel(QObject *p)
	: QAbstractListModel(p)
{
}

QomposePreferencesListModel::~QomposePreferencesListModel()
{
	while(!widgets.empty())
	{
		QomposePreferencesWidget *w = widgets.takeFirst();
		delete w;
	}
}

int QomposePreferencesListModel::rowCount(const QModelIndex &QUNUSED(p)) const
{
	return widgets.count();
}

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
	
	
	return QVariant(QVariant::Invalid);
}

QVariant QomposePreferencesListModel::headerData(int QUNUSED(s),
	Qt::Orientation QUNUSED(o), int QUNUSED(r)) const
{
	return QVariant(QVariant::Invalid);
}

const QomposePreferencesWidget *QomposePreferencesListModel::widgetAt(int i) const
{
	if( (i < 0) || (i >= rowCount()) )
		return NULL;
	
	return widgets.at(i);
}

void QomposePreferencesListModel::addPreferencesWidget(QomposePreferencesWidget *w)
{
	widgets.append(w);
}
