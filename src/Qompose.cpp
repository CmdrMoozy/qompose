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

#include <QLocalSocket>

#include "QomposeApplication.h"
#include "QomposeDefines.h"
#include "QomposeWindow.h"

int main(int argc, char *argv[])
{
	QomposeApplication app(argc, argv);
	
	QLocalSocket s;
	s.connectToServer(QOMPOSE_GUID);
	if(s.waitForConnected(2000))
	{
		// Qompose is already running - bail out.
		return 0;
	}
	
	app.initializeLocalServer();
	
	QomposeWindow w;
	w.show();
	
	return app.exec();
}
