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

#include <algorithm>
#include <iterator>
#include <utility>

#include <QDir>
#include <QFileInfo>

#include <bdrck/fs/Util.hpp>

#include "QomposeCommon/editor/Buffer.h"
#include "QomposeCommon/editor/pane/Pane.h"

namespace qompose
{
PaneModel::PaneModel(QObject *p) : QObject(p), panes(), closed()
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
	QFileInfo path(p);
	for(const auto &pane : panes)
	{
		QFileInfo panePath(pane->getBuffer()->getPath());
		if(path == panePath)
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
	        bdrck::fs::commonParentPath(getOpenPaths()));
}

QString PaneModel::getDefaultDirectory(Pane *current) const
{
	// If the current pane has a valid path, return it as-is.
	if(current != nullptr)
	{
		if(!current->getBuffer()->getDirectory().isNull())
			return current->getBuffer()->getDirectory();
	}

	auto currentIt = std::find(panes.begin(), panes.end(), current);
	if(currentIt == panes.end())
		currentIt = panes.begin();
	std::reverse_iterator<decltype(currentIt)> currentRevIt(currentIt),
	        currentRevEnd(panes.end());

	// See if any panes before the current pane have a valid path.
	for(; currentRevIt != currentRevEnd; ++currentRevIt)
	{
		if(!(*currentRevIt)->getBuffer()->getDirectory().isNull())
			return (*currentRevIt)->getBuffer()->getDirectory();
	}

	// See if any panes after the current pane have a valid path.
	for(; currentIt != panes.end(); ++currentIt)
	{
		if(!(*currentIt)->getBuffer()->getDirectory().isNull())
			return (*currentIt)->getBuffer()->getDirectory();
	}

	// Instead, just return a reasonable default value.
	return QDir::homePath();
}

void PaneModel::movePane(std::size_t f, std::size_t t)
{
	if(f == t)
		return;
	Pane *p = panes[f];

	int direction = f < t ? -1 : 1;
	for(std::size_t idx =
	            static_cast<std::size_t>(static_cast<int>(f) - direction);
	    idx != t;
	    idx = static_cast<std::size_t>(static_cast<int>(idx) + direction))
	{
		panes[static_cast<std::size_t>(static_cast<int>(idx) +
		                               direction)] = panes[idx];
	}

	panes[t] = p;
}

Pane *PaneModel::newPane()
{
	Pane *p = new Pane(nullptr);
	panes.push_back(p);
	Q_EMIT paneAdded(p, panes.size() - 1);
	return p;
}

Pane *PaneModel::openDescriptor(const FileDescriptor &d)
{
	// If we only have one "Untitled" buffer, we'll replace it.
	if(panes.size() == 1)
	{
		if(!panes[0]->getBuffer()->hasBeenSaved() &&
		   !panes[0]->getBuffer()->isModified())
		{
			removePane(static_cast<std::size_t>(0));
		}
	}

	// If the given file is already open, then stop.
	Pane *existing = findPaneWithPath(d.fileName);
	if(existing != nullptr)
		return existing;

	// Open the file descriptor.
	Pane *p = newPane();
	p->getBuffer()->open(d);
	return p;
}

Pane *PaneModel::reopen()
{
	if(closed.empty())
		return nullptr;

	ClosedBufferDescriptor d = closed.back();
	Pane *p = openDescriptor(d.file);
	closed.pop_back();

	QTextCursor curs = p->getBuffer()->textCursor();
	curs.setPosition(d.cursorPosition);
	p->getBuffer()->setTextCursor(curs);

	return p;
}

void PaneModel::removePane(std::size_t i)
{
	auto it = panes.begin();
	std::advance(it, i);

	ClosedBufferDescriptor descriptor = {
	        (*it)->getBuffer()->getFileDescriptor(),
	        (*it)->getBuffer()->textCursor().position()};

	panes.erase(it);
	Q_EMIT paneRemoved(i);

	closed.push_back(descriptor);
	while(closed.size() > 20)
		closed.pop_front();
}

void PaneModel::removePane(const Pane *p)
{
	auto it = std::find(panes.begin(), panes.end(), p);
	if(it == panes.end())
		return;
	removePane(static_cast<std::size_t>(std::distance(panes.begin(), it)));
}
}
