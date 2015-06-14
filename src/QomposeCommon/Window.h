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

#ifndef INCLUDE_QOMPOSECOMMON_WINDOW_H
#define INCLUDE_QOMPOSECOMMON_WINDOW_H

#include <vector>

#include <QMainWindow>
#include <QIcon>

#include "QomposeCommon/editor/Editor.h"

class QCloseEvent;
class QPrinter;

namespace qompose
{
class AboutDialog;
class BrowserDockWidget;
class BufferWidget;
class FindDialog;
class GoToDialog;
class MainMenu;
class PreferencesDialog;
class ReplaceDialog;
class StatusBar;

/*!
 * \brief This class implements one of our application's standard windows.
 */
class Window : public QMainWindow
{
	Q_OBJECT

public:
	/*!
	 * This is a static function which opens a new Qompose window.
	 */
	static void openNewWindow();

private:
	static std::vector<Window *> windows;

public:
	virtual ~Window() = default;

protected:
	/*!
	 * This is our default constructor, which creates a new Qompose window,
	 * and initializes its contents.
	 *
	 * \param p Our parent widget, if any.
	 * \param f The Qt window flags to use for this window.
	 */
	Window(QWidget *p = nullptr, Qt::WindowFlags f = nullptr);

	Window(const Window &) = delete;
	Window &operator=(const Window &) = delete;

	/*!
	 * This function handles our window being closed by notifying our
	 * buffers of the imminent close event, and then proceeding according
	 * to what it reports back to us.
	 *
	 * \param e The event being handled.
	 */
	void closeEvent(QCloseEvent *e);

private:
	PreferencesDialog *preferencesDialog;
	FindDialog *findDialog;
	ReplaceDialog *replaceDialog;
	GoToDialog *goToDialog;
	AboutDialog *aboutDialog;

	MainMenu *mainMenu;

	BufferWidget *buffers;

	StatusBar *statusBar;

	BrowserDockWidget *browserWidget;

	/*!
	 * This function initializes our dialog objects.
	 */
	void initializeDialogs();

	/*!
	 * This function initializes our main menu bar. Note that this function
	 * must be called AFTER initializeActions, as we need actions to be
	 * initialized before we can add them to menus.
	 */
	void initializeMenus();

	/*!
	 * This function initializes the various dock widgets which can be
	 * displayed as components of the main window.
	 */
	void initializeDockWidgets();

	/*!
	 * This function reads various settings properties from our settings
	 * object, and applies their values to our UI. Additionally, we connect
	 * the settings object to our slots, so we can listen for settings
	 * changes.
	 */
	void applyExistingSettings();

	/*!
	 * This function handles the result of a find operation by alerting the
	 * user about what happened. This includes things like trying to find
	 * using a bad regular expression, or finding something for which no
	 * matches were found.
	 *
	 * \param r The find result to process.
	 */
	void handleFindResult(Editor::FindResult r);

private Q_SLOTS:
	/*!
	 * This slot updates our window title. This slot should be connected to
	 * any actions which might alter the window title - the current buffer
	 * changing, certain settings changing, etc.
	 */
	void doUpdateWindowTitle();

	/*!
	 * This slot handles one of our tab's paths being changed by updating
	 * that tab's path label.
	 *
	 * \param p The new path for the current tab.
	 */
	void doTabPathChanged(const QString &p);

	/*!
	 * This slot handles our "print" action being triggered by displaying a
	 * standard print dialog and, if it is accepted, printing our current
	 * buffer using the printer object it configures.
	 */
	void doPrint();

	/*!
	 * This slot handles our "print preview" action being triggered by
	 * displaying a standard print preview dialog using our buffers'
	 * standard print slot.
	 */
	void doPrintPreview();

	/*!
	 * This slot handles our preferences dialog action being triggered
	 * by resetting and showing our preferences dialog (if it isn't already
	 * visible).
	 */
	void doPreferencesDialog();

	/*!
	 * This slot shows or hides the file browser dock widget.
	 *
	 * \param show Whether or not the file browser widget should be shown.
	 */
	void doShowBrowser(bool s);

	/*!
	 * This slot handles the browser widget's visiblity changing by
	 * updating the associated setting, as well as by emitting a signal
	 * to let anyone else who cares know about the change.
	 *
	 * \param v Whether or not the browser widget is now visible.
	 */
	void doBrowserWidgetVisibilityChanged(bool v);

	/*!
	 * This function handles our "find" action being triggered by showing
	 * our find dialog, if our replace dialog isn't already open (they are
	 * mutually exclusive).
	 */
	void doFindDialog();

	/*!
	 * This slot performs a "find next" operation by extracting the current
	 * find query from the find dialog, telling our buffers widget to
	 * execute the query, and then dealing with the result.
	 */
	void doFindNext();

	/*!
	 * This slot performs a "find previous" operation by extracting the
	 * current find query from the find dialog, telling our buffers widget
	 * to execute the query, and then dealing with the result.
	 */
	void doFindPrevious();

	/*!
	 * This function handles our "replace" action being triggered by
	 * showing our replace dialog, if our find dialog isn't already open
	 * (they are mutually exclusive).
	 */
	void doReplaceDialog();

	/*!
	 * This slot performs a single "replace" operation by extracting the
	 * current replace query from the replace dialog, telling our buffers
	 * widget to execute the query, and then dealing with the result.
	 */
	void doReplace();

	/*!
	 * This slot performs a "replace find" operation. This is effectively
	 * the same as a "find next" operation, but we use the query from the
	 * replace dialog, instead of the one from the find dialog. We perform
	 * this operation by extracting the current replace query from the
	 * replace dialog, telling our buffers widget to execute the query as
	 * a "find next" operation, and then we deal with the result.
	 */
	void doReplaceFind();

	/*!
	 * This slot performs a "replace in selection" operation by extracting
	 * the current replace query from the replace dialog, telling our
	 * buffers widget to execute the query, and then dealing with the
	 * result.
	 */
	void doReplaceSelection();

	/*!
	 * This slot performs a "replace all" operation by extracting the
	 * current replace query from the replace dialog, telling our buffers
	 * widget to execute the query, and then dealing with the result.
	 */
	void doReplaceAll();

	/*!
	 * This function handles our "go to" dialog being accepted by telling
	 * our buffers widget to perform the "go to" operation.
	 */
	void doGoToAccepted();

#ifdef QOMPOSE_DEBUG
	/*!
	 * This function performs some action to help with debugging.
	 */
	void doDebug();
#endif

	/*!
	 * This function handles a setting changed by applying that change to
	 * our window.
	 *
	 * \param k The setting key that was changed.
	 * \param v The new value for the given setting.
	 */
	void doSettingChanged(const QString &k, const QVariant &v);

Q_SIGNALS:
	void browserWidgetVisibilityChanged(bool);
};
}

#endif
