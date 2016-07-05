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

#include "Configuration.hpp"

#include <mutex>

namespace
{
constexpr char const *CONFIG_ID_APPLICATION = "qompose";
constexpr char const *CONFIG_ID_NAME = "core";

bdrck::config::ConfigurationIdentifier getConfigurationIdentifier()
{
	static bdrck::config::ConfigurationIdentifier identifier{
	        CONFIG_ID_APPLICATION, CONFIG_ID_NAME};
	return identifier;
}

qompose::core::messages::Configuration getDefaultConfiguration()
{
	static std::mutex mutex;
	static bool initialized{false};
	static qompose::core::messages::Configuration defaults;

	std::lock_guard<std::mutex> lock(mutex);
	if(!initialized)
	{
		initialized = true;

		defaults.set_show_file_in_title(true);
		defaults.set_show_status_bar(true);
		defaults.set_recent_list_size_max(10);
		defaults.set_window_save_attributes(true);
		defaults.set_show_gutter(true);
		defaults.set_save_strip_trailing_spaces(true);
		defaults.mutable_editor_font()->set_family("Courier");
		defaults.mutable_editor_font()->set_point_size(11);
		defaults.set_editor_indentation_width(8);
		defaults.set_editor_indentation_mode(
		        qompose::core::messages::Configuration::
		                INDENTATION_TABS);
		defaults.set_editor_wrap_guide_visible(true);
		defaults.set_editor_wrap_guide_width(90);
		defaults.mutable_editor_wrap_guide_color()->set_alpha(255);
		defaults.mutable_editor_wrap_guide_color()->set_red(127);
		defaults.mutable_editor_wrap_guide_color()->set_green(127);
		defaults.mutable_editor_wrap_guide_color()->set_blue(127);
		defaults.mutable_editor_foreground()->set_alpha(255);
		defaults.mutable_editor_foreground()->set_red(255);
		defaults.mutable_editor_foreground()->set_green(255);
		defaults.mutable_editor_foreground()->set_blue(255);
		defaults.mutable_editor_background()->set_alpha(255);
		defaults.mutable_editor_background()->set_red(39);
		defaults.mutable_editor_background()->set_green(40);
		defaults.mutable_editor_background()->set_blue(34);
		defaults.mutable_editor_current_line()->set_alpha(255);
		defaults.mutable_editor_current_line()->set_red(70);
		defaults.mutable_editor_current_line()->set_green(72);
		defaults.mutable_editor_current_line()->set_blue(61);
		defaults.mutable_gutter_foreground()->set_alpha(255);
		defaults.mutable_gutter_foreground()->set_red(255);
		defaults.mutable_gutter_foreground()->set_green(255);
		defaults.mutable_gutter_foreground()->set_blue(255);
		defaults.mutable_gutter_background()->set_alpha(255);
		defaults.mutable_gutter_background()->set_red(0);
		defaults.mutable_gutter_background()->set_green(0);
		defaults.mutable_gutter_background()->set_blue(0);
		defaults.set_show_file_browser(true);
	}

	return defaults;
}
}

namespace qompose
{
namespace core
{
namespace config
{
ConfigurationInstance::ConfigurationInstance(
        boost::optional<std::string> const &customPath)
        : instanceHandle(getConfigurationIdentifier(),
                         getDefaultConfiguration(), customPath)
{
}

bdrck::config::Configuration<qompose::core::messages::Configuration> &instance()
{
	return bdrck::config::
	        Configuration<qompose::core::messages::Configuration>::instance(
	                getConfigurationIdentifier());
}
}
}
}
