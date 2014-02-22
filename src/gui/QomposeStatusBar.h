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

#ifndef INCLUDE_QOMPOSE_STATUS_BAR_H
#define INCLUDE_QOMPOSE_STATUS_BAR_H

#include <QStatusBar>

class QGridLayout;
class QLabel;

class QomposeStatusBar : public QStatusBar
{
	Q_OBJECT

	public:
		QomposeStatusBar(QWidget *p = 0);
		virtual ~QomposeStatusBar();

		void setCurrentTabPath(const QString &p);

	private:
		QWidget *statusWidget;
		QGridLayout *statusLayout;

		QLabel *tabPathLabel;
};

#endif
