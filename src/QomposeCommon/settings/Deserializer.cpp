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

#include "Deserializer.h"

#include <cstdint>
#include <stdexcept>

#include <QDataStream>

namespace qompose
{
namespace settings
{
bool Deserializer<bool>::operator()(std::string const &s) const
{
	if(s == "true")
		return true;
	else if(s == "false")
		return false;
	else
		throw std::runtime_error("Deserializing boolean failed.");
}

QColor Deserializer<QColor>::operator()(std::string const &s) const
{
	std::istringstream iss(s);
	std::vector<int> values;
	int tmp;
	while((values.size() < 4) && (iss >> tmp))
		values.push_back(tmp);

	if((values.size() != 4) || !iss.eof())
		throw std::runtime_error("Deserializing QColor failed.");

	return QColor(values[0], values[1], values[2], values[3]);
}

QByteArray Deserializer<QByteArray>::operator()(std::string const &s) const
{
	return QByteArray(s.data(), s.length());
}

QFont Deserializer<QFont>::operator()(std::string const &s) const
{
	Deserializer<QByteArray> deserializer;
	QByteArray bytes = deserializer(s);

	QDataStream stream(bytes);
	QFont f;
	stream >> f;
	return f;
}

std::vector<std::string> Deserializer<std::vector<std::string>>::
operator()(std::string const &s) const
{
	std::istringstream iss(s);
	std::vector<std::string> v;
	while(iss.good())
	{
		uint64_t l;
		iss.read(reinterpret_cast<char *>(&l), sizeof(uint64_t));
		if(iss.eof())
			break;
		if(iss.gcount() != sizeof(uint64_t))
		{
			throw std::runtime_error(
			        std::to_string(l) +
			        "Deserializing length string list failed.");
		}

		std::vector<char> chars(l);
		iss.read(chars.data(), static_cast<std::streamsize>(l));
		if(static_cast<uint64_t>(iss.gcount()) != l)
		{
			throw std::runtime_error(
			        "Deserializing string list failed.");
		}
		v.emplace_back(chars.data(), l);
	}
	return v;
}
}
}