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

#ifndef INCLUDE_QOMPOSE_BUFFER_WIDGET_H
#define INCLUDE_QOMPOSE_BUFFER_WIDGET_H

#include <QWidget>
#include <QSet>

class QGridLayout;

class QomposeBuffer;
class QomposeTabWidget;

class QomposeBufferWidget : public QWidget
{
	Q_OBJECT
	
	public:
		QomposeBufferWidget(QWidget *p = 0);
		virtual ~QomposeBufferWidget();
		
		int count() const;
		QomposeBuffer *bufferAt(int i) const;
		QomposeBuffer *currentBuffer() const;
		void setCurrentBuffer(int i);
		
		bool prepareCloseParent();
		
	private:
		QGridLayout *layout;
		QomposeTabWidget *tabWidget;
		
		QSet<QomposeBuffer *> tabs;
		
		QomposeBuffer *newBuffer();
		void removeCurrentBuffer();
		void moveBuffer(int f, int t);
		
	public slots:
		void doNew();
		void doOpen();
		void doRevert();
		void doSave();
		void doSaveAs();
		void doClose();
		void doPreviousBuffer();
		void doNextBuffer();
		void doMoveBufferLeft();
		void doMoveBufferRight();
		
	private slots:
		void doTabChanged(int i);
		void doTabCloseRequested(int i);
		
		void doTabTitleChanged(const QString &t);
		void doTabPathChanged(const QString &p);
		
	signals:
		void pathChanged(const QString &);
};

#endif
