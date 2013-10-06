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

#ifndef INCLUDE_QOMPOSE_EDITOR_H
#define INCLUDE_QOMPOSE_EDITOR_H

#include <QPlainTextEdit>
#include <QSize>

class QFocusEvent;
class QResizeEvent;
class QKeyEvent;
class QWheelEvent;
class QMouseEvent;

class QomposeFindQuery;
class QomposeReplaceQuery;

/*!
 * \brief This class implements a text editor widget.
 */
class QomposeEditor : public QPlainTextEdit
{
	Q_OBJECT
	
	public:
		/*!
		 * \brief This enum denotes the possible results of a find operation.
		 */
		enum FindResult
		{
			NoDocument,
			BadRegularExpression,
			NoMatches,
			Found
		};
		
		QomposeEditor(QWidget *p = NULL);
		virtual ~QomposeEditor();
		
		void setGutterVisible(bool v);
		bool isGutterVisible();
		
		virtual QFont font() const;
		virtual void setFont(const QFont &f);
		int fontZoom() const;
		qreal fontZoomSize() const;
		void setFontZoom(int z);
		
		int tabWidthSpaces() const;
		void setTabWidthSpaces(int w);
		
		QColor getEditorForeground() const;
		void setEditorForeground(const QColor &c);
		QColor getEditorBackground() const;
		void setEditorBackground(const QColor &c);
		QColor getCurrentLineColor() const;
		void setCurrentLineColor(const QColor &c);
		QColor getGutterForeground() const;
		void setGutterForeground(const QColor &c);
		QColor getGutterBackground() const;
		void setGutterBackground(const QColor &c);
		
	protected:
		virtual void focusInEvent(QFocusEvent *e);
		virtual void focusOutEvent(QFocusEvent *e);
		
		virtual void resizeEvent(QResizeEvent *e);
		virtual void keyPressEvent(QKeyEvent *e);
		virtual void wheelEvent(QWheelEvent *e);
		virtual void mouseReleaseEvent(QMouseEvent *e);
		
	private:
		class QomposeGutter : public QWidget
		{
			public:
				QomposeGutter(QomposeEditor *e);
				virtual ~QomposeGutter();
				
				QSize sizeHint() const;
				
			protected:
				virtual void paintEvent(QPaintEvent *e);
				
			private:
				QomposeEditor *editor;
		};
		
		QomposeGutter *gutter;
		bool gutterVisible;
		
		QFont currentFont;
		qreal originalFontSize;
		int currentFontZoom;
		
		int tabWidth;
		
		QColor currentLineHighlight;
		QColor gutterForeground;
		QColor gutterBackground;
		
		void gutterPaintEvent(QPaintEvent *e);
		int gutterWidth();
		
		void doNewline(bool preserveIndent = false);
		void doMoveHome(bool moveAnchor = true);
		
		FindResult doFind(bool forward, const QomposeFindQuery *q);
		FindResult doBatchReplace(const QomposeReplaceQuery *q,
			int start = -1, int end = -1);
	
	public Q_SLOTS:
		void duplicateLine();
		void deselect();
		void increaseSelectionIndent();
		void decreaseSelectionIndent();
		FindResult findNext(const QomposeFindQuery *q);
		FindResult findPrevious(const QomposeFindQuery *q);
		FindResult replace(const QomposeReplaceQuery *q);
		FindResult replaceSelection(const QomposeReplaceQuery *q);
		FindResult replaceAll(const QomposeReplaceQuery *q);
		void goToLine(int l);
		
	private Q_SLOTS:
		void highlightCurrentLine();
		void updateGutterWidth();
		void updateGutter(const QRect &r, int dy);
		
};

#endif