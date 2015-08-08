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

#include "RecentList.h"

#include <algorithm>
#include <vector>

#include "QomposeCommon/settings/Settings.h"

namespace
{
std::size_t loadCapacity()
{
	return qompose::settings::Settings::instance().getAsOr<std::size_t>(
	        "recent-list-size", 10);
}

void trim(std::deque<std::string> &recentList, std::size_t capacity)
{
	while(recentList.size() > capacity)
		recentList.pop_back();
}

void save(std::deque<std::string> const &recentList, std::size_t capacity)
{
	std::vector<std::string> l;
	for(auto const &s : recentList)
		l.push_back(s);

	auto &settings = qompose::settings::Settings::instance();
	settings.putAs("recent-list", l);
	settings.putAs("recent-list-size", capacity);
}

void load(std::deque<std::string> &recentList, std::size_t &capacity)
{
	capacity = loadCapacity();

	recentList.clear();
	std::vector<std::string> contents =
	        qompose::settings::Settings::instance()
	                .getAsOr<std::vector<std::string>>(
	                        "recent-list", std::vector<std::string>());
	for(auto const &s : contents)
		recentList.push_back(s);
	trim(recentList, capacity);
	save(recentList, capacity);
}
}

namespace qompose
{
namespace settings
{
RecentList::RecentList() : recentList(), capacity(0)
{
	load(recentList, capacity);
}

std::size_t RecentList::getCapacity() const
{
	return capacity;
}

void RecentList::addPath(std::string const &p)
{
	recentList.push_front(p);
	trim(recentList, capacity);
	save(recentList, capacity);
}
}
}
