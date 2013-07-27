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

#include "QomposeWindow.h"

#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QApplication>
#include <QStatusBar>
#include <QFile>
#include <QLabel>

#include "QomposeDefines.h"
#include "dialogs/QomposeAboutDialog.h"
#include "gui/QomposeTabWidget.h"

/*!
 * This is our default constructor, which creates a new Qompose window, and initializes
 * its contents.
 *
 * \param p Our parent widget, if any.
 * \param f The Qt window flags to use for this window.
 */
QomposeWindow::QomposeWindow(QWidget *p, Qt::WindowFlags f)
	: QMainWindow(p, f)
{
	buffers = new QomposeTabWidget(this);
	buffers->doNew();
	setCentralWidget(buffers);
	
	statusBar = new QStatusBar(this);
	
	tabPathLabel = new QLabel(statusBar);
	tabPathLabel->setAlignment(Qt::AlignRight);
	
	statusBar->addWidget(tabPathLabel, 1);
	setStatusBar(statusBar);
	
	initializeActions();
	initializeMenus();
	initializeDialogs();
	
	QObject::connect( buffers, SIGNAL( pathChanged(const QString &) ),
		this, SLOT( doTabPathChanged(const QString &) ) );
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
QomposeWindow::~QomposeWindow()
{
}

/*!
 * This function initializes our actions, which are used for e.g. menus and
 * toolsbars, and connects them to the appropriate slots.
 */
void QomposeWindow::initializeActions()
{
	newAction = new QAction(tr("&New"), this);
	newAction->setShortcut(Qt::CTRL + Qt::Key_N);
	newAction->setIcon(getIconFromTheme("document-new"));
	
	openAction = new QAction(tr("&Open..."), this);
	openAction->setShortcut(Qt::CTRL + Qt::Key_O);
	openAction->setIcon(getIconFromTheme("document-open"));
	
	revertAction = new QAction(tr("&Revert"), this);
	revertAction->setShortcut(Qt::CTRL + Qt::Key_R);
	revertAction->setIcon(getIconFromTheme("document-revert"));
	
	saveAction = new QAction(tr("&Save"), this);
	saveAction->setShortcut(Qt::CTRL + Qt::Key_S);
	saveAction->setIcon(getIconFromTheme("document-save"));
	
	saveAsAction = new QAction(tr("Save &As..."), this);
	saveAsAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
	saveAsAction->setIcon(getIconFromTheme("document-save-as"));
	
	printAction = new QAction(tr("&Print..."), this);
	printAction->setShortcut(Qt::CTRL + Qt::Key_P);
	printAction->setIcon(getIconFromTheme("document-print"));
	
	printPreviewAction = new QAction(tr("Print Pre&view..."), this);
	printPreviewAction->setIcon(getIconFromTheme("document-print-preview"));
	
	closeAction = new QAction(tr("Clos&e"), this);
	closeAction->setShortcut(Qt::CTRL + Qt::Key_W);
	closeAction->setIcon(getIconFromTheme("window-close"));
	
	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setIcon(getIconFromTheme("application-exit"));
	
	undoAction = new QAction(tr("&Undo"), this);
	undoAction->setShortcut(Qt::CTRL + Qt::Key_Z);
	undoAction->setIcon(getIconFromTheme("edit-undo"));
	
	redoAction = new QAction(tr("&Redo"), this);
	redoAction->setShortcut(Qt::CTRL + Qt::Key_Y);
	redoAction->setIcon(getIconFromTheme("edit-redo"));
	
	cutAction = new QAction(tr("Cu&t"), this);
	cutAction->setShortcut(Qt::CTRL + Qt::Key_X);
	cutAction->setIcon(getIconFromTheme("edit-cut"));
	
	copyAction = new QAction(tr("&Copy"), this);
	copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
	copyAction->setIcon(getIconFromTheme("edit-copy"));
	
	pasteAction = new QAction(tr("&Paste"), this);
	pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);
	pasteAction->setIcon(getIconFromTheme("edit-paste"));
	
	duplicateLineAction = new QAction(tr("Duplicat&e Line"), this);
	duplicateLineAction->setShortcut(Qt::CTRL + Qt::Key_D);
	
	selectAllAction = new QAction(tr("Select &All"), this);
	selectAllAction->setShortcut(Qt::CTRL + Qt::Key_A);
	selectAllAction->setIcon(getIconFromTheme("edit-select-all"));
	
	deselectAction = new QAction(tr("Dese&lect"), this);
	deselectAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_A);
	
	increaseIndentAction = new QAction(tr("&Increase Selection Indent"), this);
	
	decreaseIndentAction = new QAction(tr("&Decrease Selection Indent"), this);
	
	preferencesAction = new QAction(tr("Pre&ferences..."), this);
	preferencesAction->setIcon(getIconFromTheme("preferences-other"));
	
	findAction = new QAction(tr("&Find..."), this);
	findAction->setShortcut(Qt::CTRL + Qt::Key_F);
	findAction->setIcon(getIconFromTheme("edit-find"));
	
	findNextAction = new QAction(tr("Find &Next"), this);
	findNextAction->setShortcut(Qt::Key_F3);
	findNextAction->setIcon(getIconFromTheme("go-next"));
	
	findPreviousAction = new QAction(tr("Find Previou&s"), this);
	findPreviousAction->setShortcut(Qt::SHIFT + Qt::Key_F3);
	findPreviousAction->setIcon(getIconFromTheme("go-previous"));
	
	replaceAction = new QAction(tr("R&eplace..."), this);
	replaceAction->setShortcut(Qt::CTRL + Qt::Key_H);
	replaceAction->setIcon(getIconFromTheme("edit-find-replace"));
	
	goToAction = new QAction(tr("&Go To Line..."), this);
	goToAction->setShortcut(Qt::CTRL + Qt::Key_G);
	
	previousBufferAction = new QAction(tr("&Previous Buffer"), this);
	previousBufferAction->setShortcut(Qt::ALT + Qt::Key_Left);
	previousBufferAction->setIcon(getIconFromTheme("go-previous"));
	
	nextBufferAction = new QAction(tr("&Next Buffer"), this);
	nextBufferAction->setShortcut(Qt::ALT + Qt::Key_Right);
	nextBufferAction->setIcon(getIconFromTheme("go-next"));
	
	moveBufferLeftAction = new QAction(tr("Move Buffer &Left"), this);
	moveBufferLeftAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Left);
	moveBufferLeftAction->setIcon(getIconFromTheme("go-previous"));
	
	moveBufferRightAction = new QAction(tr("Move Buffer &Right"), this);
	moveBufferRightAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Right);
	moveBufferRightAction->setIcon(getIconFromTheme("go-next"));
	
	aboutQomposeAction = new QAction(tr("&About Qompose..."), this);
	aboutQomposeAction->setIcon(getIconFromTheme("help-about"));
	
	aboutQtAction = new QAction(tr("About &Qt..."), this);
	aboutQtAction->setIcon(getIconFromTheme("help-about"));
	
	QObject::connect( newAction,    SIGNAL( triggered(bool) ), buffers, SLOT( doNew()    ) );
	QObject::connect( openAction,   SIGNAL( triggered(bool) ), buffers, SLOT( doOpen()   ) );
	QObject::connect( revertAction, SIGNAL( triggered(bool) ), buffers, SLOT( doRevert() ) );
	QObject::connect( saveAction,   SIGNAL( triggered(bool) ), buffers, SLOT( doSave()   ) );
	QObject::connect( saveAsAction, SIGNAL( triggered(bool) ), buffers, SLOT( doSaveAs() ) );
	QObject::connect( closeAction,  SIGNAL( triggered(bool) ), buffers, SLOT( doClose()  ) );
	
	QObject::connect( printAction,           SIGNAL( triggered(bool) ), this, SLOT( doPrint(bool)           ) );
	QObject::connect( printPreviewAction,    SIGNAL( triggered(bool) ), this, SLOT( doPrintPreview(bool)    ) );
	QObject::connect( exitAction,            SIGNAL( triggered(bool) ), this, SLOT( doExit(bool)            ) );
	QObject::connect( undoAction,            SIGNAL( triggered(bool) ), this, SLOT( doUndo(bool)            ) );
	QObject::connect( redoAction,            SIGNAL( triggered(bool) ), this, SLOT( doRedo(bool)            ) );
	QObject::connect( cutAction,             SIGNAL( triggered(bool) ), this, SLOT( doCut(bool)             ) );
	QObject::connect( copyAction,            SIGNAL( triggered(bool) ), this, SLOT( doCopy(bool)            ) );
	QObject::connect( pasteAction,           SIGNAL( triggered(bool) ), this, SLOT( doPaste(bool)           ) );
	QObject::connect( duplicateLineAction,   SIGNAL( triggered(bool) ), this, SLOT( doDuplicateLine(bool)   ) );
	QObject::connect( selectAllAction,       SIGNAL( triggered(bool) ), this, SLOT( doSelectAll(bool)       ) );
	QObject::connect( deselectAction,        SIGNAL( triggered(bool) ), this, SLOT( doDeselect(bool)        ) );
	QObject::connect( increaseIndentAction,  SIGNAL( triggered(bool) ), this, SLOT( doIncreaseIndent(bool)  ) );
	QObject::connect( decreaseIndentAction,  SIGNAL( triggered(bool) ), this, SLOT( doDecreaseIndent(bool)  ) );
	QObject::connect( preferencesAction,     SIGNAL( triggered(bool) ), this, SLOT( doPreferences(bool)     ) );
	QObject::connect( findAction,            SIGNAL( triggered(bool) ), this, SLOT( doFind(bool)            ) );
	QObject::connect( findNextAction,        SIGNAL( triggered(bool) ), this, SLOT( doFindNext(bool)        ) );
	QObject::connect( findPreviousAction,    SIGNAL( triggered(bool) ), this, SLOT( doFindPrevious(bool)    ) );
	QObject::connect( replaceAction,         SIGNAL( triggered(bool) ), this, SLOT( doReplace(bool)         ) );
	QObject::connect( goToAction,            SIGNAL( triggered(bool) ), this, SLOT( doGoTo(bool)            ) );
	QObject::connect( previousBufferAction,  SIGNAL( triggered(bool) ), this, SLOT( doPreviousBuffer(bool)  ) );
	QObject::connect( nextBufferAction,      SIGNAL( triggered(bool) ), this, SLOT( doNextBuffer(bool)      ) );
	QObject::connect( moveBufferLeftAction,  SIGNAL( triggered(bool) ), this, SLOT( doMoveBufferLeft(bool)  ) );
	QObject::connect( moveBufferRightAction, SIGNAL( triggered(bool) ), this, SLOT( doMoveBufferRight(bool) ) );
	QObject::connect( aboutQomposeAction,    SIGNAL( triggered(bool) ), this, SLOT( doAboutQompose(bool)    ) );
	QObject::connect( aboutQtAction,         SIGNAL( triggered(bool) ), this, SLOT( doAboutQt(bool)         ) );
}

