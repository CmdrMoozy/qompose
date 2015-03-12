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

#include <boost/variant/static_visitor.hpp>

#include "QomposeCommon/gui/GUIUtils.h"

namespace qompose
{
namespace menu_desc
{
template <typename DescriptorType>
void constructDescriptor(QObject *parent, QMenu *menu,
                         const DescriptorType &descriptor);

class MenuDescriptorVisitor : public boost::static_visitor<void>
{
public:
	MenuDescriptorVisitor(QObject *p, QMenu *m);

	MenuDescriptorVisitor(const MenuDescriptorVisitor &) = delete;
	MenuDescriptorVisitor &
	operator=(const MenuDescriptorVisitor &) = delete;

	template <typename Descriptor>
	void operator()(const Descriptor &descriptor) const
	{
		constructDescriptor(parent, menu, descriptor);
	}

private:
	QObject *parent;
	QMenu *menu;
};

struct MenuItemDescriptor
{
	QString text;
	QKeySequence shortcut;
	QString icon;
	gui_utils::ConnectionList connections;
	bool checkable;

	MenuItemDescriptor(
	        const std::string &,
	        const gui_utils::ConnectionList & = gui_utils::ConnectionList(),
	        QKeySequence = QKeySequence(), const QString & = QString(),
	        bool chk = false);
	MenuItemDescriptor(const std::string &,
	                   const gui_utils::Connection & =
	                           gui_utils::Connection(nullptr, nullptr),
	                   QKeySequence = QKeySequence(),
	                   const QString & = QString(), bool chk = false);
};

struct SeparatorDescriptor
{
};

template <>
void constructDescriptor(QObject *parent, QMenu *menu,
                         const MenuItemDescriptor &descriptor);

template <>
void constructDescriptor(QObject *, QMenu *menu, const SeparatorDescriptor &);
}
}

#endif
