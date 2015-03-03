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

#ifndef INCLUDE_QOMPOSECOMMON_GUI_MENUS_MAIN_MENU_H
#define INCLUDE_QOMPOSECOMMON_GUI_MENUS_MAIN_MENU_H

#include <QMenuBar>

class QMenu;
class QAction;

namespace qompose
{
class BufferWidget;
class Settings;
class RecentMenu;

/*!
 * \brief This class implements Qompose's main menu bar.
 *
 * This menu bar contains a large number of actions, which are categorized
 * into either "buffer actions" or "application actions."
 *
 * "Buffer actions" should be connected directly to a buffer widget (via
 * connectBufferWidget()).
 *
 * "Application actions" can be handled in several different ways, and
 * as such are simply emitted from instances of this class as signals.
 */
class MainMenu : public QMenuBar
{
	Q_OBJECT

public:
	MainMenu(Settings *, QWidget * = nullptr);
	virtual ~MainMenu();

	void connectBufferWidget(const BufferWidget *);

private:
	Settings *settings;

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *searchMenu;
	QMenu *buffersMenu;
	QMenu *helpMenu;

	QAction *newAction;
	QAction *newWindowAction;
	QAction *openAction;
	RecentMenu *recentMenu;
	QAction *reopenAction;
	QAction *revertAction;
	QAction *revertAllAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *printAction;
	QAction *printPreviewAction;
	QAction *closeAction;
	QAction *exitAction;

	QAction *undoAction;
	QAction *redoAction;
	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *duplicateLineAction;
	QAction *selectAllAction;
	QAction *deselectAction;
	QAction *increaseIndentAction;
	QAction *decreaseIndentAction;
	QAction *preferencesAction;

	QAction *showBrowserAction;

	QAction *findAction;
	QAction *findNextAction;
	QAction *findPreviousAction;
	QAction *replaceAction;
	QAction *goToAction;

	QAction *previousBufferAction;
	QAction *nextBufferAction;
	QAction *moveBufferLeftAction;
	QAction *moveBufferRightAction;

	QAction *aboutQomposeAction;
	QAction *aboutQtAction;

#ifdef QOMPOSE_DEBUG
	QAction *debugAction;
#endif

	MainMenu(const MainMenu &);
	MainMenu &operator=(const MainMenu &);

private Q_SLOTS:
	void doNewWindow();
	void doFileOpened(const QString &);

Q_SIGNALS:
	void printTriggered(bool);
	void printPreviewTriggered(bool);
	void exitTriggered(bool);
	void preferencesTriggered(bool);
	void showBrowserTriggered(bool);
	void findTriggered(bool);
	void findNextTriggered(bool);
	void findPreviousTriggered(bool);
	void replaceTriggered(bool);
	void goToTriggered(bool);
	void aboutQomposeTriggered(bool);
	void aboutQtTriggered(bool);

#ifdef QOMPOSE_DEBUG
	void debugTriggered(bool);
#endif
};
}

#endif
