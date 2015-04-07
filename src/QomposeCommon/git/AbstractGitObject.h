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

#ifndef INCLUDE_QOMPOSECOMMON_GIT_ABSTRACT_GIT_OBJECT_H
#define INCLUDE_QOMPOSECOMMON_GIT_ABSTRACT_GIT_OBJECT_H

#include <cassert>
#include <functional>
#include <memory>
#include <stdexcept>

#include "QomposeCommon/git/GitAPI.h"

namespace qompose
{
namespace git
{
/*!
 * \brief This superclass provides common behavior for all Git object wrappers.
 */
template <typename object_t> class AbstractGitObject
{
public:
	/*!
	 * This constructor creates a new, empty Git object wrapper. This
	 * constructor should never be available directly in subclasses.
	 */
	AbstractGitObject() : pointer()
	{
		assert(GitAPI::isInitialized());
	}

	AbstractGitObject(const AbstractGitObject &) = default;
	virtual ~AbstractGitObject() = default;

	AbstractGitObject &operator=(const AbstractGitObject &) = default;

	/*!
	 * This function returns a pointer to this object's internal Git
	 * object pointer.
	 *
	 * \return This object's internal pointer.
	 */
	object_t *get() const
	{
		return pointer.get();
	}

protected:
	/*!
	 * \brief This typedef defines the deleter type for this object.
	 */
	typedef std::function<void(object_t *)> deleter_t;

	/*!
	 * \brief This typedef defines the shared pointer type for this object.
	 */
	typedef std::shared_ptr<object_t> pointer_t;

	/*!
	 * This constructor creates a new object from the given raw Git
	 * pointer. The resulting object takes ownership of the pointer.
	 *
	 * \param p The pointer to create an object wrapper for.
	 */
	AbstractGitObject(object_t *p) : AbstractGitObject()
	{
		set(p);
	}

	/*!
	 * This function replaces our object's contents with the given pointer.
	 * This object takes ownership of the given pointer.
	 *
	 * \param p The new pointer to be an object wrapper for.
	 */
	void set(object_t *p)
	{
		if(p == nullptr)
		{
			throw std::runtime_error(
			        "Can't construct Git object from NULL.");
		}
		pointer.reset(p, getDeleter());
	}

	/*!
	 * A subclass should override this function in order to return the
	 * function which can free pointers of type object_t.
	 *
	 * \return The deleter function for objects of this type.
	 */
	virtual deleter_t getDeleter() const = 0;

private:
	pointer_t pointer;
};
}
}

#endif
