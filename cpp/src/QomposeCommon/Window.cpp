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
#include <QCloseEvent>
#include <QFile>
#include <QIcon>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>

#include "core/config/Configuration.hpp"

#include "QomposeCommon/Defines.h"
#include "QomposeCommon/dialogs/AboutDialog.h"
#include "QomposeCommon/dialogs/FindDialog.h"
#include "QomposeCommon/dialogs/GoToDialog.h"
#include "QomposeCommon/dialogs/ReplaceDialog.h"
#include "QomposeCommon/dialogs/preferences/PreferencesDialog.h"
#include "QomposeCommon/editor/Buffer.h"
#include "QomposeCommon/gui/BufferWidget.h"
#include "QomposeCommon/gui/GUIUtils.h"
#include "QomposeCommon/gui/dock/BrowserDockWidget.h"
#include "QomposeCommon/gui/menus/MainMenu.h"

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
          configWatcher(new qompose::util::ConfigurationWatcher(this)),
          preferencesDialog(nullptr),
          findDialog(nullptr),
          replaceDialog(nullptr),
          goToDialog(nullptr),
          aboutDialog(nullptr),
          mainMenu(nullptr),
          buffers(nullptr),
          browserWidget(nullptr)
{
	// Set some of our window's properties.

	setWindowIcon(QIcon(":/icons/qompose.png"));

	// Initialize our window.

	buffers = new BufferWidget(this);
	buffers->doNew();
	setCentralWidget(buffers);

	initializeDialogs();
	initializeMenus();
	initializeDockWidgets();

	QObject::connect(buffers, &BufferWidget::pathChanged, this,
	                 &Window::doUpdateWindowTitle);

	// Apply any existing settings values to our UI.

	applyExistingSettings();

	// Update our window title initially.

	doUpdateWindowTitle();
}

void Window::closeEvent(QCloseEvent *e)
{
	if(buffers->prepareCloseParent())
	{
		auto config = qompose::core::config::instance().get();
		config.set_window_geometry(saveGeometry().toStdString());
		config.set_window_state(
		        saveState(QOMPOSE_VERSION_MAJ).toStdString());
		qompose::core::config::instance().set(config);

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

	preferencesDialog = new PreferencesDialog(this);

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

	mainMenu = new MainMenu(this);

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

void Window::initializeDockWidgets()
{
	browserWidget = new BrowserDockWidget(this);
	addDockWidget(Qt::RightDockWidgetArea, browserWidget);
	browserWidget->setVisible(
	        qompose::core::config::instance().get().show_file_browser());

	QObject::connect(browserWidget, SIGNAL(visibilityChanged(bool)), this,
	                 SLOT(doBrowserWidgetVisibilityChanged(bool)));
}

void Window::applyExistingSettings()
{
	// Load our initial settings, and connect our settings object.

	QObject::connect(configWatcher,
	                 SIGNAL(configurationFieldChanged(std::string const &)),
	                 this, SLOT(doSettingChanged(std::string const &)));

	// Restore our window's geometry and state.

	QByteArray winGeometry = QByteArray::fromStdString(
	        qompose::core::config::instance().get().window_geometry());
	if(!winGeometry.isEmpty())
	{
		if(!restoreGeometry(winGeometry))
		{
			auto config = qompose::core::config::instance().get();
			config.clear_window_geometry();
			qompose::core::config::instance().set(config);
		}
	}

	QByteArray winState = QByteArray::fromStdString(
	        qompose::core::config::instance().get().window_state());
	if(!winState.isEmpty())
	{
		if(!restoreState(winState, QOMPOSE_VERSION_MAJ))
		{
			auto config = qompose::core::config::instance().get();
			config.clear_window_state();
			qompose::core::config::instance().set(config);
		}
	}
}

void Window::handleFindResult(editor::search::FindResult r)
{
	// Deal with the find result.

	switch(r)
	{
	case editor::search::FindResult::BadRegularExpression:
		QMessageBox::critical(
		        this, tr("Invalid Regular Expression"),
		        tr("The regular expression you were trying "
		           "to find was invalid."),
		        QMessageBox::Ok, QMessageBox::Ok);
		break;

	case editor::search::FindResult::NoMatches:
		QMessageBox::information(this, tr("No Matches Found"),
		                         tr("No more matches could be found."),
		                         QMessageBox::Ok, QMessageBox::Ok);
		break;

	case editor::search::FindResult::NoDocument:
	case editor::search::FindResult::Found:
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

	if(qompose::core::config::instance().get().show_file_in_title())
	{
		editor::Buffer *buf = buffers->currentBuffer();

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

void Window::doShowBrowser(bool s)
{
	browserWidget->setVisible(s);
}

void Window::doBrowserWidgetVisibilityChanged(bool v)
{
	auto config = qompose::core::config::instance().get();
	config.set_show_file_browser(v);
	qompose::core::config::instance().set(config);
	Q_EMIT browserWidgetVisibilityChanged(v);
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

void Window::doSettingChanged(std::string const &name)
{
	if(name == "show_file_in_title")
	{
		doUpdateWindowTitle();
	}
	else if(name == "show_file_browser")
	{
		doShowBrowser(qompose::core::config::instance()
		                      .get()
		                      .show_file_browser());
	}
}
}
