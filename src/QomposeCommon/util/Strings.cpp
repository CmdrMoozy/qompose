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
}

namespace qompose
{
namespace string_utils
{
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
}
}
