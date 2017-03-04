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

#include "GUIUtils.h"

#include <QFile>

namespace qompose
{
namespace gui_utils
{
QString translate(const std::string &s)
{
	return QObject::tr(s.c_str());
}

ConnectionFunctor::ConnectionFunctor(const QObject *o) : object(o)
{
}

Connection ConnectionFunctor::operator()(const char *connection) const
{
	return std::make_pair(object, connection);
}

void connectAll(const QObject *sender, const char *signal,
                const ConnectionList &connections)
{
	if(sender == nullptr || signal == nullptr)
		return;

	for(const auto &connection : connections)
	{
		QObject::connect(sender, signal, connection.first,
		                 connection.second);
	}
}

void connectAll(const ConnectionList &signals, const QObject *receiver,
                const char *connection)
{
	if(receiver == nullptr || connection == nullptr)
		return;

	for(const auto &signal : signals)
	{
		QObject::connect(signal.first, signal.second, receiver,
		                 connection);
	}
}

QIcon getIconFromTheme(const QString &n)
{
	QIcon defaultIcon;

	QString defaultPath = n;
	defaultPath.prepend(":/icons/");
	defaultPath.append(".png");

	if(QFile::exists(defaultPath))
		defaultIcon = QIcon(defaultPath);
	else
		qDebug("Couldn't find icon: %s", qPrintable(n));

	return QIcon::fromTheme(n, defaultIcon);
}
}
}
