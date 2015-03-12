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
#include "QomposeCommon/editor/Buffer.h"
#include "QomposeCommon/gui/BufferWidget.h"
#include "QomposeCommon/gui/GUIUtils.h"
#include "QomposeCommon/gui/StatusBar.h"
#include "QomposeCommon/gui/menus/MainMenu.h"
#include "QomposeCommon/util/FindQuery.h"
#include "QomposeCommon/util/ReplaceQuery.h"
#include "QomposeCommon/util/Settings.h"

namespace qompose
{
std::vector<Window *> Window::windows = std::vector<Window *>();

void Window::openNewWindow()
{
	Window *w = new Window();
	windows.push_back(w);
	w->show();
}

Window::Window(QWidget *p, Qt::WindowFlags f)
        : QMainWindow(p, f),
          settings(nullptr),
          preferencesDialog(nullptr),
          findDialog(nullptr),
          replaceDialog(nullptr),
          goToDialog(nullptr),
          aboutDialog(nullptr),
          mainMenu(nullptr),
          buffers(nullptr),
          statusBar(nullptr)
{
	settings = new Settings(this);

	// Set some of our window's properties.

	setWindowIcon(QIcon(":/icons/qompose.png"));

	// Initialize our window.

	buffers = new BufferWidget(settings, this);
	buffers->doNew();
	setCentralWidget(buffers);

	statusBar = new StatusBar(this);
	setStatusBar(statusBar);

	initializeDialogs();
	initializeMenus();

	QObject::connect(buffers, SIGNAL(pathChanged(const QString &)), this,
	                 SLOT(doTabPathChanged(const QString &)));
	QObject::connect(buffers, SIGNAL(cursorPositionChanged(int, int)), this,
	                 SLOT(doCursorPositionChanged(int, int)));
	QObject::connect(buffers, SIGNAL(searchWrapped()), this,
	                 SLOT(doSearchWrapped()));

	// Apply any existing settings values to our UI.

	applyExistingSettings();

	// Update our window title initially.

	doUpdateWindowTitle();
}

void Window::closeEvent(QCloseEvent *e)
{
	if(buffers->prepareCloseParent())
	{
		// Save our window geometry and state.

		settings->setSetting("window-geometry",
		                     QVariant(saveGeometry()));

		settings->setSetting("window-state",
		                     QVariant(saveState(QOMPOSE_VERSION_MAJ)));

		// Close the window.

		e->accept();
	}
	else
	{
		e->ignore();
	}
}

void Window::initializeDialogs()
{
	// Create our dialog objects.

	preferencesDialog = new PreferencesDialog(settings, this);

	findDialog = new FindDialog(this);

	replaceDialog = new ReplaceDialog(this);

	goToDialog = new GoToDialog(this);

	aboutDialog = new AboutDialog(this);

	// Connect our dialog actions.

	QObject::connect(findDialog, SIGNAL(accepted()), this,
	                 SLOT(doFindNext()));
	QObject::connect(replaceDialog, SIGNAL(replaceClicked()), this,
	                 SLOT(doReplace()));
	QObject::connect(replaceDialog, SIGNAL(findClicked()), this,
	                 SLOT(doReplaceFind()));
	QObject::connect(replaceDialog, SIGNAL(replaceSelectionClicked()), this,
	                 SLOT(doReplaceSelection()));
	QObject::connect(replaceDialog, SIGNAL(replaceAllClicked()), this,
	                 SLOT(doReplaceAll()));
	QObject::connect(goToDialog, SIGNAL(accepted()), this,
	                 SLOT(doGoToAccepted()));
}

void Window::initializeMenus()
{
	// Create our main menu.

	mainMenu = new MainMenu(settings, this);

	setMenuBar(mainMenu);

	// Connect our main menu's actions to our UI.

	mainMenu->connectBufferWidget(buffers);

	QObject::connect(mainMenu, SIGNAL(printTriggered(bool)), this,
	                 SLOT(doPrint()));
	QObject::connect(mainMenu, SIGNAL(printPreviewTriggered(bool)), this,
	                 SLOT(doPrintPreview()));
	QObject::connect(mainMenu, SIGNAL(exitTriggered(bool)), this,
	                 SLOT(close()));
	QObject::connect(mainMenu, SIGNAL(preferencesTriggered(bool)), this,
	                 SLOT(doPreferencesDialog()));
	QObject::connect(mainMenu, SIGNAL(findTriggered(bool)), this,
	                 SLOT(doFindDialog()));
	QObject::connect(mainMenu, SIGNAL(findNextTriggered(bool)), this,
	                 SLOT(doFindNext()));
	QObject::connect(mainMenu, SIGNAL(findPreviousTriggered(bool)), this,
	                 SLOT(doFindPrevious()));
	QObject::connect(mainMenu, SIGNAL(replaceTriggered(bool)), this,
	                 SLOT(doReplaceDialog()));
	QObject::connect(mainMenu, SIGNAL(goToTriggered(bool)), goToDialog,
	                 SLOT(show()));
	QObject::connect(mainMenu, SIGNAL(aboutQomposeTriggered(bool)),
	                 aboutDialog, SLOT(show()));
	QObject::connect(mainMenu, SIGNAL(aboutQtTriggered(bool)), qApp,
	                 SLOT(aboutQt()));

#ifdef QOMPOSE_DEBUG
	QObject::connect(mainMenu, SIGNAL(debugTriggered(bool)), this,
	                 SLOT(doDebug()));
#endif
}

void Window::applyExistingSettings()
{
	// Load our initial settings, and connect our settings object.

	statusBar->setVisible(settings->getSetting("show-status-bar").toBool());

	QObject::connect(
	        settings,
	        SIGNAL(settingChanged(const QString &, const QVariant &)), this,
	        SLOT(doSettingChanged(const QString &, const QVariant &)));

	// Restore our window's geometry and state.

	QByteArray winGeometry =
	        settings->getSetting("window-geometry").toByteArray();

	if(!winGeometry.isNull())
		if(!restoreGeometry(winGeometry))
			settings->setSetting("window-geometry",
			                     QVariant(QByteArray()));

	QByteArray winState =
	        settings->getSetting("window-state").toByteArray();

	if(!winState.isNull())
		if(!restoreState(winState, QOMPOSE_VERSION_MAJ))
			settings->setSetting("window-state",
			                     QVariant(QByteArray()));
}

void Window::handleFindResult(Editor::FindResult r)
{
	// Deal with the find result.

	switch(r)
	{
	case Editor::BadRegularExpression:
		QMessageBox::critical(
		        this, tr("Invalid Regular Expression"),
		        tr("The regular expression you were trying "
		           "to find was invalid."),
		        QMessageBox::Ok, QMessageBox::Ok);
		break;

	case Editor::NoMatches:
		QMessageBox::information(this, tr("No Matches Found"),
		                         tr("No more matches could be found."),
		                         QMessageBox::Ok, QMessageBox::Ok);
		break;

	default:
		break;
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

void Window::doUpdateWindowTitle()
{
	QString title("Qompose");

	bool showFile = settings->getSetting("show-file-in-title").toBool();

	if(showFile)
	{
		Buffer *buf = buffers->currentBuffer();

		if(buf != nullptr)
		{
			QString file = buf->getFile();

			if(file.length() == 0)
				file = "Untitled";

			title += " - ";
			title += file;
		}
	}

#ifdef QOMPOSE_DEBUG
	title += " [DEBUG]";
#endif

	setWindowTitle(title);
}

void Window::doTabPathChanged(const QString &p)
{
	doUpdateWindowTitle();
	statusBar->setCurrentTabPath(p);
}

void Window::doCursorPositionChanged(int l, int c)
{
	statusBar->setLine(l);
	statusBar->setColumn(c);
}

void Window::doSearchWrapped()
{
	statusBar->displayNotification(tr("Search wrapped around buffer."));
}

void Window::doPrint()
{
	if(!buffers->hasCurrentBuffer())
	{
		QMessageBox::warning(
		        this, tr("Nothing To Print"),
		        tr("No buffers are open; there is nothing to print!"),
		        QMessageBox::Ok, QMessageBox::Ok);

		return;
	}

	QPrinter printer(QPrinter::ScreenResolution);
	QPrintDialog dialog(&printer, this);

	if(dialog.exec() == QDialog::Accepted)
		buffers->doPrint(&printer);
}

void Window::doPrintPreview()
{
	if(!buffers->hasCurrentBuffer())
	{
		QMessageBox::warning(
		        this, tr("Nothing To Preview"),
		        tr("No buffers are open; there is nothing to "
		           "preview!"),
		        QMessageBox::Ok, QMessageBox::Ok);

		return;
	}

	QPrintPreviewDialog *dialog = new QPrintPreviewDialog(this, nullptr);

	QObject::connect(dialog, SIGNAL(paintRequested(QPrinter *)), buffers,
	                 SLOT(doPrint(QPrinter *)));

	dialog->exec();

	delete dialog;
}

void Window::doPreferencesDialog()
{
	if(!preferencesDialog->isVisible())
	{
		preferencesDialog->discardChanges();
		preferencesDialog->show();
	}
}

void Window::doFindDialog()
{
	if(!replaceDialog->isVisible())
	{
		findDialog->show();
	}
}

void Window::doFindNext()
{
	handleFindResult(buffers->doFindNext(findDialog->getQuery()));
}

void Window::doFindPrevious()
{
	handleFindResult(buffers->doFindPrevious(findDialog->getQuery()));
}

void Window::doReplaceDialog()
{
	if(!findDialog->isVisible())
	{
		replaceDialog->show();
	}
}

void Window::doReplace()
{
	handleFindResult(buffers->doReplace(replaceDialog->getQuery()));
}

void Window::doReplaceFind()
{
	handleFindResult(buffers->doFindNext(replaceDialog->getQuery()));
}

void Window::doReplaceSelection()
{
	handleFindResult(
	        buffers->doReplaceSelection(replaceDialog->getQuery()));
}

void Window::doReplaceAll()
{
	handleFindResult(buffers->doReplaceAll(replaceDialog->getQuery()));
}

void Window::doGoToAccepted()
{
	buffers->doGoTo(goToDialog->getSelectedLine());
}

#ifdef QOMPOSE_DEBUG
void Window::doDebug()
{
}
#endif

void Window::doSettingChanged(const QString &k, const QVariant &v)
{
	if(k == "show-file-in-title")
		doUpdateWindowTitle();
	else if(k == "show-status-bar")
		statusBar->setVisible(v.toBool());
}
}
