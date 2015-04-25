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

#include "RepositoryWatcher.h"

#include <cassert>

#include <QFileSystemWatcher>

#include "QomposeCommon/git/Utils.h"

namespace qompose
{
namespace git
{
RepositoryWatcher::RepositoryWatcher(QObject *p)
        : QObject(p), watched(), watcher(new QFileSystemWatcher(this))
{
	QObject::connect(watcher, SIGNAL(directoryChanged(const QString &)),
	                 this, SIGNAL(repositoryChanged(const QString &)));
}

bool RepositoryWatcher::addPath(const std::string &p)
{
	std::string path = git_utils::discoverRepository(p);
	if(watched.find(path) != watched.end())
		return false;

	auto it =
	        (watched.insert(std::make_pair(path, Repository(path)))).first;
	assert(watcher->addPath(
	        QString::fromStdString(it->second.getWorkDirectory())));
	return true;
}

bool RepositoryWatcher::removePath(const std::string &p)
{
	std::string path = git_utils::discoverRepository(p);
	auto it = watched.find(path);
	if(it == watched.end())
		return false;

	watcher->removePath(
	        QString::fromStdString(it->second.getWorkDirectory()));
	watched.erase(it);
	return true;
}
}
}
