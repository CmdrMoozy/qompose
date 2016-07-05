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

#ifndef INCLUDE_QOMPOSECOMMON_APPLICATION_H
#define INCLUDE_QOMPOSECOMMON_APPLICATION_H

#include <QApplication>
#include <QList>

#include "core/config/Configuration.hpp"

class QLocalServer;

namespace qompose
{
class Window;

/*!
 * \brief This class extends QApplication, allowing only one instance to exist.
 */
class Application : public QApplication
{
	Q_OBJECT

public:
	/*!
	 * Initializes a new QomposeApplication instance.
	 *
	 * \param ac The number of command-line arguments.
	 * \param av The command-line arguments.
	 */
	Application(int &ac, char **av);

	Application(const Application &) = delete;

	/*!
	 * This is our default destructor, which cleans up our local "single
	 * application server", and destroys all of our open window objects.
	 */
	virtual ~Application();

	Application &operator=(const Application &) = delete;

	/*!
	 * This function initializes our QLocalServer instance by attempting
	 * to listen using our pre-defined GUID value, to ensure that only one
	 * instance of our application is running at a time. Duplicate
	 * instances will be handled by notifying the original process.
	 */
	void initializeLocalServer();

private:
	QLocalServer *sappServer;
	QList<Window *> windows;
	qompose::core::config::ConfigurationInstance config;

	/*!
	 * This function cleans up our local "single application server".
	 */
	void clearServer();

private Q_SLOTS:
	/*!
	 * This slot handles a duplicate instance of our application being
	 * started by simply opening a new window on the existing instance
	 * instead.
	 */
	void doDuplicateInstanceDetected();
};
}

#endif
