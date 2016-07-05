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

#include <catch/catch.hpp>

#include <boost/optional/optional.hpp>

#include <QDir>

#include <bdrck/fs/TemporaryStorage.hpp>
#include <bdrck/util/ScopeExit.hpp>

#include "QomposeCommon/settings/Settings.h"

namespace
{
struct SettingsTestState
{
	bdrck::fs::TemporaryStorage directory;
	boost::optional<bdrck::util::ScopeExit> cleanup;

	SettingsTestState()
	        : directory(bdrck::fs::TemporaryStorageType::DIRECTORY),
	          cleanup(boost::none)
	{
		QString path = QString::fromStdString(directory.getPath()) +
		               QDir::separator() + "test.conf";
		qompose::settings::Settings::initialize(path.toStdString());
		cleanup.emplace(
		        []() { qompose::settings::Settings::destroy(); });
	}
};
}

TEST_CASE("Test contains, insertion, and removal", "[Settings]")
{
	SettingsTestState state;
	auto &settings = qompose::settings::Settings::instance();

	if(settings.contains("foobar"))
		settings.remove("foobar");

	CHECK(!settings.contains("foobar"));

	settings.put("foobar", "test");
	CHECK(settings.contains("foobar"));
	CHECK(settings.get("foobar") == "test");

	settings.remove("foobar");
	CHECK(!settings.contains("foobar"));
}
