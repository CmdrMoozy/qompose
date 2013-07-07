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
