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

#include "Editor.h"

#include <QPainter>
#include <QTextBlock>

#include "QomposeCommon/editor/Gutter.h"
#include "QomposeCommon/editor/algorithm/General.h"
#include "QomposeCommon/editor/algorithm/Indentation.h"
#include "QomposeCommon/editor/algorithm/Movement.h"
#include "QomposeCommon/editor/search/Find.h"
#include "QomposeCommon/editor/search/Replace.h"
#include "QomposeCommon/editor/search/applyAlgorithm.h"
#include "QomposeCommon/util/FontMetrics.h"

namespace qompose
{
namespace editor
{
Editor::Editor(QWidget *p)
        : hotkey::HotkeyedWidget<QPlainTextEdit>(p),
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
	initializeHotkeys();

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

void Editor::setGutterVisible(bool v)
{
	gutterVisible = v;
	gutter->setVisible(v);
	updateGutterWidth();
}

bool Editor::isGutterVisible()
{
	return gutterVisible;
}

QFont Editor::font() const
{
	// Return a font identical to ours, with a size excluding scaling.

	QFont f = currentFont;
	f.setPointSizeF(originalFontSize);

	return f;
}

void Editor::setFont(const QFont &f)
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

int Editor::fontZoom() const
{
	return currentFontZoom;
}

qreal Editor::fontZoomSize() const
{
	qreal scale = static_cast<qreal>(fontZoom()) / 100.0;
	qreal fsize = originalFontSize + (scale * originalFontSize);

	return qMax(fsize, 0.0);
}

void Editor::setFontZoom(int z)
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

void Editor::resetFontZoom()
{
	setFontZoom(0);
}

std::size_t Editor::getIndentationWidth() const
{
	return static_cast<std::size_t>(indentationWidth);
}

void Editor::setIndentationWidth(int w)
{
	indentationWidth = qAbs(w);

	FontMetrics metrics(currentFont);
	qreal tabw = metrics.getColumnWidthF(indentationWidth);

	QTextOption opt = document()->defaultTextOption();
	opt.setTabStop(tabw);
	document()->setDefaultTextOption(opt);
}

IndentationMode Editor::getIndentationMode() const
{
	return indentationMode;
}

void Editor::setIndentationMode(const QString &mode)
{
	IndentationMode m = IndentationMode::Tabs;

	if(mode == "tabs")
		m = IndentationMode::Tabs;
	else if(mode == "spaces")
		m = IndentationMode::Spaces;

	setIndentationMode(m);
}

void Editor::setIndentationMode(IndentationMode mode)
{
	indentationMode = mode;
}

bool Editor::isWrapGuideVisible() const
{
	return wrapGuideVisible;
}

void Editor::setWrapGuideVisible(bool v)
{
	wrapGuideVisible = v;

	fullUpdate();
}

int Editor::getWrapGuideColumnWidth() const
{
	return wrapGuideWidth;
}

void Editor::setWrapGuideColumnWidth(int w)
{
	wrapGuideWidth = qAbs(w);

	fullUpdate();
}

QColor Editor::getWrapGuideColor() const
{
	return wrapGuideColor;
}

void Editor::setWrapGuideColor(const QColor &c)
{
	wrapGuideColor = c;

	fullUpdate();
}

bool Editor::getLineWrapping() const
{
	return lineWrapMode() == QPlainTextEdit::WidgetWidth;
}

void Editor::setLineWrapping(bool enabled)
{
	setLineWrapMode(enabled ? QPlainTextEdit::WidgetWidth
	                        : QPlainTextEdit::NoWrap);
}

QColor Editor::getEditorForeground() const
{
	return palette().color(QPalette::Text);
}

void Editor::setEditorForeground(const QColor &c)
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

QColor Editor::getEditorBackground() const
{
	return palette().color(QPalette::Active, QPalette::Base);
}

void Editor::setEditorBackground(const QColor &c)
{
	QPalette p = palette();

	p.setColor(QPalette::Active, QPalette::Base, c);
	p.setColor(QPalette::Inactive, QPalette::Base, c);

	setPalette(p);
}

QColor Editor::getCurrentLineColor() const
{
	return currentLineHighlight;
}

void Editor::setCurrentLineColor(const QColor &c)
{
	currentLineHighlight = c;

	highlightCurrentLine();
}

QColor Editor::getGutterForeground() const
{
	return gutterForeground;
}

void Editor::setGutterForeground(const QColor &c)
{
	gutterForeground = c;

	gutter->update();
}

QColor Editor::getGutterBackground() const
{
	return gutterBackground;
}

void Editor::setGutterBackground(const QColor &c)
{
	gutterBackground = c;

	gutter->update();
}

int Editor::getCurrentLine() const
{
	return textCursor().blockNumber() + 1;
}

int Editor::getCurrentColumn() const
{
	FontMetrics metrics(currentFont);

	qreal xoff = static_cast<qreal>(cursorRect().x());

	xoff -= x();
	xoff -= contentOffset().x();
	xoff -= document()->documentMargin();

	xoff /= metrics.getColumnWidthF(1);

	return qRound(xoff) + 1;
}

void Editor::paintEvent(QPaintEvent *e)
{
	// Let our superclass do its painting, and prepare to do our own.

	QPlainTextEdit::paintEvent(e);

	QRect eventRect = e->rect();
	QPainter painter(viewport());
	FontMetrics metrics(currentFont);

// Draw our offset and margin lines, if debugging is enabled.

#ifdef QOMPOSE_DEBUG
	painter.setPen(QPen(QColor(255, 0, 0)));

	painter.drawLine(static_cast<int>(contentOffset().x()), eventRect.top(),
	                 static_cast<int>(contentOffset().x()),
	                 eventRect.bottom());

	painter.setPen(QPen(QColor(0, 255, 0)));

	painter.drawLine(static_cast<int>(document()->documentMargin()),
	                 eventRect.top(),
	                 static_cast<int>(document()->documentMargin()),
	                 eventRect.bottom());
#endif

// Draw lines at each character column, if debugging is enabled.

#ifdef QOMPOSE_DEBUG
	painter.setPen(QPen(QColor(0, 0, 0)));

	for(int col = 1; col <= 100; ++col)
	{
		qreal coff = metrics.getColumnWidthF() * col;

		coff += contentOffset().x();
		coff += document()->documentMargin();

		painter.drawLine(static_cast<int>(coff), eventRect.top(),
		                 static_cast<int>(coff), eventRect.bottom());
	}
#endif

	// Draw our line wrap guide, if it is enabled.

	if(isWrapGuideVisible() && metrics.isMonospaced())
	{
		qreal offset = wrapGuideOffset();

		painter.setPen(QPen(getWrapGuideColor()));
		painter.drawLine(static_cast<int>(offset), eventRect.top(),
		                 static_cast<int>(offset), eventRect.bottom());
	}
}

void Editor::focusInEvent(QFocusEvent *e)
{
	highlightCurrentLine();
	QPlainTextEdit::focusInEvent(e);
}

void Editor::focusOutEvent(QFocusEvent *e)
{
	highlightCurrentLine();
	QPlainTextEdit::focusOutEvent(e);
}

void Editor::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	gutter->setGeometry(
	        QRect(cr.left(), cr.top(), gutterWidth(), cr.height()));
}

void Editor::wheelEvent(QWheelEvent *e)
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

void Editor::mouseReleaseEvent(QMouseEvent *e)
{
	highlightCurrentLine();

	// Do our superclass's normal mouse action.

	QPlainTextEdit::mouseReleaseEvent(e);
}

QString Editor::getIndentString() const
{
	switch(getIndentationMode())
	{
	case IndentationMode::Spaces:
		return QString(" ").repeated(
		        static_cast<int>(getIndentationWidth()));

	case IndentationMode::Tabs:
		return QString("\t");
	}
}

qreal Editor::contentMargin() const
{
	return static_cast<qreal>(contentOffset().x()) +
	       static_cast<qreal>(document()->documentMargin());
}

qreal Editor::singleColumnWidth() const
{
	FontMetrics metrics(currentFont);
	return metrics.getColumnWidthF(1);
}

qreal Editor::columnOffset(int column) const
{
	return singleColumnWidth() * static_cast<qreal>(column) +
	       contentMargin();
}

qreal Editor::wrapGuideOffset() const
{
	return columnOffset(getWrapGuideColumnWidth());
}

void Editor::initializeHotkeys()
{
	// Backspace

	addHotkey(Hotkey(Qt::Key_Backspace, 0, ~Qt::KeyboardModifiers(0)),
	          [this]() {
		          algorithm::applyAlgorithm(*this, algorithm::backspace,
		                                    getIndentationMode(),
		                                    getIndentationWidth());
		  });

	// Delete

	addHotkey(Hotkey(Qt::Key_Delete, 0, ~Qt::KeyboardModifiers(0)),
	          [this]() {
		          algorithm::applyAlgorithm(*this,
		                                    algorithm::deleteCharacter);
		  });

	// Enter

	addHotkey(Hotkey(Qt::Key_Return, 0, ~Qt::KeyboardModifiers(0)),
	          [this]() {
		          algorithm::applyAlgorithm(*this, algorithm::newline);
		  });

	addHotkey(Hotkey(Qt::Key_Enter, 0, ~Qt::KeyboardModifiers(0)),
	          [this]() {
		          algorithm::applyAlgorithm(*this, algorithm::newline);
		  });

	// Tab

	addHotkey(Hotkey(Qt::Key_Tab), [this]() {
		algorithm::applyAlgorithm(*this, algorithm::tab,
		                          getIndentationMode(),
		                          getIndentationWidth());
	});

	// Shift + Tab

	addHotkey(Hotkey(Qt::Key_Tab, Qt::ShiftModifier), [this]() {
		algorithm::applyAlgorithm(
		        *this, algorithm::decreaseSelectionIndent,
		        getIndentationMode(), getIndentationWidth());
	});

	addHotkey(Hotkey(Qt::Key_Backtab, Qt::ShiftModifier), [this]() {
		algorithm::applyAlgorithm(
		        *this, algorithm::decreaseSelectionIndent,
		        getIndentationMode(), getIndentationWidth());
	});

	// Home

	addHotkey(Hotkey(Qt::Key_Home), [this]() {
		algorithm::applyAlgorithm(*this, algorithm::home, true);
	});

	// Shift + Home

	addHotkey(Hotkey(Qt::Key_Home, Qt::ShiftModifier), [this]() {
		algorithm::applyAlgorithm(*this, algorithm::home, false);
	});

	// Ctrl+D

	addHotkey(Hotkey(Qt::Key_D, Qt::ControlModifier), [this]() {
		algorithm::applyAlgorithm(*this, algorithm::duplicateBlock);
	});

	// Ctrl+(Zero)

	addHotkey(Hotkey(Qt::Key_0, Qt::ControlModifier),
	          std::bind(&Editor::resetFontZoom, this));

	// Ctrl+Shift+Left

	addHotkey(Hotkey(Qt::Key_Left, Qt::ControlModifier | Qt::ShiftModifier),
	          []() {});

	// Ctrl+Shift+Right

	addHotkey(
	        Hotkey(Qt::Key_Right, Qt::ControlModifier | Qt::ShiftModifier),
	        []() {});

	// Ctrl+Insert

	addHotkey(Hotkey(Qt::Key_Insert, Qt::ControlModifier), []() {});

	// Ctrl+K

	addHotkey(Hotkey(Qt::Key_K, Qt::ControlModifier), []() {});

	// Shift+Insert

	addHotkey(Hotkey(Qt::Key_Insert, Qt::ShiftModifier), []() {});

	// Shift+Delete

	addHotkey(Hotkey(Qt::Key_Delete, Qt::ShiftModifier), []() {});
}

void Editor::gutterPaintEvent(QPaintEvent *e)
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

int Editor::gutterWidth()
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

void Editor::fullUpdate()
{
	updateGutterWidth();

	update();
	viewport()->update();

	highlightCurrentLine();
}

void Editor::fullRepaint()
{
	repaint();
	viewport()->repaint();

	highlightCurrentLine();
}

void Editor::duplicateLine()
{
	algorithm::applyAlgorithm(*this, algorithm::duplicateBlock);
}

void Editor::deselect()
{
	algorithm::applyAlgorithm(*this, algorithm::deselect);
}

void Editor::increaseSelectionIndent()
{
	algorithm::applyAlgorithm(*this, algorithm::increaseSelectionIndent,
	                          getIndentationMode(), getIndentationWidth());
}

void Editor::decreaseSelectionIndent()
{
	algorithm::applyAlgorithm(*this, algorithm::decreaseSelectionIndent,
	                          getIndentationMode(), getIndentationWidth());
}

search::FindResult Editor::findNext(search::FindQuery const &q)
{
	return search::applyAlgorithm(*this, search::find, true, q);
}

search::FindResult Editor::findPrevious(search::FindQuery const &q)
{
	return search::applyAlgorithm(*this, search::find, false, q);
}

search::FindResult Editor::replace(search::ReplaceQuery const &q)
{
	return search::applyAlgorithm(*this, search::replace, q);
}

search::FindResult Editor::replaceSelection(search::ReplaceQuery const &q)
{
	QTextCursor c = textCursor();
	return search::applyAlgorithm(*this, search::batchReplace, q,
	                              std::min(c.anchor(), c.position()),
	                              std::max(c.anchor(), c.position()));
}

search::FindResult Editor::replaceAll(search::ReplaceQuery const &q)
{
	return search::applyAlgorithm(*this, search::batchReplace, q, 0,
	                              boost::none);
}

void Editor::goToLine(int l)
{
	algorithm::applyAlgorithm(*this, algorithm::goToBlock, l - 1);
}

void Editor::highlightCurrentLine()
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

void Editor::updateGutterWidth()
{
	setViewportMargins(gutterWidth(), 0, 0, 0);
}

void Editor::updateGutter(const QRect &r, int dy)
{
	if(dy)
		gutter->scroll(0, dy);
	else
		gutter->update(0, r.y(), gutter->width(), r.height());

	if(r.contains(viewport()->rect()))
		updateGutterWidth();
}
}
}
