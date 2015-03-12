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

#include "MainMenu.h"

#include <utility>
#include <vector>

#include <QAction>
#include <QMenu>
#include <QObject>

#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/variant.hpp>

#include "QomposeCommon/Window.h"
#include "QomposeCommon/gui/BufferWidget.h"
#include "QomposeCommon/gui/GUIUtils.h"
#include "QomposeCommon/gui/menus/RecentMenu.h"
#include "QomposeCommon/util/Settings.h"

namespace
{
using qompose::menu_desc::MenuItemDescriptor;
using qompose::menu_desc::SeparatorDescriptor;
using qompose::menu_desc::RecentMenuDescriptor;

typedef std::vector<boost::variant<MenuItemDescriptor, SeparatorDescriptor,
                                   RecentMenuDescriptor>> DescriptorList;

template <typename VariantType>
QMenu *buildMenu(QWidget *parent, const std::string &title,
                 const std::vector<VariantType> &items)
{
	QMenu *menu = new QMenu(qompose::gui_utils::translate(title), parent);
	for(const auto &item : items)
	{
		boost::apply_visitor(
		        qompose::menu_desc::MenuDescriptorVisitor(parent, menu),
		        item);
	}
	return menu;
}

QMenu *createFileMenu(QWidget *parent, qompose::Settings *settings)
{
	qompose::gui_utils::ConnectionFunctor parentConn(parent);
	const DescriptorList items(
	        {MenuItemDescriptor("&New",
	                            parentConn(SIGNAL(newTriggered(bool))),
	                            Qt::CTRL + Qt::Key_N, "document-new"),
	         MenuItemDescriptor(
	                 "New &Window", parentConn(SLOT(doNewWindow())),
	                 Qt::CTRL + Qt::SHIFT + Qt::Key_N, "window-new"),
	         SeparatorDescriptor(),
	         MenuItemDescriptor("&Open...",
	                            parentConn(SIGNAL(openTriggered(bool))),
	                            Qt::CTRL + Qt::Key_O, "document-open"),
	         RecentMenuDescriptor(
	                 settings,
	                 parentConn(SIGNAL(recentTriggered(const QString &))),
	                 parentConn(SIGNAL(pathOpened(const QString &)))),
	         MenuItemDescriptor("Repoen Tab",
	                            parentConn(SIGNAL(reopenTriggered(bool))),
	                            Qt::CTRL + Qt::SHIFT + Qt::Key_T,
	                            "document-open"),
	         MenuItemDescriptor("&Revert",
	                            parentConn(SIGNAL(revertTriggered(bool))),
	                            Qt::CTRL + Qt::Key_R, "document-revert"),
	         MenuItemDescriptor(
	                 "Revert All",
	                 parentConn(SIGNAL(revertAllTriggered(bool))),
	                 Qt::CTRL + Qt::SHIFT + Qt::Key_R, "document-revert"),
	         SeparatorDescriptor(),
	         MenuItemDescriptor("&Save",
	                            parentConn(SIGNAL(saveTriggered(bool))),
	                            Qt::CTRL + Qt::Key_S, "document-save"),
	         MenuItemDescriptor("Save &As...",
	                            parentConn(SIGNAL(saveAsTriggered(bool))),
	                            Qt::CTRL + Qt::SHIFT + Qt::Key_S,
	                            "document-save-as"),
	         SeparatorDescriptor(),
	         MenuItemDescriptor("&Print...",
	                            parentConn(SIGNAL(printTriggered(bool))),
	                            Qt::CTRL + Qt::Key_P, "document-print"),
	         MenuItemDescriptor(
	                 "Print Pre&view...",
	                 parentConn(SIGNAL(printPreviewTriggered(bool))),
	                 QKeySequence(), "document-print-preview"),
	         SeparatorDescriptor(),
	         MenuItemDescriptor("Clos&e",
	                            parentConn(SIGNAL(closeTriggered(bool))),
	                            Qt::CTRL + Qt::Key_W, "window-close"),
	         MenuItemDescriptor("E&xit",
	                            parentConn(SIGNAL(exitTriggered(bool))),
	                            QKeySequence(), "application-exit")});
	return buildMenu(parent, "&File", items);
}
}

