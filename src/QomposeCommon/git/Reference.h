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

#ifndef INCLUDE_QOMPOSECOMMON_GIT_REFERENCE_H
#define INCLUDE_QOMPOSECOMMON_GIT_REFERENCE_H

#include <memory>

#include <git2.h>

#include "QomposeCommon/git/AbstractGitObject.h"

namespace qompose
{
namespace git
{
class Repository;

/*!
 * \brief This class represents a Git reference.
 */
class Reference : public AbstractGitObject<git_reference>
{
public:
	/*!
	 * This function returns a Reference to the given repository's HEAD.
	 *
	 * \param repository The repository to inspect.
	 * \return A Reference to the repository's head.
	 */
	static Reference head(Repository &repository);

	virtual ~Reference() = default;

protected:
	/*!
	 * This constructor creates a new object from the given raw Git
	 * pointer. The resulting object takes ownership of the pointer.
	 *
	 * \param p The pointer to create an object wrapper for.
	 */
	Reference(git_reference *r);

	/*!
	 * Return the function which can free git_reference pointers.
	 *
	 * \return The deleter function for objects of this type.
	 */
	virtual deleter_t getDeleter() const override;

private:
};
}
}

#endif
