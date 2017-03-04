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

#ifndef qompose_core_file_InMemoryFile_HPP
#define qompose_core_file_InMemoryFile_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace qompose
{
namespace core
{
namespace file
{
/*!
 * \brief A disk file that has been fully loaded into memory.
 *
 * Implements the TextResource Concept defined in
 * core/document/PieceTable.hpp.
 */
class InMemoryFile
{
public:
	InMemoryFile(std::string const &path);

	InMemoryFile(InMemoryFile const &) = delete;
	InMemoryFile(InMemoryFile &&) = default;
	InMemoryFile &operator=(InMemoryFile const &) = delete;
	InMemoryFile &operator=(InMemoryFile &&) = default;

	~InMemoryFile() = default;

	uint8_t const *data() const;
	std::size_t size() const;

private:
	std::vector<uint8_t> contents;
};
}
}
}

#endif
