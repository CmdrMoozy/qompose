#ifndef INCLUDE_QOMPOSE_EDITOR_H
#define INCLUDE_QOMPOSE_EDITOR_H

#include <QPlainTextEdit>
#include <QSize>

class QFocusEvent;
class QResizeEvent;
class QKeyEvent;
class QWheelEvent;
class QMouseEvent;

class QomposeSyntaxHighlighter;

class QomposeEditor : public QPlainTextEdit
{
	Q_OBJECT
	
	public:
		QomposeEditor(QWidget *p = NULL);
		virtual ~QomposeEditor();
		
		void setGutterVisible(bool v);
		bool isGutterVisible();
		
		virtual QFont font() const;
		virtual void setFont(const QFont &f);
		int fontZoom() const;
		qreal fontZoomSize() const;
		void setFontZoom(int z);
		
	protected:
		virtual void focusInEvent(QFocusEvent *e);
		virtual void focusOutEvent(QFocusEvent *e);
		
		virtual void resizeEvent(QResizeEvent *e);
		virtual void keyPressEvent(QKeyEvent *e);
		virtual void wheelEvent(QWheelEvent *e);
		virtual void mouseReleaseEvent(QMouseEvent *e);
		
	private:
		class QomposeGutter : public QWidget
		{
			public:
				QomposeGutter(QomposeEditor *e);
				virtual ~QomposeGutter();
				
				QSize sizeHint() const;
				
			protected:
				virtual void paintEvent(QPaintEvent *e);
				
			private:
				QomposeEditor *editor;
		};
		
		QomposeGutter *gutter;
		bool gutterVisible;
		
		QFont currentFont;
		qreal originalFontSize;
		int currentFontZoom;
		
		int tabWidth;
		
		QomposeSyntaxHighlighter *highlighter;
		
		void gutterPaintEvent(QPaintEvent *e);
		int gutterWidth();
		
		void doNewline(bool preserveIndent = false);
		void doMoveHome(bool moveAnchor = true);
	
	public slots:
		void highlightCurrentLine();
	
	private slots:
		void updateGutterWidth();
		void updateGutter(const QRect &r, int dy);
		
};

#endif