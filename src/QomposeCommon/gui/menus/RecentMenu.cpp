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

#include "RecentMenu.h"

#include <QAction>
#include <QFileInfo>
#include <QMenu>
#include <QStringList>
#include <QVariant>

#include "core/config/Configuration.hpp"

#include "QomposeCommon/gui/GUIUtils.h"

namespace qompose
{
RecentMenu::RecentMenu(QObject *p)
        : QObject(p),
          configWatcher(new qompose::util::ConfigurationWatcher(this)),
          capacity(0),
          menu(nullptr),
          menuActions(QList<QAction *>()),
          recentList(QQueue<QString>())
{
	// Initialize our menu.
	menu = new QMenu(tr("Open Recent"));
	menu->setIcon(gui_utils::getIconFromTheme("document-open"));

	// Load our list's size from the settings instance.
	setCapacity(
	        qompose::core::config::instance().get().recent_list_size_max());

	// Load our list's contents from the settings instance.
	auto items = qompose::core::config::instance().get().recent_list();
	setListContents(items.begin(), items.end());

	QObject::connect(
	        configWatcher,
	        &qompose::util::ConfigurationWatcher::configurationFieldChanged,
	        this, &RecentMenu::doSettingChanged);
}

RecentMenu::~RecentMenu()
{
	delete menu;
}

int RecentMenu::getCapacity() const
{
	return capacity;
}

QMenu *RecentMenu::getMenu() const
{
	return menu;
}

void RecentMenu::saveContents()
{
	auto config = qompose::core::config::instance().get();
	config.clear_recent_list();
	for(int i = 0; i < recentList.count(); ++i)
		config.add_recent_list(recentList.at(i).toStdString());
	qompose::core::config::instance().set(config);
}

void RecentMenu::addPath(const QString &p)
{
	// If we already have this path, remove it first (move to the top).

	for(int i = 0; i < recentList.count(); ++i)
	{
		if(recentList.at(i) == p)
		{
			recentList.removeAt(i);
			break;
		}
	}

	// Add the item to our list.

	recentList.enqueue(p);

	while(recentList.count() > getCapacity())
	{
		if(recentList.isEmpty())
			break;

		recentList.dequeue();
	}

	updateActionsListSize();

	// Save our new list, which will also re-render it.

	saveContents();
}

void RecentMenu::updateActionsListSize()
{
	// Make sure our items list isn't too long.

	while(menuActions.count() > recentList.count())
	{
		QAction *a = menuActions.takeLast();
		menu->removeAction(a);
		delete a;
	}

	// Make sure our items list isn't too short.

	while(menuActions.count() < recentList.count())
	{
		QAction *a = new QAction(menu);

		menu->addAction(a);

		QObject::connect(a, SIGNAL(triggered(bool)), this,
		                 SLOT(doActionClicked()));

		menuActions.append(a);
	}
}

void RecentMenu::setCapacity(int c)
{
	capacity = c;

	// Make sure our list of paths isn't too long.

	while(recentList.count() > capacity)
	{
		if(recentList.isEmpty())
			break;

		recentList.dequeue();
	}

	// Update the size of our list of actions.

	updateActionsListSize();

	// (Re-)render the list of paths.

	renderListContents();
}

void RecentMenu::renderListContents()
{
	for(int i = (recentList.count() - 1); i >= 0; --i)
	{
		int j = recentList.count() - 1 - i;
		QFileInfo f(recentList.at(i));

		menuActions.at(j)->setText(
		        QString("%1. %2").arg(j + 1).arg(f.fileName()));
	}
}

void RecentMenu::doActionClicked()
{
	// Get the action that was clicked.

	QObject *s = sender();
	QAction *action = dynamic_cast<QAction *>(s);

	if(action == NULL)
		return;

	// Find the index of the action in our actions list.

	int j = menuActions.indexOf(action);

	if(j == -1)
		return;

	// Emit the recentClicked signal with the item's path.

	int i = recentList.count() - 1 - j;

	Q_EMIT recentClicked(recentList.at(i));
}

void RecentMenu::doSettingChanged(std::string const &name)
{
	if(name == "recent_list_size_max")
	{
		setCapacity(qompose::core::config::instance()
		                    .get()
		                    .recent_list_size_max());
	}
	else if(name == "recent_list")
	{
		auto items =
		        qompose::core::config::instance().get().recent_list();
		setListContents(items.begin(), items.end());
	}
}

namespace menu_desc
{
RecentMenuDescriptor::RecentMenuDescriptor(
        const gui_utils::ConnectionList &sigc,
        const gui_utils::ConnectionList &slotc)
        : signalConnections(sigc), slotConnections(slotc)
{
}

RecentMenuDescriptor::RecentMenuDescriptor(const gui_utils::Connection &sigc,
                                           const gui_utils::Connection &slotc)
        : signalConnections(), slotConnections()
{
	if(sigc.first != nullptr && sigc.second != nullptr)
		signalConnections.push_back(sigc);

	if(slotc.first != nullptr && slotc.second != nullptr)
		slotConnections.push_back(slotc);
}

template <>
void constructDescriptor(QObject *parent, QMenu *menu,
                         const RecentMenuDescriptor &descriptor)
{
	RecentMenu *recentMenu = new RecentMenu(parent);

	gui_utils::connectAll(recentMenu,
	                      SIGNAL(recentClicked(const QString &)),
	                      descriptor.signalConnections);
	gui_utils::connectAll(descriptor.slotConnections, recentMenu,
	                      SLOT(addPath(const QString &)));

	menu->addMenu(recentMenu->getMenu());
}
}
}