/*!
 * This function initializes our main menu bar. Note that this function must
 * be called AFTER initializeActions, as we need actions to be initialized before
 * we can add them to menus.
 */
void QomposeWindow::initializeMenus()
{
	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(newAction);
	fileMenu->addSeparator();
	fileMenu->addAction(openAction);
	fileMenu->addAction(revertAction);
	fileMenu->addSeparator();
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(printAction);
	fileMenu->addAction(printPreviewAction);
	fileMenu->addSeparator();
	fileMenu->addAction(closeAction);
	fileMenu->addAction(exitAction);
	
	editMenu = new QMenu(tr("&Edit"), this);
	editMenu->addAction(undoAction);
	editMenu->addAction(redoAction);
	editMenu->addSeparator();
	editMenu->addAction(cutAction);
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	editMenu->addAction(duplicateLineAction);
	editMenu->addAction(selectAllAction);
	editMenu->addAction(deselectAction);
	editMenu->addSeparator();
	editMenu->addAction(increaseIndentAction);
	editMenu->addAction(decreaseIndentAction);
	editMenu->addSeparator();
	editMenu->addAction(preferencesAction);
	
	searchMenu = new QMenu(tr("&Search"), this);
	searchMenu->addAction(findAction);
	searchMenu->addAction(findNextAction);
	searchMenu->addAction(findPreviousAction);
	searchMenu->addAction(replaceAction);
	searchMenu->addAction(goToAction);
	
	buffersMenu = new QMenu(tr("&Buffers"), this);
	buffersMenu->addAction(previousBufferAction);
	buffersMenu->addAction(nextBufferAction);
	buffersMenu->addSeparator();
	buffersMenu->addAction(moveBufferLeftAction);
	buffersMenu->addAction(moveBufferRightAction);
	
	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutQomposeAction);
	helpMenu->addAction(aboutQtAction);
	
	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(editMenu);
	menuBar()->addMenu(searchMenu);
	menuBar()->addMenu(buffersMenu);
	menuBar()->addMenu(helpMenu);
}

