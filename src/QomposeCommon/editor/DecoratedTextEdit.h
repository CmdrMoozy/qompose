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

#include "QomposeCommon/Types.h"

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
	/*!
	 * This is our default constructor, which creates a new decorated text
	 * exit.
	 *
	 * \param p Our parent widget.
	 */
	DecoratedTextEdit(QWidget *p = nullptr);

	DecoratedTextEdit(const DecoratedTextEdit &) = delete;
	virtual ~DecoratedTextEdit() = default;

	DecoratedTextEdit &operator=(const DecoratedTextEdit &) = delete;

	/*!
	 * This function sets whether or not our gutter is visible.
	 *
	 * \param v Whether or not our gutter should be visible.
	 */
	void setGutterVisible(bool v);

	/*!
	 * This function returns whether or not our gutter is currently
	 * visible.
	 *
	 * \return Whether or not our gutter is visible.
	 */
	bool isGutterVisible();

	/*!
	 * This function returns our current font. Note that the size in the
	 * returned font object does NOT include any changes due to our current
	 * zoom factor. Our zoomed size can be determined from fontZoomSize().
	 *
	 * \return Our current font.
	 */
	virtual QFont font() const;

	/*!
	 * This function sets our widget's font to the given value. Note that
	 * fonts which are assigned a PIXEL SIZE are not allowed - these will
	 * be resized to 10 points.
	 *
	 * \param f The new font to use.
	 */
	virtual void setFont(const QFont &f);

	/*!
	 * This function returns our widget's current font zoom factor. The
	 * value returned is relative to zero; e.g., a value of 100 means
	 * doubling the original font size, whereas a value of -100 means the
	 * original size reduced to 0.
	 *
	 * The minimum value this can return is -100, and there is no maximum
	 * value.
	 *
	 * \return Our current font zooming factor.
	 */
	int fontZoom() const;

	/*!
	 * This function returns the current font size, including any zooming
	 * currently in place.
	 *
	 * \return Our current font size, including zooming.
	 */
	qreal fontZoomSize() const;

	/*!
	 * This function sets our widget's font zoom. The value given is
	 * relative to the current size; e.g., passing 100 means doubling the
	 * size, whereas passing -100 means a size of 0.
	 *
	 * The minimum value that can be set is -100; this results in a size of
	 * 0. There is no maximum value, however.
	 *
	 * \param z The new zoom factor for our font size.
	 */
	void setFontZoom(int z);

	/*!
	 * This is a simple convenience function, which resets our editor's
	 * text zoom to the default level (i.e., normally sized text).
	 *
	 * This is equivalent to calling setFontZoom(0).
	 */
	void resetFontZoom();

	/*!
	 * This function returns the width of a tab stop, in spaces (based upon
	 * the current font) - NOT in pixels.
	 *
	 * \return The editor's tab width.
	 */
	int getIndentationWidth() const;

	/*!
	 * This function sets the width of our editor's tab stops, in spaces
	 * (based upon the current font) - NOT in pixels.
	 *
	 * \param w The new tab width to use.
	 */
	void setIndentationWidth(int w);

	/*!
	 * This function returns the indentation mode this editor is currently
	 * using.
	 *
	 * \return Our current indentation mode.
	 */
	IndentationMode getIndentationMode() const;

	/*!
	 * This function sets the indentation mode this editor should use,
	 * given a string representatino of an IndentationMode value.
	 *
	 * \param mode The new indentation mode.
	 */
	void setIndentationMode(const QString &mode);

	/*!
	 * This function sets the indentation mode this editor should use.
	 *
	 * \param mode The new indentation mode.
	 */
	void setIndentationMode(IndentationMode mode);

	/*!
	 * This function returns whether or not our line wrap guide is
	 * currently visible.
	 *
	 * \return True if the line wrap guide is visible, or false otherwise.
	 */
	bool isWrapGuideVisible() const;

	/*!
	 * This function sets whether or not our line wrap guide should be
	 * visible. Note that this function will result in a call to repaint(),
	 * so this new visibility value will take effect.
	 *
	 * \param v Whether or not the line wrap guide should now be visible.
	 */
	void setWrapGuideVisible(bool v);

	/*!
	 * This function returns the current width of our line wrap guide, in
	 * character columns (that is, the guide will be rendered this many
	 * characters away from the left edge of the document).
	 *
	 * Note that this value is only very approximate for non-monospace
	 * fonts.
	 *
	 * \return The current width of our line wrap guide, in char columns.
	 */
	int getWrapGuideColumnWidth() const;

	/*!
	 * This function sets the width of our line wrap guide, in character
	 * columns (that is, the guide will be rendered this many characters
	 * away from the left edge of the document).
	 *
	 * Note that this value is only very approximate for non-monospace
	 * fonts, and that this function will result in a call to repaint(),
	 * so this new width value will take effect.
	 *
	 * \param w The new width for the line wrap guide.
	 */
	void setWrapGuideColumnWidth(int w);

	/*!
	 * This function returns the current color of the line wrap guide.
	 *
	 * \return The color of the line wrap guide.
	 */
	QColor getWrapGuideColor() const;

	/*!
	 * This function sets the color the line wrap guide should be rendered
	 * in. Note that this function will result in a call to repaint(), so
	 * the new color value will take effect.
	 *
	 * \param c The new color for the line wrap guide.
	 */
	void setWrapGuideColor(const QColor &c);

	/*!
	 * This function returns whether or not line wrapping is enabled for
	 * this decorated text edit.
	 *
	 * \return Whether or not line wrapping is enabled.
	 */
	bool getLineWrapping() const;

	/*!
	 * This function sets whether or not line wrapping is enabled for this
	 * decorated text edit.
	 *
	 * \param enabled Whether or not line wrapping should be enabled.
	 */
	void setLineWrapping(bool enabled);

	/*!
	 * This function returns our editor's foreground (text) color.
	 *
	 * \return Our editor's foreground color.
	 */
	QColor getEditorForeground() const;

	/*!
	 * This function sets our editor's foreground (text) color.
	 *
	 * \param c The new foreground color to use.
	 */
	void setEditorForeground(const QColor &c);

	/*!
	 * This function returns our editor's background color.
	 *
	 * \return Our editor's background color.
	 */
	QColor getEditorBackground() const;

	/*!
	 * This function sets our editor's background color.
	 *
	 * \param c The new background color to use.
	 */
	void setEditorBackground(const QColor &c);

	/*!
	 * This function returns our editor's current line highlight color.
	 *
	 * \return Our editor's current line color.
	 */
	QColor getCurrentLineColor() const;

	/*!
	 * This function sets our editor's current line highlight color.
	 *
	 * \param c The new current line color to use.
	 */
	void setCurrentLineColor(const QColor &c);

	/*!
	 * This function returns our editor's gutter's foreground (text) color.
	 *
	 * \return Our gutter's foreground color.
	 */
	QColor getGutterForeground() const;

	/*!
	 * This function sets our editor's gutter's foreground (text) color.
	 * This automatically repaints our gutter, so this change takes effect
	 * immediately.
	 *
	 * \param c The new gutter foreground color to use.
	 */
	void setGutterForeground(const QColor &c);

	/*!
	 * This function returns our editor's gutter's background color.
	 *
	 * \return Our gutter's background color.
	 */
	QColor getGutterBackground() const;

	/*!
	 * This function sets our editor's gutter's background color. This
	 * automatically repaints our gutter, so this change takes effect
	 * immediately.
	 *
	 * \param c The new gutter background color to use.
	 */
	void setGutterBackground(const QColor &c);

	/*!
	 * This function returns the current cursor's 1-indexed line number.
	 *
	 * \return The current cursor's line number.
	 */
	int getCurrentLine() const;

	/*!
	 * This function returns the current cursor's 1-indexed column number.
	 * This is based upon the pixel offset of the cursor, so we properly
	 * account for tab stops.
	 *
	 * \return The current cursor's column number.
	 */
	int getCurrentColumn() const;

