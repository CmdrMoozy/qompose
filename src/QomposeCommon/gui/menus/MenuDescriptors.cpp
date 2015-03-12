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

#include "MenuDescriptors.h"

#include <QObject>

namespace qompose
{
namespace menu_desc
{
MenuDescriptorVisitor::MenuDescriptorVisitor(QObject *p, QMenu *m)
        : parent(p), menu(m)
{
}

MenuItemDescriptor::MenuItemDescriptor(const std::string &t,
                                       const gui_utils::ConnectionList &c,
                                       QKeySequence s, const QString &i,
                                       bool chk)
        : text(gui_utils::translate(t)),
          shortcut(s),
          icon(i),
          connections(c),
          checkable(chk)
{
}

MenuItemDescriptor::MenuItemDescriptor(const std::string &t,
                                       const gui_utils::Connection &c,
                                       QKeySequence s, const QString &i,
                                       bool chk)
        : text(gui_utils::translate(t)),
          shortcut(s),
          icon(i),
          connections(),
          checkable(chk)
{
	if(c.first != nullptr && c.second != nullptr)
		connections.push_back(c);
}

template <>
void constructDescriptor(QObject *parent, QMenu *menu,
                         const MenuItemDescriptor &descriptor)
{
	QAction *action = new QAction(descriptor.text, parent);

	if(!descriptor.shortcut.isEmpty())
		action->setShortcut(descriptor.shortcut);

	if(!(descriptor.icon.isNull() || descriptor.icon.isEmpty()))
		action->setIcon(gui_utils::getIconFromTheme(descriptor.icon));

	gui_utils::connectAll(action, SIGNAL(triggered(bool)),
	                      descriptor.connections);

	if(descriptor.checkable)
	{
		action->setCheckable(true);
		action->setChecked(false);
	}

	menu->addAction(action);
}

template <>
void constructDescriptor(QObject *, QMenu *menu, const SeparatorDescriptor &)
{
	menu->addSeparator();
}
}
}
