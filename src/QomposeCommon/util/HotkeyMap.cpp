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
/*!
 * This is our default constructor, which creates a new hotkey map instance.
 */
HotkeyMap::HotkeyMap() : hotkeys()
{
}

/*!
 * This function adds the given hotkey to the map, with the given F value. When
 * the given hotkey is the best match for a particular QKeyEvent, our
 * getHotkeyHandler() function will return the given F.
 *
 * Note that if the map already contains an identical hotkey to the one given,
 * then we will simply update its associated F value instead of inserting.
 *
 * \param h The hotkey to add to the map.
 * \param f The hotkey handler associated with this hotkey.
 * \return True if newly added, or false if merely updated.
 */
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

/*!
 * This function returns the hotkey handler associated with the hotkey which is
 * the best match for the given KeyEvent. If no hotkey in the map matches the
 * given key event, then an instance of std::exception is thrown instead.
 *
 * \param e The event to find a matching hotkey for.
 * \return The handler associated with the best-matching hotkey.
 */
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
