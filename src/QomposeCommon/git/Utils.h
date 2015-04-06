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

#ifndef INCLUDE_QOMPOSECOMMON_GIT_UTILS_H
#define INCLUDE_QOMPOSECOMMON_GIT_UTILS_H

#include <string>

namespace qompose
{
namespace git_utils
{
/*!
 * This function returns the last libgit2 error generatd by this thread as a
 * string. If no error has been generated, an empty string is returned instead.
 *
 * \return The last error as a human-readable string.
 */
std::string lastErrorToString();

/*!
 * This function will return the path to the repository which contains the
 * given path. This search will not cross filesystem boundaries. If no
 * repository could be found, an empty string is returned instead.
 *
 * \param p The path to start searching from.
 * \return The path to the repository containing the given path.
 */
std::string discoverRepository(const std::string &p);
}
}

#endif
