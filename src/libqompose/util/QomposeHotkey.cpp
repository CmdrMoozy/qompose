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

#include "QomposeHotkey.h"

#include <QKeyEvent>

/*!
 * This constructor creates a new hotkey instance which matches the given key,
 * while not requiring (or allowing) any modifiers.
 *
 * \param k The key this hotkey should match.
 */
QomposeHotkey::QomposeHotkey(Qt::Key k)
	: key(k), rModifiers(Qt::KeyboardModifiers(Qt::NoModifier)),
		wlModifiers(Qt::KeyboardModifiers(Qt::NoModifier))
{
}

/*!
 * This constructor creates a new hotkey instance which matches the given key
 * and modifiers combination. No extra modifiers will be whitelisted.
 *
 * \param k The key this hotkey should match.
 * \param rm The modifiers required for this hotkey to match.
 */
QomposeHotkey::QomposeHotkey(Qt::Key k, Qt::KeyboardModifiers rm)
	: key(k), rModifiers(rm), wlModifiers(rm)
{
	key = k;
	rModifiers = rm;
	wlModifiers = rm;
}

/*!
 * This constructor creates a new hotkey instance which matches the gien key
 * and modifiers combination, and allows the given set of other modifiers as
 * well.
 *
 * Note that we ensure that the whitelisted modifiers list contains at least the
 * same modifiers as the required modifiers list. This is ensured via doing a
 * logical OR between the given required and whitelisted modifiers lists.
 *
 * \param k The key this hotkey should match.
 * \param rm The modifiers required for this hotkey to match.
 * \param wlm Additional modifiers which are allowed when matching this hotkey.
 */
QomposeHotkey::QomposeHotkey(Qt::Key k, Qt::KeyboardModifiers rm,
	Qt::KeyboardModifiers wlm)
	: key(k), rModifiers(rm), wlModifiers(wlm | rm)
{
}

/*!
 * This is our copy constructor, which creates a new hotkey instance that is
 * identical to the given other hotkey instance.
 *
 * \param o The other hotkey to create a copy of.
 */
QomposeHotkey::QomposeHotkey(const QomposeHotkey &o)
	: key(Qt::Key_Escape),
		rModifiers(Qt::KeyboardModifiers(Qt::NoModifier)),
		wlModifiers(Qt::KeyboardModifiers(Qt::NoModifier))
{
	*this = o;
}

/*!
 * This is our default destructor, which cleans up and destroys this hotkey
 * instance.
 */
QomposeHotkey::~QomposeHotkey()
{
}

/*!
 * This is our assignment operator, which makes our internal values identical
 * to the given other hotkey (i.e., we become a copyof it).
 *
 * \param o The other hotkey to copy.
 * \return A reference to this, for operator chaining.
 */
QomposeHotkey &QomposeHotkey::operator=(const QomposeHotkey &o)
{
	if(&o == this)
		return *this;

	key = o.key;
	rModifiers = o.rModifiers;
	wlModifiers = o.wlModifiers;

	return *this;
}

/*!
 * This is our equivalence operator, which tests if this hotkey is equal to the
 * given other hotkey. That is, for this to return true, the two hotkeys must
 * be completely identical (including whitelisted modifiers).
 *
 * \param o The other hotkey to compare ourself to.
 * \return True if we are equal, or false otherwise.
 */
bool QomposeHotkey::operator==(const QomposeHotkey &o) const
{
	return (key == o.key) &&
		(rModifiers == o.rModifiers) &&
		(wlModifiers == o.wlModifiers);
}

/*!
 * This function returns the key portion of our hotkey.
 *
 * \return Our hotkey's key.
 */
Qt::Key QomposeHotkey::getKey() const
{
	return key;
}

/*!
 * This function returns the key portion of our hotkey, as an unsigned integer.
 * This is useful for e.g. using our key value as a lookup in a data structure
 * which doesn't natively deal with Qt::Key values (e.g., QHash).
 *
 * \return Our hotkey's key, as an unsigned integer.
 */
quint64 QomposeHotkey::getKeyInteger() const
{
	return static_cast<quint64>(key);
}

/*!
 * This function returns the keyboard modifiers that must be enabled in any key
 * event this hotkey matches.
 *
 * \return Our hotkey's required keyboard modifiers.
 */
Qt::KeyboardModifiers QomposeHotkey::getRequiredModifiers() const
{
	return rModifiers;
}

/*!
 * This function returns the modifiers that are whitelisted for this hotkey. In
 * other words, this is the list of all modifiers which we allow to be on for
 * key events we should match.
 *
 * For instance, consider the simple hotkey:
 *
 *     Enter
 *
 * However, sometimes the user happens to be holding shift (e.g., when adding a
 * new line between characters that should be capitalized). One could whitlist
 * the shift modifier, so that this hotkey would match both of the following:
 *
 *     Enter
 *     Shift + Enter
 *
 * Note: by definition, this list ALWAYS includes at least all of the required
 * modifiers for this hotkey.
 *
 * \return The keyboard modifiers whitelist for this hotkey.
 */
Qt::KeyboardModifiers QomposeHotkey::getWhitelistedModifiers() const
{
	return wlModifiers;
}

/*!
 * This function tests if (and how well) this hotkey matches the given key
 * event.
 *
 * We return a score indicating how well this hotkey matches. Lower scores are
 * better, as in 0 is a perfect match (i.e., there were no extra key modifiers).
 * However, if this hotkey doesn't match the given key event at all, then we
 * will return -1 instead.
 *
 * \param e The key event to compare to this hotkey.
 * \return If (and how well) this hotkey matches the given key event.
 */
int QomposeHotkey::matches(const QKeyEvent *e) const
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

	return QomposeHotkey::opop(extra);
}

/*!
 * This is a simple utility function which counts the number of one bits set in
 * the given value. The algorithm used is fairly well known, and tends to be
 * very fast on CPU's with fast multiplication instructions. For more
 * information, see:
 *
 *     http://en.wikipedia.org/wiki/Hamming_weight
 *
 * This function is used to compute the number of extra bits set in one set of
 * flags when comparing it to another set of flags.
 *
 * \param v The value to count the number of one bits in.
 * \return The number of one bits set in the given value.
 */
int QomposeHotkey::opop(quint64 v)
{
	v -= (v >> 1) & Q_UINT64_C(0x5555555555555555);

	v = (v & Q_UINT64_C(0x3333333333333333)) +
		((v >> 2) & Q_UINT64_C(0x3333333333333333));

	v = (v + (v >> 4)) & Q_UINT64_C(0x0F0F0F0F0F0F0F0F);

	return (v * Q_UINT64_C(0x0101010101010101)) >> 56;
}
