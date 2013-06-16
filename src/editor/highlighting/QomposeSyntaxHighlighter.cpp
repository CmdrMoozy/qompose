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