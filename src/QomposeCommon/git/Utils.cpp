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

#include "Utils.h"

#include <cassert>

#include <git2.h>

#include "QomposeCommon/git/Buffer.h"
#include "QomposeCommon/git/GitAPI.h"

namespace qompose
{
namespace git_utils
{
std::string lastErrorToString()
{
	assert(git::GitAPI::isInitialized());
	const git_error *error = giterr_last();
	if(error == nullptr)
		return std::string();
	return std::string(error->message);
}

std::string discoverRepository(const std::string &p)
{
	assert(git::GitAPI::isInitialized());
	git::Buffer buf(1024);

	int ret = git_repository_discover(buf.get(), p.c_str(), 0, "");
	if(ret != 0)
		throw std::runtime_error(lastErrorToString());

	return buf.toString();
}
}
}
