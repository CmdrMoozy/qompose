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

#include "Hotkey.h"

#include <QKeyEvent>

namespace qompose
{
Hotkey::Hotkey(Qt::Key k)
        : key(k),
          rModifiers(Qt::KeyboardModifiers(Qt::NoModifier)),
          wlModifiers(Qt::KeyboardModifiers(Qt::NoModifier))
{
}

Hotkey::Hotkey(Qt::Key k, Qt::KeyboardModifiers rm)
        : key(k), rModifiers(rm), wlModifiers(rm)
{
	key = k;
	rModifiers = rm;
	wlModifiers = rm;
}

Hotkey::Hotkey(Qt::Key k, Qt::KeyboardModifiers rm, Qt::KeyboardModifiers wlm)
        : key(k), rModifiers(rm), wlModifiers(wlm | rm)
{
}

bool Hotkey::operator==(const Hotkey &o) const
{
	return (key == o.key) && (rModifiers == o.rModifiers) &&
	       (wlModifiers == o.wlModifiers);
}

Qt::Key Hotkey::getKey() const
{
	return key;
}

quint64 Hotkey::getKeyInteger() const
{
	return static_cast<quint64>(key);
}

Qt::KeyboardModifiers Hotkey::getRequiredModifiers() const
{
	return rModifiers;
}

Qt::KeyboardModifiers Hotkey::getWhitelistedModifiers() const
{
	return wlModifiers;
}

int Hotkey::matches(const QKeyEvent *e) const
{
	// If the key doesn't match, bail out now.

	if(e->key() != key)
		return -1;

	// If any required modifiers are missing, then no match.

	if((e->modifiers() & rModifiers) != rModifiers)
		return -1;

	// If the event has modifiers which aren't whitelisted, no match.

	if((e->modifiers() & ~wlModifiers) != 0)
		return -1;

	// We match - figure out how well we match!

	Qt::KeyboardModifiers em = wlModifiers & ~rModifiers;
	quint64 extra = static_cast<quint64>(e->modifiers() & em);

	return Hotkey::opop(extra);
}

int Hotkey::opop(quint64 v)
{
	v -= (v >> 1) & Q_UINT64_C(0x5555555555555555);

	v = (v & Q_UINT64_C(0x3333333333333333)) +
	    ((v >> 2) & Q_UINT64_C(0x3333333333333333));

	v = (v + (v >> 4)) & Q_UINT64_C(0x0F0F0F0F0F0F0F0F);

	return (v * Q_UINT64_C(0x0101010101010101)) >> 56;
}
}
