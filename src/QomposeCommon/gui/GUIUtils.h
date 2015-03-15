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

/*!
 * This function provides a concise way to use Qt's tr() function on a
 * standard string.
 *
 * \param s The string to translat.
 * \return The translated string.
 */
QString translate(const std::string &s);

/*!
 * \brief This structure provides a function to make Connection pairs.
 *
 * The operator() this structure provides is equivalent to calling:
 *
 *     std::make_pair(object, connection)
 *
 * This structure simply provides a much more concise way to do this, so one
 * doesn't need to repeat as much code when building a large list of
 * connections.
 */
struct ConnectionFunctor
{
	const QObject *object;

	/*!
	 * Create a new ConnectionFunctor which will create Connections for the
	 * given object.
	 *
	 * \param o The object to create connections for.
	 */
	ConnectionFunctor(const QObject *o);

	/*!
	 * This function will create a new Connection with the object given to
	 * our constructor, and the given signal or slot created using Qt's
	 * SIGNAL or SLOT macros.
	 *
	 * \param connection The signal or slot to create a connection with.
	 * \return The resulting connection pair.
	 */
	Connection operator()(const char *connection) const;
};

/*!
 * This function connects the given object's signal to all of the given
 * connections (either signals or slots).
 *
 * \param sender The object which sends the signal.
 * \param signal The signal to connect.
 * \param connections The list of connections to connect to the signal.
 */
void connectAll(const QObject *sender, const char *signal,
                const ConnectionList &connections);

/*!
 * This function connects all of the given signals to the given object's
 * signal or slot.
 *
 * \param signals The list of signals to connect to the given signal or slot.
 * \param receiver The object to connect each signal to.
 * \param connection The signal or slot to connect each signal to.
 */
void connectAll(const ConnectionList &signals, const QObject *receiver,
                const char *connection);

/*!
 * This function will return the icon for the given standard name. We try to
 * use an icon from the operating system's icon theme, but failing that, we
 * try to use a fallback from our internal icon store.
 *
 * If no icon can be found, a null QIcon is returned instead.
 *
 * The default names supported by QIcon (and, therefore, this function) can be
 * found here:
 *
 *     http://standards.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html
 *
 * \param n The name of the icon to find.
 * \return An icon object corresponding to the given name.
 */
QIcon getIconFromTheme(const QString &n);
}
}

#endif
