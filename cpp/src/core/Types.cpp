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

#include "Types.hpp"

#include <cassert>

#include "Configuration.pb.h"

namespace qompose
{
namespace core
{
IndentationMode indentationModeFromMessage(int m)
{
	if(m == qompose::core::messages::Configuration::INDENTATION_SPACES)
		return IndentationMode::Spaces;
	else if(m == qompose::core::messages::Configuration::INDENTATION_TABS)
		return IndentationMode::Tabs;
	assert(false);
	return IndentationMode::Tabs;
}

int indentationModeToMessage(IndentationMode m)
{
	switch(m)
	{
	case IndentationMode::Spaces:
		return qompose::core::messages::Configuration::
		        INDENTATION_SPACES;
	case IndentationMode::Tabs:
		return qompose::core::messages::Configuration::INDENTATION_TABS;
	}
}
}
}
