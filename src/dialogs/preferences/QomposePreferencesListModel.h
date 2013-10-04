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

#ifndef INCLUDE_QOMPOSE_PREFERENCES_LIST_MODEL_H
#define INCLUDE_QOMPOSE_PREFERENCES_LIST_MODEL_H

#include <QAbstractListModel>

#include <QModelIndex>
#include <QVariant>
#include <QList>

class QomposePreferencesWidget;

/*!
 * \brief This class implements a model to store a list of preferences widgets.
 */
class QomposePreferencesListModel : public QAbstractListModel
{
	public:
		QomposePreferencesListModel(QObject *p = 0);
		virtual ~QomposePreferencesListModel();
		
		virtual int rowCount(const QModelIndex &p = QModelIndex()) const;
		virtual QVariant data(const QModelIndex &i, int r = Qt::DisplayRole) const;
		virtual QVariant headerData(int s, Qt::Orientation o, int r = Qt::DisplayRole) const;
		
		QomposePreferencesWidget *widgetAt(int i) const;
		
		void addPreferencesWidget(QomposePreferencesWidget *w);
		
	private:
		QList<QomposePreferencesWidget *> widgets;
};

#endif
