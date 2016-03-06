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

#include <string>

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
	static Reference head(const Repository &repository);

	Reference(Reference const &) = default;
	Reference(Reference &&) = default;
	Reference &operator=(Reference const &) = default;
	Reference &operator=(Reference &&) = default;

	virtual ~Reference() = default;

	/*!
	 * This function returns an equivalent reference which is guaranteed to
	 * be a direct OID reference. If this reference isn't symbolic, this
	 * function simply returns *this.
	 *
	 * \return A resolved version of this reference.
	 */
	Reference resolve() const;

	/*!
	 * This function returns whether or not this reference refers to an
	 * OID (that is, a particular commit).
	 *
	 * \return Whether or not this is an OID reference.
	 */
	bool isOIDReference() const;

	/*!
	 * This function returns whether or not this reference is a symbolic
	 * reference (that is, it refers to a branch, for example).
	 *
	 * \return Whether or not this is a symbolic reference.
	 */
	bool isSymbolicReference() const;

	/*!
	 * This function returns the OID of this reference's target.
	 *
	 * \return This reference's target OID.
	 */
	git_oid getOID() const;

	/*!
	 * This function returns this reference's target's name. If this isn't
	 * a symbolic reference, then an empty string will be returned instead
	 * (since such a reference doesn't have a target, by definition).
	 *
	 * \return This reference's target's name.
	 */
	std::string getTargetName() const;

protected:
	/*!
	 * This constructor creates a new object from the given raw Git
	 * pointer. The resulting object takes ownership of the pointer.
	 *
	 * \param r The pointer to create an object wrapper for.
	 */
	Reference(git_reference *r);

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
