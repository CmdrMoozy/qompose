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

#include "BrowserModel.h"

namespace qompose
{
BrowserModel::BrowserModel(QObject *p) : QAbstractItemModel(p)
{
}

QModelIndex BrowserModel::index(int row, int column, const QModelIndex &) const
{
	return createIndex(row, column);
}

QModelIndex BrowserModel::parent(const QModelIndex &idx) const
{
	return idx.parent();
}

int BrowserModel::rowCount(const QModelIndex &) const
{
	return 0;
}

int BrowserModel::columnCount(const QModelIndex &) const
{
	return 0;
}

QVariant BrowserModel::data(const QModelIndex &, int) const
{
	return QVariant();
}
}
