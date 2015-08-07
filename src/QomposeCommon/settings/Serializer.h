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

#ifndef INCLUDE_QOMPOSECOMMON_SETTINGS_SERIALIZE_H
#define INCLUDE_QOMPOSECOMMON_SETTINGS_SERIALIZE_H

#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>

#include <QByteArray>
#include <QColor>
#include <QFont>

namespace qompose
{
namespace settings
{
namespace detail
{
struct FloatSerializerImpl
{
	template <typename T> std::string operator()(T const &v) const
	{
		std::ostringstream oss;
		oss << std::fixed << v;
		return oss.str();
	}
};

struct GenericSerializerImpl
{
	template <typename T> std::string operator()(T const &v) const
	{
		std::ostringstream oss;
		oss << v;
		return oss.str();
	}
};

template <typename T> struct GenericSerializerImplType
{
	typedef typename std::conditional<std::is_floating_point<T>::value,
	                                  FloatSerializerImpl,
	                                  GenericSerializerImpl>::type type;
};
}

template <typename T> struct Serializer
{
	std::string operator()(T const &v) const
	{
		typename detail::GenericSerializerImplType<T>::type serializer;
		return serializer(v);
	}
};

template <> struct Serializer<bool>
{
	std::string operator()(bool const &v) const;
};

template <> struct Serializer<QColor>
{
	std::string operator()(QColor const &v) const;
};

template <> struct Serializer<QByteArray>
{
	std::string operator()(QByteArray const &v) const;
};

template <> struct Serializer<QFont>
{
	std::string operator()(QFont const &v) const;
};

struct SerializedValue
{
	std::string value;

	template <typename T> SerializedValue(T const &v);
};

template <typename T> SerializedValue::SerializedValue(T const &v) : value()
{
	Serializer<T> s;
	value = s(v);
}
}
}

#endif
