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

/*!
 * This function initializes a new editor widget, using the given parent widget.
 *
 * \param p Our parent widget. 
 */
QomposeEditor::QomposeEditor(QWidget *p)
	: QomposeDecoratedTextEdit(p)
{
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
QomposeEditor::~QomposeEditor()
{
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
	
	if( (e->key() == Qt::Key_Return) || (e->key() == Qt::Key_Enter) )
	{
		e->accept();
		doNewline(true);
		return;
	}
	
	switch(e->modifiers())
	{
		case Qt::NoModifier:
			
			switch(e->key())
			{
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
				
				case Qt::Key_Z:
					e->accept();
					processed = true;
					undo();
					break;
					
				case Qt::Key_Y:
					e->accept();
					processed = true;
					redo();
					break;
			};
			
			break;
	};
	
	if(!processed)
		QomposeDecoratedTextEdit::keyPressEvent(e);
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
		
		line.replace(QRegExp("^([ \\t]*)\\S.*$", Qt::CaseSensitive,
			QRegExp::RegExp2), "\\1");
		
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
	
	trimmed.replace(QRegExp("^([ \\t]*)\\S.*$", Qt::CaseSensitive,
		QRegExp::RegExp2), "\\1");
	
	if(trimmed == line)
	{
		// There is only whitespace in front of us - move to the start-of-line.
		
		QTextCursor curs = textCursor();
		
		if(moveAnchor)
		{
			curs.movePosition(QTextCursor::StartOfLine,
				QTextCursor::MoveAnchor);
		}
		else
		{
			curs.movePosition(QTextCursor::StartOfLine,
				QTextCursor::KeepAnchor);
		}
		
		setTextCursor(curs);
	}
	else
	{
		// There is non-whitespace in front - move to the end of the whitespace.
		
		QTextCursor curs = textCursor();
		int places = line.length() - trimmed.length();
		
		if(moveAnchor)
		{
			curs.movePosition(QTextCursor::Left,
				QTextCursor::MoveAnchor, places);
		}
		else
		{
			curs.movePosition(QTextCursor::Left,
				QTextCursor::KeepAnchor, places);
		}
		
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

void QomposeEditor::undo()
{ /* SLOT */
	
	QomposeDecoratedTextEdit::undo();
	
}

void QomposeEditor::redo()
{ /* SLOT */
	
	QomposeDecoratedTextEdit::redo();
	
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
		int hold = spos;
		spos = epos;
		epos = hold;
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
			curs.movePosition(QTextCursor::StartOfBlock,
				QTextCursor::MoveAnchor);
			
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
			int length = qMax(q->getReplaceValue().length(), 0);
			
			curs.insertText(q->getReplaceValue());
			
			curs.setPosition(anchor, QTextCursor::MoveAnchor);
			curs.movePosition(QTextCursor::NextCharacter,
				QTextCursor::KeepAnchor, length);
			
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
