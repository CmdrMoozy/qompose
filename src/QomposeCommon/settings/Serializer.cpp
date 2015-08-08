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

#include "Serializer.h"

#include <cstdint>

#ifdef QOMPOSE_DEBUG
#include <cassert>
#include <numeric>
#endif

#include <QDataStream>

namespace qompose
{
namespace settings
{
std::string Serializer<bool>::operator()(bool const &v) const
{
	return v ? "true" : "false";
}

std::string Serializer<QColor>::operator()(QColor const &v) const
{
	std::ostringstream oss;
	oss << v.red() << " " << v.green() << " " << v.blue() << " "
	    << v.alpha();
	return oss.str();
}

std::string Serializer<QByteArray>::operator()(QByteArray const &v) const
{
	return std::string(v.data(), v.data() + v.size());
}

std::string Serializer<QFont>::operator()(QFont const &v) const
{
	QByteArray bytes;
	QDataStream stream(&bytes, QIODevice::WriteOnly);
	stream << v;

	Serializer<QByteArray> serializer;
	return serializer(bytes);
}

std::string Serializer<std::vector<std::string>>::
operator()(std::vector<std::string> const &v) const
{
	std::ostringstream oss;
	for(auto const &s : v)
	{
		uint64_t l = s.length();
		oss.write(reinterpret_cast<char const *>(&l), sizeof(uint64_t));
		oss.write(s.data(), static_cast<std::streamsize>(s.length()));
	}

#ifdef QOMPOSE_DEBUG
	std::string::size_type totalLength =
	        std::accumulate<decltype(v.begin()), std::string::size_type>(
	                v.begin(), v.end(), 0,
	                [](std::string::size_type total, std::string const &s)
	                {
		                return total + s.length();
		        });
	assert(oss.tellp() > 0);
	assert(static_cast<std::string::size_type>(oss.tellp()) ==
	       (totalLength + sizeof(uint64_t) * v.size()));
#endif

	return oss.str();
}
}
}
