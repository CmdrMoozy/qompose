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

class QMenu;
class QAction;
class QStatusBar;
class QLabel;
class QCloseEvent;

class QomposeAboutDialog;
class QomposePreferencesDialog;
class QomposeBufferWidget;
class QomposeSettings;

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
		
		QomposeAboutDialog *aboutDialog;
		QomposePreferencesDialog *preferencesDialog;
		
		QMenu *fileMenu;
		QMenu *editMenu;
		QMenu *searchMenu;
		QMenu *buffersMenu;
		QMenu *helpMenu;
		
		QAction *newAction;
		QAction *openAction;
		QAction *revertAction;
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
		
		QomposeBufferWidget *buffers;
		
		QStatusBar *statusBar;
		QLabel *tabPathLabel;
		
		void initializeActions();
		void initializeMenus();
		void initializeDialogs();
		
		QIcon getIconFromTheme(const QString &n) const;
		
	private slots:
		void doTabPathChanged(const QString &p);
		
		void doPrint(bool c);
		void doPrintPreview(bool c);
		void doDuplicateLine(bool c);
		void doIncreaseIndent(bool c);
		void doDecreaseIndent(bool c);
		void doFind(bool c);
		void doFindNext(bool c);
		void doFindPrevious(bool c);
		void doReplace(bool c);
		void doGoTo(bool c);
};

#endif