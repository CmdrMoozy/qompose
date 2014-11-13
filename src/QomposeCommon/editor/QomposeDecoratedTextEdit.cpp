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

#include "QomposeDecoratedTextEdit.h"

#include <QPainter>
#include <QTextBlock>

#include "Config.h"
#include "editor/QomposeGutter.h"
#include "util/QomposeFontMetrics.h"

/*!
 * This is our default constructor, which creates a new decorated text exit.
 *
 * \param p Our parent widget.
 */
QomposeDecoratedTextEdit::QomposeDecoratedTextEdit(QWidget *p)
	: QPlainTextEdit(p), gutter(nullptr), gutterVisible(false),
		currentFont(QFont("Courier")), originalFontSize(11.0),
		currentFontZoom(1), tabWidth(8), wrapGuideVisible(false),
		wrapGuideWidth(0), wrapGuideColor(QColor(255, 255, 255)),
		currentLineHighlight(QColor(128, 128, 128)),
		gutterForeground(QColor(255, 255, 255)),
		gutterBackground(QColor(0, 0, 0))
{
	// Set our editor's default font.

	setFont(QFont("Courier", 11));
	setFontZoom(0);

	// Initialize our gutter.

	gutter = new QomposeGutter(this);

	setGutterForeground(QColor(255, 255, 255));
	setGutterBackground(QColor(0, 0, 0));

	QObject::connect(this, SIGNAL(blockCountChanged(int)),
		this, SLOT(updateGutterWidth()));
	QObject::connect(this, SIGNAL(updateRequest(const QRect &, int)),
		this, SLOT(updateGutter(const QRect &, int)));
	QObject::connect(this, SIGNAL(cursorPositionChanged()),
		this, SLOT(highlightCurrentLine()));

	// Set some of our widget's default properties.

	setCurrentLineColor(QColor(70, 72, 61));

	setWrapGuideVisible(true);
	setWrapGuideColumnWidth(100);
	setWrapGuideColor(QColor(127, 127, 127));

	setLineWrapMode(QPlainTextEdit::NoWrap);
	setFocusPolicy(Qt::ClickFocus);

	setGutterVisible(true);

	highlightCurrentLine();
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
QomposeDecoratedTextEdit::~QomposeDecoratedTextEdit()
{
}

/*!
 * This function sets whether or not our gutter is visible.
 *
 * \param v Whether or not our gutter should be visible.
 */
void QomposeDecoratedTextEdit::setGutterVisible(bool v)
{
	gutterVisible = v;
	gutter->setVisible(v);
	updateGutterWidth();
}

/*!
 * This function returns whether or not our gutter is currently visible.
 *
 * \return Whether or not our gutter is visible.
 */
bool QomposeDecoratedTextEdit::isGutterVisible()
{
	return gutterVisible;
}

/*!
 * This function returns our current font. Note that the size in the returned
 * font object does NOT include any changes due to our current zoom factor. Our
 * zoomed size can be determined from fontZoomSize().
 *
 * \return Our current font.
 */
QFont QomposeDecoratedTextEdit::font() const
{
	// Return a font identical to ours, except with a size excluding scaling.

	QFont f = currentFont;
	f.setPointSizeF(originalFontSize);

	return f;
}

/*!
 * This function sets our widget's font to the given value. Note that fonts which
 * are assigned a PIXEL SIZE are not allowed - these will be resized to 10
 * points.
 *
 * \param f The new font to use.
 */
void QomposeDecoratedTextEdit::setFont(const QFont &f)
{
	// Make sure we aren't given a pixel-sized font.

	currentFont = f;

	if(currentFont.pointSize() == -1)
		currentFont.setPointSize(10);

	originalFontSize = currentFont.pointSizeF();
	originalFontSize = qMax(originalFontSize, 1.0);

	// Set our font!

	QPlainTextEdit::setFont(currentFont);

	// Make sure our tab width is still the same (in spaces).

	setTabWidthSpaces(tabWidth);

	// Update our line wrap guide, since it depends on our font.

	fullUpdate();
}

/*!
 * This function returns our widget's current font zoom factor. The value
 * returned is relative to zero; e.g., a value of 100 means doubling the original
 * font size, whereas a value of -100 means the original size reduced to 0.
 *
 * The minimum value this can return is -100, and there is no maximum value.
 *
 * \return Our current font zooming factor.
 */
int QomposeDecoratedTextEdit::fontZoom() const
{
	return currentFontZoom;
}

/*!
 * This function returns the current font size, including any zooming currently
 * in place.
 *
 * \return Our current font size, including zooming.
 */
qreal QomposeDecoratedTextEdit::fontZoomSize() const
{
	qreal scale = static_cast<qreal>(fontZoom()) / 100.0;
	qreal fsize = originalFontSize + (scale * originalFontSize);

	return qMax(fsize, 0.0);
}

/*!
 * This function sets our widget's font zoom. The value given is relative to the
 * current size; e.g., passing 100 means doubling the size, whereas passing -100
 * means a size of 0.
 *
 * The minimum value that can be set is -100; this results in a size of 0. There
 * is no maximum value, however.
 *
 * \param z The new zoom factor for our font size.
 */
void QomposeDecoratedTextEdit::setFontZoom(int z)
{
	// Update the current font zoom factor.

	z = qMax(-100, z);
	currentFontZoom = z;

	// Update our editor's actual font size.

	qreal sizef = fontZoomSize();

	if(sizef > 0.0)
		currentFont.setPointSizeF(sizef);

	QPlainTextEdit::setFont(currentFont);

	// Reset our tab width.

	setTabWidthSpaces(tabWidth);
}

/*!
 * This is a simple convenience function, which resets our editor's text zoom
 * to the default level (i.e., normally sized text).
 *
 * This is equivalent to calling setFontZoom(0).
 */
void QomposeDecoratedTextEdit::resetFontZoom()
{
	setFontZoom(0);
}

/*!
 * This function returns the width of a tab stop, in spaces (based upon the
 * current font) - NOT in pixels.
 *
 * \return The editor's tab width.
 */
int QomposeDecoratedTextEdit::tabWidthSpaces() const
{
	return tabWidth;
}

/*!
 * This function sets the width of our editor's tab stops, in spaces (based upon
 * the current font) - NOT in pixels.
 *
 * \param w The new tab width to use.
 */
void QomposeDecoratedTextEdit::setTabWidthSpaces(int w)
{
	tabWidth = qAbs(w);

	QomposeFontMetrics metrics(currentFont);
	qreal tabw = metrics.getColumnWidthF(tabWidth);

#ifdef HAVE_QREAL_TAB_STOPS
	setTabStopWidthF(tabw);
#else
	setTabStopWidth(qRound(tabw));
#endif
}

/*!
 * This function returns whether or not our line wrap guide is currently
 * visible.
 *
 * \return True if the line wrap guide is visible, or false otherwise.
 */
bool QomposeDecoratedTextEdit::isWrapGuideVisible() const
{
	return wrapGuideVisible;
}

/*!
 * This function sets whether or not our line wrap guide should be visible.
 * Note that this function will result in a call to repaint(), so this new
 * visibility value will take effect.
 *
 * \param v Whether or not the line wrap guide should now be visible.
 */
void QomposeDecoratedTextEdit::setWrapGuideVisible(bool v)
{
	wrapGuideVisible = v;

	fullUpdate();
}

/*!
 * This function returns the current width of our line wrap guide, in
 * character columns (that is, the guide will be rendered this many
 * characters away from the left edge of the document).
 *
 * Note that this value is only very approximate for non-monospace
 * fonts.
 *
 * \return The current width of our line wrap guide, in character columns.
 */
int QomposeDecoratedTextEdit::getWrapGuideColumnWidth() const
{
	return wrapGuideWidth;
}

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
void QomposeDecoratedTextEdit::setWrapGuideColumnWidth(int w)
{
	wrapGuideWidth = qAbs(w);

	fullUpdate();
}

/*!
 * This function returns the current color of the line wrap guide.
 *
 * \return The color of the line wrap guide.
 */
QColor QomposeDecoratedTextEdit::getWrapGuideColor() const
{
	return wrapGuideColor;
}

/*!
 * This function sets the color the line wrap guide should be rendered
 * in. Note that this function will result in a call to repaint(), so
 * the new color value will take effect.
 *
 * \param c The new color for the line wrap guide.
 */
void QomposeDecoratedTextEdit::setWrapGuideColor(const QColor &c)
{
	wrapGuideColor = c;

	fullUpdate();
}

/*!
 * This function returns our editor's foreground (text) color.
 *
 * \return Our editor's foreground color.
 */
QColor QomposeDecoratedTextEdit::getEditorForeground() const
{
	return palette().color(QPalette::Text);
}

/*!
 * This function sets our editor's foreground (text) color.
 *
 * \param c The new foreground color to use.
 */
void QomposeDecoratedTextEdit::setEditorForeground(const QColor &c)
{
	QPalette p = palette();

	p.setColor(QPalette::Active, QPalette::Window, c);
	p.setColor(QPalette::Active, QPalette::WindowText, c);
	p.setColor(QPalette::Active, QPalette::Text, c);

	p.setColor(QPalette::Inactive, QPalette::Window, c);
	p.setColor(QPalette::Inactive, QPalette::WindowText, c);
	p.setColor(QPalette::Inactive, QPalette::Text, c);

	setPalette(p);
}

/*!
 * This function returns our editor's background color.
 *
 * \return Our editor's background color.
 */
QColor QomposeDecoratedTextEdit::getEditorBackground() const
{
	return palette().color(QPalette::Active, QPalette::Base);
}

/*!
 * This function sets our editor's background color.
 *
 * \param c The new background color to use.
 */
void QomposeDecoratedTextEdit::setEditorBackground(const QColor &c)
{
	QPalette p = palette();

	p.setColor(QPalette::Active, QPalette::Base, c);
	p.setColor(QPalette::Inactive, QPalette::Base, c);

	setPalette(p);
}

/*!
 * This function returns our editor's current line highlight color.
 *
 * \return Our editor's current line color.
 */
QColor QomposeDecoratedTextEdit::getCurrentLineColor() const
{
	return currentLineHighlight;
}

/*!
 * This function sets our editor's current line highlight color.
 *
 * \param c The new current line color to use.
 */
void QomposeDecoratedTextEdit::setCurrentLineColor(const QColor &c)
{
	currentLineHighlight = c;

	highlightCurrentLine();
}

/*!
 * This function returns our editor's gutter's foreground (text) color.
 *
 * \return Our gutter's foreground color.
 */
QColor QomposeDecoratedTextEdit::getGutterForeground() const
{
	return gutterForeground;
}

/*!
 * This function sets our editor's gutter's foreground (text) color. This
 * automatically repaints our gutter, so this change takes effect immediately.
 *
 * \param c The new gutter foreground color to use.
 */
void QomposeDecoratedTextEdit::setGutterForeground(const QColor &c)
{
	gutterForeground = c;

	gutter->update();
}

/*!
 * This function returns our editor's gutter's background color.
 *
 * \return Our gutter's background color.
 */
QColor QomposeDecoratedTextEdit::getGutterBackground() const
{
	return gutterBackground;
}

/*!
 * This function sets our editor's gutter's background color. This automatically
 * repaints our gutter, so this change takes effect immediately.
 *
 * \param c The new gutter background color to use.
 */
void QomposeDecoratedTextEdit::setGutterBackground(const QColor &c)
{
	gutterBackground = c;

	gutter->update();
}

/*!
 * This function returns the current cursor's 1-indexed line number.
 *
 * \return The current cursor's line number.
 */
int QomposeDecoratedTextEdit::getCurrentLine() const
{
	return textCursor().blockNumber() + 1;
}

/*!
 * This function returns the current cursor's 1-indexed column number. This is
 * based upon the pixel offset of the cursor, so we properly account for tab
 * stops.
 *
 * \return The current cursor's column number.
 */
int QomposeDecoratedTextEdit::getCurrentColumn() const
{
	QomposeFontMetrics metrics(currentFont);

	qreal xoff = static_cast<qreal>(cursorRect().x());

	xoff -= x();
	xoff -= contentOffset().x();
	xoff -= document()->documentMargin();

	xoff /= metrics.getColumnWidthF(1);

	return qRound(xoff) + 1;
}

/*!
 * We override our superclass's paint event to draw some additional decorations
 * on our text edit's viewport.
 *
 * \param e The paint event being handled.
 */
void QomposeDecoratedTextEdit::paintEvent(QPaintEvent *e)
{
	// Let our superclass do its normal painting, and prepare to do our own.

	QPlainTextEdit::paintEvent(e);

	QRect eventRect = e->rect();
	QPainter painter(viewport());
	QomposeFontMetrics metrics(currentFont);

	// Draw our offset and margin lines, if debugging is enabled.

#ifdef QOMPOSE_DEBUG
	painter.setPen(QPen(QColor(255, 0, 0)));

	painter.drawLine(contentOffset().x(), eventRect.top(),
		contentOffset().x(), eventRect.bottom());

	painter.setPen(QPen(QColor(0, 255, 0)));

	painter.drawLine(document()->documentMargin(), eventRect.top(),
		document()->documentMargin(), eventRect.bottom());
#endif

	// Draw lines at each character column, if debugging is enabled.

#ifdef QOMPOSE_DEBUG
	painter.setPen(QPen(QColor(0, 0, 0)));

	for(int col = 1; col <= 100; ++col)
	{
		qreal coff = metrics.getColumnWidthF() * col;

		coff += contentOffset().x();
		coff += document()->documentMargin();

		painter.drawLine(coff, eventRect.top(),
			coff, eventRect.bottom());
	}
#endif

	// Draw our line wrap guide, if it is enabled.

	if(isWrapGuideVisible() && metrics.isMonospaced())
	{
		qreal offset = wrapGuideOffset();

		painter.setPen(QPen(getWrapGuideColor()));
		painter.drawLine(offset, eventRect.top(),
			offset, eventRect.bottom());
	}
}

/*!
 * We override our parent's focus-in event to update our UI when that event
 * occurs.
 *
 * \param e The event being handled.
 */
void QomposeDecoratedTextEdit::focusInEvent(QFocusEvent *e)
{
	highlightCurrentLine();
	QPlainTextEdit::focusInEvent(e);
}

/*!
 * We override our parent's focus-out event to update our UI when that event
 * occurs.
 *
 * \param e The event being handled.
 */
void QomposeDecoratedTextEdit::focusOutEvent(QFocusEvent *e)
{
	highlightCurrentLine();
	QPlainTextEdit::focusOutEvent(e);
}

/*!
 * This function handles the resetting of our gutter's geometry when we get
 * resized.
 *
 * \param event The resize event we are handling.
 */
void QomposeDecoratedTextEdit::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	gutter->setGeometry(QRect(cr.left(), cr.top(),
		gutterWidth(), cr.height()));
}

