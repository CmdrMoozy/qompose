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

#include "DecoratedTextEdit.h"

#include <QPainter>
#include <QTextBlock>

#include "QomposeCommon/editor/Gutter.h"
#include "QomposeCommon/util/FontMetrics.h"

namespace qompose
{
DecoratedTextEdit::DecoratedTextEdit(QWidget *p)
        : QPlainTextEdit(p),
          gutter(nullptr),
          gutterVisible(false),
          currentFont(QFont("Courier")),
          originalFontSize(11.0),
          currentFontZoom(1),
          indentationWidth(8),
          indentationMode(IndentationMode::Tabs),
          wrapGuideVisible(false),
          wrapGuideWidth(0),
          wrapGuideColor(QColor(255, 255, 255)),
          currentLineHighlight(QColor(128, 128, 128)),
          gutterForeground(QColor(255, 255, 255)),
          gutterBackground(QColor(0, 0, 0))
{
	// Set our editor's default font and color scheme.

	setFont(QFont("Courier", 11));
	setFontZoom(0);

	setEditorForeground(QColor(0, 0, 0));
	setEditorBackground(QColor(255, 255, 255));

	// Initialize our gutter.

	gutter = new Gutter(this);

	setGutterForeground(QColor(255, 255, 255));
	setGutterBackground(QColor(0, 0, 0));

	QObject::connect(this, SIGNAL(blockCountChanged(int)), this,
	                 SLOT(updateGutterWidth()));
	QObject::connect(this, SIGNAL(updateRequest(const QRect &, int)), this,
	                 SLOT(updateGutter(const QRect &, int)));
	QObject::connect(this, SIGNAL(cursorPositionChanged()), this,
	                 SLOT(highlightCurrentLine()));

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

void DecoratedTextEdit::setGutterVisible(bool v)
{
	gutterVisible = v;
	gutter->setVisible(v);
	updateGutterWidth();
}

bool DecoratedTextEdit::isGutterVisible()
{
	return gutterVisible;
}

QFont DecoratedTextEdit::font() const
{
	// Return a font identical to ours, with a size excluding scaling.

	QFont f = currentFont;
	f.setPointSizeF(originalFontSize);

	return f;
}

void DecoratedTextEdit::setFont(const QFont &f)
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

	setIndentationWidth(indentationWidth);

	// Update our line wrap guide, since it depends on our font.

	fullUpdate();
}

int DecoratedTextEdit::fontZoom() const
{
	return currentFontZoom;
}

qreal DecoratedTextEdit::fontZoomSize() const
{
	qreal scale = static_cast<qreal>(fontZoom()) / 100.0;
	qreal fsize = originalFontSize + (scale * originalFontSize);

	return qMax(fsize, 0.0);
}

void DecoratedTextEdit::setFontZoom(int z)
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

	setIndentationWidth(indentationWidth);
}

void DecoratedTextEdit::resetFontZoom()
{
	setFontZoom(0);
}

std::size_t DecoratedTextEdit::getIndentationWidth() const
{
	return static_cast<std::size_t>(indentationWidth);
}

void DecoratedTextEdit::setIndentationWidth(int w)
{
	indentationWidth = qAbs(w);

	FontMetrics metrics(currentFont);
	qreal tabw = metrics.getColumnWidthF(indentationWidth);

	QTextOption opt = document()->defaultTextOption();
	opt.setTabStop(tabw);
	document()->setDefaultTextOption(opt);
}

IndentationMode DecoratedTextEdit::getIndentationMode() const
{
	return indentationMode;
}

void DecoratedTextEdit::setIndentationMode(const QString &mode)
{
	IndentationMode m = IndentationMode::Tabs;

	if(mode == "tabs")
		m = IndentationMode::Tabs;
	else if(mode == "spaces")
		m = IndentationMode::Spaces;

	setIndentationMode(m);
}

void DecoratedTextEdit::setIndentationMode(IndentationMode mode)
{
	indentationMode = mode;
}

bool DecoratedTextEdit::isWrapGuideVisible() const
{
	return wrapGuideVisible;
}

void DecoratedTextEdit::setWrapGuideVisible(bool v)
{
	wrapGuideVisible = v;

	fullUpdate();
}

int DecoratedTextEdit::getWrapGuideColumnWidth() const
{
	return wrapGuideWidth;
}

void DecoratedTextEdit::setWrapGuideColumnWidth(int w)
{
	wrapGuideWidth = qAbs(w);

	fullUpdate();
}

QColor DecoratedTextEdit::getWrapGuideColor() const
{
	return wrapGuideColor;
}

void DecoratedTextEdit::setWrapGuideColor(const QColor &c)
{
	wrapGuideColor = c;

	fullUpdate();
}

bool DecoratedTextEdit::getLineWrapping() const
{
	return lineWrapMode() == QPlainTextEdit::WidgetWidth;
}

void DecoratedTextEdit::setLineWrapping(bool enabled)
{
	setLineWrapMode(enabled ? QPlainTextEdit::WidgetWidth
	                        : QPlainTextEdit::NoWrap);
}

QColor DecoratedTextEdit::getEditorForeground() const
{
	return palette().color(QPalette::Text);
}

void DecoratedTextEdit::setEditorForeground(const QColor &c)
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

QColor DecoratedTextEdit::getEditorBackground() const
{
	return palette().color(QPalette::Active, QPalette::Base);
}

void DecoratedTextEdit::setEditorBackground(const QColor &c)
{
	QPalette p = palette();

	p.setColor(QPalette::Active, QPalette::Base, c);
	p.setColor(QPalette::Inactive, QPalette::Base, c);

	setPalette(p);
}

