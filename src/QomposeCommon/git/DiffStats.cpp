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

#include "DiffStats.h"

#include "QomposeCommon/git/Diff.h"
#include "QomposeCommon/git/Utils.h"

namespace qompose
{
namespace git
{
DiffStats::DiffStats(const Diff &diff)
{
	set(git_utils::newGitObject(git_diff_get_stats, git_diff_stats_free,
	                            diff.get()));
}

std::size_t DiffStats::getDeletions() const
{
	return git_diff_stats_deletions(get());
}

std::size_t DiffStats::getFilesChanged() const
{
	return git_diff_stats_files_changed(get());
}

std::size_t DiffStats::getInsertions() const
{
	return git_diff_stats_insertions(get());
}

DiffStats::deleter_t DiffStats::getDeleter() const
{
	return git_diff_stats_free;
}
}
}
