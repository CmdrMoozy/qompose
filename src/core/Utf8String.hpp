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

#ifndef qompose_core_Utf8String_HPP
#define qompose_core_Utf8String_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

#include <boost/flyweight.hpp>

namespace qompose
{
namespace core
{
class Utf8String
{
public:
	Utf8String();

	Utf8String(Utf8String const &) = default;
	Utf8String(Utf8String &&) = default;
	Utf8String &operator=(Utf8String const &) = default;
	Utf8String &operator=(Utf8String &&) = default;

	~Utf8String() = default;

	std::size_t length() const;
	std::size_t size() const;

private:
	const boost::flyweights::flyweight<std::vector<uint8_t>> bytes;
	const std::size_t characterLength;
};
}
}

#endif
