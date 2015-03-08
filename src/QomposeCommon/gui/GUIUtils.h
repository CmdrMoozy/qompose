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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_GUI_UTILS_H
#define INCLUDE_QOMPOSECOMMON_GUI_GUI_UTILS_H

#include <utility>
#include <vector>

#include <QIcon>
#include <QObject>
#include <QString>

namespace qompose
{
namespace gui_utils
{
typedef std::pair<const QObject *, const char *> Connection;
typedef std::vector<Connection> ConnectionList;

QString translate(const std::string &s);

struct ConnectionFunctor
{
	const QObject *object;

	ConnectionFunctor(const QObject *o);
	Connection operator()(const char *connection) const;
};

void connectAll(const QObject *sender, const char *signal,
                const ConnectionList &slots);
void connectAll(const ConnectionList &signals, const QObject *receiver,
                const char *slot);

QIcon getIconFromTheme(const QString &n);
}
}

#endif
