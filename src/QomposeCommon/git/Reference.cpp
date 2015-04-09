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

#include <stdexcept>

#include "QomposeCommon/git/Repository.h"
#include "QomposeCommon/git/Utils.h"

namespace qompose
{
namespace git
{
Reference Reference::head(const Repository &repository)
{
	std::function<int(git_reference **, git_repository *)> creator =
	        [](git_reference **out, git_repository *repo)
	{
		int ret = git_repository_head(out, repo);
		if(ret == GIT_EUNBORNBRANCH)
		{
			throw std::runtime_error(
			        "HEAD is a nonexistant branch.");
		}
		else if(ret == GIT_ENOTFOUND)
		{
			throw std::runtime_error("HEAD is missing.");
		}
		return ret;
	};
	return Reference(git_utils::newGitObject(creator, git_reference_free,
	                                         repository.get()));
}

Reference Reference::resolve() const
{
	if(isOIDReference())
		return *this;
	const git_reference *thisReference = get();
	return Reference(git_utils::newGitObject(
	        git_reference_resolve, git_reference_free, thisReference));
}

bool Reference::isOIDReference() const
{
	return git_reference_type(get()) == GIT_REF_OID;
}

bool Reference::isSymbolicReference() const
{
	return git_reference_type(get()) == GIT_REF_SYMBOLIC;
}

git_oid Reference::getOID() const
{
	Reference resolved = resolve();
	const git_oid *id = git_reference_target(resolved.get());
	if(id == nullptr)
		throw std::runtime_error("Getting reference OID failed.");
	git_oid copy;
	git_oid_cpy(&copy, id);
	return copy;
}

std::string Reference::getTargetName() const
{
	const char *name = git_reference_symbolic_target(get());
	if(name == nullptr)
		return std::string();
	return std::string(name);
}

Reference::Reference(git_reference *r) : AbstractGitObject(r)
{
}

Reference::deleter_t Reference::getDeleter() const
{
	return git_reference_free;
}
}
}
