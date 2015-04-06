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

#ifndef INCLUDE_QOMPOSECOMMON_GIT_GIT_API_H
#define INCLUDE_QOMPOSECOMMON_GIT_GIT_API_H

#include <memory>
#include <mutex>

namespace qompose
{
namespace git
{
/*!
 * \brief This class provides a singleton which manages libgit2 initialization.
 */
class GitAPI
{
public:
	/*!
	 * This function initializes the global singleton for libgit2 calls.
	 * This function must be called at least once by a process before any
	 * libgit2 functions can be called.
	 *
	 * Note that calling this function multiple times in the same program
	 * is a no-op.
	 */
	static void initialize();

	/*!
	 * This destructor should only be called at program termination, when
	 * the singleton instance is destructed. It shuts down the libgit2
	 * internals which this class initializes.
	 */
	~GitAPI();

private:
	static std::mutex mutex;
	static std::unique_ptr<GitAPI> api;

	/*!
	 * This constructor initializes libgit2, so all other future libgit2
	 * calls will work properly.
	 */
	GitAPI();

	GitAPI(const GitAPI &) = delete;
	GitAPI &operator=(const GitAPI &) = delete;
};
}
}

#endif