/*!
 * This function initializes our dialog objects.
 */
void QomposeWindow::initializeDialogs()
{
	aboutDialog = new QomposeAboutDialog(this);
}

/*!
 * This function will return the icon for the given standard name. We try to
 * use an icon from the operating system's icon theme, but failing that, we
 * try to use a fallback from our internal icon store.
 *
 * If no icon can be found, a null QIcon is returned instead.
 *
 * The default names supported by QIcon (and, therefore, this function) can be
 * found here:
 *
 *     http://standards.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html
 *
 * \param n The name of the icon to find.
 * \return An icon object corresponding to the given name.
 */
QIcon QomposeWindow::getIconFromTheme(const QString &n) const
{
	QIcon defaultIcon;
	
	QString defaultPath = n;
	defaultPath.prepend(":/icons/");
	defaultPath.append(".png");
	
	if(QFile::exists(defaultPath))
		defaultIcon = QIcon(defaultPath);
	else
		qDebug("Couldn't find icon: %s", qPrintable(n));
	
	return QIcon::fromTheme(n, defaultIcon);
}

void QomposeWindow::doTabPathChanged(const QString &p)
{ /* SLOT */
	
	tabPathLabel->setText(p);
	
}

void QomposeWindow::doPrint(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doPrintPreview(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doExit(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doUndo(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doRedo(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doCut(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doCopy(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doPaste(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doDuplicateLine(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doSelectAll(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doDeselect(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doIncreaseIndent(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doDecreaseIndent(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doPreferences(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doFind(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doFindNext(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doFindPrevious(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doReplace(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doGoTo(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doPreviousBuffer(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doNextBuffer(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doMoveBufferLeft(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doMoveBufferRight(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doAboutQompose(bool QUNUSED(c))
{ /* SLOT */
	
	aboutDialog->show();
	
}

void QomposeWindow::doAboutQt(bool QUNUSED(c))
{ /* SLOT */
	
	qApp->aboutQt();
	
}
