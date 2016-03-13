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

#include "MemoryMappedFile.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <stdexcept>

#include <bdrck/fs/Util.hpp>

#include "QomposeCommon/util/Errno.h"

namespace qompose
{
MemoryMappedFile::MemoryMappedFile(const std::string &path)
        : fileDescriptor(-1), file(nullptr), length(0)
{
	std::string canonical = bdrck::fs::resolvePath(path);
	length = static_cast<std::size_t>(bdrck::fs::fileSize(canonical));

	if(length > 0)
	{
		fileDescriptor = open(canonical.c_str(), O_RDONLY | O_NOFOLLOW);
		if(fileDescriptor == -1)
			throw std::runtime_error(
			        "Opening the specified file failed.");

		void *f = mmap(nullptr, length, PROT_READ, MAP_PRIVATE,
		               fileDescriptor, 0);
		if(f == MAP_FAILED)
		{
			int ret = close(fileDescriptor);
			if(ret != 0)
				util::throwErrnoError();

			util::throwErrnoError();
		}
		file = reinterpret_cast<uint8_t *>(f);
	}
}

MemoryMappedFile::~MemoryMappedFile()
{
	if((file != nullptr) && (file != MAP_FAILED))
	{
		int ret = munmap(file, length);
		if(ret != 0)
			util::throwErrnoError();
	}

	if(fileDescriptor != -1)
	{
		int ret = close(fileDescriptor);
		if(ret != 0)
			util::throwErrnoError();
	}
}

const uint8_t *MemoryMappedFile::getData() const
{
	return file;
}

std::size_t MemoryMappedFile::getLength() const
{
	return length;
}
}
