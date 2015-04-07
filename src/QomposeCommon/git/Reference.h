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

namespace qompose
{
namespace git
{
class Repository;

/*!
 * \brief This class represents a Git reference.
 */
class Reference
{
public:
	/*!
	 * This function returns a Reference to the given repository's HEAD.
	 *
	 * \param repository The repository to inspect.
	 * \return A Reference to the repository's head.
	 */
	static Reference head(Repository &repository);

	Reference(const Reference &) = default;
	~Reference() = default;

	Reference &operator=(const Reference &) = default;

private:
	std::shared_ptr<git_reference> reference;

	/*!
	 * Construct a new Reference using the given bare git_reference
	 * pointer. The resulting object takes ownership of the given pointer.
	 *
	 * \param r The pointer to construct an object from.
	 */
	Reference(git_reference *r);
};
}
}

#endif
