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
namespace editor
{
class DecoratedTextEdit;

/*!
 * \brief This class implements our gutter widget.
 */
class Gutter : public QWidget
{
public:
	/*!
	 * This is our default constructor, which creates a new gutter for the
	 * given editor object.
	 *
	 * \param e The editor this gutter is attached to.
	 */
	Gutter(DecoratedTextEdit *e);

	Gutter(const Gutter &) = delete;
	virtual ~Gutter() = default;

	Gutter &operator=(const Gutter &) = delete;

	/*!
	 * This function sets the editor component this gutter is attached to.
	 *
	 * \param e The editor we should attach ourself to.
	 */
	void setEditor(DecoratedTextEdit *e);

	/*!
	 * This function returns the width of our gutter.
	 *
	 * \return The gutter's width.
	 */
	virtual int width() const;

	/*!
	 * This function returns the size hint for our gutter, to let our
	 * parent widget know how wide to render this gutter widget.
	 *
	 * \return The size hint for this gutter.
	 */
	virtual QSize sizeHint() const;

protected:
	/*!
	 * This function handles our gutter's paint event by passing the event
	 * up to our parent editor, so it can render the gutter based upon the
	 * editor's state.
	 *
	 * \param e The paint event being handled.
	 */
	virtual void paintEvent(QPaintEvent *e);

private:
	DecoratedTextEdit *editor;
};
}
}

#endif
