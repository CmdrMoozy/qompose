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

#include "QomposeSyntaxHighlighter.h"

#include <QTextDocument>

QomposeSyntaxHighlighter::QomposeSyntaxHighlighter(QTextDocument *d, QObject *p)
	: QObject(p), document(NULL)
{
	setDocument(d);
}

QomposeSyntaxHighlighter::~QomposeSyntaxHighlighter()
{
}

QTextDocument *QomposeSyntaxHighlighter::getDocument() const
{
	return document;
}

void QomposeSyntaxHighlighter::setDocument(QTextDocument *d)
{
	if(document != NULL)
	{
		QObject::disconnect( document, SIGNAL(contentsChange(int, int, int)),
			this, SLOT(doDocumentContentsChange(int, int, int)) );
	}
	
	document = d;
	
	QObject::connect( document, SIGNAL(contentsChange(int, int, int)),
		this, SLOT(doDocumentContentsChange(int, int, int)) );
}

/*!
 * This function handles our document's contents being changed by updating the syntax
 * highlighting for the block that was changed (and possibly all blocks following that
 * block).
 *
 * \param p The position of the change.
 * \param r The number of characters removed.
 * \param a The number of characters added.
 */
void QomposeSyntaxHighlighter::doDocumentContentsChange(int p, int r, int a)
{ /* SLOT */
	
	qDebug("HIGHLIGHTER: Rehighlighting position %d", p);
	
}