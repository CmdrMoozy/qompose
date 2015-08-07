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

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_PATHS_H
#define INCLUDE_QOMPOSECOMMON_UTIL_PATHS_H

#include <cstddef>
#include <string>
#include <vector>

#include <QString>

namespace qompose
{
namespace path_utils
{
/*!
 * This function returns the path to the deepest directory which contains all
 * of the files or directories in the given list of paths. If no such path
 * exists (either because the list is empty, or because their paths have
 * nothing in common), then an empty string is returned instead.
 *
 * \param paths The list of paths to examine.
 * \return The path containing all the given paths.
 */
std::string getCommonParentPath(const std::vector<std::string> &paths);

/*!
 * This function returns a canonical version of the given path, similar to
 * QFileInfo's canonicalFilePath() function. The difference is that this
 * function works even if the given file doesn't already exist.
 *
 * \param path The path to canonicalize.
 * \return The canonical version of the given path.
 */
QString getCanonicalPath(const QString &path);

/*!
 * This function returns the path the given symlink points to, or the given
 * path unchanged if it isn' a symlink.
 *
 * \param path The path to de-symlink.
 * \return The dereferenced path.
 */
std::string stripSymlink(const std::string &path);

/*!
 * \param file The path to the file to examine.
 * \return The size of the given file, in bytes.
 */
std::size_t getSize(const std::string &file);
}
}

#endif
