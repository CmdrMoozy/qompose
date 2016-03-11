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

#include "MMIOFile.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <bdrck/fs/ExclusiveFileLock.hpp>
#include <bdrck/util/Error.hpp>

namespace qompose
{
namespace core
{
namespace file
{
namespace detail
{
struct MMIOFileImpl
{
	bdrck::fs::ExclusiveFileLock lock;
	int fd;
	struct stat stats;
	void *file;

	MMIOFileImpl(std::string const &path)
	        : lock(path), fd(-1), stats(), file(nullptr)
	{
		int ret = open(path.c_str(), O_RDWR);
		if(ret == -1)
			bdrck::util::error::throwErrnoError();

		ret = fstat(fd, &stats);
		if(ret == -1)
		{
			close(fd);
			bdrck::util::error::throwErrnoError();
		}

		file = mmap(nullptr, static_cast<std::size_t>(stats.st_size),
		            PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
		if(file == MAP_FAILED)
		{
			close(fd);
			bdrck::util::error::throwErrnoError();
		}

		ret = madvise(file, static_cast<std::size_t>(stats.st_size),
		              MADV_RANDOM);
		if(ret == -1)
		{
			munmap(file, static_cast<std::size_t>(stats.st_size));
			close(fd);
			bdrck::util::error::throwErrnoError();
		}
	}

	MMIOFileImpl(MMIOFileImpl const &) = delete;
	MMIOFileImpl(MMIOFileImpl &&) = default;
	MMIOFileImpl &operator=(MMIOFileImpl const &) = delete;
	MMIOFileImpl &operator=(MMIOFileImpl &&) = default;

	~MMIOFileImpl()
	{
		munmap(file, static_cast<std::size_t>(stats.st_size));
		close(fd);
	}

	uint8_t const *data() const
	{
		return static_cast<uint8_t const *>(file);
	}

	std::size_t size() const
	{
		return static_cast<std::size_t>(stats.st_size);
	}
};
}

MMIOFile::MMIOFile(std::string const &path)
        : impl(std::make_unique<detail::MMIOFileImpl>(path))
{
}

MMIOFile::~MMIOFile()
{
}

uint8_t const *MMIOFile::data() const
{
	return impl->data();
}

std::size_t MMIOFile::size() const
{
	return impl->size();
}
}
}
}
