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

#include "Strings.h"

#include <cerrno>
#include <cstring>
#include <vector>

#include <boost/locale/encoding_utf.hpp>

namespace
{
const std::size_t ERROR_MESSAGE_BUFFER_SIZE = 256;

template <typename ReturnType>
std::string getErrorMessage(ReturnType ret, const std::vector<char> &buffer)
{
	if(ret == 0)
		return std::string();

	return std::string(buffer.data());
}

template <>
std::string getErrorMessage<char *>(char *ret, const std::vector<char> &)
{
	return std::string(ret);
}

#ifdef _WIN32
template <typename StringPointer> std::string lptstrToString(StringPointer);

template <> std::string lptstrToString<LPSTR>(LPSTR s)
{
	return std::string(s);
}

template <> std::string lptstrToString<LPWSTR>(LPWSTR s)
{
	return qompose::string_utils::wstringToString(std::wstring(s));
}
#endif
}

namespace qompose
{
namespace string_utils
{
std::string wstringToString(const std::wstring &s)
{
	return boost::locale::conv::utf_to_utf<char>(s.c_str(),
	                                             s.c_str() + s.length());
}

std::string getErrnoError(const std::string &defaultMessage,
                          boost::optional<int> errnoValue)
{
	if(!errnoValue)
		errnoValue = errno;

	std::vector<char> buffer(ERROR_MESSAGE_BUFFER_SIZE);

#if defined(_WIN32)
	auto ret = strerror_s(buffer.data(), buffer.size(), *errnoValue);
#else
	auto ret = strerror_r(*errnoValue, buffer.data(), buffer.size());
#endif

	std::string error = getErrorMessage(ret, buffer);
	if(error.empty())
		error = defaultMessage;
	return error;
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
#endif
}
}
