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

#ifndef INCLUDE_QOMPOSECOMMON_GIT_UTILS_H
#define INCLUDE_QOMPOSECOMMON_GIT_UTILS_H

#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

namespace qompose
{
namespace git_utils
{
/*!
 * This function returns the last libgit2 error generatd by this thread as a
 * string. If no error has been generated, an empty string is returned instead.
 *
 * \return The last error as a human-readable string.
 */
std::string lastErrorToString();

/*!
 * This function will return the path to the repository which contains the
 * given path. This search will not cross filesystem boundaries. If no
 * repository could be found, an empty string is returned instead.
 *
 * \param p The path to start searching from.
 * \return The path to the repository containing the given path.
 */
std::string discoverRepository(const std::string &p);

/*!
 * This function constructs a new Git object of any type using the given
 * creator and deleter functions. This function guarantees that either a valid
 * pointer will be returned, or an exception will be thrown, and that there is
 * no possibility for leaking memory prior to returning.
 *
 * \param creator The creator function to use.
 * \param deleter The deleter function to use.
 * \param args The extra arguments to pass to the creator function.
 * \return The newly created Git object.
 */
template <typename object_t, typename... args_t>
object_t *
newGitObject(const std::function<int(object_t **, args_t...)> &creator,
             const std::function<void(object_t *)> &deleter, args_t... args)
{
	object_t *obj = nullptr;

	try
	{
		int ret = creator(&obj, std::forward<args_t>(args)...);
		if(ret != 0)
			throw std::runtime_error(lastErrorToString());
	}
	catch(...)
	{
		if(obj != nullptr)
			deleter(obj);

		throw;
	}

	return obj;
}

/*!
 * This function is a wapper for the main newGitObject() overload, which allows
 * a function pointer to be used as a creator.
 *
 * \param creator The creator function to use.
 * \param deleter The deleter function to use.
 * \param args The extra arguments to pass to the creator function.
 * \return The newly created Git object.
 */
template <typename object_t, typename... args_t>
object_t *
newGitObject(const std::function<int(object_t **, args_t...)> &creator,
             void(&deleter)(object_t *), args_t... args)
{
	return newGitObject(creator, std::function<void(object_t *)>(deleter),
	                    args...);
}

/*!
 * This function is a wapper for the main newGitObject() overload, which allows
 * a function pointer to be used as both the creator and deleter.
 *
 * \param creator The creator function to use.
 * \param deleter The deleter function to use.
 * \param args The extra arguments to pass to the creator function.
 * \return The newly created Git object.
 */
template <typename object_t, typename... args_t>
object_t *newGitObject(int(&creator)(object_t **, args_t...),
                       void(&deleter)(object_t *), args_t... args)
{
	return newGitObject(std::function<int(object_t **, args_t...)>(creator),
	                    std::function<void(object_t *)>(deleter), args...);
}
}
}

#endif
