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

#ifndef INCLUDE_QOMPOSECOMMON_GIT_TREE_H
#define INCLUDE_QOMPOSECOMMON_GIT_TREE_H

#include <git2.h>

#include "QomposeCommon/git/AbstractGitObject.h"

namespace qompose
{
namespace git
{
class Repository;

/*!
 * \brief This class represents a Git tree.
 */
class Tree : public AbstractGitObject<git_tree>
{
public:
	/*!
	 * This constructor creates a new Tree from the given OID in the given
	 * repository.
	 *
	 * \param repository The Repository to lookup the given OID in.
	 * \param id The OID of the Tree to lookup.
	 */
	Tree(const Repository &repository, const git_oid &id);

	virtual ~Tree() = default;

protected:
	/*!
	 * Return the function which can free git_reference pointers.
	 *
	 * \return The deleter function for objects of this type.
	 */
	virtual deleter_t getDeleter() const override;
};
}
}

#endif