protected:
	/*!
	 * We override our superclass's paint event to draw some additional
	 * decorations on our text edit's viewport.
	 *
	 * \param e The paint event being handled.
	 */
	virtual void paintEvent(QPaintEvent *e);

	/*!
	 * We override our parent's focus-in event to update our UI when that
	 * event occurs.
	 *
	 * \param e The event being handled.
	 */
	virtual void focusInEvent(QFocusEvent *e);

	/*!
	 * We override our parent's focus-out event to update our UI when that
	 * event occurs.
	 *
	 * \param e The event being handled.
	 */
	virtual void focusOutEvent(QFocusEvent *e);

	/*!
	 * This function handles the resetting of our gutter's geometry when we
	 * get resized.
	 *
	 * \param e The resize event we are handling.
	 */
	virtual void resizeEvent(QResizeEvent *e);

	/*!
	 * We handle mouse wheel events to implement text zooming. We implement
	 * two wheel actions:
	 *
	 *     Ctrl + Wheel Forward - Increase font zoom.
	 *     Ctrl + Wheel Backward - Decrease font zoom.
	 *
	 * All other wheel events are handled normally by our superclass.
	 *
	 * \param e The event being handled.
	 */
	virtual void wheelEvent(QWheelEvent *e);

	/*!
	 * This function handles any "mouse release" events on our editor by
	 * re-highlighting the current line. Ths is necessary to avoid a bug
	 * where rapidly clicking/arrow-key-ing can result in the wrong line
	 * being highlighted; cursorPositionChanged is somewhat unreliable.
	 *
	 * \param e The event being handled.
	 */
	virtual void mouseReleaseEvent(QMouseEvent *e);

	/*!
	 * This function returns a string which contains a single indentation
	 * for this editor. This depends on the indentation mode and width.
	 *
	 * \return A single indentation string for this editor.
	 */
	QString getIndentString() const;

	/*!
	 * This function returns the margin around the editor's content area.
	 * This value is added to a bare column width in order to get a
	 * columnOffset().
	 *
	 * \return This editor's content margin.
	 */
	qreal contentMargin() const;

	/*!
	 * This function returns the width of a single column.
	 *
	 * \return The width of one character column.
	 */
	qreal singleColumnWidth() const;

	/*!
	 * This function computes and returns the offset from the left-hand
	 * side of our text editor widget to the end of the given column.
	 *
	 * \param column The column to get an offset for.
	 */
	qreal columnOffset(int column) const;

	/*!
	 * This function computes and returns the offset from the left-hand
	 * side of our text editor widget at which our line wrap guide will be
	 * painted. This is a shorthand for passing the line wrap guide's
	 * column width to columnOffset().
	 *
	 * \return The offset of the line wrap guide.
	 */
	qreal wrapGuideOffset() const;

