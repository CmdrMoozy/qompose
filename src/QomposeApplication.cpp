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

#include "QomposeApplication.h"

#include <cstdlib>

#include <QLocalServer>

#include "QomposeDefines.h"
#include "QomposeWindow.h"

QomposeApplication::QomposeApplication(int &ac, char **av)
	: QApplication(ac, av), sappServer(NULL)
{
}

QomposeApplication::QomposeApplication(int &ac, char **av, bool g)
	: QApplication(ac, av, g), sappServer(NULL)
{
}

QomposeApplication::QomposeApplication(int &ac, char **av, Type t)
	: QApplication(ac, av, t), sappServer(NULL)
{
}

QomposeApplication::QomposeApplication(Display *d, Qt::HANDLE v, Qt::HANDLE c)
	: QApplication(d, v, c), sappServer(NULL)
{
}

QomposeApplication::QomposeApplication(Display *d, int &ac, char **av,
	Qt::HANDLE v, Qt::HANDLE c)
	: QApplication(d, ac, av, v, c), sappServer(NULL)
{
}

QomposeApplication::~QomposeApplication()
{
	if(sappServer != NULL)
		delete sappServer;
	
	while(windows.length() > 0)
	{
		QomposeWindow *w = windows.takeFirst();
		delete w;
	}
}

void QomposeApplication::initializeLocalServer()
{
	sappServer = new QLocalServer();
	
	if(!sappServer->listen(QOMPOSE_GUID))
	{
		/*
		 * If we got to this point, it means that we didn't get a response
		 * from the person that created our socket, but it is still present
		 * for some reason. Try removing it; if that doesn't work, crash.
		 */
		
		QLocalServer::removeServer(QOMPOSE_GUID);
		
		if(!sappServer->listen(QOMPOSE_GUID))
			qApp->quit();
	}
	
	QObject::connect(sappServer, SIGNAL(newConnection()),
		this, SLOT(doDuplicateInstanceDetected()));
}

void QomposeApplication::doDuplicateInstanceDetected()
{ /* SLOT */
	
	QomposeWindow *w = new QomposeWindow();
	windows.append(w);
	w->show();
	
}
