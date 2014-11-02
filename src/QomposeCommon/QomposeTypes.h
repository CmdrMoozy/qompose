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

#ifndef INCLUDE_QOMPOSE_TYPES_H
#define INCLUDE_QOMPOSE_TYPES_H

/*!
 * \brief This structure stores a filename and its detected character encoding.
 */
struct QomposeFileDescriptor
{
	QString fileName;
	QString textCodec;

	QomposeFileDescriptor()
		: fileName(QString()), textCodec(QString())
	{
	}

	QomposeFileDescriptor(const QString &f, const QString &c)
		: fileName(f), textCodec(c)
	{
	}

	QomposeFileDescriptor(const QomposeFileDescriptor &o)
		: fileName(QString()), textCodec(QString())
	{
		*this = o;
	}

	QomposeFileDescriptor &operator=(const QomposeFileDescriptor &o)
	{
		if(&o == this)
			return *this;

		fileName = o.fileName;
		textCodec = o.textCodec;

		return *this;
	}
};

/*!
 * \brief This structure stores closed buffer state, so it can be reopened.
 */
struct QomposeClosedBufferDescriptor
{
	QomposeFileDescriptor file;
	int cursorPosition;

	QomposeClosedBufferDescriptor()
		: file(QomposeFileDescriptor()), cursorPosition(0)
	{
	}

	QomposeClosedBufferDescriptor(QomposeFileDescriptor f, int c)
		: file(f), cursorPosition(c)
	{
	}

	QomposeClosedBufferDescriptor(const QomposeClosedBufferDescriptor &o)
		: file(QomposeFileDescriptor()),
			cursorPosition(0)
	{
		*this = o;
	}

	QomposeClosedBufferDescriptor &operator=(
		const QomposeClosedBufferDescriptor &o)
	{
		if(&o == this)
			return *this;

		file = o.file;
		cursorPosition = o.cursorPosition;

		return *this;
	}
};

#endif
