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

#include "Application.h"

#include <QLocalServer>

#include "QomposeCommon/Defines.h"
#include "QomposeCommon/Window.h"

namespace qompose
{
Application::Application(int &ac, char **av)
        : QApplication(ac, av), sappServer(nullptr), windows(QList<Window *>())
{
}

Application::~Application()
{
	clearServer();
}

void Application::initializeLocalServer()
{
	clearServer();
	sappServer = new QLocalServer();

	if(!sappServer->listen(QOMPOSE_GUID))
	{
		/*
		 * If we got to this point, it means that we didn't get a
		 * response from the person that created our socket, but it is
		 * still present for some reason. Try removing it; if that
		 * doesn't work, crash.
		 */

		QLocalServer::removeServer(QOMPOSE_GUID);

		if(!sappServer->listen(QOMPOSE_GUID))
			qApp->quit();
	}

	QObject::connect(sappServer, SIGNAL(newConnection()), this,
	                 SLOT(doDuplicateInstanceDetected()));
}

void Application::clearServer()
{
	if(sappServer != nullptr)
	{
		delete sappServer;
		sappServer = nullptr;
	}
}

void Application::doDuplicateInstanceDetected()
{
	Window::openNewWindow();
}
}
