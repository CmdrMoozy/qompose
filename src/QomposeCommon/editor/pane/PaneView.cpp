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

#include "PaneView.h"

#include "QomposeCommon/editor/Buffer.h"
#include "QomposeCommon/editor/pane/Pane.h"
#include "QomposeCommon/editor/pane/PaneModel.h"

namespace qompose
{
PaneView::PaneView(PaneModel *m, QWidget *p, Qt::WindowFlags f)
        : QWidget(p, f), model(m), current(nullptr), connections()
{
}

PaneView::~PaneView()
{
	while(!connections.empty())
	{
		QObject::disconnect(connections.back());
		connections.pop_back();
	}
}

Pane *PaneView::currentPane() const
{
	return current;
}

bool PaneView::hasCurrentPane() const
{
	return current != nullptr;
}

void PaneView::setCurrentPane(std::size_t i)
{
	current = model->paneAt(i);
	Q_EMIT currentPaneChanged(model->paneAt(i));
}

bool PaneView::prepareToClose()
{
	for(std::size_t i = 0; i < model->count(); ++i)
	{
		setCurrentPane(i);
		if(!model->paneAt(i)->getBuffer()->prepareToClose())
			return false;
	}

	return true;
}
}