/*!
 * We handle mouse wheel events to implement text zooming. We implement two
 * wheel actions:
 *
 *     Ctrl + Wheel Forward - Increase font zoom.
 *     Ctrl + Wheel Backward - Decrease font zoom.
 *
 * All other wheel events are handled normally by our superclass.
 *
 * \param e The event being handled.
 */
void QomposeDecoratedTextEdit::wheelEvent(QWheelEvent *e)
{
	if(e->modifiers() == Qt::ControlModifier)
	{
		/*
		 * If the user does Ctrl+Wheel, zoom in/out on our text. Delta
		 * is degrees turned * 8. We scale by 100% for each 120 degrees
		 * turned.
		 */

		qreal scale = static_cast<qreal>(e->delta()) / 8.0;
		scale *= 0.8333;

		setFontZoom(currentFontZoom + qRound(scale));
	}
	else
	{
		// Otherwise, just perform our parent class's default action.

		QPlainTextEdit::wheelEvent(e);
	}

}

/*!
 * This function handles any "mouse release" events on our editor by
 * re-highlighting the current line. Ths is necessary to avoid a bug where
 * rapidly clicking/arrow-key-ing can result in the wrong line being highlighted;
 * cursorPositionChanged is somewhat unreliable.
 *
 * \param e The event being handled.
 */
