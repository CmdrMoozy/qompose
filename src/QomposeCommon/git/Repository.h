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

#ifndef INCLUDE_QOMPOSECOMMON_GIT_REPOSITORY_H
#define INCLUDE_QOMPOSECOMMON_GIT_REPOSITORY_H

#include <memory>
#include <string>

#include <git2.h>

namespace qompose
{
namespace git
{
/*!
 * \brief This class represents a Git repository.
 */
class Repository
{
public:
	/*!
	 * This constructor initializes a new Git repository from the given
	 * path. The given path can be any path inside the Git repository; the
	 * true repository path will be automatically discovered.
	 *
	 * \param p The path of the repository to open.
	 */
	Repository(const std::string &p);

	Repository(const Repository &) = default;
	~Repository() = default;

	Repository &operator=(const Repository &) = default;

	/*!
	 * This function returns this object's internal git_repository.
	 *
	 * \return This object's internal git_repository.
	 */
	git_repository *get();

	/*!
	 * This function returns this object's internal git_repository.
	 *
	 * \return This object's internal git_repository.
	 */
	const git_repository *get() const;

	/*!
	 * This function returns this repository's work directory. If the
	 * repository has no work directory (for instance, if it is a bare
	 * repository), then an empty string is returned instead.
	 *
	 * \return This repository's work directory.
	 */
	std::string getWorkDirectory() const;

private:
	std::shared_ptr<git_repository> repository;
};
}
}

#endif
