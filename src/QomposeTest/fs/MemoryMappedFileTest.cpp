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

#include <boost/optional/optional.hpp>

#include <QFileInfo>

#include <bdrck/fs/TemporaryStorage.hpp>

#include "QomposeCommon/fs/MemoryMappedFile.h"

TEST_CASE("Memory mapping an empty file should work as expected",
          "[MemoryMappedFile]")
{
	bdrck::fs::TemporaryStorage file(bdrck::fs::TemporaryStorageType::FILE);
	QFileInfo info(QString::fromStdString(file.getPath()));
	CHECK(info.exists());
	CHECK(info.isFile());
	CHECK(info.size() == 0);

	boost::optional<qompose::MemoryMappedFile> memoryFile;
	CHECK_NOTHROW(memoryFile.emplace(file.getPath()));
	CHECK(nullptr == memoryFile->getData());
	CHECK(0 == memoryFile->getLength());
}
