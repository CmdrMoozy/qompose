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

#include "Reference.h"

#include <cassert>
#include <stdexcept>

#include "QomposeCommon/git/GitAPI.h"
#include "QomposeCommon/git/Repository.h"
#include "QomposeCommon/git/Utils.h"

namespace qompose
{
namespace git
{
Reference Reference::head(Repository &repository)
{
	git_reference *reference;
	int ret = git_repository_head(&reference, repository.get());
	if(ret != 0)
	{
		if(reference != nullptr)
			git_reference_free(reference);
		switch(ret)
		{
		case GIT_EUNBORNBRANCH:
			throw std::runtime_error(
			        "HEAD is a nonexistant branch.");
		case GIT_ENOTFOUND:
			throw std::runtime_error("HEAD is missing.");
		default:
			throw std::runtime_error(
			        git_utils::lastErrorToString());
		}
	}
	return Reference(reference);
}

Reference::Reference(git_reference *r)
{
	assert(GitAPI::isInitialized());
	if(r == nullptr)
	{
		throw std::runtime_error(
		        "Can't construct Reference from NULL.");
	}
	reference.reset(r, git_reference_free);
}
}
}
