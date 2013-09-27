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
#include <QCloseEvent>

#include "QomposeDefines.h"
#include "dialogs/QomposeAboutDialog.h"
#include "dialogs/QomposeFindDialog.h"
#include "dialogs/QomposeGoToDialog.h"
#include "dialogs/QomposeReplaceDialog.h"
#include "dialogs/preferences/QomposePreferencesDialog.h"
#include "gui/QomposeBufferWidget.h"
#include "gui/QomposeGUIUtils.h"
#include "util/QomposeSettings.h"

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
	settings = new QomposeSettings(this);
	
	// Initialize our window.
	
	buffers = new QomposeBufferWidget(settings, this);
	buffers->doNew();
	setCentralWidget(buffers);
	
	statusBar = new QStatusBar(this);
	
	tabPathLabel = new QLabel(statusBar);
	tabPathLabel->setAlignment(Qt::AlignRight);
	
	statusBar->addWidget(tabPathLabel, 1);
	setStatusBar(statusBar);
	
	initializeDialogs();
	initializeActions();
	initializeMenus();
	
	QObject::connect( buffers, SIGNAL( pathChanged(const QString &) ),
		this, SLOT( doTabPathChanged(const QString &) ) );
	
	// Load our initial settings, and connect our settings object.
	
	statusBar->setVisible(settings->getSetting("show-status-bar").toBool());
	
	QObject::connect( settings, SIGNAL( settingChanged(const QString &, const QVariant &) ),
		this, SLOT( doSettingChanged(const QString &, const QVariant &) ) );
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
QomposeWindow::~QomposeWindow()
{
}

/*!
 * This function handles our window being closed by notifying our buffers of the
 * imminent close event, and then proceeding according to what it reports back to
 * us.
 *
 * \param e The event being handled.
 */
void QomposeWindow::closeEvent(QCloseEvent *e)
{
	if(buffers->prepareCloseParent())
		e->accept();
	else
		e->ignore();
}

/*!
 * This function initializes our actions, which are used for e.g. menus and
 * toolsbars, and connects them to the appropriate slots.
 */
