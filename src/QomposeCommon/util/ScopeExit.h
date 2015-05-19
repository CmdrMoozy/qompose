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

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_SCOPE_EXIT_H
#define INCLUDE_QOMPOSECOMMON_UTIL_SCOPE_EXIT_H

#include <functional>

namespace qompose
{
/*!
 * \brief This is a very simple utility to execute some code on destruction.
 */
class ScopeExit
{
public:
	/*!
	 * \param f The function to execute when this object is destructed.
	 */
	ScopeExit(const std::function<void()> &f);

	ScopeExit(const ScopeExit &) = delete;

	/*!
	 * Execute this object's function.
	 */
	~ScopeExit();

	ScopeExit &operator=(const ScopeExit &) = delete;

private:
	std::function<void()> function;
};
}

#endif
