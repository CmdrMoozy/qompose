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

#include <QMainWindow>
#include <QIcon>

#include "QomposeCommon/editor/Editor.h"

class QCloseEvent;
class QPrinter;

class QomposeSettings;
class QomposeStatusBar;

namespace qompose
{

class PreferencesDialog;
class BufferWidget;
class AboutDialog;
class FindDialog;
class ReplaceDialog;
class GoToDialog;
class MainMenu;

/*!
 * \brief This class implements one of our application's standard windows.
 */
class Window : public QMainWindow
{
	Q_OBJECT

public:
	Window(QWidget * = nullptr, Qt::WindowFlags = nullptr);
	virtual ~Window();

protected:
	void closeEvent(QCloseEvent *);

private:
	QomposeSettings *settings;

	PreferencesDialog *preferencesDialog;
	FindDialog *findDialog;
	ReplaceDialog *replaceDialog;
	GoToDialog *goToDialog;
	AboutDialog *aboutDialog;

	MainMenu *mainMenu;

	BufferWidget *buffers;

	QomposeStatusBar *statusBar;

	Window(const Window &);
	Window &operator=(const Window &);

	void initializeDialogs();
	void initializeMenus();
	void applyExistingSettings();

	void handleFindResult(Editor::FindResult);

private Q_SLOTS:
	void doTabPathChanged(const QString &);
	void doCursorPositionChanged(int, int);
	void doSearchWrapped();

	void doPreferencesDialog();
	void doPrint();
	void doPrintPreview();
	void doFindDialog();
	void doFindNext();
	void doFindPrevious();
	void doReplaceDialog();
	void doReplace();
	void doReplaceFind();
	void doReplaceSelection();
	void doReplaceAll();
	void doGoToAccepted();

	#ifdef QOMPOSE_DEBUG
		void doDebug();
	#endif

	void doSettingChanged(const QString &, const QVariant &);
};

}

#endif
