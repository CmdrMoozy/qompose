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

#include "Repository.h"

#include <cassert>
#include <stdexcept>

#include "QomposeCommon/git/Utils.h"

namespace qompose
{
namespace git
{
Repository::Repository(const std::string &p) : AbstractGitObject()
{
	std::string path = git_utils::discoverRepository(p);
	set(git_utils::newGitObject(git_repository_open, git_repository_free,
	                            path.c_str()));
}

std::string Repository::getWorkDirectory() const
{
	const char *dir = git_repository_workdir(get());
	if(dir == nullptr)
		return std::string();
	return std::string(dir);
}

Repository::deleter_t Repository::getDeleter() const
{
	return git_repository_free;
}
}
}
