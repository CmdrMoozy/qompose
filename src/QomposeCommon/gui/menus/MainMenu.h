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
class Window;

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
	/*!
	 * This is our default constructor, which creates a new main menu.
	 *
	 * \param s The settings object to connect to menu items.
	 * \param p The menu bar's parent widget.
	 */
	MainMenu(Settings *s, Window *p = nullptr);

	MainMenu(const MainMenu &) = delete;
	virtual ~MainMenu() = default;

	MainMenu &operator=(const MainMenu &) = delete;

	/*!
	 * This function connects all of this menu's signals to the given
	 * buffer widget.
	 *
	 * \param b The buffer widget to connect.
	 */
	void connectBufferWidget(const BufferWidget *b);

private:
	Settings *settings;
	const BufferWidget *bufferWidget;

private Q_SLOTS:
	/*!
	 * This slot handles the "new window" action by opening a new Qompose
	 * window.
	 */
	void doNewWindow();

	/*!
	 * This slot handles the current buffer being changed by updating our
	 * buffer-specific menu items.
	 */
	void doBufferChanged();

	/*!
	 * This slot handles a file being opened by updating our recently
	 * opened menu with the newly opened path.
	 *
	 * \param p The path to the file that was opened.
	 */
	void doFileOpened(const QString &p);

	/*!
	 * This slot handles our file browser widget menu item's visibility
	 * changing.
	 *
	 * \param s Whether or not the file browser widget should be shown.
	 */
	void doShowBrowser(bool s);

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
	void lineWrappingTriggered(bool);
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
	void lineWrappingChanged(bool);
	void browserWidgetVisibilityChanged(bool);
};
}

#endif
