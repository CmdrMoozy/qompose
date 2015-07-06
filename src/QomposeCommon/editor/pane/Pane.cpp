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

#include "Pane.h"

#include <stdexcept>

#include <QGridLayout>

#include "QomposeCommon/editor/Buffer.h"
#include "QomposeCommon/gui/BufferWidget.h"
#include "QomposeCommon/gui/StatusBar.h"

namespace qompose
{
Pane::Pane(BufferWidget *pc, QWidget *p, Qt::WindowFlags f)
        : QWidget(p, f),
          parentContainer(pc),
          layout(nullptr),
          buffer(nullptr),
          statusBar(nullptr)
{
	layout = new QGridLayout(this);

	buffer = new editor::Buffer(this);
	statusBar = new StatusBar(this);

	layout->addWidget(buffer, 0, 0, 1, 1);
	layout->addWidget(statusBar, 1, 0, 1, 1);
	layout->setRowStretch(0, 1);
	layout->setColumnStretch(0, 1);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	setLayout(layout);

	QObject::connect(buffer, &editor::Buffer::pathChanged, statusBar,
	                 &StatusBar::setFilePath);
	QObject::connect(buffer, &editor::Buffer::cursorPositionChanged, this,
	                 &Pane::doCursorPositionChanged);
}

BufferWidget *Pane::getParentContainer() const
{
	return parentContainer;
}

editor::Buffer *Pane::getBuffer() const
{
	return buffer;
}

void Pane::doCursorPositionChanged()
{
	statusBar->setCursorPosition(buffer->getCurrentLine(),
	                             buffer->getCurrentColumn());
}
}
