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

#include "FontMetrics.h"

#include <QFontInfo>
#include <QFontMetricsF>

namespace qompose
{
FontMetrics::FontMetrics(const QFont &f) : font(f)
{
}

FontMetrics::~FontMetrics()
{
}

QFont FontMetrics::getFont() const
{
	return font;
}

void FontMetrics::setFont(const QFont &f)
{
	font = f;
}

bool FontMetrics::isMonospaced() const
{
	QFontInfo into(font);

	return into.fixedPitch();
}

qreal FontMetrics::getColumnWidthF(int columns) const
{
	QFontMetricsF metrics(font);
	return (metrics.averageCharWidth() * static_cast<qreal>(columns));
}

int FontMetrics::getColumnWidth(int columns) const
{
	return qRound(getColumnWidthF(columns));
}

qreal FontMetrics::getWidthF(const QString &t) const
{
	QFontMetricsF metrics(font);
	return metrics.width(t);
}

int FontMetrics::getWidth(const QString &t) const
{
	return qRound(getWidthF(t));
}

QString FontMetrics::ellipsizedText(const QString &t, qreal w,
                                    Qt::TextElideMode m, int f) const
{
	QFontMetricsF metrics(font);
	return metrics.elidedText(t, m, w, f);
}
}
