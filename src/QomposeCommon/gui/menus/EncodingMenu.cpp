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

#include "EncodingMenu.h"

#include <stdexcept>

#include <QAction>
#include <QActionGroup>
#include <QByteArray>
#include <QList>
#include <QTextCodec>

#include "QomposeCommon/gui/GUIUtils.h"

namespace
{
typedef std::set<qompose::EncodingMenu::EncodingMenuItem,
                 qompose::EncodingMenu::EncodingMenuItemComparator>
        EncodingSet_t;

EncodingSet_t buildEncodingMenuItems(QObject *p, QActionGroup *group)
{
	EncodingSet_t encodings;

	QList<QByteArray> codecs = QTextCodec::availableCodecs();
	for(auto it = codecs.begin(); it != codecs.end(); ++it)
	{
		qompose::EncodingMenu::EncodingMenuItem item;
		item.name = std::string(it->data());
		item.action = nullptr;

		QTextCodec *codec = QTextCodec::codecForName(*it);
		if(codec == nullptr)
		{
			throw std::runtime_error(
			        "Populating encoding menu failed.");
		}

		// If this codec is a duplicate for some already-added codec,
		// we want to skip it (and not add it to the menu).
		QList<QByteArray> aliases = codec->aliases();
		bool duplicate = false;
		for(auto aliasIt = aliases.begin(); aliasIt != aliases.end();
		    ++aliasIt)
		{
			qompose::EncodingMenu::EncodingMenuItem aliasItem;
			aliasItem.name = std::string(aliasIt->data());
			aliasItem.action = nullptr;

			if(encodings.find(aliasItem) != encodings.end())
			{
				duplicate = true;
				break;
			}
		}

		if(!duplicate)
		{
			item.action =
			        new QAction(QObject::tr(item.name.c_str()), p);
			item.action->setActionGroup(group);
			item.action->setCheckable(true);
			encodings.insert(item);
		}
	}

	return encodings;
}
}

namespace qompose
{
EncodingMenu::EncodingMenu(const QString &label, QObject *p)
        : QObject(p), menu(new QMenu(label)), group(new QActionGroup(this))
{
	menu->setStyleSheet("QMenu { menu-scrollable: 1; }");

	EncodingSet_t encodingSet = buildEncodingMenuItems(this, group);
	for(const auto &encoding : encodingSet)
	{
		menu->addAction(encoding.action);
		QObject::connect(encoding.action, SIGNAL(toggled(bool)), this,
		                 SLOT(doEncodingChanged()));
	}
}

EncodingMenu::~EncodingMenu()
{
	delete menu;
}

QMenu *EncodingMenu::getMenu() const
{
	return menu;
}

void EncodingMenu::setEncoding(const QString &e)
{
	EncodingMenuItem item;
	item.name = std::string(e.toLatin1().data());
	item.action = nullptr;
	auto it = encodings.find(item);
	if(it != encodings.end())
		it->action->setChecked(true);
}

bool EncodingMenu::EncodingMenuItemComparator::
operator()(const EncodingMenuItem &a, const EncodingMenuItem &b)
{
	return a.name < b.name;
}

void EncodingMenu::doEncodingChanged()
{
	for(const auto &encoding : encodings)
	{
		if(encoding.action->isChecked())
		{
			Q_EMIT(encodingChanged(
			        QString::fromStdString(encoding.name)));
		}
	}
}

namespace menu_desc
{
EncodingMenuDescriptor::EncodingMenuDescriptor(const std::string &l)
        : label(gui_utils::translate(l))
{
}

template <>
void constructDescriptor(QObject *parent, QMenu *menu,
                         const EncodingMenuDescriptor &descriptor)
{
	EncodingMenu *encodingMenu = new EncodingMenu(descriptor.label, parent);
	menu->addMenu(encodingMenu->getMenu());
}
}
}
