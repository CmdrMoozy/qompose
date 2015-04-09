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

#ifndef INCLUDE_QOMPOSECOMMON_GIT_DIFF_STATS_H
#define INCLUDE_QOMPOSECOMMON_GIT_DIFF_STATS_H

#include <cstddef>

#include <git2.h>

#include "QomposeCommon/git/AbstractGitObject.h"

namespace qompose
{
namespace git
{
class Diff;

/*!
 * \brief This class represents statistics for a Git diff.
 */
class DiffStats : public AbstractGitObject<git_diff_stats>
{
public:
	/*!
	 * This constructor creates a new stats object for the given Diff.
	 *
	 * \param diff The Diff to compute stats for.
	 */
	DiffStats(const Diff &diff);

	virtual ~DiffStats() = default;

	/*!
	 * This function returns the total number of deletions.
	 *
	 * \return The number of deletions.
	 */
	std::size_t getDeletions() const;

	/*!
	 * This function returns the total number of files that were changed.
	 *
	 * \return The number of files changed.
	 */
	std::size_t getFilesChanged() const;

	/*!
	 * This function returns the total number of insertions.
	 *
	 * \return The number of insertions.
	 */
	std::size_t getInsertions() const;

protected:
	/*!
	 * Return the function which can free git_diff_stats pointers.
	 *
	 * \return The deleter function for objects of this type.
	 */
	virtual deleter_t getDeleter() const override;
};
}
}

#endif
