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

#include "QomposeEditor.h"

#include <cstddef>

#include <QGridLayout>
#include <QPainter>
#include <QTextBlock>
#include <QColor>
#include <QRect>
#include <QFont>
#include <QList>
#include <QWheelEvent>
#include <QFocusEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QTextCursor>
#include <QRegExp>
#include <QMouseEvent>
#include <QFontMetrics>
#include <QRegExp>

#include "util/QomposeFindQuery.h"
#include "util/QomposeReplaceQuery.h"

/*****************
 * QomposeGutter *
 *****************/

/*!
 * This is our default constructor, which creates a new gutter for the given editor
 * object.
 *
 * \param e The editor this gutter is attached to.
 */
QomposeEditor::QomposeGutter::QomposeGutter(QomposeEditor *e)
	: QWidget(e), editor(e)
{
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
QomposeEditor::QomposeGutter::~QomposeGutter()
{
}

/*!
 * This function returns the size hint for our gutter, to let our parent widget know how
 * wide to render this gutter widget.
 *
 * \return The size hint for this gutter.
 */
QSize QomposeEditor::QomposeGutter::sizeHint() const
{
	int w = (editor == NULL) ? 0 : editor->gutterWidth();
	return QSize(w, 0);
}

/*!
 * This function handles our gutter's paint event by passing the event up to our parent editor,
 * so it can render the gutter based upon the editor's state.
 *
 * \param e The paint event being handled.
 */
void QomposeEditor::QomposeGutter::paintEvent(QPaintEvent *e)
{
	if(editor != NULL)
		editor->gutterPaintEvent(e);
}

/*****************
 * QomposeEditor *
 *****************/

/*!
 * This function initializes a new editor widget, using the given parent widget.
 *
 * \param p Our parent widget. 
 */
QomposeEditor::QomposeEditor(QWidget *p)
	: QPlainTextEdit(p), currentFont(QFont("Courier", 10)),
		originalFontSize(10.0), currentFontZoom(1)
{
	// Set some default colors.
	
	currentLineHighlight = QColor(127, 127, 127);
	gutterForeground = QColor(255, 255, 255);
	gutterBackground = QColor(0, 0, 0);
	
	setFont(QFont("Courier", 10));
	
	// Initialize our widget.
	
	gutter = new QomposeGutter(this);
	
	QObject::connect( this, SIGNAL( blockCountChanged(int)            ), this, SLOT( updateGutterWidth()              ) );
	QObject::connect( this, SIGNAL( updateRequest(const QRect &, int) ), this, SLOT( updateGutter(const QRect &, int) ) );
	QObject::connect( this, SIGNAL( cursorPositionChanged()           ), this, SLOT( highlightCurrentLine()           ) );
	
	setTabWidthSpaces(8);
	
	setLineWrapMode(QPlainTextEdit::NoWrap);
	setFocusPolicy(Qt::ClickFocus);
	
	setGutterVisible(true);
	
	highlightCurrentLine();
	
	setFont(QFont("Courier", 11));
	setFontZoom(0);
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
QomposeEditor::~QomposeEditor()
{
}

/*!
 * This function sets whether or not our gutter is visible.
 *
 * \param v Whether or not our gutter should be visible.
 */
void QomposeEditor::setGutterVisible(bool v)
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
bool QomposeEditor::isGutterVisible()
{
	return gutterVisible;
}

/*!
 * This function returns our current font. Note that the size in the returned font object
 * does NOT include any changes due to our current zoom factor. Our zoomed size can be determined
 * from fontZoomSize().
 *
 * \return Our current font.
 */
QFont QomposeEditor::font() const
{
	// Return a font identical to ours, except with a size not including scaling.
	
	QFont f = currentFont;
	f.setPointSizeF(originalFontSize);
	
	return f;
}

/*!
 * This function sets our widget's font to the given value. Note that fonts which are
 * assigned a PIXEL SIZE are not allowed - these will be resized to 10 points.
 *
 * \param f The new font to use.
 */
void QomposeEditor::setFont(const QFont &f)
{
	// Make a note of our current tab width (in spaces).
	
	int oldWidth = tabWidthSpaces();
	
	// Make sure we aren't given a pixel-sized font.
	
	currentFont = f;
	
	if(currentFont.pointSize() == -1)
		currentFont.setPointSize(10);
	
	originalFontSize = currentFont.pointSizeF();
	originalFontSize = qMax(originalFontSize, 1.0);
	
	// Set our font!
	
	QPlainTextEdit::setFont(currentFont);
	
	// Make sure our tab width is still the same (in spaces).
	
	setTabWidthSpaces(oldWidth);
}

/*!
 * This function returns our widget's current font zoom factor. The value returned is
 * relative to zero; e.g., a value of 100 means doubling the original font size, whereas
 * a value of -100 means the original size reduced to 0.
 *
 * The minimum value this can return is -100, and there is no maximum value.
 *
 * \return Our current font zooming factor.
 */
int QomposeEditor::fontZoom() const
{
	return currentFontZoom;
}

/*!
 * This function returns the current font size, including any zooming currently in place.
 *
 * \return Our current font size, including zooming.
 */
qreal QomposeEditor::fontZoomSize() const
{
	qreal scale = static_cast<qreal>(fontZoom()) / 100.0;
	qreal fsize = originalFontSize + (scale * originalFontSize);
	
	return fsize;
}

/*!
 * This function sets our widget's font zoom. The value given is relative to the current
 * size; e.g., passing 100 means doubling the size, whereas passing -100 means a size of 0.
 *
 * The minimum value that can be set is -100; this results in a size of 0. There is no
 * maximum value, however.
 *
 * \param z The new zoom factor for our font size.
 */
void QomposeEditor::setFontZoom(int z)
{
	z = qMax(-100, z);
	currentFontZoom = z;
	
	qreal scale = static_cast<qreal>(currentFontZoom) / 100.0;
	qreal fsize = originalFontSize + (scale * originalFontSize);
	
	fsize = qMax(fsize, 1.0);
	
	currentFont.setPointSizeF(fsize);
	QPlainTextEdit::setFont(currentFont);
}

/*!
 * This function returns the width of a tab stop, in spaces (based upon the current
 * font) - NOT in pixels.
 *
 * \return The editor's tab width.
 */
int QomposeEditor::tabWidthSpaces() const
{
	double w = static_cast<double>(tabStopWidth());
	
	QFontMetrics m(font());
	
	w /= static_cast<double>(m.width(' '));
	qRound(w);
	
	return static_cast<int>(w);
}

/*!
 * This function sets the width of our editor's tab stops, in spaces (based upon the
 * current font) - NOT in pixels.
 *
 * \param w The new tab width to use.
 */
void QomposeEditor::setTabWidthSpaces(int w)
{
	w = qAbs(w);
	
	QFontMetrics m(font());
	
	setTabStopWidth(w * m.width(' '));
}

/*!
 * This function returns our editor's foreground (text) color.
 *
 * \return Our editor's foreground color.
 */
QColor QomposeEditor::getEditorForeground() const
{
	return palette().color(QPalette::Text);
}

/*!
 * This function sets our editor's foreground (text) color.
 *
 * \param c The new foreground color to use.
 */
void QomposeEditor::setEditorForeground(const QColor &c)
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
QColor QomposeEditor::getEditorBackground() const
{
	return palette().color(QPalette::Active, QPalette::Base);
}

/*!
 * This function sets our editor's background color.
 *
 * \param c The new background color to use.
 */
void QomposeEditor::setEditorBackground(const QColor &c)
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
QColor QomposeEditor::getCurrentLineColor() const
{
	return currentLineHighlight;
}

/*!
 * This function sets our editor's current line highlight color.
 *
 * \param c The new current line color to use.
 */
void QomposeEditor::setCurrentLineColor(const QColor &c)
{
	currentLineHighlight = c;
	
	highlightCurrentLine();
}

/*!
 * This function returns our editor's gutter's foreground (text) color.
 *
 * \return Our gutter's foreground color.
 */
QColor QomposeEditor::getGutterForeground() const
{
	return gutterForeground;
}

/*!
 * This function sets our editor's gutter's foreground (text) color. This
 * automatically repaints our gutter, so this change takes effect immediately.
 *
 * \param c The new gutter foreground color to use.
 */
void QomposeEditor::setGutterForeground(const QColor &c)
{
	gutterForeground = c;
	
	gutter->update();
}

/*!
 * This function returns our editor's gutter's background color.
 *
 * \return Our gutter's background color.
 */
QColor QomposeEditor::getGutterBackground() const
{
	return gutterBackground;
}

/*!
 * This function sets our editor's gutter's background color. This automatically
 * repaints our gutter, so this change takes effect immediately.
 *
 * \param c The new gutter background color to use.
 */
void QomposeEditor::setGutterBackground(const QColor &c)
{
	gutterBackground = c;
	
	gutter->update();
}

/*!
 * We override our parent's focus-in event to update our UI when that event occurs.
 *
 * \param e The event being handled.
 */
void QomposeEditor::focusInEvent(QFocusEvent *e)
{
	highlightCurrentLine();
	QPlainTextEdit::focusInEvent(e);
}

/*!
 * We override our parent's focus-out event to update our UI when that event occurs.
 *
 * \param e The event being handled.
 */
void QomposeEditor::focusOutEvent(QFocusEvent *e)
{
	highlightCurrentLine();
	QPlainTextEdit::focusOutEvent(e);
}

/*!
 * This function handles the resetting of our gutter's geometry when we get resized.
 *
 * \param event The resize event we are handling.
 */
void QomposeEditor::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);
	
	QRect cr = contentsRect();
	gutter->setGeometry(QRect(cr.left(), cr.top(), gutterWidth(), cr.height()));
}

