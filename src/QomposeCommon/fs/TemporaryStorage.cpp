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

#include "TemporaryStorage.h"

#include <cassert>
#include <stdexcept>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QUuid>

namespace
{
QString combinePaths(QString const &a, QString const &b)
{
	return QDir::cleanPath(a + QDir::separator() + b);
}
}

namespace qompose
{
TemporaryStorage::TemporaryStorage(TemporaryStorageType t) : type(t), path()
{
	QFileInfo info;

	do
	{
		path = combinePaths(QDir::tempPath(),
		                    QString("qompose-") +
		                            QUuid::createUuid().toString() +
		                            QString(".tmp"));
		info.setFile(path);
	} while(info.exists());

	switch(type)
	{
	case TemporaryStorageType::FILE:
	{
		QFile f(path);
		if(!f.open(QIODevice::WriteOnly))
		{
			throw std::runtime_error(
			        "Creating temporary file failed.");
		}
		f.close();
	}
	break;

	case TemporaryStorageType::DIRECTORY:
	{
		QDir d;
		if(!d.mkpath(path))
		{
			throw std::runtime_error(
			        "Creating temporary directory failed.");
		}
	}
	break;
	}
}

TemporaryStorage::~TemporaryStorage()
{
	switch(type)
	{
	case TemporaryStorageType::FILE:
		QFile::remove(path);
		break;

	case TemporaryStorageType::DIRECTORY:
	{
		QDir d(path);
		d.removeRecursively();
	}
	break;
	}
}

QString TemporaryStorage::getPath() const
{
	return path;
}
}
