#include "QomposeWindow.h"

#include "editor/QomposeBuffer.h"

QomposeWindow::QomposeWindow(QWidget *p, Qt::WindowFlags f)
	: QMainWindow(p, f)
{
	editor = new QomposeBuffer(this);
	setCentralWidget(editor);
}

QomposeWindow::~QomposeWindow()
{
}
