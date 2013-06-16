#include <QApplication>

#include "QomposeWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QomposeWindow w;
	w.show();
	return app.exec();
}
