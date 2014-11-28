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

#include "QomposeRecentMenu.h"

#include <QMenu>
#include <QStringList>
#include <QAction>
#include <QFileInfo>
#include <QVariant>

#include "QomposeCommon/gui/QomposeGUIUtils.h"
#include "QomposeCommon/util/QomposeSettings.h"

/*!
 * This is our default constructor, which creates a new instance of our
 * class. Our capacity and our contents will be automatically loaded from
 * the given settings instance.
 *
 * \param s The settings instance to use.
 * \param p This menu's parent object.
 */
QomposeRecentMenu::QomposeRecentMenu(QomposeSettings *s, QObject *p)
	: QObject(p), settings(s), capacity(0), menu(nullptr),
		menuActions(QList<QAction *>()), recentList(QQueue<QString>())
{
	// Initialize our menu.

	menu = new QMenu(tr("Open Recent"));
	menu->setIcon(QomposeGUIUtils::getIconFromTheme("document-open"));

	// Load our list's size from our settings instance.

	bool ok;
	QVariant vcap = settings->getSetting("recent-list-size");

	int cap = vcap.toInt(&ok);

	if(!ok)
	{
		settings->resetDefault("recent-list-size");
		vcap = settings->getSetting("recent-list-size");
		cap = vcap.toInt(&ok);

		if(!ok)
			cap = 10;
	}

	setCapacity(cap);

	// Load our list's contents from our settings instance.

	QVariant vcontents = settings->getSetting("recent-list");

	if(vcontents.canConvert(QMetaType::QStringList))
		setListContents(vcontents.toStringList());
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
QomposeRecentMenu::~QomposeRecentMenu()
{
	delete menu;
}

/*!
 * This function returns our recent menu's capacity.
 *
 * \return The current capacity of our menu.
 */
int QomposeRecentMenu::getCapacity() const
{
	return capacity;
}

/*!
 * This function returns a pointer to the QMenu instance which contains
 * our list of items. Note that we retain ownership of this menu - our
 * caller should probably not alter it, and DEFINITELY shouldn't delete
 * it.
 *
 * \return A pointer to our internal QMenu instance.
 */
QMenu *QomposeRecentMenu::getMenu() const
{
	return menu;
}

/*!
 * This function adds the given path to our recent menu.
 *
 * If this would make our list of recent items exceed our capacity, then
 * the oldest item(s) in the list will be removed to make room for this
 * new item.
 *
 * If this item is already in our list, then it will simply be moved
 * to the top of the list.
 *
 * \param p The path to add to our list.
 */
void QomposeRecentMenu::addPath(const QString &p)
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

/*!
 * This function saves our menu's current contents using the settings
 * instance given to our constructor.
 */
void QomposeRecentMenu::saveContents()
{
	QStringList l;

	for(int i = 0; i < recentList.count(); ++i)
		l.append(recentList.at(i));

	settings->setSetting("recent-list", QVariant(l));
}

/*!
 * This function updates our actions list's size, for instance after our
 * capacity or list of recent items has been changed.
 *
 * If our actions list is larger or smaller than our recent items list,
 * then new actions will be removed or added (respectively) to make them
 * the same size.
 *
 * Note that this function does NOT alter the text of the actions; this
 * should be done by renderListContents() AFTER calling this function.
 */
void QomposeRecentMenu::updateActionsListSize()
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

		QObject::connect(a, SIGNAL( triggered(bool) ),
			this, SLOT( doActionClicked() ) );

		menuActions.append(a);
	}
}

/*!
 * This function alters our list's capacity. If our existing list of recent
 * items is larger than this new capacity, the oldest item(s) will be removed
 * until our recent list's size is equal to our capacity.
 *
 * After this is done, we update the size of our actions list, and then we
 * update the text of the actions to match our new items list.
 *
 * \param c The new capacity for our menu.
 */
void QomposeRecentMenu::setCapacity(int c)
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

/*!
 * This function renders our list contents by setting the appropriate text
 * for each action, based upon the contents of our recent items list.
 */
void QomposeRecentMenu::renderListContents()
{
	for(int i = (recentList.count() - 1); i >= 0; --i)
	{
		int j = recentList.count() - 1 - i;
		QFileInfo f(recentList.at(i));

		menuActions.at(j)->setText(QString("%1. %2")
			.arg(j + 1).arg(f.fileName()));
	}
}

/*!
 * This function sets our list contents to the given list. It is assumed
 * that the given list is in order such that the LAST item in the given
 * list is the MOST recent item.
 *
 * If the given list is larger than our capacity, then after the items
 * are all added to our list then we will remove the oldest item(s) until
 * the recent list size is the same as our capacity.
 *
 * \param l Our new list of items.
 */
void QomposeRecentMenu::setListContents(const QStringList &l)
{
	// Set our recent list's contents to the given list.

	recentList.clear();

	for(int i = 0; i < l.count(); ++i)
		recentList.enqueue(l.at(i));

	while(recentList.count() > capacity)
	{
		if(recentList.isEmpty())
			break;

		recentList.dequeue();
	}

	// Update our menu actions list size.

	updateActionsListSize();

	// set our contents to the items in the list.

	renderListContents();
}

/*!
 * This function handles an action being clicked by determining which action
 * was clicked, and then emitting an appropriate "recentClicked" signal with
 * the path corresponding to the action that was clicked.
 */
void QomposeRecentMenu::doActionClicked()
{ /* SLOT */

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

/*!
 * This slot handles a settings value being changed by, if it's a setting we
 * care about (namely, the recent-list-size and recent-list settings), then we will
 * update our internal state to match that setting's new value.
 *
 * \param k The key of the setting that was changed.
 * \param v The setting's new value.
 */
void QomposeRecentMenu::doSettingChanged(const QString &k, const QVariant &v)
{ /* SLOT */

	if(k == "recent-list-size")
	{
		bool ok;
		int cap = v.toInt(&ok);

		if(ok)
			setCapacity(cap);
	}
	else if(k == "recent-list")
	{
		if(v.canConvert(QMetaType::QStringList))
			setListContents(v.toStringList());
	}

}
