#ifndef INCLUDE_QOMPOSE_APPLICATION_H
#define INCLUDE_QOMPOSE_APPLICATION_H

#include <QApplication>
#include <QList>

class QLocalServer;
class QomposeWindow;

class QomposeApplication : public QApplication
{
	Q_OBJECT
	
	public:
		QomposeApplication(int &ac, char **av);
		QomposeApplication(int &ac, char **av, bool g);
		QomposeApplication(int &ac, char **av, Type t);
		QomposeApplication(Display *d, Qt::HANDLE v = 0, Qt::HANDLE c = 0);
		QomposeApplication(Display *d, int &ac, char **av,
			Qt::HANDLE v = 0, Qt::HANDLE c = 0);
		virtual ~QomposeApplication();
		
		void initializeLocalServer();
		
	private:
		QLocalServer *sappServer;
		QList<QomposeWindow *> windows;
		
	private slots:
		void doDuplicateInstanceDetected();
};

#endif
