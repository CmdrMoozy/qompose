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

#ifndef INCLUDE_QOMPOSECOMMON_SETTINGS_RECENT_LIST_H
#define INCLUDE_QOMPOSECOMMON_SETTINGS_RECENT_LIST_H

#include <cstddef>
#include <deque>
#include <string>

namespace qompose
{
namespace settings
{
class RecentList
{
public:
	RecentList();

	RecentList(RecentList const &) = delete;
	RecentList(RecentList &&) = delete;
	RecentList &operator=(RecentList const &) = delete;
	RecentList &operator=(RecentList &&) = delete;

	~RecentList() = default;

	std::size_t getCapacity() const;

	void addPath(std::string const &p);

private:
	std::deque<std::string> recentList;
	std::size_t capacity;
};
}
}

#endif
