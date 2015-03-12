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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_MENUS_RECENT_MENU_H
#define INCLUDE_QOMPOSECOMMON_GUI_MENUS_RECENT_MENU_H

#include <QList>
#include <QObject>
#include <QQueue>
#include <QString>

#include "QomposeCommon/gui/menus/MenuDescriptors.h"

class QMenu;
class QAction;

namespace qompose
{
class Settings;

/*!
 * \brief This object implements a "recently opened files" menu.
 */
class RecentMenu : public QObject
{
	Q_OBJECT

public:
	RecentMenu(Settings *, QObject * = nullptr);
	virtual ~RecentMenu();

	int getCapacity() const;
	QMenu *getMenu() const;

	void saveContents();

public Q_SLOTS:
	void addPath(const QString &);

private:
	Settings *settings;
	int capacity;

	QMenu *menu;
	QList<QAction *> menuActions;

	QQueue<QString> recentList;

	RecentMenu(const RecentMenu &);
	RecentMenu &operator=(const RecentMenu &);

	void updateActionsListSize();
	void setCapacity(int);
	void renderListContents();
	void setListContents(const QStringList &);

private Q_SLOTS:
	void doActionClicked();
	void doSettingChanged(const QString &, const QVariant &);

Q_SIGNALS:
	void recentClicked(const QString &);
};

namespace menu_desc
{
struct RecentMenuDescriptor
{
	Settings *settings;
	gui_utils::ConnectionList signalConnections;
	gui_utils::ConnectionList slotConnections;

	RecentMenuDescriptor(Settings *s,
	                     const gui_utils::ConnectionList &sigc =
	                             gui_utils::ConnectionList(),
	                     const gui_utils::ConnectionList &slotc =
	                             gui_utils::ConnectionList());
	RecentMenuDescriptor(Settings *s,
	                     const gui_utils::Connection &sigc =
	                             gui_utils::Connection(nullptr, nullptr),
	                     const gui_utils::Connection &slotc =
	                             gui_utils::Connection(nullptr, nullptr));

	RecentMenuDescriptor(const RecentMenuDescriptor &) = default;
	RecentMenuDescriptor &operator=(const RecentMenuDescriptor &) = default;
};

template <>
void constructDescriptor(QObject *parent, QMenu *menu,
                         const RecentMenuDescriptor &descriptor);
}
}

#endif
