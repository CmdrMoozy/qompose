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

#include <catch/catch.hpp>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>

#include "QomposeCommon/fs/TemporaryStorage.h"

namespace
{
QString combinePaths(QString const &a, QString const &b)
{
	return QDir::cleanPath(a + QDir::separator() + b);
}

bool createFile(QString const &p)
{
	QFile f(p);
	if(!f.open(QIODevice::WriteOnly))
		return false;
	f.close();
	return true;
}

bool createDirectory(QString const &p)
{
	QDir d;
	return d.mkpath(p);
}
}

TEST_CASE("Test temporary file behavior", "[TemporaryStorage]")
{
	QString path;

	{
		qompose::TemporaryStorage file(
		        qompose::TemporaryStorageType::FILE);
		path = file.getPath();
		QFileInfo info(path);
		info.refresh();
		CHECK(info.exists());
		CHECK(info.isFile());
	}

	QFileInfo info(path);
	info.refresh();
	CHECK(!info.exists());
	CHECK(!info.isFile());
}

TEST_CASE("Test temporary directory behavior", "[TemporaryStorage]")
{
	QString path;

	{
		qompose::TemporaryStorage directory(
		        qompose::TemporaryStorageType::DIRECTORY);
		path = directory.getPath();
		QFileInfo info(path);
		info.refresh();
		CHECK(info.exists());
		CHECK(info.isDir());

		// Add some random files and directories to the directory, to
		// make sure the removal still works if it is non-empty.

		QString afile = combinePaths(path, "a.txt");
		QString bdir = combinePaths(path, "b");
		QString bdirafile = combinePaths(bdir, "a.txt");
		QString bdircdir = combinePaths(bdir, "c");
		QString bdircdirafile = combinePaths(bdircdir, "a.txt");

		CHECK(createFile(afile));
		CHECK(createDirectory(bdir));
		CHECK(createFile(bdirafile));
		CHECK(createDirectory(bdircdir));
		CHECK(createFile(bdircdirafile));
	}

	QFileInfo info(path);
	info.refresh();
	CHECK(!info.exists());
	CHECK(!info.isDir());
}
