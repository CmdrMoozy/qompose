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
/*!
 * \brief This object implements a "recently opened files" menu.
 */
class RecentMenu : public QObject
{
	Q_OBJECT

public:
	/*!
	 * This is our default constructor, which creates a new instance of our
	 * class. Our capacity and our contents will be automatically loaded
	 * from the global settings instance.
	 *
	 * \param p This menu's parent object.
	 */
	RecentMenu(QObject *p = nullptr);

	RecentMenu(const RecentMenu &) = delete;

	/*!
	 * This is our default destructor, which deletes this recent menu's
	 * internal menu object.
	 */
	virtual ~RecentMenu();

	RecentMenu &operator=(const RecentMenu &) = delete;

	/*!
	 * This function returns our recent menu's capacity.
	 *
	 * \return The current capacity of our menu.
	 */
	int getCapacity() const;

	/*!
	 * This function returns a pointer to the QMenu instance which contains
	 * our list of items. Note that we retain ownership of this menu - our
	 * caller should probably not alter it, and DEFINITELY shouldn't delete
	 * it.
	 *
	 * \return A pointer to our internal QMenu instance.
	 */
	QMenu *getMenu() const;

	/*!
	 * This function saves our menu's current contents using the global
	 * settings instance.
	 */
	void saveContents();

public Q_SLOTS:
	/*!
	 * This function adds the given path to our recent menu.
	 *
	 * If this would make our list of recent items exceed our capacity,
	 * then the oldest item(s) in the list will be removed to make room for
	 * this new item.
	 *
	 * If this item is already in our list, then it will simply be moved
	 * to the top of the list.
	 *
	 * \param p The path to add to our list.
	 */
	void addPath(const QString &p);

private:
	int capacity;

	QMenu *menu;
	QList<QAction *> menuActions;

	QQueue<QString> recentList;

	/*!
	 * This function updates our actions list's size, for instance after
	 * our capacity or list of recent items has been changed.
	 *
	 * If our actions list is larger or smaller than our recent items list,
	 * then new actions will be removed or added (respectively) to make
	 * them the same size.
	 *
	 * Note that this function does NOT alter the text of the actions; this
	 * should be done by renderListContents() AFTER calling this function.
	 */
	void updateActionsListSize();

	/*!
	 * This function alters our list's capacity. If our existing list of
	 * recent items is larger than this new capacity, the oldest item(s)
	 * will be removed until our recent list's size is equal to our
	 * capacity.
	 *
	 * After this is done, we update the size of our actions list, and then
	 * we update the text of the actions to match our new items list.
	 *
	 * \param c The new capacity for our menu.
	 */
	void setCapacity(int c);

	/*!
	 * This function renders our list contents by setting the appropriate
	 * text for each action, based upon the contents of our recent items
	 * list.
	 */
	void renderListContents();

	/*!
	 * This function sets our list contents to the given list. It is
	 * assumed that the given list is in order such that the LAST item in
	 * the given list is the MOST recent item.
	 *
	 * If the given list is larger than our capacity, then after the items
	 * are all added to our list then we will remove the oldest item(s)
	 * until the recent list size is the same as our capacity.
	 *
	 * \param l Our new list of items.
	 */
	void setListContents(const QStringList &l);

private Q_SLOTS:
	/*!
	 * This function handles an action being clicked by determining which
	 * action was clicked, and then emitting an appropriate "recentClicked"
	 * signal with the path corresponding to the action that was clicked.
	 */
	void doActionClicked();

	/*!
	 * This slot handles a settings value being changed by, if it's a
	 * setting we care about (namely, the recent-list-size and recent-list
	 * settings), then we will update our internal state to match that
	 * setting's new value.
	 *
	 * \param k The key of the setting that was changed.
	 * \param v The setting's new value.
	 */
	void doSettingChanged(const QString &k, const QVariant &v);

Q_SIGNALS:
	void recentClicked(const QString &);
};

namespace menu_desc
{
/*!
 * \brief This structure defines a descriptor for constructing recent menus.
 */
struct RecentMenuDescriptor
{
	gui_utils::ConnectionList signalConnections;
	gui_utils::ConnectionList slotConnections;

	/*!
	 * This constructor creates a new recent menu descriptor, connecting
	 * its signal and slot to the given lists of connections.
	 *
	 * \param sigc The connections for the recent menu's signal.
	 * \param slotc The connections for the recent menu's slot.
	 */
	RecentMenuDescriptor(const gui_utils::ConnectionList &sigc =
	                             gui_utils::ConnectionList(),
	                     const gui_utils::ConnectionList &slotc =
	                             gui_utils::ConnectionList());

	/*!
	 * This constructor creates a new recent menu descriptor, connecting
	 * its signal and slot to the given connections, respectively.
	 *
	 * \param sigc The connection for the recent menu's signal.
	 * \param slotc The connection for the recent menu's slot.
	 */
	RecentMenuDescriptor(const gui_utils::Connection &sigc =
	                             gui_utils::Connection(nullptr, nullptr),
	                     const gui_utils::Connection &slotc =
	                             gui_utils::Connection(nullptr, nullptr));

	RecentMenuDescriptor(const RecentMenuDescriptor &) = default;
	~RecentMenuDescriptor() = default;
	RecentMenuDescriptor &operator=(const RecentMenuDescriptor &) = default;
};

/*!
 * We specialize this function in order to create a recent menu from a
 * descriptor, and to add it to the given menu.
 *
 * \param parent The parent for the new recent menu.
 * \param menu The menu to add the new recent menu to.
 * \param descriptor The descriptor to create the new recent menu from.
 */
template <>
void constructDescriptor(QObject *parent, QMenu *menu,
                         const RecentMenuDescriptor &descriptor);
}
}

#endif
