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

#include <QWidget>

#include "QomposeCommon/editor/pane/PaneView.h"

class QGridLayout;
class QStackedLayout;
class QTabBar;

namespace qompose
{
class PaneModel;

class TabbedPaneView : public PaneView
{
	Q_OBJECT

public:
	TabbedPaneView(PaneModel *m, QWidget *p = nullptr,
	               Qt::WindowFlags f = 0);

	virtual ~TabbedPaneView() = default;

private:
	PaneModel *model;

	QGridLayout *layout;
	QTabBar *tabBar;
	QStackedLayout *currentTabLayout;

private Q_SLOTS:
	void doCurrentChanged(int index);
	void doTabMoved(int from, int to);
	void doCloseRequested(int index);
};
}
