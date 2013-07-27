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

#include "QomposeBuffer.h"

QomposeBuffer::QomposeBuffer(QWidget *p)
	: QomposeEditor(p)
{
}

QomposeBuffer::~QomposeBuffer()
{
}

void QomposeBuffer::open(const QomposeFileDescriptor &f)
{
	
}

QString QomposeBuffer::getTitle() const
{
	return "Untitled";
}

QString QomposeBuffer::getPath() const
{
	return "";
}

/*!
 * This function returns whether or not this buffer has ever been saved, or if it
 * is a brand-new document (possibly with unsaved changes).
 */
bool QomposeBuffer::hasBeenSaved() const
{
	return false;
}

/*!
 * This function returns whether or not this buffer has any unsaved changes.
 */
bool QomposeBuffer::isModified() const
{
	return false;
}
