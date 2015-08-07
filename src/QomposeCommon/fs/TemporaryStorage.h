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

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_TEMPORARY_STORAGE_H
#define INCLUDE_QOMPOSECOMMON_UTIL_TEMPORARY_STORAGE_H

#include <QString>

namespace qompose
{
enum class TemporaryStorageType
{
	FILE,
	DIRECTORY
};

class TemporaryStorage
{
public:
	TemporaryStorage(TemporaryStorageType type);

	TemporaryStorage(TemporaryStorage const &) = delete;
	TemporaryStorage(TemporaryStorage &&) = default;
	TemporaryStorage &operator=(TemporaryStorage const &) = delete;
	TemporaryStorage &operator=(TemporaryStorage &&) = default;

	~TemporaryStorage();

	QString getPath() const;

private:
	TemporaryStorageType type;
	QString path;
};
}

#endif
