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

QMenu *createEditMenu(QWidget *parent)
{
	qompose::gui_utils::ConnectionFunctor parentConn(parent);
	const DescriptorList items(
	        {MenuItemDescriptor("&Undo",
	                            parentConn(SIGNAL(undoTriggered(bool))),
	                            Qt::CTRL + Qt::Key_Z, "edit-undo"),
	         MenuItemDescriptor("&Redo",
	                            parentConn(SIGNAL(redoTriggered(bool))),
	                            Qt::CTRL + Qt::Key_Y, "edit-redo"),
	         SeparatorDescriptor(),
	         MenuItemDescriptor("Cu&t",
	                            parentConn(SIGNAL(cutTriggered(bool))),
	                            Qt::CTRL + Qt::Key_X, "edit-cut"),
	         MenuItemDescriptor("&Copy",
	                            parentConn(SIGNAL(copyTriggered(bool))),
	                            Qt::CTRL + Qt::Key_C, "edit-copy"),
	         MenuItemDescriptor("&Paste",
	                            parentConn(SIGNAL(pasteTriggered())),
	                            Qt::CTRL + Qt::Key_V, "edit-paste"),
	         MenuItemDescriptor(
	                 "Duplicat&e Line",
	                 parentConn(SIGNAL(duplicateLineTriggered(bool))),
	                 Qt::CTRL + Qt::Key_D),
	         MenuItemDescriptor(
	                 "Select &All",
	                 parentConn(SIGNAL(selectAllTriggered(bool))),
	                 Qt::CTRL + Qt::Key_A, "edit-select-all"),
	         MenuItemDescriptor("Dese&lect",
	                            parentConn(SIGNAL(deselectTriggered(bool))),
	                            Qt::CTRL + Qt::SHIFT + Qt::Key_A),
	         SeparatorDescriptor(),
	         MenuItemDescriptor(
	                 "&Increase Selection Indent",
	                 parentConn(SIGNAL(increaseIndentTriggered(bool)))),
	         MenuItemDescriptor(
	                 "&Decrease Selection Indent",
	                 parentConn(SIGNAL(decreaseIndentTriggered(bool)))),
	         SeparatorDescriptor(),
	         MenuItemDescriptor(
	                 "Pre&ferences...",
	                 parentConn(SIGNAL(preferencesTriggered(bool))),
	                 QKeySequence(), "preferences-other")});
	return buildMenu(parent, "&Edit", items);
}

QMenu *createViewMenu(QWidget *parent)
{
	qompose::gui_utils::ConnectionFunctor parentConn(parent);
	const DescriptorList items({MenuItemDescriptor(
	        "Show File &Browser",
	        parentConn(SIGNAL(showBrowserTriggered(bool))), QKeySequence(),
	        QString(), true)});
	return buildMenu(parent, "&View", items);
}

QMenu *createSearchMenu(QWidget *parent)
{
	qompose::gui_utils::ConnectionFunctor parentConn(parent);
	const DescriptorList items(
	        {MenuItemDescriptor("&Find...",
	                            parentConn(SIGNAL(findTriggered(bool))),
	                            Qt::CTRL + Qt::Key_F, "edit-find"),
	         MenuItemDescriptor("Find &Next",
	                            parentConn(SIGNAL(findNextTriggered(bool))),
	                            Qt::Key_F3, "go-next"),
	         MenuItemDescriptor(
	                 "Find Previou&s",
	                 parentConn(SIGNAL(findPreviousTriggered(bool))),
	                 Qt::SHIFT + Qt::Key_F3, "go-previous"),
	         MenuItemDescriptor("R&eplace...",
	                            parentConn(SIGNAL(replaceTriggered(bool))),
	                            Qt::CTRL + Qt::Key_H, "edit-find-replace"),
	         MenuItemDescriptor("&Go To Line...",
	                            parentConn(SIGNAL(goToTriggered(bool))),
	                            Qt::CTRL + Qt::Key_G)});
	return buildMenu(parent, "&Search", items);
}
}

namespace qompose
{
MainMenu::MainMenu(Settings *s, QWidget *p)
        : QMenuBar(p),
          settings(s),
          buffersMenu(nullptr),
          helpMenu(nullptr),
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
	addMenu(createEditMenu(this));
	addMenu(createViewMenu(this));
	addMenu(createSearchMenu(this));

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

	addMenu(buffersMenu);
	addMenu(helpMenu);

	// Connect certain signals.

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

	// Connect edit menu actions.

	QObject::connect(this, SIGNAL(undoTriggered(bool)), b, SLOT(doUndo()));
	QObject::connect(this, SIGNAL(redoTriggered(bool)), b, SLOT(doRedo()));
	QObject::connect(this, SIGNAL(cutTriggered(bool)), b, SLOT(doCut()));
	QObject::connect(this, SIGNAL(copyTriggered(bool)), b, SLOT(doCopy()));
	QObject::connect(this, SIGNAL(pasteTriggered(bool)), b,
	                 SLOT(doPaste()));
	QObject::connect(this, SIGNAL(duplicateLineTriggered(bool)), b,
	                 SLOT(doDuplicateLine()));
	QObject::connect(this, SIGNAL(selectAllTriggered(bool)), b,
	                 SLOT(doSelectAll()));
	QObject::connect(this, SIGNAL(deselectTriggered(bool)), b,
	                 SLOT(doDeselect()));
	QObject::connect(this, SIGNAL(increaseIndentTriggered(bool)), b,
	                 SLOT(doIncreaseIndent()));
	QObject::connect(this, SIGNAL(decreaseIndentTriggered(bool)), b,
	                 SLOT(doDecreaseIndent()));

	// Connect the rest of our actions.

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
