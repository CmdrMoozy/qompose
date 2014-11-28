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

#include "Window.h"

#include <QApplication>
#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QIcon>

#include "QomposeCommon/Defines.h"
#include "QomposeCommon/dialogs/AboutDialog.h"
#include "QomposeCommon/dialogs/FindDialog.h"
#include "QomposeCommon/dialogs/GoToDialog.h"
#include "QomposeCommon/dialogs/ReplaceDialog.h"
#include "QomposeCommon/dialogs/preferences/PreferencesDialog.h"
#include "QomposeCommon/gui/BufferWidget.h"
#include "QomposeCommon/gui/GUIUtils.h"
#include "QomposeCommon/gui/StatusBar.h"
#include "QomposeCommon/gui/menus/MainMenu.h"
#include "QomposeCommon/util/FindQuery.h"
#include "QomposeCommon/util/ReplaceQuery.h"
#include "QomposeCommon/util/Settings.h"

namespace qompose
{

/*!
 * This is our default constructor, which creates a new Qompose window, and
 * initializes its contents.
 *
 * \param p Our parent widget, if any.
 * \param f The Qt window flags to use for this window.
 */
Window::Window(QWidget *p, Qt::WindowFlags f)
	: QMainWindow(p, f), settings(nullptr), preferencesDialog(nullptr),
		findDialog(nullptr), replaceDialog(nullptr),
		goToDialog(nullptr), aboutDialog(nullptr), mainMenu(nullptr),
		buffers(nullptr), statusBar(nullptr)
{
	settings = new Settings(this);

	// Set some of our window's properties.

	setWindowIcon(QIcon(":/icons/qompose.png"));

	#ifdef QOMPOSE_DEBUG
		setWindowTitle(tr("Qompose [DEBUG]"));
	#else
		setWindowTitle(tr("Qompose"));
	#endif

	// Initialize our window.

	buffers = new BufferWidget(settings, this);
	buffers->doNew();
	setCentralWidget(buffers);

	statusBar = new StatusBar(this);
	setStatusBar(statusBar);

	initializeDialogs();
	initializeMenus();

	QObject::connect( buffers, SIGNAL( pathChanged(const QString &) ),
		this, SLOT( doTabPathChanged(const QString &) ) );
	QObject::connect( buffers, SIGNAL( cursorPositionChanged(int, int) ),
		this, SLOT( doCursorPositionChanged(int, int) ) );
	QObject::connect( buffers, SIGNAL( searchWrapped() ),
		this, SLOT( doSearchWrapped() ) );

	// Apply any existing settings values to our UI.

	applyExistingSettings();
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
Window::~Window()
{
}

/*!
 * This function handles our window being closed by notifying our buffers of
 * the imminent close event, and then proceeding according to what it reports
 * back to us.
 *
 * \param e The event being handled.
 */
void Window::closeEvent(QCloseEvent *e)
{
	if(buffers->prepareCloseParent())
	{
		// Save our window geometry and state.

		settings->setSetting("window-geometry", QVariant(
			saveGeometry()));

		settings->setSetting("window-state", QVariant(
			saveState(QOMPOSE_VERSION_MAJ)));

		// Close the window.

		e->accept();
	}
	else
	{
		e->ignore();
	}
}

/*!
 * This function initializes our dialog objects.
 */
void Window::initializeDialogs()
{
	// Create our dialog objects.

	preferencesDialog = new PreferencesDialog(settings, this);

	findDialog = new FindDialog(this);

	replaceDialog = new ReplaceDialog(this);

	goToDialog = new GoToDialog(this);

	aboutDialog = new AboutDialog(this);

	// Connect our dialog actions.

	QObject::connect(findDialog, SIGNAL(accepted()),
		this, SLOT(doFindNext()));
	QObject::connect(replaceDialog, SIGNAL(replaceClicked()),
		this, SLOT(doReplace()));
	QObject::connect(replaceDialog, SIGNAL(findClicked()),
		this, SLOT(doReplaceFind()));
	QObject::connect(replaceDialog, SIGNAL(replaceSelectionClicked()),
		this, SLOT(doReplaceSelection()));
	QObject::connect(replaceDialog, SIGNAL(replaceAllClicked()),
		this, SLOT(doReplaceAll()));
	QObject::connect(goToDialog, SIGNAL(accepted()),
		this, SLOT(doGoToAccepted()));
}

/*!
 * This function initializes our main menu bar. Note that this function must be
 * called AFTER initializeActions, as we need actions to be initialized before
 * we can add them to menus.
 */
void Window::initializeMenus()
{
	// Create our main menu.

	mainMenu = new MainMenu(settings, this);

	setMenuBar(mainMenu);

	// Connect our main menu's actions to our UI.

	mainMenu->connectBufferWidget(buffers);

	QObject::connect( mainMenu, SIGNAL( printTriggered(bool)        ), this,        SLOT( doPrint()             ) );
	QObject::connect( mainMenu, SIGNAL( printPreviewTriggered(bool) ), this,        SLOT( doPrintPreview()      ) );
	QObject::connect( mainMenu, SIGNAL( exitTriggered(bool)         ), this,        SLOT( close()               ) );
	QObject::connect( mainMenu, SIGNAL( preferencesTriggered(bool)  ), this,        SLOT( doPreferencesDialog() ) );
	QObject::connect( mainMenu, SIGNAL( findTriggered(bool)         ), this,        SLOT( doFindDialog()        ) );
	QObject::connect( mainMenu, SIGNAL( findNextTriggered(bool)     ), this,        SLOT( doFindNext()          ) );
	QObject::connect( mainMenu, SIGNAL( findPreviousTriggered(bool) ), this,        SLOT( doFindPrevious()      ) );
	QObject::connect( mainMenu, SIGNAL( replaceTriggered(bool)      ), this,        SLOT( doReplaceDialog()     ) );
	QObject::connect( mainMenu, SIGNAL( goToTriggered(bool)         ), goToDialog,  SLOT( show()                ) );
	QObject::connect( mainMenu, SIGNAL( aboutQomposeTriggered(bool) ), aboutDialog, SLOT( show()                ) );
	QObject::connect( mainMenu, SIGNAL( aboutQtTriggered(bool)      ), qApp,        SLOT( aboutQt()             ) );

	#ifdef QOMPOSE_DEBUG
		QObject::connect(mainMenu, SIGNAL(debugTriggered(bool)),
			this, SLOT(doDebug()));
	#endif
}

/*!
 * This function reads various settings properties from our settings object,
 * and applies their values to our UI. Additionally, we connect the settings
 * object to our slots, so we can listen for settings changes.
 */
void Window::applyExistingSettings()
{
	// Load our initial settings, and connect our settings object.

	statusBar->setVisible(settings->getSetting("show-status-bar").toBool());

	QObject::connect(settings, SIGNAL(settingChanged(
		const QString &, const QVariant &)), this,
		SLOT(doSettingChanged(const QString &, const QVariant &)));

	// Restore our window's geometry and state.

	QByteArray winGeometry = settings->getSetting("window-geometry")
		.toByteArray();

	if(!winGeometry.isNull())
		if(!restoreGeometry(winGeometry))
			settings->setSetting("window-geometry",
				QVariant(QByteArray()));

	QByteArray winState = settings->getSetting("window-state")
		.toByteArray();

	if(!winState.isNull())
		if(!restoreState(winState, QOMPOSE_VERSION_MAJ))
			settings->setSetting("window-state",
				QVariant(QByteArray()));
}

/*!
 * This function handles the result of a find operation by alerting the
 * user about what happened. This includes things like trying to find using
 * a bad regular expression, or finding something for which no matches
 * were found.
 *
 * \param r The find result to process.
 */
void Window::handleFindResult(Editor::FindResult r)
{
	// Deal with the find result.

	switch(r)
	{
		case Editor::BadRegularExpression:
			QMessageBox::critical(this, tr("Invalid Regular Expression"),
				tr("The regular expression you were trying to find was invalid."),
				QMessageBox::Ok, QMessageBox::Ok);
			break;

		case Editor::NoMatches:
			QMessageBox::information(this, tr("No Matches Found"),
				tr("No more matches could be found."),
				QMessageBox::Ok, QMessageBox::Ok);
			break;

		default: break;
	};

	// Re-focus the dialog that generated this result, if any.

	if(findDialog->isVisible())
	{
		findDialog->setFocus();
		findDialog->raise();
	}

	if(replaceDialog->isVisible())
	{
		replaceDialog->setFocus();
		replaceDialog->raise();
	}
}

/*!
 * This slot handles one of our tab's paths being changed by updating that
 * tab's path label.
 *
 * \param p The new path for the current tab.
 */
void Window::doTabPathChanged(const QString &p)
{ /* SLOT */

	statusBar->setCurrentTabPath(p);

}

/*!
 * This slot handles one of our tab's cursor positions changing by updating
 * our status bar to display the current line and column position.
 *
 * \param l The new cursor line number.
 * \param c The new cursor column number.
 */
void Window::doCursorPositionChanged(int l, int c)
{ /* SLOT */

	statusBar->setLine(l);
	statusBar->setColumn(c);

}

/*!
 * This function handles the case when whatever find/replace operation was
 * being executed has wrapped to the opposite side of the document.
 */
void Window::doSearchWrapped()
{ /* SLOT */

	statusBar->displayNotification(tr("Search wrapped around buffer."));

}

/*!
 * This slot handles our preferences dialog action being triggered
 * by resetting and showing our preferences dialog (if it isn't already
 * visible).
 *
 */
void Window::doPreferencesDialog()
{ /* SLOT */

	if(!preferencesDialog->isVisible())
	{
		preferencesDialog->discardChanges();
		preferencesDialog->show();
	}

}

/*!
 * This slot handles our "print" action being triggered by displaying a
 * standard print dialog and, if it is accepted, printing our current buffer
 * using the printer object it configures.
 */
void Window::doPrint()
{ /* SLOT */

	if(!buffers->hasCurrentBuffer())
	{
		QMessageBox::warning(this, tr("Nothing To Print"),
			tr("No buffers are open; there is nothing to print!"),
			QMessageBox::Ok, QMessageBox::Ok);

		return;
	}

	QPrinter printer(QPrinter::ScreenResolution);
	QPrintDialog dialog(&printer, this);

	if(dialog.exec() == QDialog::Accepted)
		buffers->doPrint(&printer);

}

/*!
 * This slot handles our "print preview" action being triggered by displaying
 * a standard print preview dialog using our buffers' standard print slot.
 */
void Window::doPrintPreview()
{ /* SLOT */

	if(!buffers->hasCurrentBuffer())
	{
		QMessageBox::warning(this, tr("Nothing To Preview"),
			tr("No buffers are open; there is nothing to preview!"),
			QMessageBox::Ok, QMessageBox::Ok);

		return;
	}

	QPrintPreviewDialog *dialog = new QPrintPreviewDialog(this, nullptr);

	QObject::connect( dialog, SIGNAL( paintRequested(QPrinter *) ),
		buffers, SLOT( doPrint(QPrinter *) ) );

	dialog->exec();

	delete dialog;

}

/*!
 * This function handles our "find" action being triggered by showing our find
 * dialog, if our replace dialog isn't already open (they are mutually
 * exclusive).
 */
void Window::doFindDialog()
{ /* SLOT */

	if(!replaceDialog->isVisible())
	{
		findDialog->show();
	}

}

/*!
 * This slot performs a "find next" operation by extracting the current
 * find query from the find dialog, telling our buffers widget to execute the
 * query, and then dealing with the result.
 */
void Window::doFindNext()
{ /* SLOT */

	handleFindResult(buffers->doFindNext(findDialog->getQuery()));

}

/*!
 * This slot performs a "find previous" operation by extracting the current
 * find query from the find dialog, telling our buffers widget to execute the
 * query, and then dealing with the result.
 */
void Window::doFindPrevious()
{ /* SLOT */

	handleFindResult(buffers->doFindPrevious(findDialog->getQuery()));

}

/*!
 * This function handles our "replace" action being triggered by showing our
 * replace dialog, if our find dialog isn't already open (they are mutually
 * exclusive).
 */
void Window::doReplaceDialog()
{ /* SLOT */

	if(!findDialog->isVisible())
	{
		replaceDialog->show();
	}

}

/*!
 * This slot performs a single "replace" operation by extracting the current
 * replace query from the replace dialog, telling our buffers widget to execute
 * the query, and then dealing with the result.
 */
void Window::doReplace()
{ /* SLOT */

	handleFindResult(buffers->doReplace(replaceDialog->getQuery()));

}

/*!
 * This slot performs a "replace find" operation. This is effectively the same
 * as a "find next" operation, but we use the query from the replace dialog,
 * instead of the one from the find dialog. We perform this operation by
 * extracting the current replace query from the replace dialog, telling our
 * buffers widget to execute the query as a "find next" operation, and then
 * we deal with the result.
 */
void Window::doReplaceFind()
{ /* SLOT */

	handleFindResult(buffers->doFindNext(replaceDialog->getQuery()));

}

/*!
 * This slot performs a "replace in selection" operation by extracting the
 * current replace query from the replace dialog, telling our buffers widget
 * to execute the query, and then dealing with the result.
 */
void Window::doReplaceSelection()
{ /* SLOT */

	handleFindResult(buffers->doReplaceSelection(
		replaceDialog->getQuery()));

}

/*!
 * This slot performs a "replace all" operation by extracting the current
 * replace query from the replace dialog, telling our buffers widget to
 * execute the query, and then dealing with the result.
 */
void Window::doReplaceAll()
{ /* SLOT */

	handleFindResult(buffers->doReplaceAll(replaceDialog->getQuery()));

}

/*!
 * This function handles our "go to" dialog being accepted by telling our
 * buffers widget to perform the "go to" operation.
 */
void Window::doGoToAccepted()
{ /* SLOT */

	buffers->doGoTo(goToDialog->getSelectedLine());

}

#ifdef QOMPOSE_DEBUG
	/*!
	 * This function performs some action to help with debugging.
	 */
	void Window::doDebug()
	{ /* SLOT */



	}
#endif

/*!
 * This function handles a setting changed by applying that change to our
 * window.
 *
 * \param k The setting key that was changed.
 * \param v The new value for the given setting.
 */
void Window::doSettingChanged(const QString &k, const QVariant &v)
{ /* SLOT */

	if(k == "show-status-bar")
		statusBar->setVisible(v.toBool());

}

}