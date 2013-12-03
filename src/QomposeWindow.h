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

#ifndef INCLUDE_QOMPOSE_WINDOW_H
#define INCLUDE_QOMPOSE_WINDOW_H

#include <QMainWindow>
#include <QIcon>

#include "editor/QomposeEditor.h"

class QCloseEvent;
class QPrinter;

class QomposeAboutDialog;
class QomposePreferencesDialog;
class QomposeBufferWidget;
class QomposeSettings;
class QomposeFindDialog;
class QomposeReplaceDialog;
class QomposeGoToDialog;
class QomposeMainMenu;
class QomposeStatusBar;

/*!
 * \brief This class implements one of our application's standard windows.
 */
class QomposeWindow : public QMainWindow
{
	Q_OBJECT
	
	public:
		QomposeWindow(QWidget *p = NULL, Qt::WindowFlags f = 0);
		virtual ~QomposeWindow();
		
	protected:
		void closeEvent(QCloseEvent *e);
		
	private:
		QomposeSettings *settings;
		
		QomposePreferencesDialog *preferencesDialog;
		QomposeFindDialog *findDialog;
		QomposeReplaceDialog *replaceDialog;
		QomposeGoToDialog *goToDialog;
		QomposeAboutDialog *aboutDialog;
		
		QomposeMainMenu *mainMenu;
		
		QomposeBufferWidget *buffers;
		
		QomposeStatusBar *statusBar;
		
		void initializeDialogs();
		void initializeMenus();
		void applyExistingSettings();
		
		void handleFindResult(QomposeEditor::FindResult r);
		
	private Q_SLOTS:
		void doTabPathChanged(const QString &p);
		
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
		
		void doSettingChanged(const QString &k, const QVariant &v);
};

#endif