/*!
 * We implement the following hotkeys:
 *
 * Custom (implemented by QomposeTextEdit):
 * 	Return           Move to a new line, maintaining indent.
 * 	Enter            Move to a new line, maintaining indent.
 * 	Tab              Increase indent on selection.
 * 	Shift+Backtab    Decrease indent on selection.
 *      Shift+Tab        Decrease indent on selection.
 * 	Home             Move to start of non-indent, then to start of line.
 * 	Ctrl+Wheel       Zoom text in and out.
 *      Ctrl+D           Duplicate the current line.
 *      Shift+Return     The same as Return.
 *      Shift+Ender      The same as Enter.
 *      Ctrl+(Zero)      Reset text zoom to default level.
 * 
 * Inherited (implemented by QPlainTextEdit):
 * 	Backspace        Deletes the character to the left of the cursor.
 * 	Delete           Deletes the character to the right of the cursor.
 * 	Ctrl+C           Copy selected text to clipboard.
 * 	Ctrl+V           Paste clipboard into text edit.
 * 	Ctrl+X           Deletes selected text + copies to clipboard.
 * 	Ctrl+Z           Undo.
 * 	Ctrl+Y           Redo.
 * 	Left             Moves the cursor one character left.
 * 	Ctrl+Left        Moves the cursor one word left.
 * 	Right            Moves the cursor one character right.
 * 	Ctrl+Right       Moves the cursor one word right.
 * 	Up               Moves the cursor one line up.
 * 	Down             Moves the cursor one line down.
 * 	PageUp           Moves the cursor one page up.
 * 	PageDown         Moves the cursor one page down.
 * 	Ctrl+Home        Moves the cursor to the beginning of the document.
 * 	End              Moves the cursor to the end of the line.
 * 	Ctrl+End         Moves the cursor to the end of the document.
 * 	Alt+Wheel        Scrolls the page horizontally.
 * 
 * Inherited but ignored (implemented by QPlainTextEdit, but ignored by QomposeTextEdit):
 *      Ctrl+Shift+Left  Select to beginning of line.
 *      Ctrl+Shift+Right Select to end of line.
 * 	Ctrl+Insert      Copy selected text to clipboard.
 * 	Ctrl+K           Deletes to the end of the line.
 * 	Shift+Insert     Paste clipboard into text edit.
 * 	Shift+Delete     Deletes selected text + copies to clipboard.
 *
 * \param e The event being handled.
 */
