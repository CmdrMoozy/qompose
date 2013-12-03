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

#ifndef INCLUDE_QOMPOSE_MAIN_MENU_H
#define INCLUDE_QOMPOSE_MAIN_MENU_H

#include <QMenuBar>

class QMenu;
class QAction;

class QomposeRecentMenu;
class QomposeSettings;
class QomposeBufferWidget;

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
class QomposeMainMenu : public QMenuBar
{
	Q_OBJECT
	
	public:
		QomposeMainMenu(QomposeSettings *s, QWidget *p = 0);
		virtual ~QomposeMainMenu();
		
		void connectBufferWidget(const QomposeBufferWidget *b);
		
	private:
		QomposeSettings *settings;
		
		QMenu *fileMenu;
		QMenu *editMenu;
		QMenu *searchMenu;
		QMenu *buffersMenu;
		QMenu *helpMenu;
		
		QAction *newAction;
		QAction *openAction;
		QomposeRecentMenu *recentMenu;
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
		
		#ifdef QOMPOSE_DEBUG
			QAction *debugAction;
		#endif
		
	private Q_SLOTS:
		void doFileOpened(const QString &p);
		
	Q_SIGNALS:
		void printTriggered(bool);
		void printPreviewTriggered(bool);
		void exitTriggered(bool);
		void preferencesTriggered(bool);
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

#endif
