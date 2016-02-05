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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_MENUS_MENU_DESCRIPTORS_H
#define INCLUDE_QOMPOSECOMMON_GUI_MENUS_MENU_DESCRIPTORS_H

#include <string>
#include <utility>

#include <QAction>
#include <QKeySequence>
#include <QMenu>
#include <QString>

#include <mapbox_variant/recursive_wrapper.hpp>
#include <mapbox_variant/variant.hpp>

#include "QomposeCommon/gui/GUIUtils.h"

namespace qompose
{
namespace menu_desc
{
/*!
 * This template function should be specialized for each descriptor type to
 * create a new object from the descriptor, and to add it to the menu.
 *
 * \param parent The new object's parent.
 * \param menu The menu to add the new object to.
 * \param descriptor The descriptor to create the new object from.
 */
template <typename DescriptorType>
void constructDescriptor(QObject *parent, QMenu *menu,
                         const DescriptorType &descriptor);

/*!
 * \brief This class provides a variant visitor for a menu descriptor variant.
 *
 * This class provides a variant visitor which will call the appropriate
 * constructDescriptor() function for the variant, depending on its type.
 */
class MenuDescriptorVisitor : public mapbox::util::static_visitor<>
{
public:
	/*!
	 * This constructor creates a new menu descriptor visitor, which will
	 * construct objects from descriptors using the given parent and menu.
	 *
	 * \param p The parent for the new objects.
	 * \param m The menu to add the new objects to.
	 */
	MenuDescriptorVisitor(QObject *p, QMenu *m);

	MenuDescriptorVisitor(const MenuDescriptorVisitor &) = default;
	~MenuDescriptorVisitor() = default;

	MenuDescriptorVisitor &
	operator=(const MenuDescriptorVisitor &) = default;

	/*!
	 * This call operator will construct a new object from the given
	 * descriptor, as well as the object and menu given to this visitor in
	 * its constructor.
	 *
	 * \param descriptor The descriptor to create an object from.
	 */
	template <typename Descriptor>
	void operator()(const Descriptor &descriptor) const
	{
		constructDescriptor(parent, menu, descriptor);
	}

private:
	QObject *parent;
	QMenu *menu;
};

/*!
 * \brief This structure provides a descriptor for menu items (QActions).
 */
struct MenuItemDescriptor
{
	QString text;
	QKeySequence shortcut;
	QString icon;
	gui_utils::ConnectionList signalConnections;
	bool checkable;
	bool defaultChecked;
	gui_utils::ConnectionList slotConnections;

	/*!
	 * This constructor creates a new MenuItemDescriptor with the given
	 * properties and connections for its triggered(bool) signal.
	 *
	 * \param t The text for this menu item.
	 * \param sigc The connections for this item's triggered(bool) signal.
	 * \param s The hotkey for this menu item.
	 * \param i The icon for this menu item (see getIconFromTheme).
	 * \param chk Whether or not this item should be checkable.
	 * \param dchk Whether or not this item should be checked by default.
	 * \param slotc The connections for this menu item's setChecked slot.
	 */
	MenuItemDescriptor(const std::string &t,
	                   const gui_utils::ConnectionList &sigc =
	                           gui_utils::ConnectionList(),
	                   QKeySequence s = QKeySequence(),
	                   const QString &i = QString(), bool chk = false,
	                   bool dchk = false,
	                   const gui_utils::ConnectionList &slotc =
	                           gui_utils::ConnectionList());

	/*!
	 * This constructor creates a new MenuItemDescriptor with the given
	 * properties and connection for its triggered(bool) signal.
	 *
	 * \param t The text for this menu item.
	 * \param sigc The connection for this menu item's triggered signal.
	 * \param s The hotkey for this menu item.
	 * \param i The icon for this menu item (see getIconFromTheme).
	 * \param chk Whether or not this item should be checkable.
	 * \param dchk Whether or not this item should be checked by default.
	 * \param slotc The connection for this menu item's setChecked slot.
	 */
	MenuItemDescriptor(const std::string &t,
	                   const gui_utils::Connection &sigc =
	                           gui_utils::Connection(nullptr, nullptr),
	                   QKeySequence s = QKeySequence(),
	                   const QString &i = QString(), bool chk = false,
	                   bool dchk = false,
	                   const gui_utils::Connection &slotc =
	                           gui_utils::Connection(nullptr, nullptr));

	MenuItemDescriptor(const MenuItemDescriptor &) = default;
	~MenuItemDescriptor() = default;

	MenuItemDescriptor &operator=(const MenuItemDescriptor &) = default;
};

/*!
 * We specialize this function in order to create a menu item from a
 * descriptor, and add it to the given menu.
 *
 * \param parent The parent for the new menu item.
 * \param menu The menu to add the new menu item to.
 * \param descriptor The descriptor to create the new menu item from.
 */
template <>
void constructDescriptor(QObject *parent, QMenu *menu,
                         const MenuItemDescriptor &descriptor);

/*!
 * \brief This structure provides a descriptor for a menu separator.
 */
struct SeparatorDescriptor
{
};

/*!
 * We specialize this function in order to create a menu separator from a
 * descriptor, and add it to the given menu.
 *
 * \param parent The parent for the new menu item.
 * \param menu The menu to add the new separator to.
 * \param descriptor The descriptor to create the new separator from.
 */
template <>
void constructDescriptor(QObject *parent, QMenu *menu,
                         const SeparatorDescriptor &descriptor);
}
}

#endif