void QomposeEditor::keyPressEvent(QKeyEvent *e)
{
	// Process the key event normally.
	
	bool processed = false;
	
	switch(e->modifiers())
	{
		case Qt::NoModifier:
			
			switch(e->key())
			{
				case Qt::Key_Return:
				case Qt::Key_Enter:
					e->accept();
					processed = true;
					doNewline(true);
					break;
				
				case Qt::Key_Tab:
					if(textCursor().hasSelection())
					{
						e->accept();
						processed = true;
						
						increaseSelectionIndent();
					}
					break;
				
				case Qt::Key_Home:
					e->accept();
					processed = true;
					doMoveHome();
					break;
			};
			
			break;
		
		case Qt::ShiftModifier:
			
			switch(e->key())
			{
				// Ignore our superclass's Shift+Insert hotkey.
				case Qt::Key_Insert:
					e->ignore();
					processed = true;
					break;
				
				// Ignore our superclass's Shift+Delete hotkey.
				case Qt::Key_Delete:
					e->ignore();
					processed = true;
					break;
				
				case Qt::Key_Backtab:
				case Qt::Key_Tab:
					if(textCursor().hasSelection())
					{
						e->accept();
						processed = true;
						
						decreaseSelectionIndent();
					}
					break;
				
				case Qt::Key_Home:
					e->accept();
					processed = true;
					doMoveHome(false);
					break;
				
				case Qt::Key_Return:
				case Qt::Key_Enter:
					e->accept();
					processed = true;
					doNewline();
					break;
			};
			
			break;
		
		case Qt::ControlModifier:
			
			switch(e->key())
			{
				// Ignore our superclass's Control+Insert hotkey.
				case Qt::Key_Insert:
					e->ignore();
					processed = true;
					break;
				
				// Ignore our superclass's Control+K hotkey.
				case Qt::Key_K:
					e->ignore();
					processed = true;
					break;
				
				// CTRL+(Zero) resets our text zoom to 100%.
				case Qt::Key_0:
					e->accept();
					processed = true;
					setFontZoom(0);
					break;
			};
			
			break;
	};
	
	if(!processed)
		QPlainTextEdit::keyPressEvent(e);
}

