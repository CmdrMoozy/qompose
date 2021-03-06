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

#include <cassert>
#include <stdexcept>
#include <utility>
#include <vector>

#include <QMenu>
#include <QObject>

#include <mapbox_variant/recursive_wrapper.hpp>
#include <mapbox_variant/variant.hpp>

#include "core/config/Configuration.hpp"

#include "QomposeCommon/Window.h"
#include "QomposeCommon/editor/Buffer.h"
#include "QomposeCommon/gui/BufferWidget.h"
#include "QomposeCommon/gui/GUIUtils.h"
#include "QomposeCommon/gui/menus/EncodingMenu.h"
#include "QomposeCommon/gui/menus/RecentMenu.h"

namespace
{
using qompose::menu_desc::EncodingMenuDescriptor;
using qompose::menu_desc::MenuItemDescriptor;
using qompose::menu_desc::RecentMenuDescriptor;
using qompose::menu_desc::SeparatorDescriptor;

typedef std::vector<
        mapbox::util::variant<MenuItemDescriptor, RecentMenuDescriptor,
                              EncodingMenuDescriptor, SeparatorDescriptor>>
        DescriptorList;

template <typename VariantType>
QMenu *buildMenu(QWidget *parent, const std::string &title,
                 const std::vector<VariantType> &items)
{
	QMenu *menu = new QMenu(qompose::gui_utils::translate(title), parent);
	for(const auto &item : items)
	{
		mapbox::util::apply_visitor(
		        qompose::menu_desc::MenuDescriptorVisitor(parent, menu),
		        item);
	}
	return menu;
}

QMenu *createFileMenu(QWidget *parent)
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
	                            parentConn(SIGNAL(pasteTriggered(bool))),
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
	const DescriptorList items(
	        {MenuItemDescriptor(
	                 "&Line Wrapping",
	                 parentConn(SIGNAL(lineWrappingTriggered(bool))),
	                 QKeySequence(), QString(), true, false,
	                 parentConn(SIGNAL(lineWrappingChanged(bool)))),
	         MenuItemDescriptor(
	                 "Show File &Browser",
	                 parentConn(SLOT(doShowBrowser(bool))), QKeySequence(),
	                 QString(), true, qompose::core::config::instance()
	                                          .get()
	                                          .show_file_browser(),
	                 parentConn(SIGNAL(
	                         browserWidgetVisibilityChanged(bool))))});
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

QMenu *createBuffersMenu(QWidget *parent)
{
	qompose::gui_utils::ConnectionFunctor parentConn(parent);
	const DescriptorList items(
	        {MenuItemDescriptor(
	                 "&Previous Buffer",
	                 parentConn(SIGNAL(previousBufferTriggered(bool))),
	                 Qt::ALT + Qt::Key_Left, "go-previous"),
	         MenuItemDescriptor(
	                 "&Next Buffer",
	                 parentConn(SIGNAL(nextBufferTriggered(bool))),
	                 Qt::ALT + Qt::Key_Right, "go-next"),
	         SeparatorDescriptor(),
	         MenuItemDescriptor(
	                 "Move Buffer &Left",
	                 parentConn(SIGNAL(moveBufferLeftTriggered(bool))),
	                 Qt::CTRL + Qt::ALT + Qt::Key_Left, "go-previous"),
	         MenuItemDescriptor(
	                 "Move Buffer &Right",
	                 parentConn(SIGNAL(moveBufferRightTriggered(bool))),
	                 Qt::CTRL + Qt::ALT + Qt::Key_Right, "go-next")});
	return buildMenu(parent, "&Buffers", items);
}

QMenu *createEncodingMenu(QWidget *parent)
{
	qompose::gui_utils::ConnectionFunctor parentConn(parent);
	const DescriptorList items({EncodingMenuDescriptor(
	        "Buffer Encoding",
	        parentConn(SIGNAL(encodingTriggered(const QByteArray &))),
	        parentConn(SIGNAL(encodingChanged(const QByteArray &))))});
	return buildMenu(parent, "&Encoding", items);
}

QMenu *createHelpMenu(QWidget *parent)
{
	qompose::gui_utils::ConnectionFunctor parentConn(parent);
	const DescriptorList items(
	        {MenuItemDescriptor(
	                 "&About Qompose...",
	                 parentConn(SIGNAL(aboutQomposeTriggered(bool))),
	                 QKeySequence(), "help-about"),
	         MenuItemDescriptor("About &Qt...",
	                            parentConn(SIGNAL(aboutQtTriggered(bool))),
	                            QKeySequence(), "help-about")
#ifdef QOMPOSE_DEBUG
	                 ,
	         SeparatorDescriptor(),
	         MenuItemDescriptor("Debugging...",
	                            parentConn(SIGNAL(debugTriggered(bool))))
#endif
	        });
	return buildMenu(parent, "&Help", items);
}
}

