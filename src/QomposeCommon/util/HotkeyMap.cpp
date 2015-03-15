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

#include "HotkeyMap.h"

namespace qompose
{
HotkeyMap::HotkeyMap() : hotkeys()
{
}

bool HotkeyMap::addHotkey(const Hotkey &h, const std::function<void()> &f)
{
	bool insert = true;

	// Handle being given a duplicate Hotkey.

	HotkeyContainer::iterator it;
	for(it = hotkeys.begin(); it != hotkeys.end(); ++it)
	{
		if(it->second.first == h)
		{
			insert = false;
			it->second.second = f;
			break;
		}
	}

	// Insert this hotkey into the hash.

	if(insert)
	{
		hotkeys.insert(
		        std::make_pair(h.getKeyInteger(), HotkeyPair(h, f)));
	}

	// Done! Return whether or not a new item was inserted.

	return insert;
}

const std::function<void()> *
HotkeyMap::getHotkeyHandler(const QKeyEvent *e) const
{
	const std::function<void()> *handler = nullptr;
	int matchQuality = -1;

	// Search all the values for a best match.

	HotkeyContainer::const_iterator it;
	for(it = hotkeys.cbegin(); it != hotkeys.cend(); ++it)
	{
		int match = it->second.first.matches(e);

		if(match == -1)
			continue;

		if((match < matchQuality) || (matchQuality == -1))
		{
			handler = &it->second.second;
			matchQuality = match;
		}
	}

	return handler;
}
}
