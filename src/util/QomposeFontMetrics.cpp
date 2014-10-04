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

#include "QomposeFontMetrics.h"

#include <QFontInfo>

/*!
 * This is our default constructor, which creates a new instance of our font
 * metrics class.
 *
 * \param f The font we will be examining.
 */
QomposeFontMetrics::QomposeFontMetrics(const QFont &f)
{
	font = f;
}

/*!
 * This function returns the font this object is currently examining.
 *
 * \return Our current font.
 */
QFont QomposeFontMetrics::getFont() const
{
	return font;
}

/*!
 * This function sets the font our object will examine from here on.
 *
 * \param f The new font to examine.
 */
void QomposeFontMetrics::setFont(const QFont &f)
{
	font = f;
}

/*!
 * This function returns whether or not our current font is monospaced.
 *
 * \return True if our current font is monospaced, or false otherwise.
 */
bool QomposeFontMetrics::isMonospaced() const
{
	QFontInfo into(font);

	return into.fixedPitch();
}
