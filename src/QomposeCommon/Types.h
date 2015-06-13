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

#ifndef INCLUDE_QOMPOSECOMMON_TYPES_H
#define INCLUDE_QOMPOSECOMMON_TYPES_H

#include <QString>

namespace qompose
{
/*!
 * \brief This enumeration denotes an editor indentation mode.
 */
enum class IndentationMode
{
	Tabs,
	Spaces
};

/*!
 * \brief This structure stores a filename and its detected character encoding.
 */
struct FileDescriptor
{
	QString fileName;
	QString textCodec;

	FileDescriptor();
	FileDescriptor(const QString &f, const QString &c);

	FileDescriptor(const FileDescriptor &) = default;
	FileDescriptor &operator=(const FileDescriptor &) = default;
};

/*!
 * \brief This structure stores closed buffer state, so it can be reopened.
 */
struct ClosedBufferDescriptor
{
	FileDescriptor file;
	int cursorPosition;

	ClosedBufferDescriptor();
	ClosedBufferDescriptor(FileDescriptor f, int c);

	ClosedBufferDescriptor(const ClosedBufferDescriptor &) = default;
	ClosedBufferDescriptor &
	operator=(const ClosedBufferDescriptor &) = default;
};
}

#endif