namespace qompose
{
MainMenu::MainMenu(Settings *s, QWidget *p)
        : QMenuBar(p),
          settings(s),
          editMenu(nullptr),
          viewMenu(nullptr),
          searchMenu(nullptr),
          buffersMenu(nullptr),
          helpMenu(nullptr),
          undoAction(nullptr),
          redoAction(nullptr),
          cutAction(nullptr),
          copyAction(nullptr),
          pasteAction(nullptr),
          duplicateLineAction(nullptr),
          selectAllAction(nullptr),
          deselectAction(nullptr),
          increaseIndentAction(nullptr),
          decreaseIndentAction(nullptr),
          preferencesAction(nullptr),
          showBrowserAction(nullptr),
          findAction(nullptr),
          findNextAction(nullptr),
          findPreviousAction(nullptr),
          replaceAction(nullptr),
          goToAction(nullptr),
          previousBufferAction(nullptr),
          nextBufferAction(nullptr),
          moveBufferLeftAction(nullptr),
          moveBufferRightAction(nullptr),
          aboutQomposeAction(nullptr),
          aboutQtAction(nullptr)
#ifdef QOMPOSE_DEBUG
          ,
          debugAction(nullptr)
#endif
{
	// Initialize our actions.

	undoAction = new QAction(tr("&Undo"), this);
	undoAction->setShortcut(Qt::CTRL + Qt::Key_Z);
	undoAction->setIcon(gui_utils::getIconFromTheme("edit-undo"));

	redoAction = new QAction(tr("&Redo"), this);
	redoAction->setShortcut(Qt::CTRL + Qt::Key_Y);
	redoAction->setIcon(gui_utils::getIconFromTheme("edit-redo"));

	cutAction = new QAction(tr("Cu&t"), this);
	cutAction->setShortcut(Qt::CTRL + Qt::Key_X);
	cutAction->setIcon(gui_utils::getIconFromTheme("edit-cut"));

	copyAction = new QAction(tr("&Copy"), this);
	copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
	copyAction->setIcon(gui_utils::getIconFromTheme("edit-copy"));

	pasteAction = new QAction(tr("&Paste"), this);
	pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);
	pasteAction->setIcon(gui_utils::getIconFromTheme("edit-paste"));

	duplicateLineAction = new QAction(tr("Duplicat&e Line"), this);
	duplicateLineAction->setShortcut(Qt::CTRL + Qt::Key_D);

	selectAllAction = new QAction(tr("Select &All"), this);
	selectAllAction->setShortcut(Qt::CTRL + Qt::Key_A);
	selectAllAction->setIcon(
	        gui_utils::getIconFromTheme("edit-select-all"));

	deselectAction = new QAction(tr("Dese&lect"), this);
	deselectAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_A);

	increaseIndentAction =
	        new QAction(tr("&Increase Selection Indent"), this);

	decreaseIndentAction =
	        new QAction(tr("&Decrease Selection Indent"), this);

	preferencesAction = new QAction(tr("Pre&ferences..."), this);
	preferencesAction->setIcon(
	        gui_utils::getIconFromTheme("preferences-other"));

	showBrowserAction = new QAction(tr("Show File &Browser"), this);
	showBrowserAction->setCheckable(true);
	showBrowserAction->setChecked(false);

	findAction = new QAction(tr("&Find..."), this);
	findAction->setShortcut(Qt::CTRL + Qt::Key_F);
	findAction->setIcon(gui_utils::getIconFromTheme("edit-find"));

	findNextAction = new QAction(tr("Find &Next"), this);
	findNextAction->setShortcut(Qt::Key_F3);
	findNextAction->setIcon(gui_utils::getIconFromTheme("go-next"));

	findPreviousAction = new QAction(tr("Find Previou&s"), this);
	findPreviousAction->setShortcut(Qt::SHIFT + Qt::Key_F3);
	findPreviousAction->setIcon(gui_utils::getIconFromTheme("go-previous"));

	replaceAction = new QAction(tr("R&eplace..."), this);
	replaceAction->setShortcut(Qt::CTRL + Qt::Key_H);
	replaceAction->setIcon(
	        gui_utils::getIconFromTheme("edit-find-replace"));

	goToAction = new QAction(tr("&Go To Line..."), this);
	goToAction->setShortcut(Qt::CTRL + Qt::Key_G);

	previousBufferAction = new QAction(tr("&Previous Buffer"), this);
	previousBufferAction->setShortcut(Qt::ALT + Qt::Key_Left);
	previousBufferAction->setIcon(
	        gui_utils::getIconFromTheme("go-previous"));

	nextBufferAction = new QAction(tr("&Next Buffer"), this);
	nextBufferAction->setShortcut(Qt::ALT + Qt::Key_Right);
	nextBufferAction->setIcon(gui_utils::getIconFromTheme("go-next"));

	moveBufferLeftAction = new QAction(tr("Move Buffer &Left"), this);
	moveBufferLeftAction->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_Left);
	moveBufferLeftAction->setIcon(
	        gui_utils::getIconFromTheme("go-previous"));

	moveBufferRightAction = new QAction(tr("Move Buffer &Right"), this);
	moveBufferRightAction->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_Right);
	moveBufferRightAction->setIcon(gui_utils::getIconFromTheme("go-next"));

	aboutQomposeAction = new QAction(tr("&About Qompose..."), this);
	aboutQomposeAction->setIcon(gui_utils::getIconFromTheme("help-about"));

	aboutQtAction = new QAction(tr("About &Qt..."), this);
	aboutQtAction->setIcon(gui_utils::getIconFromTheme("help-about"));

