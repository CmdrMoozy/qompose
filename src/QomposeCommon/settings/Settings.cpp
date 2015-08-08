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

#include "Settings.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <map>
#include <sstream>
#include <stdexcept>
#include <utility>

#include <QDir>

#include <leveldb/db.h>
#include <leveldb/write_batch.h>

namespace
{
#ifdef QOMPOSE_DEBUG
const std::string CONFIGURATION_FILE_NAME{"QomposeDebug.conf"};
#else
const std::string CONFIGURATION_FILE_NAME{"Qompose.conf"};
#endif

// clang-format off
const std::map<std::string, qompose::settings::SerializedValue> DEFAULT_SETTINGS{
	{"show-file-in-title", true},
	{"show-status-bar", true},
	{"recent-list-size", 10},
	{"recent-list", std::vector<std::string>()},
	{"window-save-attributes", true},
	{"show-gutter", true},
	{"save-strip-trailing-spaces", true},
	{"editor-font", QFont("Courier", 11)},
	{"editor-indentation-width", 8},
	{"editor-indentation-mode", "tabs"},
	{"editor-wrap-guide-visible", true},
	{"editor-wrap-guide-width", 90},
	{"editor-wrap-guide-color", QColor(127, 127, 127)},
	{"editor-foreground", QColor(255, 255, 255)},
	{"editor-background", QColor(39, 40, 34)},
	{"editor-current-line", QColor(70, 72, 61)},
	{"gutter-foreground", QColor(255, 255, 255)},
	{"gutter-background", QColor(0, 0, 0)},
	{"show-file-browser", false},
	{"window-geometry", QByteArray()},
	{"window-state", QByteArray()},
};
// clang-format on

void initializeWithDefaults(leveldb::DB *db)
{
	leveldb::WriteBatch batch;
	for(auto const &kv : DEFAULT_SETTINGS)
	{
		std::string v;
		leveldb::Status s =
		        db->Get(leveldb::ReadOptions(), kv.first, &v);
		if(!s.ok())
			batch.Put(kv.first, kv.second.value);
	}

	leveldb::Status s = db->Write(leveldb::WriteOptions(), &batch);
	if(!s.ok())
	{
		throw std::runtime_error(
		        "Initializing settings with defaults failed.");
	}
}
}

namespace qompose
{
namespace settings
{
std::string getDefaultSettingsPath()
{
	std::string path;

#if defined(_WIN32) || defined(_WIN64)
	char const *configPath = std::getenv("APPDATA");
	if(!configPath)
		configPath = std::getenv("COMMON_APPDATA");
	if(!configPath)
	{
		throw std::runtime_error(
		        "Computing default settings path failed.");
	}
	path.assign(configPath);
#else
	char const *configPath = std::getenv("XDG_CONFIG_HOME");
	if(configPath)
	{
		path.assign(configPath);
	}
	else
	{
		path = QDir::homePath().toStdString() +
		       QDir::separator().toLatin1() + ".config";
	}
#endif

	return path + QDir::separator().toLatin1() + CONFIGURATION_FILE_NAME;
}

std::mutex Settings::mutex;
std::unique_ptr<Settings> Settings::singleton;

struct Settings::SettingsImpl
{
	leveldb::DB *db;

	SettingsImpl(std::string const &path) : db(nullptr)
	{
		leveldb::Options options;
		options.create_if_missing = true;

		leveldb::Status status = leveldb::DB::Open(options, path, &db);
		if(!status.ok())
			throw std::runtime_error(status.ToString());
	}

	~SettingsImpl()
	{
		assert(!!db);
		delete db;
	}
};

void Settings::initialize(std::string const &path)
{
	std::lock_guard<std::mutex> lock(mutex);
	assert(!singleton);
	singleton.reset(new Settings(path));
}

void Settings::destroy()
{
	std::lock_guard<std::mutex> lock(mutex);
	assert(!!singleton);
	singleton.reset();
}

Settings &Settings::instance()
{
	std::lock_guard<std::mutex> lock(mutex);
	assert(!!singleton);
	return *singleton;
}

bool Settings::contains(std::string const &k) const
{
	std::string v;
	leveldb::Status s = impl->db->Get(leveldb::ReadOptions(), k, &v);
	return s.ok();
}

std::string Settings::get(std::string const &k) const
{
	std::string v;
	leveldb::Status s = impl->db->Get(leveldb::ReadOptions(), k, &v);
	if(!s.ok())
	{
		std::ostringstream oss;
		oss << "Retrieving value for settings key '" << k
		    << "' failed.";
		throw std::runtime_error(oss.str());
	}
	return v;
}

void Settings::put(std::string const &k, std::string const &v)
{
	leveldb::Status s = impl->db->Put(leveldb::WriteOptions(), k, v);
	if(!s.ok())
	{
		std::ostringstream oss;
		oss << "Setting value for settings key '" << k << "' failed.";
		throw std::runtime_error(oss.str());
	}
}

void Settings::remove(std::string const &k)
{
	leveldb::Status s = impl->db->Delete(leveldb::WriteOptions(), k);
	if(!s.ok())
	{
		std::ostringstream oss;
		oss << "Deleting value for settings key '" << k << "' failed.";
		throw std::runtime_error(oss.str());
	}
}

void Settings::resetToDefaults()
{
	leveldb::WriteBatch batch;
	for(auto const &kv : DEFAULT_SETTINGS)
		batch.Put(kv.first, kv.second.value);
	impl->db->Write(leveldb::WriteOptions(), &batch);
}

void Settings::resetToDefault(std::string const &k)
{
	auto it = DEFAULT_SETTINGS.find(k);
	if(it == DEFAULT_SETTINGS.end())
		return;
	put(k, it->second.value);
}

Settings::Settings(std::string const &path) : impl(new SettingsImpl(path))
{
	initializeWithDefaults(impl->db);
}
}
}