/*!
 * We handle mouse wheel events to implement text zooming.
 *
 * \param e The event being handled.
 */
void QomposeEditor::wheelEvent(QWheelEvent *e)
{
	if(e->modifiers() == Qt::ControlModifier)
	{
		// If the user does Ctrl+Wheel, zoom in/out on our text.
		
		qreal scale = static_cast<qreal>(e->delta()) / 8.0;	// Delta is degrees turned * 8.
		scale *= 0.8333; 					// Scale by 100% for each 120 degrees.
		
		setFontZoom(currentFontZoom + qRound(scale));
	}
	else
	{
		// Otherwise, just perform our parent class's default action.
		
		QPlainTextEdit::wheelEvent(e);
	}

}

/*!
 * This function handles any "mouse release" events on our editor by re-highlighting
 * the current line. Ths is necessary to avoid a bug where rapidly clicking/arrow-key-ing
 * can result in the wrong line being highlighted - cursorPositionChanged is somewhat
 * unreliable.
 *
 * \param e The event being handled.
 */
void QomposeEditor::mouseReleaseEvent(QMouseEvent *e)
{
	highlightCurrentLine();
	
	// Do our superclass's normal mouse action.
	
	QPlainTextEdit::mouseReleaseEvent(e);
}

/*!
 * This function handles a paint event passed up to us by our gutter by rendering the gutter
 * according to our editor's current state.
 *
 * \param e The paint event being handled.
 */
void QomposeEditor::gutterPaintEvent(QPaintEvent *e)
{
	QPainter painter(gutter);
	
	// Paint our background.
	painter.fillRect(e->rect(), gutterBackground);
	
	// Paint our line numbers.
	
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();
	
	while(block.isValid() && top <= e->rect().bottom())
	{
		if(block.isVisible() && bottom >= e->rect().top())
		{
			QString number = QString::number(blockNumber + 1);
			painter.setPen(gutterForeground);
			painter.drawText(0, top, gutter->width(), fontMetrics().height(), Qt::AlignCenter, number);
		}
		
		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		blockNumber++;
	}
}

/*!
 * This function computes the width our gutter should have, based upon the number of lines
 * in our document. We ensure that we have enough space to render our line numbers inside
 * the gutter.
 *
 * \return The width of our gutter.
 */
int QomposeEditor::gutterWidth()
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
 * This function inserts a new line at the current text cursor position, optionally
 * preserving the current line's leading whitespace (i.e., indent).
 *
 * \param preserveIndent Whether or not the current line's indent should be preserved.
 */
void QomposeEditor::doNewline(bool preserveIndent)
{
	// Compute the text to insert, optionally preserving indent.
	
	QString insert = "\n";
	
	if(preserveIndent)
	{
		QTextCursor l = textCursor();
		
		if(l.hasSelection())
			l.setPosition(l.selectionStart(), QTextCursor::MoveAnchor);
		
		l.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);

		QString line = l.selectedText();
		
		line.replace(QRegExp("^([ \\t]*)\\S.*$", Qt::CaseSensitive, QRegExp::RegExp2), "\\1");
		insert.append(line);
	}
	
	// Insert that text, and set our text cursor.
	
	QTextCursor curs = textCursor();
	curs.insertText(insert);
	setTextCursor(curs);
}

