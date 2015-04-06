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

#ifndef INCLUDE_QOMPOSECOMMON_GIT_BUFFER_H
#define INCLUDE_QOMPOSECOMMON_GIT_BUFFER_H

#include <cstddef>

#include <git2.h>

namespace qompose
{
namespace git
{
/*!
 * \brief This class provides an RAII interface for git_buf structures.
 */
class Buffer
{
public:
	/*!
	 * This constructor allocates a new git_buf with the given size.
	 *
	 * \param size The size of the new git_buf.
	 */
	Buffer(std::size_t size = 0);

	Buffer(const Buffer &) = delete;

	/*!
	 * This destructor frees the memory allocated for this git_buf.
	 */
	~Buffer();

	Buffer &operator=(const Buffer &) = delete;

	/*!
	 * This function returns a pointer to this object's internal git_buf.
	 * Note that this pointer is only valid as long as this object has not
	 * been freed.
	 *
	 * \return This object's internal git_buf.
	 */
	git_buf *get();

	/*!
	 * This function expands this git_buf to be the given size. Note that,
	 * as per the libgit2 API, this function can only increase the size of
	 * the buffer, not decrease it.
	 *
	 * \param s The new size for this buffer.
	 */
	void grow(std::size_t s);

	/*!
	 * This function returns whether or not the current buffer contains a
	 * NUL byte (0x00).
	 *
	 * \return Whether or not this buffer contains a NUL byte.
	 */
	bool containsNul() const;

	/*!
	 * This function returns whether or not the current buffer seems to
	 * contain binary data. This is a fast check, which is not guaranteed
	 * to be completely accurate.
	 *
	 * \return Whether or not this buffer contains binary data.
	 */
	bool isBinary() const;

	/*!
	 * This function sets the contents of this buffer to the given data.
	 *
	 * \param d The data to store in this buffer.
	 * \param s The size of the given data, in bytes.
	 */
	void set(const void *d, std::size_t s);

private:
	git_buf buffer;
};
}
}

#endif
