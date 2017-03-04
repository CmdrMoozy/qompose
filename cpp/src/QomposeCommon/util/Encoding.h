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

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_ENCODING_H
#define INCLUDE_QOMPOSECOMMON_UTIL_ENCODING_H

#include <QString>

namespace qompose
{
namespace encoding_utils
{
/*!
 * This function attempts to detet the character encoding in the specified
 * file. The character encoding will be returned as a string which can be used
 * with QTextCodec's codecForName function.
 *
 * If the character encoding cannot be determined, or if some other error
 * occurs, then we will return a null QString instead.
 *
 * \param f The path to the file whose encoding will be detected.
 * \return The encoding the given file seems to be using.
 */
QString detectTextCodec(const QString &f);
}
}

#endif
