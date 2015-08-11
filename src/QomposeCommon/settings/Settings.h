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

#ifndef INCLUDE_QOMPOSECOMMON_SETTINGS_SETTINGS_H
#define INCLUDE_QOMPOSECOMMON_SETTINGS_SETTINGS_H

#include <memory>
#include <mutex>
#include <string>

#include "QomposeCommon/compat/Optional.h"
#include "QomposeCommon/settings/Deserializer.h"
#include "QomposeCommon/settings/Serializer.h"

namespace qompose
{
namespace settings
{
std::string getDefaultSettingsPath();

class Settings
{
public:
	static void
	initialize(std::string const &path = getDefaultSettingsPath());
	static void destroy();

	static Settings &instance();

	Settings(Settings const &) = delete;
	Settings(Settings &&) = delete;
	Settings &operator=(Settings const &) = delete;
	Settings &operator=(Settings &&) = delete;

	~Settings() = default;

	bool contains(std::string const &k) const;

	std::experimental::optional<std::string>
	tryGet(std::string const &k) const;
	std::string get(std::string const &k) const;

	template <typename T> T getAs(std::string const &k) const;
	template <typename T>
	T getAsOr(std::string const &k, T const &defaultValue) const;

	void put(std::string const &k, std::string const &v);

	template <typename T> void putAs(std::string const &k, T const &v);

	void remove(std::string const &k);

	void resetToDefaults();
	void resetToDefault(std::string const &k);

private:
	static std::mutex mutex;
	static std::unique_ptr<Settings> singleton;

	struct SettingsImpl;
	std::unique_ptr<SettingsImpl> impl;

	Settings(std::string const &path);
};

template <typename T> T Settings::getAs(std::string const &k) const
{
	Deserializer<T> d;
	return d(get(k));
}

template <typename T>
T Settings::getAsOr(std::string const &k, T const &defaultValue) const
{
	auto v = tryGet(k);
	if(!v)
		return defaultValue;
	Deserializer<T> d;
	return d(*v);
}

template <typename T> void Settings::putAs(std::string const &k, T const &v)
{
	Serializer<T> s;
	put(k, s(v));
}
}
}

#endif