/*!
 * This function moves the current cursor as if the "Home" key was pressed. If the text
 * between the current position and the start-of-line contains non-whitespace characters,
 * then we just move to the beginning of the leading whitespace. Otherwise, we move to the
 * start-of-line.
 *
 * When we do this, we will optionally keep the anchor (selecting the characters we move
 * past).
 *
 * \param moveAnchor Whether or not the anchor should be moved (i.e., selection discarded).
 */
void QomposeEditor::doMoveHome(bool moveAnchor)
{
	// Get the characters between the current cursor and the start-of-line.
	
	QString line, trimmed;
	
	{
		QTextCursor l = textCursor();
		
		if(l.hasSelection())
			l.setPosition(l.selectionStart(), QTextCursor::MoveAnchor);
		
		l.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
		
		line = l.selectedText();
	}
	
	// Move the cursor to its appropriate location.
	
	trimmed = line;
	trimmed.replace(QRegExp("^([ \\t]*)\\S.*$", Qt::CaseSensitive, QRegExp::RegExp2), "\\1");
	
	if(trimmed == line)
	{
		// There is only whitespace in front of us - move to the start-of-line.
		
		QTextCursor curs = textCursor();
		
		if(moveAnchor)
			curs.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
		else
			curs.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
		
		setTextCursor(curs);
	}
	else
	{
		// There is non-whitespace in front of us - move to the end of the whitespace.
		
		QTextCursor curs = textCursor();
		int places = line.length() - trimmed.length();
		
		if(moveAnchor)
			curs.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, places);
		else
			curs.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, places);
		
		setTextCursor(curs);
	}
}

/*!
 * This function executes a given find query, using the given predefined direction. Note
 * that we do not inspect the direction properties of the given query - it is assumed that
 * our caller has resolved these into a single direction, based upon whether this is a
 * "find next" or a "find previous" operation, and whether or not the query is reversed.
 *
 * \param f True means we should go forward, false means backward.
 * \param q The query to execute.
 * \return The results of executing this find query.
 */
QomposeEditor::FindResult QomposeEditor::doFind(bool f, const QomposeFindQuery *q)
{
	// Prepare our find flags.
	
	QTextDocument::FindFlags flags = q->getFindFlags();
	
	if(!f)
		flags |= QTextDocument::FindBackward;
	
	// Prepare our cursors.
	
	QTextCursor current = textCursor();
	
	QTextCursor restart = textCursor();
	
	if(f)
		restart.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
	else
		restart.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
	
	// Execute the find query, based on type.
	
	if(q->isRegularExpression())
	{
		QRegExp regex(q->getExpression(), Qt::CaseSensitive, QRegExp::RegExp2);
		
		if(!regex.isValid())
			return BadRegularExpression;
		
		QTextCursor found = document()->find(regex, current, flags);
		
		if(found.isNull())
		{
			if(q->isWrapping())
			{
				found = document()->find(regex, restart, flags);
			}
		}
		
		if(!found.isNull())
		{
			setTextCursor(found);
			return Found;
		}
	}
	else
	{
		QString expression = q->getExpression();
		
		QTextCursor found = document()->find(expression, current, flags);
		
		if(found.isNull())
		{
			if(q->isWrapping())
			{
				found = document()->find(expression, restart, flags);
			}
		}
		
		if(!found.isNull())
		{
			setTextCursor(found);
			return Found;
		}
	}
	
	return NoMatches;
}

/*!
 * This is a utility function which will replace ever match of the given replace
 * query from the given start position until the given end position.
 *
 * If the start position is omitted, then we will simply use the starting position
 * from the current cursor (the smaller of its anchor and position).
 *
 * If the end position is omitted, then we will simply continue until the end of
 * the document.
 *
 * \param q The replace query to execute.
 * \param start The cursor position to start searching from.
 * \param end The cursor position to stop searching at.
 * \return The result of this replacement's find operation.
 */
