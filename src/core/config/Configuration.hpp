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

#ifndef qompose_core_config_Configuration_HPP
#define qompose_core_config_Configuration_HPP

#include <string>

#include <boost/optional/optional.hpp>

#include <QColor>
#include <QFont>

#include <bdrck/config/Configuration.hpp>

#include "Configuration.pb.h"

namespace qompose
{
namespace core
{
namespace config
{
class ConfigurationInstance
{
public:
	ConfigurationInstance(
	        boost::optional<std::string> const &customPath = boost::none);

private:
	bdrck::config::ConfigurationInstance<
	        qompose::core::messages::Configuration>
	        instanceHandle;
};

bdrck::config::Configuration<qompose::core::messages::Configuration> &
instance();

QColor toQColor(qompose::core::messages::Color const &c);
QFont toQFont(qompose::core::messages::Font const &f);
void fromQColor(qompose::core::messages::Color *dest, QColor const &c);
void fromQFont(qompose::core::messages::Font *dest, QFont const &f);

std::string
toString(qompose::core::messages::Configuration::IndentationMode im);
}
}
}

#endif
