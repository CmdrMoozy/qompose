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

#ifndef INCLUDE_QOMPOSECOMMON_SETTINGS_DESERIALIZE_H
#define INCLUDE_QOMPOSECOMMON_SETTINGS_DESERIALIZE_H

#include <sstream>
#include <string>

#include <QByteArray>
#include <QColor>
#include <QFont>

namespace qompose
{
namespace settings
{
template <typename T> struct Deserializer
{
	T operator()(std::string const &s) const
	{
		T v;
		std::istringstream iss(s);
		iss >> v;
		return v;
	}
};

template <> struct Deserializer<bool>
{
	bool operator()(std::string const &s) const;
};

template <> struct Deserializer<QColor>
{
	QColor operator()(std::string const &s) const;
};

template <> struct Deserializer<QByteArray>
{
	QByteArray operator()(std::string const &s) const;
};

template <> struct Deserializer<QFont>
{
	QFont operator()(std::string const &s) const;
};
}
}

#endif