void QomposeWindow::initializeActions()
{
	newAction = new QAction(tr("&New"), this);
	newAction->setShortcut(Qt::CTRL + Qt::Key_N);
	newAction->setIcon(QomposeGUIUtils::getIconFromTheme("document-new"));
	
	openAction = new QAction(tr("&Open..."), this);
	openAction->setShortcut(Qt::CTRL + Qt::Key_O);
	openAction->setIcon(QomposeGUIUtils::getIconFromTheme("document-open"));
	
	revertAction = new QAction(tr("&Revert"), this);
	revertAction->setShortcut(Qt::CTRL + Qt::Key_R);
	revertAction->setIcon(QomposeGUIUtils::getIconFromTheme("document-revert"));
	
	saveAction = new QAction(tr("&Save"), this);
	saveAction->setShortcut(Qt::CTRL + Qt::Key_S);
	saveAction->setIcon(QomposeGUIUtils::getIconFromTheme("document-save"));
	
	saveAsAction = new QAction(tr("Save &As..."), this);
	saveAsAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
	saveAsAction->setIcon(QomposeGUIUtils::getIconFromTheme("document-save-as"));
	
	printAction = new QAction(tr("&Print..."), this);
	printAction->setShortcut(Qt::CTRL + Qt::Key_P);
	printAction->setIcon(QomposeGUIUtils::getIconFromTheme("document-print"));
	
	printPreviewAction = new QAction(tr("Print Pre&view..."), this);
	printPreviewAction->setIcon(QomposeGUIUtils::getIconFromTheme("document-print-preview"));
	
	closeAction = new QAction(tr("Clos&e"), this);
	closeAction->setShortcut(Qt::CTRL + Qt::Key_W);
	closeAction->setIcon(QomposeGUIUtils::getIconFromTheme("window-close"));
	
	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setIcon(QomposeGUIUtils::getIconFromTheme("application-exit"));
	
	undoAction = new QAction(tr("&Undo"), this);
	undoAction->setShortcut(Qt::CTRL + Qt::Key_Z);
	undoAction->setIcon(QomposeGUIUtils::getIconFromTheme("edit-undo"));
	
	redoAction = new QAction(tr("&Redo"), this);
	redoAction->setShortcut(Qt::CTRL + Qt::Key_Y);
	redoAction->setIcon(QomposeGUIUtils::getIconFromTheme("edit-redo"));
	
	cutAction = new QAction(tr("Cu&t"), this);
	cutAction->setShortcut(Qt::CTRL + Qt::Key_X);
	cutAction->setIcon(QomposeGUIUtils::getIconFromTheme("edit-cut"));
	
	copyAction = new QAction(tr("&Copy"), this);
	copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
	copyAction->setIcon(QomposeGUIUtils::getIconFromTheme("edit-copy"));
	
	pasteAction = new QAction(tr("&Paste"), this);
	pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);
	pasteAction->setIcon(QomposeGUIUtils::getIconFromTheme("edit-paste"));
	
	duplicateLineAction = new QAction(tr("Duplicat&e Line"), this);
	duplicateLineAction->setShortcut(Qt::CTRL + Qt::Key_D);
	
	selectAllAction = new QAction(tr("Select &All"), this);
	selectAllAction->setShortcut(Qt::CTRL + Qt::Key_A);
	selectAllAction->setIcon(QomposeGUIUtils::getIconFromTheme("edit-select-all"));
	
	deselectAction = new QAction(tr("Dese&lect"), this);
	deselectAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_A);
	
	increaseIndentAction = new QAction(tr("&Increase Selection Indent"), this);
	
	decreaseIndentAction = new QAction(tr("&Decrease Selection Indent"), this);
	
	preferencesAction = new QAction(tr("Pre&ferences..."), this);
	preferencesAction->setIcon(QomposeGUIUtils::getIconFromTheme("preferences-other"));
	
	findAction = new QAction(tr("&Find..."), this);
	findAction->setShortcut(Qt::CTRL + Qt::Key_F);
	findAction->setIcon(QomposeGUIUtils::getIconFromTheme("edit-find"));
	
	findNextAction = new QAction(tr("Find &Next"), this);
	findNextAction->setShortcut(Qt::Key_F3);
	findNextAction->setIcon(QomposeGUIUtils::getIconFromTheme("go-next"));
	
	findPreviousAction = new QAction(tr("Find Previou&s"), this);
	findPreviousAction->setShortcut(Qt::SHIFT + Qt::Key_F3);
	findPreviousAction->setIcon(QomposeGUIUtils::getIconFromTheme("go-previous"));
	
	replaceAction = new QAction(tr("R&eplace..."), this);
	replaceAction->setShortcut(Qt::CTRL + Qt::Key_H);
	replaceAction->setIcon(QomposeGUIUtils::getIconFromTheme("edit-find-replace"));
	
	goToAction = new QAction(tr("&Go To Line..."), this);
	goToAction->setShortcut(Qt::CTRL + Qt::Key_G);
	
	previousBufferAction = new QAction(tr("&Previous Buffer"), this);
	previousBufferAction->setShortcut(Qt::ALT + Qt::Key_Left);
	previousBufferAction->setIcon(QomposeGUIUtils::getIconFromTheme("go-previous"));
	
	nextBufferAction = new QAction(tr("&Next Buffer"), this);
	nextBufferAction->setShortcut(Qt::ALT + Qt::Key_Right);
	nextBufferAction->setIcon(QomposeGUIUtils::getIconFromTheme("go-next"));
	
	moveBufferLeftAction = new QAction(tr("Move Buffer &Left"), this);
	moveBufferLeftAction->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_Left);
	moveBufferLeftAction->setIcon(QomposeGUIUtils::getIconFromTheme("go-previous"));
	
	moveBufferRightAction = new QAction(tr("Move Buffer &Right"), this);
	moveBufferRightAction->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_Right);
	moveBufferRightAction->setIcon(QomposeGUIUtils::getIconFromTheme("go-next"));
	
	aboutQomposeAction = new QAction(tr("&About Qompose..."), this);
	aboutQomposeAction->setIcon(QomposeGUIUtils::getIconFromTheme("help-about"));
	
	aboutQtAction = new QAction(tr("About &Qt..."), this);
	aboutQtAction->setIcon(QomposeGUIUtils::getIconFromTheme("help-about"));
	
	QObject::connect( newAction,             SIGNAL( triggered(bool) ), buffers,     SLOT( doNew()                   ) );
	QObject::connect( openAction,            SIGNAL( triggered(bool) ), buffers,     SLOT( doOpen()                  ) );
	QObject::connect( revertAction,          SIGNAL( triggered(bool) ), buffers,     SLOT( doRevert()                ) );
	QObject::connect( saveAction,            SIGNAL( triggered(bool) ), buffers,     SLOT( doSave()                  ) );
	QObject::connect( saveAsAction,          SIGNAL( triggered(bool) ), buffers,     SLOT( doSaveAs()                ) );
	QObject::connect( closeAction,           SIGNAL( triggered(bool) ), buffers,     SLOT( doClose()                 ) );
	QObject::connect( exitAction,            SIGNAL( triggered(bool) ), this,        SLOT( close()                   ) );
	QObject::connect( undoAction,            SIGNAL( triggered(bool) ), buffers,     SLOT( doUndo()                  ) );
	QObject::connect( redoAction,            SIGNAL( triggered(bool) ), buffers,     SLOT( doRedo()                  ) );
	QObject::connect( cutAction,             SIGNAL( triggered(bool) ), buffers,     SLOT( doCut()                   ) );
	QObject::connect( copyAction,            SIGNAL( triggered(bool) ), buffers,     SLOT( doCopy()                  ) );
	QObject::connect( pasteAction,           SIGNAL( triggered(bool) ), buffers,     SLOT( doPaste()                 ) );
	QObject::connect( selectAllAction,       SIGNAL( triggered(bool) ), buffers,     SLOT( doSelectAll()             ) );
	QObject::connect( deselectAction,        SIGNAL( triggered(bool) ), buffers,     SLOT( doDeselect()              ) );
	QObject::connect( preferencesAction,     SIGNAL( triggered(bool) ), this,        SLOT( doPreferencesDialog(bool) ) );
	QObject::connect( previousBufferAction,  SIGNAL( triggered(bool) ), buffers,     SLOT( doPreviousBuffer()        ) );
	QObject::connect( nextBufferAction,      SIGNAL( triggered(bool) ), buffers,     SLOT( doNextBuffer()            ) );
	QObject::connect( moveBufferLeftAction,  SIGNAL( triggered(bool) ), buffers,     SLOT( doMoveBufferLeft()        ) );
	QObject::connect( moveBufferRightAction, SIGNAL( triggered(bool) ), buffers,     SLOT( doMoveBufferRight()       ) );
	QObject::connect( aboutQomposeAction,    SIGNAL( triggered(bool) ), aboutDialog, SLOT( show()                    ) );
	QObject::connect( aboutQtAction,         SIGNAL( triggered(bool) ), qApp,        SLOT( aboutQt()                 ) );
	
	QObject::connect( printAction,           SIGNAL( triggered(bool) ), this, SLOT( doPrint(bool)           ) );
	QObject::connect( printPreviewAction,    SIGNAL( triggered(bool) ), this, SLOT( doPrintPreview(bool)    ) );
	QObject::connect( duplicateLineAction,   SIGNAL( triggered(bool) ), this, SLOT( doDuplicateLine(bool)   ) );
	QObject::connect( increaseIndentAction,  SIGNAL( triggered(bool) ), this, SLOT( doIncreaseIndent(bool)  ) );
	QObject::connect( decreaseIndentAction,  SIGNAL( triggered(bool) ), this, SLOT( doDecreaseIndent(bool)  ) );
	QObject::connect( findAction,            SIGNAL( triggered(bool) ), this, SLOT( doFind(bool)            ) );
	QObject::connect( findNextAction,        SIGNAL( triggered(bool) ), this, SLOT( doFindNext(bool)        ) );
	QObject::connect( findPreviousAction,    SIGNAL( triggered(bool) ), this, SLOT( doFindPrevious(bool)    ) );
	QObject::connect( replaceAction,         SIGNAL( triggered(bool) ), this, SLOT( doReplace(bool)         ) );
	QObject::connect( goToAction,            SIGNAL( triggered(bool) ), this, SLOT( doGoTo(bool)            ) );
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
	preferencesDialog = new QomposePreferencesDialog(settings, this);
	
	findDialog = new QomposeFindDialog(this);
	
	replaceDialog = new QomposeReplaceDialog(this);
	
	goToDialog = new QomposeGoToDialog(this);
	
	aboutDialog = new QomposeAboutDialog(this);
}

