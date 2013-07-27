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

#ifndef INCLUDE_QOMPOSE_TAB_WIDGET_H
#define INCLUDE_QOMPOSE_TAB_WIDGET_H

#include <QWidget>
#include <QMap>

class QGridLayout;
class QTabBar;
class QStackedLayout;

class QomposeBuffer;

class QomposeTabWidget : public QWidget
{
	Q_OBJECT
	
	public:
		QomposeTabWidget(QWidget *p = 0);
		virtual ~QomposeTabWidget();
		
		QomposeBuffer *currentBuffer() const;
		
	private:
		QGridLayout *layout;
		QTabBar *tabBar;
		
		QWidget *tabDisplayWidget;
		QStackedLayout *tabDisplayLayout;
		
		QMap<int, QomposeBuffer *> tabs;
		
		QomposeBuffer *newBuffer();
		void removeCurrentBuffer();
		
	public slots:
		void doNew();
		void doOpen();
		void doRevert();
		void doSave();
		void doSaveAs();
		void doClose();
		
	private slots:
		void doTabChanged(int i);
		void doTabCloseRequested(int i);
		
		void doTabTitleChanged(const QString &t);
		void doTabPathChanged(const QString &p);
		
	signals:
		void pathChanged(const QString &);
};

#endif
