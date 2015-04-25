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

#ifndef INCLUDE_QOMPOSECOMMON_GIT_REPOSITORY_WATCHER_H
#define INCLUDE_QOMPOSECOMMON_GIT_REPOSITORY_WATCHER_H

#include <map>
#include <string>

#include <QObject>
#include <QString>

#include "QomposeCommon/git/Repository.h"

class QFileSystemWatcher;

namespace qompose
{
namespace git
{
class RepositoryWatcher : public QObject
{
	Q_OBJECT

public:
	RepositoryWatcher(QObject *p = nullptr);

	bool addPath(const std::string &p);
	bool removePath(const std::string &p);

private:
	std::map<std::string, Repository> watched;
	QFileSystemWatcher *watcher;

Q_SIGNALS:
	void repositoryChanged(const QString &);
};
}
}

#endif
