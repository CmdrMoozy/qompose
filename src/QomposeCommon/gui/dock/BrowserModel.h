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

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#ifndef INCLUDE_QOMPOSECOMMON_GUI_DOCK_BROWSER_MODEL_H
#define INCLUDE_QOMPOSECOMMON_GUI_DOCK_BROWSER_MODEL_H

namespace qompose
{
class BrowserModel : public QAbstractItemModel
{
public:
	BrowserModel(QObject * = nullptr);
	virtual ~BrowserModel() = default;

	virtual QModelIndex index(int, int,
	                          const QModelIndex & = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &) const;
	virtual int rowCount(const QModelIndex & = QModelIndex()) const;
	virtual int columnCount(const QModelIndex & = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &, int = Qt::DisplayRole) const;
};
}

#endif
