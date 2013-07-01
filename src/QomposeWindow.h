#ifndef INCLUDE_QOMPOSE_WINDOW_H
#define INCLUDE_QOMPOSE_WINDOW_H

#include <QMainWindow>

class QMenu;
class QAction;

class QomposeBuffer;

class QomposeWindow : public QMainWindow
{
	Q_OBJECT
	
	public:
		QomposeWindow(QWidget *p = NULL, Qt::WindowFlags f = 0);
		virtual ~QomposeWindow();
		
	private:
		QMenu *fileMenu;
		
		QAction *openAction;
		
		QomposeBuffer *editor;
		
		void initializeActions();
		void initializeMenus();
		
	private slots:
		void doOpen(bool c);
};

#endif
