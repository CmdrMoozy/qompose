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

#include "Diff.h"

#include <stdexcept>

#include "QomposeCommon/git/Reference.h"
#include "QomposeCommon/git/Repository.h"
#include "QomposeCommon/git/Tree.h"
#include "QomposeCommon/git/Utils.h"

namespace qompose
{
namespace git
{
Diff Diff::diffHEAD(const Repository &repository)
{
	Reference head = Reference::head(repository);
	Tree headTree(repository, head.getOID());
	const git_diff_options *options = nullptr;
	return Diff(git_utils::newGitObject(git_diff_tree_to_workdir_with_index,
	                                    git_diff_free, repository.get(),
	                                    headTree.get(), options));
}

Diff::Diff(git_diff *p) : AbstractGitObject(p)
{
}

Diff::deleter_t Diff::getDeleter() const
{
	return git_diff_free;
}
}
}
