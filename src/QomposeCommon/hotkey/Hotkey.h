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

#ifndef INCLUDE_QOMPOSECOMMON_HOTKEY_HOTKEY_H
#define INCLUDE_QOMPOSECOMMON_HOTKEY_HOTKEY_H

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
	/*!
	 * This constructor creates a new hotkey instance which matches the
	 * given key, while not requiring (or allowing) any modifiers.
	 *
	 * \param k The key this hotkey should match.
	 */
	Hotkey(Qt::Key k);

	/*!
	 * This constructor creates a new hotkey instance which matches the
	 * given key and modifiers combination. No extra modifiers will be
	 * whitelisted.
	 *
	 * \param k The key this hotkey should match.
	 * \param rm The modifiers required for this hotkey to match.
	 */
	Hotkey(Qt::Key k, Qt::KeyboardModifiers rm);

	/*!
	 * This constructor creates a new hotkey instance which matches the
	 * given key and modifiers combination, and allows the given set of
	 * other modifiers as well.
	 *
	 * Note that we ensure that the whitelisted modifiers list contains at
	 * least the same modifiers as the required modifiers list. This is
	 * ensured via doing a logical OR between the given required and
	 * whitelisted modifiers lists.
	 *
	 * \param k The key this hotkey should match.
	 * \param rm The modifiers required for this hotkey to match.
	 * \param wlm Additional modifiers which are allowed when matching.
	 */
	Hotkey(Qt::Key k, Qt::KeyboardModifiers rm, Qt::KeyboardModifiers wlm);

	Hotkey(const Hotkey &o) = default;
	~Hotkey() = default;

	Hotkey &operator=(const Hotkey &o) = default;

	/*!
	 * This is our equivalence operator, which tests if this hotkey is
	 * equal to the given other hotkey. That is, for this to return true,
	 * the two hotkeys must be completely identical (including whitelisted
	 * modifiers).
	 *
	 * \param o The other hotkey to compare ourself to.
	 * \return True if we are equal, or false otherwise.
	 */
	bool operator==(const Hotkey &o) const;

	/*!
	 * This function returns the key portion of our hotkey.
	 *
	 * \return Our hotkey's key.
	 */
	Qt::Key getKey() const;

	/*!
	 * This function returns the key portion of our hotkey, as an unsigned
	 * integer. This is useful for e.g. using our key value as a lookup in
	 * a data structure which doesn't natively deal with Qt::Key values
	 * (e.g., QHash).
	 *
	 * \return Our hotkey's key, as an unsigned integer.
	 */
	quint64 getKeyInteger() const;

	/*!
	 * This function returns the keyboard modifiers that must be enabled in
	 * any key event this hotkey matches.
	 *
	 * \return Our hotkey's required keyboard modifiers.
	 */
	Qt::KeyboardModifiers getRequiredModifiers() const;

	/*!
	 * This function returns the modifiers that are whitelisted for this
	 * hotkey. In other words, this is the list of all modifiers which we
	 * allow to be on for key events we should match.
	 *
	 * For instance, consider the simple hotkey:
	 *
	 *     Enter
	 *
	 * However, sometimes the user happens to be holding shift (e.g., when
	 * adding a new line between characters that should be capitalized).
	 * One could whitlist the shift modifier, so that this hotkey would
	 * match both of the following:
	 *
	 *     Enter
	 *     Shift + Enter
	 *
	 * Note: by definition, this list ALWAYS includes at least all of the
	 * required modifiers for this hotkey.
	 *
	 * \return The keyboard modifiers whitelist for this hotkey.
	 */
	Qt::KeyboardModifiers getWhitelistedModifiers() const;

	/*!
	 * This function tests if (and how well) this hotkey matches the given
	 * key event.
	 *
	 * We return a score indicating how well this hotkey matches. Lower
	 * scores are better, as in 0 is a perfect match (i.e., there were no
	 * extra key modifiers). However, if this hotkey doesn't match the
	 * given key event at all, then we will return -1 instead.
	 *
	 * \param e The key event to compare to this hotkey.
	 * \return If (and how well) this hotkey matches the given key event.
	 */
	int matches(const QKeyEvent *e) const;

private:
	Qt::Key key;
	Qt::KeyboardModifiers rModifiers;
	Qt::KeyboardModifiers wlModifiers;

	/*!
	 * This is a simple utility function which counts the number of one
	 * bits set in the given value. The algorithm used is fairly well
	 * known, and tends to be very fast on CPU's with fast multiplication
	 * instructions. For more information, see:
	 *
	 *     http://en.wikipedia.org/wiki/Hamming_weight
	 *
	 * This function is used to compute the number of extra bits set in one
	 * set of flags when comparing it to another set of flags.
	 *
	 * \param v The value to count the number of one bits in.
	 * \return The number of one bits set in the given value.
	 */
	static int opop(quint64 v);
};
}

#endif