private:
	Gutter *gutter;
	bool gutterVisible;

	QFont currentFont;
	qreal originalFontSize;
	int currentFontZoom;

	int indentationWidth;
	IndentationMode indentationMode;

	bool wrapGuideVisible;
	int wrapGuideWidth;
	QColor wrapGuideColor;

	QColor currentLineHighlight;
	QColor gutterForeground;
	QColor gutterBackground;

	/*!
	 * This function handles a paint event passed up to us by our gutter by
	 * rendering the gutter according to our editor's current state.
	 *
	 * \param e The paint event being handled.
	 */
	void gutterPaintEvent(QPaintEvent *e);

	/*!
	 * This function computes the width our gutter should have, based upon
	 * the number of lines in our document. We ensure that we have enough
	 * space to render our line numbers inside the gutter.
	 *
	 * \return The width of our gutter.
	 */
	int gutterWidth();

public Q_SLOTS:
	/*!
	 * This is a utility function which calls update() on both this widget,
	 * as well as our viewport widget, to make sure we are really updated.
	 */
	void fullUpdate();

	/*!
	 * This is a utility function which calls repaint() on both this
	 * widget, as well as our viewport widget, to make sure our
	 * paintEvent() function is called.
	 */
	void fullRepaint();

private Q_SLOTS:
	/*!
	 * This function handles highlighting the current line in our editor.
	 */
	void highlightCurrentLine();

	/*!
	 * This function updates our gutter's width, e.g. when the number of
	 * lines in our document changes.
	 */
	void updateGutterWidth();

	/*!
	 * This function updates our gutter, e.g. when our editor widget's
	 * state is updated, by resetting the gutter's scroll and width.
	 *
	 * \param r The viewport rect being updated.
	 * \param dy The editor's current y scroll offset.
	 */
	void updateGutter(const QRect &r, int dy);
};
}

#endif
