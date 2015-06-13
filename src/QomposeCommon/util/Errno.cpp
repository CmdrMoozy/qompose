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

#include "Errno.h"

#include "QomposeCommon/util/Strings.h"

#include <cerrno>
#include <cstring>
#include <stdexcept>

namespace
{
#ifdef _WIN32
std::string lptstrToString(LPSTR s)
{
	return std::string(s);
}

std::string lptstrToString(LPWSTR s)
{
	return qompose::string_utils::wstringToString(std::wstring(s));
}
#endif
}

namespace qompose
{
namespace util
{
std::string getErrnoError(int error, const std::string &defaultMessage)
{
	if(error == 0)
		error = errno;

	std::string errorString = defaultMessage;

	char buf[1024];
	return std::string(strerror_r(error, buf, 1024));
}

void throwErrnoError(int error, const std::string &defaultMessage)
{
	throw std::runtime_error(getErrnoError(error, defaultMessage));
}

#ifdef _WIN32
std::string getLastWindowsError()
{
	DWORD error = GetLastError();

	LPTSTR buffer;
	DWORD ret =
	        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, error, 0,
	                      static_cast<LPTSTR>(&buffer), 0, nullptr);

	if(ret == 0)
		return std::string("Unknown error.");

	std::string errorMessage = lptstrToString(buffer);
	LocalFree(buffer);
	return errorMessage);
}

void throwLastWindowsError()
{
	throw std::runtime_error(getLastWindowsError());
}
#endif
}
}