namespace qompose
{
MainMenu::MainMenu(Window *p) : QMenuBar(p), bufferWidget(nullptr)
{
	addMenu(createFileMenu(this));
	addMenu(createEditMenu(this));
	addMenu(createViewMenu(this));
	addMenu(createSearchMenu(this));
	addMenu(createBuffersMenu(this));
	addMenu(createEncodingMenu(this));
	addMenu(createHelpMenu(this));

	QObject::connect(p, SIGNAL(browserWidgetVisibilityChanged(bool)), this,
	                 SIGNAL(browserWidgetVisibilityChanged(bool)));
}

void MainMenu::connectBufferWidget(const BufferWidget *b)
{
	if(bufferWidget != nullptr)
	{
		throw std::runtime_error("Only one buffer widget "
		                         "can be connected to a menu.");
	}

	bufferWidget = b;

	QObject::connect(b, SIGNAL(bufferChanged()), this,
	                 SLOT(doBufferChanged()));
	QObject::connect(b, SIGNAL(pathOpened(const QString &)), this,
	                 SIGNAL(pathOpened(const QString &)));
	QObject::connect(b, SIGNAL(encodingChanged(const QByteArray &)), this,
	                 SIGNAL(encodingChanged(const QByteArray &)));

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

	// Connect view menu actions.

	QObject::connect(this, SIGNAL(lineWrappingTriggered(bool)), b,
	                 SLOT(doLineWrapping(bool)));

	// Connect encoding menu actions.

	QObject::connect(this, SIGNAL(encodingTriggered(const QByteArray &)), b,
	                 SLOT(doSetEncoding(const QByteArray &)));

	// Connect buffers menu actions.

	QObject::connect(this, SIGNAL(previousBufferTriggered(bool)), b,
	                 SLOT(doPreviousBuffer()));
	QObject::connect(this, SIGNAL(nextBufferTriggered(bool)), b,
	                 SLOT(doNextBuffer()));
	QObject::connect(this, SIGNAL(moveBufferLeftTriggered(bool)), b,
	                 SLOT(doMoveBufferLeft()));
	QObject::connect(this, SIGNAL(moveBufferRightTriggered(bool)), b,
	                 SLOT(doMoveBufferRight()));
}

void MainMenu::doNewWindow()
{
	Window::openNewWindow();
}

void MainMenu::doBufferChanged()
{
	assert(QObject::sender() == bufferWidget);

	editor::Buffer *buffer = bufferWidget->currentBuffer();
	if(buffer != nullptr)
		Q_EMIT lineWrappingChanged(buffer->getLineWrapping());
}

void MainMenu::doFileOpened(const QString &p)
{
	Q_EMIT(pathOpened(p));
}

void MainMenu::doShowBrowser(bool s)
{
	auto config = qompose::core::config::instance().get();
	config.set_show_file_browser(s);
	qompose::core::config::instance().set(config);
}
}