void QomposeDecoratedTextEdit::mouseReleaseEvent(QMouseEvent *e)
{
	highlightCurrentLine();

	// Do our superclass's normal mouse action.

	QPlainTextEdit::mouseReleaseEvent(e);
}

/*!
 * This function handles a paint event passed up to us by our gutter by rendering
 * the gutter according to our editor's current state.
 *
 * \param e The paint event being handled.
 */
void QomposeDecoratedTextEdit::gutterPaintEvent(QPaintEvent *e)
{
	QPainter painter(gutter);

	// Paint our background.
	painter.fillRect(e->rect(), gutterBackground);

	// Paint our line numbers.

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = static_cast<int>(blockBoundingGeometry(block)
		.translated(contentOffset()).top());
	int bottom = top + static_cast<int>(blockBoundingRect(block).height());

	while(block.isValid() && top <= e->rect().bottom())
	{
		if(block.isVisible() && bottom >= e->rect().top())
		{
			QString number =
				QString::number(blockNumber + 1);

			painter.setPen(gutterForeground);
			painter.drawText(0, top, gutter->width(),
				fontMetrics().height(),
				Qt::AlignCenter, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + static_cast<int>(
			blockBoundingRect(block).height());
		blockNumber++;
	}
}

/*!
 * This function computes the width our gutter should have, based upon the number
 * of lines in our document. We ensure that we have enough space to render our
 * line numbers inside the gutter.
 *
 * \return The width of our gutter.
 */
int QomposeDecoratedTextEdit::gutterWidth()
{
	if(!isGutterVisible())
		return 0;

	int digits = 1;
	int max = qMax(1, blockCount());

	while(max >= 10)
	{
		max /= 10;
		digits++;
	}

	int space = 20 + (fontMetrics().width(QLatin1Char('9')) * digits);

	return space;
}

/*!
 * This function computes and returns the offset from the left-hand side of our
 * text editor widget the line wrap guide should be painted.
 *
 * \return The offset of the line wrap guide.
 */
qreal QomposeDecoratedTextEdit::wrapGuideOffset()
{
	QomposeFontMetrics metrics(currentFont);

	qreal offset = metrics.getColumnWidthF(getWrapGuideColumnWidth());
	offset += contentOffset().x();
	offset += document()->documentMargin();

	return offset;
}

/*!
 * This is a utility function which calls update() on both this widget, as well
 * as our viewport widget, to make sure we are really updated.
 */
void QomposeDecoratedTextEdit::fullUpdate()
{ /* SLOT */

	update();
	viewport()->update();

	highlightCurrentLine();

}

/*!
 * This is a utility function which calls repaint() on both this widget, as well
 * as our viewport widget, to make sure our paintEvent() function is called.
 */
void QomposeDecoratedTextEdit::fullRepaint()
{ /* SLOT */

	repaint();
	viewport()->repaint();

	highlightCurrentLine();

}

/*!
 * This function handles highlighting the current line in our editor.
 */
void QomposeDecoratedTextEdit::highlightCurrentLine()
{ /* SLOT */

	QList<QTextEdit::ExtraSelection> es;
	QTextEdit::ExtraSelection selection;

	selection.format.setBackground(currentLineHighlight);
	selection.format.setProperty(QTextFormat::FullWidthSelection, true);

	selection.cursor = textCursor();
	selection.cursor.clearSelection();

	es.append(selection);
	setExtraSelections(es);

}

/*!
 * This function updates our gutter's width, e.g. when the number of lines in our
 * document changes.
 */
void QomposeDecoratedTextEdit::updateGutterWidth()
{ /* SLOT */

	setViewportMargins(gutterWidth(), 0, 0, 0);

}

/*!
 * This function updates our gutter, e.g. when our editor widget's state is
 * updated, by resetting the gutter's scroll and width.
 *
 * \param r The viewport rect being updated.
 * \param dy The editor's current y scroll offset.
 */
void QomposeDecoratedTextEdit::updateGutter(const QRect &r, int dy)
{ /* SLOT */

	if(dy)
		gutter->scroll(0, dy);
	else
		gutter->update(0, r.y(), gutter->width(), r.height());

	if(r.contains(viewport()->rect()))
		updateGutterWidth();

}
