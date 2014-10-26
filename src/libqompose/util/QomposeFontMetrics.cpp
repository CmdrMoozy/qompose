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
#include <QFontMetricsF>

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
 * This is our default destructor, which cleans up & destroys our object.
 */
QomposeFontMetrics::~QomposeFontMetrics()
{
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

/*!
 * This is a convenience function which returns the width of the given number of
 * columns in our current font as an integer. This is equivalent to rounding the
 * return value of getColumnWidthF() and then casting it to an int.
 *
 * \param columns The number of columns to get the width of.
 * \return The width of the given number of columns.
 */
int QomposeFontMetrics::getColumnWidth(int columns) const
{
	return static_cast<int>(qRound(getColumnWidthF(columns)));
}

/*!
 * This function returns the width, in pixels, of the given number of columns in
 * our current font. Note that this value is as exact as possible - no rounding
 * is done.
 *
 * Also note that this function only returns a useful value if our current font
 * is monospaced. It is up to the caller to ensure that this is the case.
 *
 * \param columns The number of columns to get the width of.
 * \return The width of the given number of columns.
 */
qreal QomposeFontMetrics::getColumnWidthF(int columns) const
{
	QFontMetricsF metrics(font);

	return (metrics.averageCharWidth() * static_cast<qreal>(columns));
}
