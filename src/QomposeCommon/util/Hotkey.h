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

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_HOTKEY_H
#define INCLUDE_QOMPOSECOMMON_UTIL_HOTKEY_H

#include <Qt>

class QKeyEvent;

namespace qompose
{

/*!
 * \brief This class denotes a single hotkey Key/Modifiers combination.
 *
 * Given the combination of a key, the required modifiers for the hotkey, and
 * the allowed (or whitelisted) modifiers for the hotkey, we can test if some
 * given QKeyEvent matches this hotkey.
 *
 * Note that, because more than one Hotkey can match a single QKeyEvent, we can
 * also comput a "match quality" for a given event. See the documentation on
 * Hotkey::matches for more information.
 */
class Hotkey
{
public:
	Hotkey(Qt::Key k);
	Hotkey(Qt::Key k, Qt::KeyboardModifiers rm);
	Hotkey(Qt::Key k, Qt::KeyboardModifiers rm, Qt::KeyboardModifiers wlm);
	Hotkey(const Hotkey &o);
	virtual ~Hotkey();

	Hotkey &operator=(const Hotkey &o);
	bool operator==(const Hotkey &o) const;

	Qt::Key getKey() const;
	quint64 getKeyInteger() const;
	Qt::KeyboardModifiers getRequiredModifiers() const;
	Qt::KeyboardModifiers getWhitelistedModifiers() const;

	int matches(const QKeyEvent *e) const;

private:
	Qt::Key key;
	Qt::KeyboardModifiers rModifiers;
	Qt::KeyboardModifiers wlModifiers;

	static int opop(quint64 v);
};

}

#endif
