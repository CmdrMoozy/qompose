#ifndef INCLUDE_QOMPOSE_WINDOW_H
#define INCLUDE_QOMPOSE_WINDOW_H

#include <QMainWindow>

class QomposeBuffer;

class QomposeWindow : public QMainWindow
{
	Q_OBJECT
	
	public:
		QomposeWindow(QWidget *p = NULL, Qt::WindowFlags f = 0);
		virtual ~QomposeWindow();
		
	private:
		QomposeBuffer *editor;
};

#endif
