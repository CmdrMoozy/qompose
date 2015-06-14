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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_PANE_PANE_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_PANE_PANE_H

#include <QWidget>

class QGridLayout;

namespace qompose
{
class Buffer;
class BufferWidget;
class StatusBar;

class Pane : public QWidget
{
	Q_OBJECT

public:
	Pane(BufferWidget *p, Qt::WindowFlags f = 0);

	Pane(const Pane &) = delete;
	~Pane() = default;
	Pane &operator=(const Pane &) = delete;

	BufferWidget *getParentContainer() const;

	Buffer *getBuffer() const;

private:
	QGridLayout *layout;
	Buffer *buffer;
	StatusBar *statusBar;

private Q_SLOTS:
	void doCursorPositionChanged();
};
}

#endif