QColor DecoratedTextEdit::getCurrentLineColor() const
{
	return currentLineHighlight;
}

void DecoratedTextEdit::setCurrentLineColor(const QColor &c)
{
	currentLineHighlight = c;

	highlightCurrentLine();
}

QColor DecoratedTextEdit::getGutterForeground() const
{
	return gutterForeground;
}

void DecoratedTextEdit::setGutterForeground(const QColor &c)
{
	gutterForeground = c;

	gutter->update();
}

QColor DecoratedTextEdit::getGutterBackground() const
{
	return gutterBackground;
}

void DecoratedTextEdit::setGutterBackground(const QColor &c)
{
	gutterBackground = c;

	gutter->update();
}

int DecoratedTextEdit::getCurrentLine() const
{
	return textCursor().blockNumber() + 1;
}

int DecoratedTextEdit::getCurrentColumn() const
{
	FontMetrics metrics(currentFont);

	qreal xoff = static_cast<qreal>(cursorRect().x());

	xoff -= x();
	xoff -= contentOffset().x();
	xoff -= document()->documentMargin();

	xoff /= metrics.getColumnWidthF(1);

	return qRound(xoff) + 1;
}

void DecoratedTextEdit::paintEvent(QPaintEvent *e)
{
	// Let our superclass do its painting, and prepare to do our own.

	QPlainTextEdit::paintEvent(e);

	QRect eventRect = e->rect();
	QPainter painter(viewport());
	FontMetrics metrics(currentFont);

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

		painter.drawLine(coff, eventRect.top(), coff,
		                 eventRect.bottom());
	}
#endif

	// Draw our line wrap guide, if it is enabled.

	if(isWrapGuideVisible() && metrics.isMonospaced())
	{
		qreal offset = wrapGuideOffset();

		painter.setPen(QPen(getWrapGuideColor()));
		painter.drawLine(offset, eventRect.top(), offset,
		                 eventRect.bottom());
	}
}

void DecoratedTextEdit::focusInEvent(QFocusEvent *e)
{
	highlightCurrentLine();
	QPlainTextEdit::focusInEvent(e);
}

void DecoratedTextEdit::focusOutEvent(QFocusEvent *e)
{
	highlightCurrentLine();
	QPlainTextEdit::focusOutEvent(e);
}

void DecoratedTextEdit::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	gutter->setGeometry(
	        QRect(cr.left(), cr.top(), gutterWidth(), cr.height()));
}

void DecoratedTextEdit::wheelEvent(QWheelEvent *e)
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

void DecoratedTextEdit::mouseReleaseEvent(QMouseEvent *e)
{
	highlightCurrentLine();

	// Do our superclass's normal mouse action.

	QPlainTextEdit::mouseReleaseEvent(e);
}

QString DecoratedTextEdit::getIndentString() const
{
	switch(getIndentationMode())
	{
	case IndentationMode::Spaces:
		return QString(" ").repeated(getIndentationWidth());

	case IndentationMode::Tabs:
	default:
		return QString("\t");
	}
}

qreal DecoratedTextEdit::contentMargin() const
{
	return static_cast<qreal>(contentOffset().x()) +
	       static_cast<qreal>(document()->documentMargin());
}

qreal DecoratedTextEdit::singleColumnWidth() const
{
	FontMetrics metrics(currentFont);
	return metrics.getColumnWidthF(1);
}

qreal DecoratedTextEdit::columnOffset(int column) const
{
	return singleColumnWidth() * static_cast<qreal>(column) +
	       contentMargin();
}

qreal DecoratedTextEdit::wrapGuideOffset() const
{
	return columnOffset(getWrapGuideColumnWidth());
}

void DecoratedTextEdit::gutterPaintEvent(QPaintEvent *e)
{
	QPainter painter(gutter);

	// Paint our background.
	painter.fillRect(e->rect(), gutterBackground);

	// Paint our line numbers.

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = static_cast<int>(
	        blockBoundingGeometry(block).translated(contentOffset()).top());
	int bottom = top + static_cast<int>(blockBoundingRect(block).height());

	while(block.isValid() && top <= e->rect().bottom())
	{
		if(block.isVisible() && bottom >= e->rect().top())
		{
			QString number = QString::number(blockNumber + 1);

			painter.setPen(gutterForeground);
			painter.drawText(0, top, gutter->width(),
			                 fontMetrics().height(),
			                 Qt::AlignCenter, number);
		}

		block = block.next();
		top = bottom;
		bottom = top +
		         static_cast<int>(blockBoundingRect(block).height());
		blockNumber++;
	}
}

int DecoratedTextEdit::gutterWidth()
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

void DecoratedTextEdit::fullUpdate()
{
	updateGutterWidth();

	update();
	viewport()->update();

	highlightCurrentLine();
}

void DecoratedTextEdit::fullRepaint()
{
	repaint();
	viewport()->repaint();

	highlightCurrentLine();
}

void DecoratedTextEdit::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> es;
	QTextEdit::ExtraSelection selection;

	selection.format.setBackground(currentLineHighlight);
	selection.format.setProperty(QTextFormat::FullWidthSelection, true);

	selection.cursor = textCursor();
	selection.cursor.clearSelection();

	es.append(selection);
	setExtraSelections(es);
}

void DecoratedTextEdit::updateGutterWidth()
{
	setViewportMargins(gutterWidth(), 0, 0, 0);
}

void DecoratedTextEdit::updateGutter(const QRect &r, int dy)
{
	if(dy)
		gutter->scroll(0, dy);
	else
		gutter->update(0, r.y(), gutter->width(), r.height());

	if(r.contains(viewport()->rect()))
		updateGutterWidth();
}
}
