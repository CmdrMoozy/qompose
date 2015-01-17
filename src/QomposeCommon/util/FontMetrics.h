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

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_FONT_METRICS_H
#define INCLUDE_QOMPOSECOMMON_UTIL_FONT_METRICS_H

#include <QFont>

namespace qompose
{
/*!
 * \brief This class provides some various font metric utility functions.
 */
class FontMetrics
{
public:
	FontMetrics(const QFont &f);
	virtual ~FontMetrics();

	QFont getFont() const;
	void setFont(const QFont &f);

	bool isMonospaced() const;

	int getColumnWidth(int columns = 1) const;
	qreal getColumnWidthF(int columns = 1) const;

private:
	QFont font;
};
}

#endif
