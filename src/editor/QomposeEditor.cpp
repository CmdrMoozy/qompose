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
	: QPlainTextEdit(p)
{
	gutter = new QomposeGutter(this);
	
	QObject::connect( this, SIGNAL( blockCountChanged(int)            ), this, SLOT( updateGutterWidth()              ) );
	QObject::connect( this, SIGNAL( updateRequest(const QRect &, int) ), this, SLOT( updateGutter(const QRect &, int) ) );
	QObject::connect( this, SIGNAL( cursorPositionChanged()           ), this, SLOT( highlightCurrentLine()           ) );
	
	setTabStopWidth(fontMetrics().width(QLatin1Char(' ')) * 8);
	
	setLineWrapMode(QPlainTextEdit::NoWrap);
	setFocusPolicy(Qt::ClickFocus);
	
	setGutterVisible(true);
	
	highlightCurrentLine();
	
	setFont(QFont("Droid Sans Mono", 10));
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
	currentFont = f;
	
	// Make sure we aren't given a pixel-sized font.
	
	if(currentFont.pointSize() == -1)
		currentFont.setPointSize(10);
	
	originalFontSize = currentFont.pointSizeF();
	
	// Set our font!
	
	QPlainTextEdit::setFont(currentFont);
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
	
	currentFont.setPointSizeF(fsize);
	QPlainTextEdit::setFont(currentFont);
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
 * 	Return          Move to a new line, maintaining indent.
 * 	Enter           Move to a new line, maintaining indent.
 * 	Tab             Increase indent on selection.
 * 	Shift+Backtab   Decrease indent on selection.
 *      Shift+Tab       Decrease indent on selection.
 * 	Home            Move to start of non-indent, then to start of line.
 * 	Ctrl+Wheel      Zoom text in and out.
 *      Ctrl+D          Duplicate the current line.
 *      Shift+Return    The same as Return.
 *      Shift+Ender     The same as Enter.
 *      Ctrl+(Zero)     Reset text zoom to default level.
 * 
 * Inherited (implemented by QPlainTextEdit):
 * 	Backspace       Deletes the character to the left of the cursor.
 * 	Delete          Deletes the character to the right of the cursor.
 * 	Ctrl+C          Copy selected text to clipboard.
 * 	Ctrl+V          Paste clipboard into text edit.
 * 	Ctrl+X          Deletes selected text + copies to clipboard.
 * 	Ctrl+Z          Undo.
 * 	Ctrl+Y          Redo.
 * 	Left            Moves the cursor one character left.
 * 	Ctrl+Left       Moves the cursor one word left.
 * 	Right           Moves the cursor one character right.
 * 	Ctrl+Right      Moves the cursor one word right.
 * 	Up              Moves the cursor one line up.
 * 	Down            Moves the cursor one line down.
 * 	PageUp          Moves the cursor one page up.
 * 	PageDown        Moves the cursor one page down.
 * 	Ctrl+Home       Moves the cursor to the beginning of the document.
 * 	End             Moves the cursor to the end of the line.
 * 	Ctrl+End        Moves the cursor to the end of the document.
 * 	Alt+Wheel       Scrolls the page horizontally.
 * 
 * Inherited but ignored (implemented by QPlainTextEdit, but ignored by QomposeTextEdit):
 * 	Ctrl+Insert     Copy selected text to clipboard.
 * 	Ctrl+K          Deletes to the end of the line.
 * 	Shift+Insert    Paste clipboard into text edit.
 * 	Shift+Delete    Deletes selected text + copies to clipboard.
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
					qDebug("HOTKEY: Indent (selection).");
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
					qDebug("HOTKEY: De-indent (selection).");
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
				
				case Qt::Key_D:
					qDebug("HOTKEY: Duplicate current line.");
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

void QomposeEditor::mouseReleaseEvent(QMouseEvent *e)
{
	/*
	 * This is necessary to prevent a state where rapid clicking/arrow-key-ing can
	 * result in the wrong line being highlighted - cursorPositionChanged is somewhat
	 * unreliable.
	 */
	
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
	
	QColor background(0, 0, 0), foreground(255, 255, 255);
	
	// Paint our background.
	painter.fillRect(e->rect(), Qt::black);
	
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
			painter.setPen(Qt::white);
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

/*!
 * This function handles highlighting the current line in our editor.
 */
void QomposeEditor::highlightCurrentLine()
{ /* SLOT */
	
	if( hasFocus() && (!isReadOnly()) )
	{
		QList<QTextEdit::ExtraSelection> es;
		
		QColor background(225, 225, 225);
		
		// Highlight our current line (only if we are not read only and we have focus).
		
		QTextEdit::ExtraSelection selection;
		
		selection.format.setBackground(background);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		es.append(selection);
		
		setExtraSelections(es);
	}
	
}
