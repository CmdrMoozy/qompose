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

#include <QPair>
#include <QMultiHash>
#include <QList>

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
template <typename F>
class HotkeyMap
{
public:
	/*!
	 * This is our default constructor, which creates a new hotkey
	 * map instance.
	 */
	HotkeyMap()
		: hotkeys(HotkeyHash())
	{
	}

	/*!
	 * This is our default destructor, which cleans up and destroys
	 * this hotkey map instance.
	 */
	virtual ~HotkeyMap()
	{
	}

	/*!
	 * This function adds the given hotkey to the map, with the
	 * given F value. When the given hotkey is the best match for a
	 * particular QKeyEvent, our getHotkeyHandler() function will
	 * return the given F.
	 *
	 * Note that if the map already contains an identical hotkey to
	 * the one given, then we will simply update its associated F
	 * value instead of inserting.
	 *
	 * \param h The hotkey to add to the map.
	 * \param f The hotkey handler associated with this hotkey.
	 * \return True if newly added, or false if merely updated.
	 */
	bool addHotkey(const Hotkey &h, const F &f)
	{
		bool insert = true;

		// Handle being given a duplicate Hotkey.

		typename HotkeyHash::iterator it;

		for(it = hotkeys.begin(); it != hotkeys.end(); ++it)
		{
			if(it.value().first == h)
			{
				insert = false;
				it.value().second = f;
				break;
			}
		}

		// Insert this hotkey into the hash.

		if(insert)
			hotkeys.insertMulti(h.getKeyInteger(),
				HotkeyPair(h, f));

		// Done! Return whether or not a new item was inserted.

		return insert;
	}

	/*!
	 * This function returns the hotkey handler associated with the
	 * hotkey which is the best match for the given KeyEvent. If no
	 * hotkey in the map matches the given key event, then an
	 * instance of std::exception is thrown instead.
	 *
	 * \param e The event to find a matching hotkey for.
	 * \return The handler associated with the best-matching hotkey.
	 */
	const F *getHotkeyHandler(const QKeyEvent *e) const
	{
		const F *handler = nullptr;
		int matchQuality = -1;

		// Search all the values for a best match.

		typename HotkeyHash::const_iterator it;

		for(it = hotkeys.constBegin();
			it != hotkeys.constEnd(); ++it)
		{
			int match = it.value().first.matches(e);

			if(match == -1)
				continue;

			if( (match < matchQuality) ||
				(matchQuality == -1) )
			{
				handler = &it.value().second;
				matchQuality = match;
			}
		}

		// Done!

		return handler;
	}

private:
	typedef QPair<Hotkey, F> HotkeyPair;
	typedef QMultiHash<quint64, HotkeyPair> HotkeyHash;

	HotkeyHash hotkeys;
};

}

#endif
