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

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_HOTKEY_MAP_H
#define INCLUDE_QOMPOSECOMMON_UTIL_HOTKEY_MAP_H

#include <exception>
#include <functional>
#include <map>
#include <utility>

#include "QomposeCommon/util/Hotkey.h"

namespace qompose
{
/*!
 * \brief This class implements a mapping of hotkeys to handlers.
 *
 * We map hotkeys to templated "handler function" values. Our template
 * parameter could be e.g. function pointers, or some other type of values
 * which allows our caller to take some action based on QKeyEvents.
 *
 * Internally, this class uses QHash as a lookup table, so we can lookup
 * hotkey handlers in amortized O(1) time.
 *
 * Note that a hotkey "matches" a key event if the Qt::Key and required
 * Qt::KeyboardModifiers match. This means that our list can contain more than
 * one match for a given key event. If this occurs, we'll return the hotkey
 * that is the CLOSEST match - that is, the one with the SMALLEST number of
 * "whitelisted" modifiers in addition to the required ones. For example, if we
 * have the two hotkeys:
 *
 *     Ctrl + S
 *     Ctrl + Shift + S
 *
 * And we are given a QKeyEvent for the keystroke:
 *
 *     Ctrl + S
 *
 * Then we'll return the value for the Ctrl + S hotkey, even though both of the
 * hotkeys are matches.
 */
class HotkeyMap
{
public:
	/*!
	 * This is our default constructor, which creates a new hotkey map
	 * instance.
	 */
	HotkeyMap();

	HotkeyMap(const HotkeyMap &) = default;
	~HotkeyMap() = default;

	HotkeyMap &operator=(const HotkeyMap &) = default;

	/*!
	 * This function adds the given hotkey to the map, with the given
	 * function. When the given hotkey is the best match for a particular
	 * QKeyEvent, our getHotkeyHandler() function will return the given
	 * function.
	 *
	 * Note that if the map already contains an identical hotkey to the one
	 * given, then we will simply update its associated F value instead of
	 * inserting.
	 *
	 * \param h The hotkey to add to the map.
	 * \param f The hotkey handler associated with this hotkey.
	 * \return True if newly added, or false if merely updated.
	 */
	bool addHotkey(const Hotkey &h, const std::function<void()> &f);

	/*!
	 * This function returns the hotkey handler associated with the hotkey
	 * which is the best match for the given KeyEvent. If no hotkey in the
	 * map matches the given key event, then an instance of std::exception
	 * is thrown instead.
	 *
	 * \param e The event to find a matching hotkey for.
	 * \return The handler associated with the best-matching hotkey.
	 */
	const std::function<void()> *getHotkeyHandler(const QKeyEvent *e) const;

private:
	typedef std::pair<Hotkey, std::function<void()>> HotkeyPair;
	typedef std::multimap<uint64_t, HotkeyPair> HotkeyContainer;

	HotkeyContainer hotkeys;
};
}

#endif
