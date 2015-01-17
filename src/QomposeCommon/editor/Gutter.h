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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_GUTTER_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_GUTTER_H

#include <QWidget>
#include <QSize>

class QPaintEvent;

namespace qompose
{
class DecoratedTextEdit;

/*!
 * \brief This class implements our gutter widget.
 */
class Gutter : public QWidget
{
public:
	Gutter(DecoratedTextEdit *e);
	virtual ~Gutter();

	void setEditor(DecoratedTextEdit *e);

	virtual int width() const;
	virtual QSize sizeHint() const;

protected:
	virtual void paintEvent(QPaintEvent *e);

private:
	DecoratedTextEdit *editor;

	Gutter(const Gutter &);
	Gutter &operator=(const Gutter &);
};
}

#endif
