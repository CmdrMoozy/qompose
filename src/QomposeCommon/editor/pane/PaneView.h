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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_PANE_PANE_VIEW_H
#define INCLUDE_QOMPOSECOMMON_EDITOR_PANE_PANE_VIEW_H

#include <cstddef>
#include <utility>
#include <vector>

#include <QMetaObject>
#include <QObject>
#include <QWidget>

namespace qompose
{
class Pane;
class PaneModel;

class PaneView : public QWidget
{
	Q_OBJECT

public:
	PaneView(PaneModel *m, QWidget *p = nullptr, Qt::WindowFlags f = 0);
	virtual ~PaneView();

	Pane *currentPane() const;

	bool hasCurrentPane() const;

	void setCurrentPane(std::size_t i);

	/*!
	 * This function prepares all of this view's buffers to be closed. See
	 * Buffer::prepareToClose() for details on what this means for each
	 * buffer.
	 *
	 * This function iterates over each buffer in the model, and stops
	 * when any of those buffers return false. If all buffers are prepared
	 * successfully, then true is returned.
	 *
	 * \return True if this buffer can now be closed, or false otherwise.
	 */
	bool prepareToClose();

	template <typename Signaler, typename... Arg>
	QMetaObject::Connection
	connectToCurrentPane(const Signaler *signaler,
	                     void (Signaler::*signal)(Arg...),
	                     void (Pane::*slot)(Arg...),
	                     Qt::ConnectionType type = Qt::AutoConnection);

private:
	PaneModel *model;
	Pane *current;

	std::vector<QMetaObject::Connection> connections;

Q_SIGNALS:
	void currentPaneChanged(Pane *);
};

template <typename Signaler, typename... Arg>
QMetaObject::Connection PaneView::connectToCurrentPane(
        const Signaler *signaler, void (Signaler::*signal)(Arg...),
        void (Pane::*slot)(Arg...), Qt::ConnectionType type)
{
	QMetaObject::Connection connection = QObject::connect(
	        signaler, signal, [this, slot](Arg... arg)
	        {
		        if(current == nullptr)
			        return;
		        ((current).*(slot))(std::forward<Arg>(arg)...);
		},
	        type);
	connections.push_back(connection);
	return connection;
}
}

#endif
