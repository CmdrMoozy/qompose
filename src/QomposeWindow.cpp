#include "QomposeWindow.h"

#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QList>
#include <QDir>

#include "dialogs/QomposeFileDialog.h"
#include "editor/QomposeBuffer.h"

QomposeWindow::QomposeWindow(QWidget *p, Qt::WindowFlags f)
	: QMainWindow(p, f)
{
	editor = new QomposeBuffer(this);
	setCentralWidget(editor);
	
	initializeActions();
	initializeMenus();
}

QomposeWindow::~QomposeWindow()
{
}

void QomposeWindow::initializeActions()
{
	openAction = new QAction(tr("Open"), this);
	openAction->setShortcuts(QKeySequence::Open);
	
	QObject::connect( openAction, SIGNAL(triggered(bool)), this, SLOT(doOpen(bool)) );
}

void QomposeWindow::initializeMenus()
{
	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAction);
	
	menuBar()->addMenu(fileMenu);
}

void QomposeWindow::doOpen(bool c)
{ /* SLOT */
	
	QList<QomposeFileDescriptor> files = QomposeFileDialog::getOpenFileNames(
		this, tr("Open Files"), QDir::homePath());
	
	qDebug("File list length: %d", files.length());
	
}