/*!
 * This slot handles one of our tab's paths being changed by updating that
 * tab's path label.
 *
 * \param p The new path for the current tab.
 */
void QomposeWindow::doTabPathChanged(const QString &p)
{ /* SLOT */
	
	tabPathLabel->setText(p);
	
}

/*!
 * This slot handles our preferences dialog action being triggered
 * by resetting and showing our preferences dialog (if it isn't already
 * visible).
 *
 * \param c The action's check state (ignored).
 */
void QomposeWindow::doPreferencesDialog(bool QUNUSED(c))
{ /* SLOT */
	
	if(!preferencesDialog->isVisible())
	{
		preferencesDialog->discardChanges();
		preferencesDialog->show();
	}
	
}

void QomposeWindow::doPrint(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doPrintPreview(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doDuplicateLine(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doIncreaseIndent(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doDecreaseIndent(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doFind(bool QUNUSED(c))
{ /* SLOT */
	
	findDialog->show();
	
}

void QomposeWindow::doFindNext(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doFindPrevious(bool QUNUSED(c))
{ /* SLOT */
}

void QomposeWindow::doReplace(bool QUNUSED(c))
{ /* SLOT */
	
	replaceDialog->show();
	
}

void QomposeWindow::doGoTo(bool QUNUSED(c))
{ /* SLOT */
	
	goToDialog->show();
	
}

void QomposeWindow::doSettingChanged(const QString &k, const QVariant &v)
{ /* SLOT */
	
	if(k == "show-status-bar")
		statusBar->setVisible(v.toBool());
	
}