QomposeEditor::FindResult QomposeEditor::doBatchReplace(
	const QomposeReplaceQuery *q, int start, int end)
{
	// If we weren't given a start position, use the current cursor.
	
	if(start < 0)
	{
		QTextCursor curs = textCursor();
		start = qMin(curs.anchor(), curs.position());
	}
	
	// Create a new query with "replace in selection"-compatible properties.
	
	QomposeReplaceQuery query;
	
	query.setExpression(q->getExpression());
	query.setReplaceValue(q->getReplaceValue());
	query.setWrapping(false);
	query.setWholeWords(q->isWholeWords());
	query.setCaseSensitive(q->isCaseSensitive());
	query.setReversed(false);
	query.setRegularExpression(q->isRegularExpression());
	
	q = NULL;
	
	// Move our cursor to our start position.
	
	QTextCursor curs = textCursor();
	curs.setPosition(start, QTextCursor::MoveAnchor);
	setTextCursor(curs);
	
	// Replace each match we find after our start position.
	
	curs = textCursor();
	QomposeEditor::FindResult r = findNext(&query);
	bool found = false;
	
	curs.beginEditBlock();
	
	while(r == QomposeEditor::Found)
	{
		// Make sure this match is good to go.
		
		int finda = textCursor().anchor(), findp = textCursor().position();
		
		if(end >= 0)
			if(qMax(finda, findp) > end)
				break;
		
		curs.setPosition(finda, QTextCursor::MoveAnchor);
		curs.setPosition(findp, QTextCursor::KeepAnchor);
		
		found = true;
		
		// Replace this match.
		
		int anchor = qMin(curs.anchor(), curs.position());
		
		curs.insertText(query.getReplaceValue());
		
		curs.setPosition(anchor, QTextCursor::MoveAnchor);
		curs.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,
			query.getReplaceValue().length());
		
		// Find the next match!
		
		r = findNext(&query);
	}
	
	curs.endEditBlock();
	
	setTextCursor(curs);
	
	// Return an appropriate find result.
	
	if(found)
		return QomposeEditor::Found;
	else
		return r;
}

/*!
 * This slot duplicates the current line. The new line is inserted below the current
 * line, without altering our cursor position. Note that this entire operation is a single
 * "edit block," for undo/redo operations.
 */
void QomposeEditor::duplicateLine()
{ /* SLOT */
	
	// Save our cursor's initial state.
	
	QTextCursor curs = textCursor();
	
	int cpos = curs.position();
	int apos = curs.anchor();
	
	curs.beginEditBlock();
	
	// Get into position, and copy this block (line).
	
	curs.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
	QString line = curs.block().text();
	
	// Duplicate the line.
	
	curs.insertText("\n");
	curs.insertText(line);
	
	// End our edit block, and reset our cursor position.
	
	curs.endEditBlock();
	
	curs.setPosition(apos, QTextCursor::MoveAnchor);
	curs.setPosition(cpos, QTextCursor::KeepAnchor);
	
	// Done.
	
	setTextCursor(curs);
	
}

/*!
 * This slot clears any selection our editor may have. This is done by simply
 * discarding the "anchor" portion of the current cursor, leaving its actual
 * "position" where it was.
 */
void QomposeEditor::deselect()
{ /* SLOT */
	
	QTextCursor curs = textCursor();
	curs.setPosition(curs.position(), QTextCursor::MoveAnchor);
	setTextCursor(curs);
	
}

/*!
 * This slot increases the indent of all the lines in the current selection.
 * Partially selected lines are included. This function will insert a single tab
 * character at the beginning of each included line.
 *
 * The resulting selection will have an anchor at the beginning of the first line,
 * and a cursor position at the end of the last line.
 *
 * Also note that this operation is done in a single "edit block," for undo/redo
 * actions.
 */
void QomposeEditor::increaseSelectionIndent()
{ /* SLOT */
	
	QTextCursor curs = textCursor();
	
	// Do nothing if we don't have a selection.
	
	if(!curs.hasSelection())
		return;
	
	// Get the first and count of lines to indent.
	
	int spos = curs.anchor(), epos = curs.position();
	
	if(spos > epos)
	{
		spos = spos ^ epos;
		epos = spos ^ epos;
		spos = spos ^ epos;
	}
	
	curs.setPosition(spos, QTextCursor::MoveAnchor);
	int sblock = curs.block().blockNumber();
	
	curs.setPosition(epos, QTextCursor::MoveAnchor);
	int eblock = curs.block().blockNumber();
	
	// Do the indent.
	
	curs.setPosition(spos, QTextCursor::MoveAnchor);
	
	curs.beginEditBlock();
	
	for(int i = 0; i <= (eblock - sblock); ++i)
	{
		curs.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
		
		curs.insertText("\t");
		
		curs.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
	}
	
	curs.endEditBlock();
	
	// Set our cursor's selection to span all of the involved lines.
	
	curs.setPosition(spos, QTextCursor::MoveAnchor);
	curs.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
	
	while(curs.block().blockNumber() < eblock)
	{
		curs.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
	}
	
	curs.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
	
	// Done!
	
	setTextCursor(curs);
	
}

