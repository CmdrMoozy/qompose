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

#include "InMemoryFile.hpp"

#include <cstdio>
#include <functional>
#include <memory>
#include <stdexcept>

#include <bdrck/util/Error.hpp>

namespace
{
void fileDeleter(FILE *f)
{
	if(f != nullptr)
		fclose(f);
}
}

namespace qompose
{
namespace core
{
namespace file
{
InMemoryFile::InMemoryFile(std::string const &path) : contents()
{
	std::unique_ptr<FILE, std::function<void(FILE *)>> f(nullptr,
	                                                     fileDeleter);
	f.reset(fopen(path.c_str(), "r"));
	if(!f)
		bdrck::util::error::throwErrnoError();

	int ret = fseek(f.get(), 0L, SEEK_END);
	if(ret != 0)
		bdrck::util::error::throwErrnoError();

	long s = ftell(f.get());
	if(s == -1)
		bdrck::util::error::throwErrnoError();

	ret = fseek(f.get(), 0L, SEEK_SET);
	if(ret != 0)
		bdrck::util::error::throwErrnoError();

	contents.resize(static_cast<std::size_t>(s));
	std::size_t read = fread(contents.data(), sizeof(uint8_t),
	                         contents.size(), f.get());
	if(read != static_cast<std::size_t>(s))
		throw std::runtime_error("Reading file contents failed.");
}

uint8_t const *InMemoryFile::data() const
{
	return contents.data();
}

std::size_t InMemoryFile::size() const
{
	return contents.size();
}
}
}
}