#ifdef QOMPOSE_DEBUG
	debugAction = new QAction(tr("Debugging..."), this);
#endif

	// Add these actions to our menu bar.

	addMenu(createFileMenu(this, settings));

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

	viewMenu = new QMenu(tr("&View"), this);
	viewMenu->addAction(showBrowserAction);

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

#ifdef QOMPOSE_DEBUG
	helpMenu->addSeparator();
	helpMenu->addAction(debugAction);
#endif

	addMenu(editMenu);
	addMenu(viewMenu);
	addMenu(searchMenu);
	addMenu(buffersMenu);
	addMenu(helpMenu);

	// Connect certain signals.

	QObject::connect(preferencesAction, SIGNAL(triggered(bool)), this,
	                 SIGNAL(preferencesTriggered(bool)));
	QObject::connect(showBrowserAction, SIGNAL(triggered(bool)), this,
	                 SIGNAL(showBrowserTriggered(bool)));
	QObject::connect(findAction, SIGNAL(triggered(bool)), this,
	                 SIGNAL(findTriggered(bool)));
	QObject::connect(findNextAction, SIGNAL(triggered(bool)), this,
	                 SIGNAL(findNextTriggered(bool)));
	QObject::connect(findPreviousAction, SIGNAL(triggered(bool)), this,
	                 SIGNAL(findPreviousTriggered(bool)));
	QObject::connect(replaceAction, SIGNAL(triggered(bool)), this,
	                 SIGNAL(replaceTriggered(bool)));
	QObject::connect(goToAction, SIGNAL(triggered(bool)), this,
	                 SIGNAL(goToTriggered(bool)));
	QObject::connect(aboutQomposeAction, SIGNAL(triggered(bool)), this,
	                 SIGNAL(aboutQomposeTriggered(bool)));
	QObject::connect(aboutQtAction, SIGNAL(triggered(bool)), this,
	                 SIGNAL(aboutQtTriggered(bool)));

#ifdef QOMPOSE_DEBUG
	QObject::connect(debugAction, SIGNAL(triggered(bool)), this,
	                 SIGNAL(debugTriggered(bool)));
#endif
}

MainMenu::~MainMenu()
{
}

void MainMenu::connectBufferWidget(const BufferWidget *b)
{
	QObject::connect(b, SIGNAL(pathOpened(const QString &)), this,
	                 SIGNAL(pathOpened(const QString &)));

	// Connect file menu actions.

	QObject::connect(this, SIGNAL(newTriggered(bool)), b, SLOT(doNew()));
	QObject::connect(this, SIGNAL(openTriggered(bool)), b, SLOT(doOpen()));
	QObject::connect(this, SIGNAL(recentTriggered(const QString &)), b,
	                 SLOT(doOpenPath(const QString &)));
	QObject::connect(this, SIGNAL(reopenTriggered(bool)), b,
	                 SLOT(doReopen()));
	QObject::connect(this, SIGNAL(revertTriggered(bool)), b,
	                 SLOT(doRevert()));
	QObject::connect(this, SIGNAL(revertAllTriggered(bool)), b,
	                 SLOT(doRevertAll()));
	QObject::connect(this, SIGNAL(saveTriggered(bool)), b, SLOT(doSave()));
	QObject::connect(this, SIGNAL(saveAsTriggered(bool)), b,
	                 SLOT(doSaveAs()));
	QObject::connect(this, SIGNAL(closeTriggered(bool)), b,
	                 SLOT(doClose()));

	// Connect the rest of our actions.

	QObject::connect(undoAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doUndo()));
	QObject::connect(redoAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doRedo()));
	QObject::connect(cutAction, SIGNAL(triggered(bool)), b, SLOT(doCut()));
	QObject::connect(copyAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doCopy()));
	QObject::connect(pasteAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doPaste()));
	QObject::connect(duplicateLineAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doDuplicateLine()));
	QObject::connect(selectAllAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doSelectAll()));
	QObject::connect(deselectAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doDeselect()));
	QObject::connect(increaseIndentAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doIncreaseIndent()));
	QObject::connect(decreaseIndentAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doDecreaseIndent()));
	QObject::connect(previousBufferAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doPreviousBuffer()));
	QObject::connect(nextBufferAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doNextBuffer()));
	QObject::connect(moveBufferLeftAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doMoveBufferLeft()));
	QObject::connect(moveBufferRightAction, SIGNAL(triggered(bool)), b,
	                 SLOT(doMoveBufferRight()));
}

/*!
 * This slot handles the "new window" action by opening a new Qompose window.
 */
void MainMenu::doNewWindow()
{
	Window::openNewWindow();
}

/*!
 * This slot handles a file being opened by updating our recently opened menu
 * with the newly opened path.
 *
 * \param p The path to the file that was opened.
 */
void MainMenu::doFileOpened(const QString &p)
{
	Q_EMIT(pathOpened(p));
}
}
