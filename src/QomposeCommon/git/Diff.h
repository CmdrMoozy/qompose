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

#ifndef INCLUDE_QOMPOSECOMMON_GIT_DIFF_H
#define INCLUDE_QOMPOSECOMMON_GIT_DIFF_H

#include <git2.h>

#include "QomposeCommon/git/AbstractGitObject.h"

namespace qompose
{
namespace git
{
class Repository;

/*!
 * \brief This class represents a Git diff.
 */
class Diff : public AbstractGitObject<git_diff>
{
public:
	/*!
	 * This function diffs the given repository's HEAD with its work
	 * directory. This is equivalent to the command "git diff".
	 *
	 * \param repository The repository to inspect.
	 * \return The resulting diff.
	 */
	static Diff diffHEAD(const Repository &repository);

	virtual ~Diff() = default;

protected:
	/*!
	 * This constructor creates a new object from the given raw Git
	 * pointer. The resulting object takes ownership of the pointer.
	 *
	 * \param p The pointer to create an object wrapper for.
	 */
	Diff(git_diff *p);

	/*!
	 * Return the function which can free git_diff pointers.
	 *
	 * \return The deleter function for objects of this type.
	 */
	virtual deleter_t getDeleter() const override;
};
}
}

#endif
