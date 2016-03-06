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

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_ERRNO_H
#define INCLUDE_QOMPOSECOMMON_UTIL_ERRNO_H

#include <string>

namespace qompose
{
namespace util
{
/*!
 * This function returns a human readable string for the given error number.
 *
 * \param error The error number to interpret. Default is 0, meaning use errno.
 * \param defaultMessage The default error message to return.
 * \return A human readable error message for the given error.
 */
std::string getErrnoError(int error = 0,
                          const std::string &defaultMessage = "Unknown error.");

/*!
 * Throw an exception with the getErrnoError() message for the given error.
 *
 * \param error The error number to interpret. Default is 0, meaning use errno.
 * \param defaultMessage The default error message to return.
 */
[[noreturn]] void
throwErrnoError(int error = 0,
                const std::string &defaultMessage = "Unknown error.");

#ifdef _WIN32
/*!
 * This function is a wrapper for Windows' GetLastError() function, which
 * returns the last error message as a human-readable string.
 *
 * \return The last error as a human-readable string.
 */
std::string getLastWindowsError();

/*!
 * Throw an exception, with the last Windows error message as its "what"
 * parameter. See getLastWindowsError() for details.
 */
void throwLastWindowsError();
#endif
}
}

#endif
