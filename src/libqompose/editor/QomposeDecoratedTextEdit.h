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

#ifndef INCLUDE_QOMPOSE_DECORATED_TEXT_EDIT
#define INCLUDE_QOMPOSE_DECORATED_TEXT_EDIT

#include <QPlainTextEdit>

class QomposeGutter;

/*!
 * \brief This class adds decorations to QPlainTextEdit.
 *
 * This class manages various decoration-related functionality of our
 * editor. This includes functionality like the gutter, highlighting,
 * font zooming, and etc.
 *
 * This class should be subclassed to implement editor functionality.
 */
class QomposeDecoratedTextEdit : public QPlainTextEdit
{
	friend class QomposeGutter;

	Q_OBJECT

	public:
		QomposeDecoratedTextEdit(QWidget *p = 0);
		virtual ~QomposeDecoratedTextEdit();

		void setGutterVisible(bool v);
		bool isGutterVisible();

		virtual QFont font() const;
		virtual void setFont(const QFont &f);
		int fontZoom() const;
		qreal fontZoomSize() const;
		void setFontZoom(int z);
		void resetFontZoom();

		int tabWidthSpaces() const;
		void setTabWidthSpaces(int w);

		bool isWrapGuideVisible() const;
		void setWrapGuideVisible(bool v);
		int getWrapGuideColumnWidth() const;
		void setWrapGuideColumnWidth(int w);
		QColor getWrapGuideColor() const;
		void setWrapGuideColor(const QColor &c);

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
		virtual void paintEvent(QPaintEvent *e);

		virtual void focusInEvent(QFocusEvent *e);
		virtual void focusOutEvent(QFocusEvent *e);
		virtual void resizeEvent(QResizeEvent *e);
		virtual void wheelEvent(QWheelEvent *e);
		virtual void mouseReleaseEvent(QMouseEvent *e);

	private:
		QomposeGutter *gutter;
		bool gutterVisible;

		QFont currentFont;
		qreal originalFontSize;
		int currentFontZoom;

		int tabWidth;

		bool wrapGuideVisible;
		int wrapGuideWidth;
		QColor wrapGuideColor;

		QColor currentLineHighlight;
		QColor gutterForeground;
		QColor gutterBackground;

		void gutterPaintEvent(QPaintEvent *e);
		int gutterWidth();
		qreal wrapGuideOffset();

	public Q_SLOTS:
		void fullUpdate();
		void fullRepaint();

	private Q_SLOTS:
		void highlightCurrentLine();
		void updateGutterWidth();
		void updateGutter(const QRect &r, int dy);
};

#endif
