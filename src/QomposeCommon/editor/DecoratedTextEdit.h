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

#ifndef INCLUDE_QOMPOSECOMMON_EDITOR_DECORATED_TEXT_EDIT
#define INCLUDE_QOMPOSECOMMON_EDITOR_DECORATED_TEXT_EDIT

#include <QPlainTextEdit>

namespace qompose
{

class Gutter;

/*!
 * \brief This class adds decorations to QPlainTextEdit.
 *
 * This class manages various decoration-related functionality of our
 * editor. This includes functionality like the gutter, highlighting,
 * font zooming, and etc.
 *
 * This class should be subclassed to implement editor functionality.
 */
class DecoratedTextEdit : public QPlainTextEdit
{
	friend class Gutter;

	Q_OBJECT

public:
	DecoratedTextEdit(QWidget * = nullptr);
	virtual ~DecoratedTextEdit();

	void setGutterVisible(bool);
	bool isGutterVisible();

	virtual QFont font() const;
	virtual void setFont(const QFont &);
	int fontZoom() const;
	qreal fontZoomSize() const;
	void setFontZoom(int);
	void resetFontZoom();

	int tabWidthSpaces() const;
	void setTabWidthSpaces(int);

	bool isWrapGuideVisible() const;
	void setWrapGuideVisible(bool);
	int getWrapGuideColumnWidth() const;
	void setWrapGuideColumnWidth(int);
	QColor getWrapGuideColor() const;
	void setWrapGuideColor(const QColor &);

	QColor getEditorForeground() const;
	void setEditorForeground(const QColor &);
	QColor getEditorBackground() const;
	void setEditorBackground(const QColor &);
	QColor getCurrentLineColor() const;
	void setCurrentLineColor(const QColor &);
	QColor getGutterForeground() const;
	void setGutterForeground(const QColor &);
	QColor getGutterBackground() const;
	void setGutterBackground(const QColor &);

	int getCurrentLine() const;
	int getCurrentColumn() const;

protected:
	virtual void paintEvent(QPaintEvent *);

	virtual void focusInEvent(QFocusEvent *);
	virtual void focusOutEvent(QFocusEvent *);
	virtual void resizeEvent(QResizeEvent *);
	virtual void wheelEvent(QWheelEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);

private:
	Gutter *gutter;
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

	void gutterPaintEvent(QPaintEvent *);
	int gutterWidth();
	qreal wrapGuideOffset();

	DecoratedTextEdit(const DecoratedTextEdit &);
	DecoratedTextEdit &operator=(const DecoratedTextEdit &);

public Q_SLOTS:
	void fullUpdate();
	void fullRepaint();

private Q_SLOTS:
	void highlightCurrentLine();
	void updateGutterWidth();
	void updateGutter(const QRect &, int);
};

}

#endif
