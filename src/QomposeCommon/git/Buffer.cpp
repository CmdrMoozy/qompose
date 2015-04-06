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

#include "Buffer.h"

#include <cassert>
#include <cstring>
#include <stdexcept>

#include "QomposeCommon/git/GitAPI.h"

namespace qompose
{
namespace git
{
Buffer::Buffer(std::size_t size) : buffer()
{
	assert(GitAPI::isInitialized());
	memset(&buffer, 0, sizeof(git_buf));
	grow(size);
}

Buffer::~Buffer()
{
	git_buf_free(&buffer);
}

git_buf *Buffer::get()
{
	return &buffer;
}

void Buffer::grow(std::size_t s)
{
	int ret = git_buf_grow(&buffer, s);
	if(ret != 0)
		throw std::runtime_error("Allocation failure.");
}

bool Buffer::containsNul() const
{
	return git_buf_contains_nul(&buffer) == 1;
}

bool Buffer::isBinary() const
{
	return git_buf_is_binary(&buffer) == 1;
}

void Buffer::set(const void *d, std::size_t s)
{
	int ret = git_buf_set(&buffer, d, s);
	if(ret != 0)
		throw std::runtime_error("Allocation failure.");
}
}
}
