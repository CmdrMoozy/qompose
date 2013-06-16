#ifndef INCLUDE_QOMPOSE_SYNTAX_HIGHLIGHTER_H
#define INCLUDE_QOMPOSE_SYNTAX_HIGHLIGHTER_H

#include <QObject>

class QTextDocument;

class QomposeSyntaxHighlighter : public QObject
{
	Q_OBJECT
	
	public:
		QomposeSyntaxHighlighter(QTextDocument *d, QObject *p = NULL);
		virtual ~QomposeSyntaxHighlighter();
		
		QTextDocument *getDocument() const;
		void setDocument(QTextDocument *d);
		
	private:
		QTextDocument *document;
	
	private slots:
		void doDocumentContentsChange(int p, int r, int a);
};

#endif