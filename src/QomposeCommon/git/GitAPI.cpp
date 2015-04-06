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

#include "GitAPI.h"

#include <cassert>

#include <git2.h>

namespace qompose
{
namespace git
{
std::mutex GitAPI::mutex;
std::unique_ptr<GitAPI> GitAPI::api;

void GitAPI::initialize()
{
	std::lock_guard<std::mutex> lock(GitAPI::mutex);
	if(!GitAPI::api)
		GitAPI::api.reset(new GitAPI());
}

GitAPI::~GitAPI()
{
	int ret = git_libgit2_shutdown();
	assert(ret == 0);
}

GitAPI::GitAPI()
{
	int ret = git_libgit2_init();
	assert(ret == 1);
}
}
}
