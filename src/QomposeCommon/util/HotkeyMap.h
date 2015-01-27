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
	HotkeyMap();
	~HotkeyMap() = default;

	bool addHotkey(const Hotkey &h, const std::function<void()> &f);
	const std::function<void()> *getHotkeyHandler(const QKeyEvent *e) const;

private:
	typedef std::pair<Hotkey, std::function<void()>> HotkeyPair;
	typedef std::multimap<uint64_t, HotkeyPair> HotkeyContainer;

	HotkeyContainer hotkeys;
};
}

#endif
