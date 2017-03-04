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

#ifndef qompose_core_file_MMIOFile_HPP
#define qompose_core_file_MMIOFile_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace qompose
{
namespace core
{
namespace file
{
namespace detail
{
struct MMIOFileImpl;
}

class MMIOFile
{
public:
	MMIOFile(std::string const &path);

	MMIOFile(MMIOFile const &) = delete;
	MMIOFile(MMIOFile &&) = default;
	MMIOFile &operator=(MMIOFile const &) = delete;
	MMIOFile &operator=(MMIOFile &&) = default;

	~MMIOFile();

	uint8_t const *data() const;
	std::size_t size() const;

private:
	std::unique_ptr<detail::MMIOFileImpl> impl;
};
}
}
}

#endif
