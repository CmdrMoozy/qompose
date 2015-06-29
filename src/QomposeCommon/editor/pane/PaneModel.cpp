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

#include "PaneModel.h"

#include "QomposeCommon/editor/Buffer.h"
#include "QomposeCommon/editor/pane/Pane.h"
#include "QomposeCommon/util/Paths.h"

namespace qompose
{
PaneModel::PaneModel(QObject *p) : QObject(p), panes()
{
}

std::size_t PaneModel::count() const
{
	return panes.size();
}

Pane *PaneModel::paneAt(std::size_t i) const
{
	if(i >= panes.size())
		return nullptr;
	return panes[i];
}

Pane *PaneModel::findPaneWithPath(const QString &p) const
{
	for(const auto &pane : panes)
	{
		if(p == pane->getBuffer()->getPath())
			return pane;
	}
	return nullptr;
}

std::vector<std::string> PaneModel::getOpenPaths() const
{
	std::vector<std::string> ret;
	for(const auto &pane : panes)
	{
		QString path = pane->getBuffer()->getPath();
		if(path.isEmpty())
			continue;
		ret.push_back(path.toStdString());
	}
	return ret;
}

QString PaneModel::getCommonParentPath() const
{
	return QString::fromStdString(
	        path_utils::getCommonParentPath(getOpenPaths()));
}
}
