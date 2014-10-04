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

#ifndef INCLUDE_QOMPOSE_FONT_METRICS_H
#define INCLUDE_QOMPOSE_FONT_METRICS_H

#include <QFont>

/*!
 * \brief This class provides some various font metric utility functions.
 */
class QomposeFontMetrics
{
	public:
		QomposeFontMetrics(const QFont &f);
		virtual ~QomposeFontMetrics() = default;

		QFont getFont() const;
		void setFont(const QFont &f);

		bool isMonospaced() const;

	private:
		QFont font;
};

#endif