/*!
 * This slot decreases the indent of all the lines in the current selection.
 * Partially selected lines are included.
 *
 * This slot considers tabs and sets of spaces the same length as the tab width
 * to be a single "indentation." This function will remove one indentation from
 * each line in the selection. If no indentations are present, then we will instead
 * look for any arbitrary leading spaces to remove.
 *
 * The resulting selection will have an anchor at the beginning of the first line,
 * and a cursor position at the end of the last line.
 *
 * Also note that this operation is done in a single "edit block," for undo/redo
 * actions.
 */
void QomposeEditor::decreaseSelectionIndent()
{ /* SLOT */
	
	QTextCursor curs = textCursor();
	
	// Do nothing if we don't have a selection.
	
	if(!curs.hasSelection())
		return;
	
	// Get the first and count of lines to de-indent.
	
	int spos = curs.anchor(), epos = curs.position();
	
	if(spos > epos)
	{
		spos = spos ^ epos;
		epos = spos ^ epos;
		spos = spos ^ epos;
	}
	
	curs.setPosition(spos, QTextCursor::MoveAnchor);
	int sblock = curs.block().blockNumber();
	int sblockpos = curs.block().position();
	
	curs.setPosition(epos, QTextCursor::MoveAnchor);
	int eblock = curs.block().blockNumber();
	
	// Do the de-indent.
	
	curs.setPosition(spos, QTextCursor::MoveAnchor);
	
	curs.beginEditBlock();
	
	bool foundIndent = false;
	
	for(int i = 0; i <= (eblock - sblock); ++i)
	{
		curs.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
		
		QString text = curs.block().text();
		
		if(text.startsWith("\t"))
		{
			curs.deleteChar();
			foundIndent = true;
		}
		else if(text.startsWith(QString(tabWidthSpaces(), ' ')))
		{
			for(int j = 0; j < tabWidthSpaces(); ++j)
			{
				curs.deleteChar();
				foundIndent = true;
			}
		}
		
		curs.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
	}
	
	if(!foundIndent)
	{
		// If no lines were indented, try to remove any leading whitespace at all.
		
		curs.setPosition(spos, QTextCursor::MoveAnchor);
		
		for(int i = 0; i <= (eblock - sblock); ++i)
		{
			curs.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
			
			for(int j = 0; j < tabWidthSpaces(); ++j)
			{
				QChar c = curs.block().text().at(0);
				
				if(!c.isSpace())
					break;
				
				if( (j > 0) && (c == '\t') )
					break;
				
				curs.deleteChar();
			}
			
			curs.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
		}
	}
	
	curs.endEditBlock();
	
	// Set our cursor's selection to span all of the involved lines.
	
	curs.setPosition(sblockpos, QTextCursor::MoveAnchor);
	
	while(curs.block().blockNumber() < eblock)
	{
		curs.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
	}
	
	curs.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
	
	// Done!
	
	setTextCursor(curs);
	
}

/*!
 * This slot moves to the next match based upon the given find query and the
 * current cursor location.
 *
 * \param q The query to execute.
 * \return The result of the find query's execution.
 */
QomposeEditor::FindResult QomposeEditor::findNext(const QomposeFindQuery *q)
{ /* SLOT */
	
	bool forward = true;
	
	if(q->isReversed())
		forward = false;
	
	return doFind(forward, q);
	
}

/*!
 * This slot moves to the previous find match based upon the given find query
 * and the current cursor location.
 *
 * \param q The query to execute.
 * \return The result of the find query's execution.
 */
QomposeEditor::FindResult QomposeEditor::findPrevious(const QomposeFindQuery *q)
{ /* SLOT */
	
	bool forward = false;
	
	if(q->isReversed())
		forward = true;
	
	return doFind(forward, q);
	
}

/*!
 * This slot performs a single replace operation. We will replace the very next
 * match of the given query with the query's replace value. Note that we will
 * begin searching at the start of the current cursor's selection (or its position,
 * if it has no selection).
 *
 * \param q The replace query to execute.
 * \return The result of this replacement's find operation.
 */
