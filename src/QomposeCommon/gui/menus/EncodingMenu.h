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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_MENUS_ENCODING_MENU_H
#define INCLUDE_QOMPOSECOMMON_GUI_MENUS_ENCODING_MENU_H

#include <set>
#include <string>

#include <QObject>
#include <QString>

#include "QomposeCommon/gui/menus/MenuDescriptors.h"

class QActionGroup;
class QMenu;

namespace qompose
{
/*!
 * \brief This menu allows the user to select a character encoding.
 */
class EncodingMenu : public QObject
{
	Q_OBJECT

public:
	struct EncodingMenuItem
	{
		std::string name;
		QAction *action;
	};

	struct EncodingMenuItemComparator
	{
		bool operator()(const EncodingMenuItem &a,
		                const EncodingMenuItem &b);
	};

	EncodingMenu(const QString &label, QObject *p = nullptr);

	EncodingMenu(const EncodingMenu &) = delete;

	virtual ~EncodingMenu();

	EncodingMenu &operator=(const EncodingMenu &) = delete;

	QMenu *getMenu() const;

public Q_SLOTS:
	void setEncoding(const QByteArray &e);

private:
	QMenu *menu;
	QActionGroup *group;
	std::set<EncodingMenuItem, EncodingMenuItemComparator> encodings;

private Q_SLOTS:
	void doEncodingChanged();

Q_SIGNALS:
	void encodingChanged(const QByteArray &);
};

namespace menu_desc
{
/*!
 * \brief This structure defines a descriptor for constructing encoding menus.
 */
struct EncodingMenuDescriptor
{
	QString label;
	gui_utils::ConnectionList signalConnections;
	gui_utils::ConnectionList slotConnections;

	EncodingMenuDescriptor(const std::string &l,
	                       const gui_utils::Connection &sigc =
	                               gui_utils::Connection(nullptr, nullptr),
	                       const gui_utils::Connection &slotc =
	                               gui_utils::Connection(nullptr, nullptr));

	EncodingMenuDescriptor(const EncodingMenuDescriptor &) = default;
	~EncodingMenuDescriptor() = default;

	EncodingMenuDescriptor &
	operator=(const EncodingMenuDescriptor &) = default;
};

/*!
 * We specialize this function in order to create an encoding menu from a
 * descriptor, and to add it to the given menu.
 *
 * \param parent The parent for the new encoding menu.
 * \param menu The menu to add the new encoding menu to.
 * \param descriptor The descriptor to create the new encoding menu from.
 */
template <>
void constructDescriptor(QObject *parent, QMenu *menu,
                         const EncodingMenuDescriptor &descriptor);
}
}

#endif
