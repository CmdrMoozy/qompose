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

#include "Tree.h"

#include <stdexcept>

#include "QomposeCommon/git/Repository.h"
#include "QomposeCommon/git/Utils.h"

namespace qompose
{
namespace git
{
Tree::Tree(const Repository &repository, const git_oid &id)
        : AbstractGitObject()
{
	git_tree *tree;
	int ret = git_tree_lookup(&tree, repository.get(), &id);
	if(tree != nullptr)
		set(tree);
	if(ret != 0)
		throw std::runtime_error(git_utils::lastErrorToString());
}

Tree::deleter_t Tree::getDeleter() const
{
	return git_tree_free;
}
}
}
