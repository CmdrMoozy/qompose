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

#include "BrowserView.h"

#include <QGridLayout>
#include <QTreeView>

namespace qompose
{
BrowserView::BrowserView(QWidget *p, Qt::WindowFlags f)
        : QWidget(p, f), layout(nullptr), treeView(nullptr)
{
	layout = new QGridLayout(this);

	treeView = new QTreeView(this);

	layout->addWidget(treeView, 0, 0, 1, 1);
	setLayout(layout);
}
}
