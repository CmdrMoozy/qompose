#ifndef INCLUDE_QOMPOSE_TAB_WIDGET_H
#define INCLUDE_QOMPOSE_TAB_WIDGET_H

#include <QWidget>
#include <QMap>

class QGridLayout;
class QTabBar;
class QStackedLayout;

class QomposeBuffer;

class QomposeTabWidget : public QWidget
{
	Q_OBJECT
	
	public:
		QomposeTabWidget(QWidget *p = 0);
		virtual ~QomposeTabWidget();
		
		QomposeBuffer *currentBuffer() const;
		
	private:
		QGridLayout *layout;
		QTabBar *tabBar;
		
		QWidget *tabDisplayWidget;
		QStackedLayout *tabDisplayLayout;
		
		QMap<int, QomposeBuffer *> tabs;
		
		QomposeBuffer *newBuffer();
		
	public slots:
		void doNew();
		void doOpen();
		
	private slots:
		void doTabChanged(int i);
		void doCloseTab(int i);
		
		void doTabTitleChanged(const QString &t);
		void doTabPathChanged(const QString &p);
		
	signals:
		void pathChanged(const QString &);
};

#endif