QomposeEditor::FindResult QomposeEditor::replace(const QomposeReplaceQuery *q)
{ /* SLOT */
	
	// Reset our cursor's position.
	
	QTextCursor curs = textCursor();
	
	curs.setPosition(qMin(curs.anchor(), curs.position()),
		QTextCursor::MoveAnchor);
	
	setTextCursor(curs);
	
	// Try to find the next match.
	
	QomposeEditor::FindResult r = findNext(q);
	
	// If we got a match, replace it with our replace value.
	
	if(r == QomposeEditor::Found)
	{
		curs = textCursor();
		
		if(curs.hasSelection())
		{
			curs.beginEditBlock();
			
			int anchor = qMin(curs.anchor(), curs.position());
			
			curs.insertText(q->getReplaceValue());
			
			curs.setPosition(anchor, QTextCursor::MoveAnchor);
			curs.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,
				qMax(q->getReplaceValue().length(), 0));
			
			curs.endEditBlock();
			
			setTextCursor(curs);
		}
	}
	
	// Done!
	
	return r;
	
}

/*!
 * This slot performs a "replace in selection" operation. We will replace every single
 * match of the given query in our editor's current selection. If we do not have any
 * selection, then we will return NoMatches instead.
 *
 * Note that we will return Found if at least one match is replaced, or NoMatches
 * otherwise.
 *
 * \param q The replace query to execute.
 * \return The result of this replacement's find operation.
 */
QomposeEditor::FindResult QomposeEditor::replaceSelection(const QomposeReplaceQuery *q)
{ /* SLOT */
	
	QTextCursor curs = textCursor();
	
	if(!curs.hasSelection())
		return QomposeEditor::NoMatches;
	
	int start = qMin(curs.anchor(), curs.position());
	int end = qMax(curs.anchor(), curs.position());
	
	return doBatchReplace(q, start, end);
	
}

/*!
 * This slot performs a "replace all" operation. We will replace every single match
 * of the given query in our editor's document.
 *
 * Note that we will return Found if at least one match is replaced, or NoMatches
 * otherwise.
 *
 * \param q The replace query to execute.
 * \return The result of this replacement's find operation.
 */
QomposeEditor::FindResult QomposeEditor::replaceAll(const QomposeReplaceQuery *q)
{ /* SLOT */
	
	return doBatchReplace(q, 0);
	
}

/*!
 * This function will move our cursor to the very beginning of the given line number.
 * Note that the resulting cursor will be at the very beginning of the block (i.e., line),
 * and it will have no selection.
 *
 * Line numbers less than 1 will result in the cursor being positioned at the very
 * beginning of the document, and line numbers larger than our document's last line
 * will result in the cursor being positioned at the beginning of the very last line
 * in the document.
 *
 * \param l The destination line number.
 */
void QomposeEditor::goToLine(int l)
{ /* SLOT */
	
	l = qMax(l, 1);
	
	QTextCursor curs = textCursor();
	
	curs.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
	curs.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, qMax(l - 1, 0));
	
	setTextCursor(curs);
	
}

/*!
 * This function handles highlighting the current line in our editor.
 */
void QomposeEditor::highlightCurrentLine()
{ /* SLOT */
	
	QList<QTextEdit::ExtraSelection> es;
	
	// Highlight our current line (only if we are not read only and we have focus).
	
	QTextEdit::ExtraSelection selection;
	
	selection.format.setBackground(currentLineHighlight);
	selection.format.setProperty(QTextFormat::FullWidthSelection, true);
	selection.cursor = textCursor();
	selection.cursor.clearSelection();
	es.append(selection);
	
	setExtraSelections(es);
	
}

/*!
 * This function updates our gutter's width, e.g. when the number of lines in our document changes.
 */
void QomposeEditor::updateGutterWidth()
{ /* SLOT */
	
	setViewportMargins(gutterWidth(), 0, 0, 0);
	
}

/*!
 * This function updates our gutter, e.g. when our editor widget's state is updated, by resetting the
 * gutter's scroll and width.
 *
 * \param r The viewport rect being updated.
 * \param dy The editor's current y scroll offset.
 */
void QomposeEditor::updateGutter(const QRect &r, int dy)
{ /* SLOT */
	
	if(dy)
		gutter->scroll(0, dy);
	else
		gutter->update(0, r.y(), gutter->width(), r.height());
	
	if(r.contains(viewport()->rect()))
		updateGutterWidth();
	
}
