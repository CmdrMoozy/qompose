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

namespace qompose
{
class BufferWidget;
class Settings;

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
	MainMenu(const MainMenu &) = delete;
	virtual ~MainMenu();

	MainMenu &operator=(const MainMenu &) = delete;

	void connectBufferWidget(const BufferWidget *);

private Q_SLOTS:
	void doNewWindow();
	void doFileOpened(const QString &);

Q_SIGNALS:
	void newTriggered(bool);
	void openTriggered(bool);
	void recentTriggered(const QString &);
	void reopenTriggered(bool);
	void revertTriggered(bool);
	void revertAllTriggered(bool);
	void saveTriggered(bool);
	void saveAsTriggered(bool);
	void printTriggered(bool);
	void printPreviewTriggered(bool);
	void closeTriggered(bool);
	void exitTriggered(bool);
	void undoTriggered(bool);
	void redoTriggered(bool);
	void cutTriggered(bool);
	void copyTriggered(bool);
	void pasteTriggered(bool);
	void duplicateLineTriggered(bool);
	void selectAllTriggered(bool);
	void deselectTriggered(bool);
	void increaseIndentTriggered(bool);
	void decreaseIndentTriggered(bool);
	void preferencesTriggered(bool);
	void showBrowserTriggered(bool);
	void findTriggered(bool);
	void findNextTriggered(bool);
	void findPreviousTriggered(bool);
	void replaceTriggered(bool);
	void goToTriggered(bool);
	void previousBufferTriggered(bool);
	void nextBufferTriggered(bool);
	void moveBufferLeftTriggered(bool);
	void moveBufferRightTriggered(bool);
	void aboutQomposeTriggered(bool);
	void aboutQtTriggered(bool);
	void debugTriggered(bool);

	void pathOpened(const QString &);
};
}

#endif
