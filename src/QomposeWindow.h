#ifndef INCLUDE_QOMPOSE_WINDOW_H
#define INCLUDE_QOMPOSE_WINDOW_H

#include <QMainWindow>
#include <QIcon>

class QMenu;
class QAction;

class QomposeBuffer;

class QomposeWindow : public QMainWindow
{
	Q_OBJECT
	
	public:
		QomposeWindow(QWidget *p = NULL, Qt::WindowFlags f = 0);
		virtual ~QomposeWindow();
		
	private:
		QMenu *fileMenu;
		QMenu *editMenu;
		QMenu *searchMenu;
		QMenu *buffersMenu;
		QMenu *helpMenu;
		
		QAction *newAction;
		QAction *openAction;
		QAction *revertAction;
		QAction *saveAction;
		QAction *saveAsAction;
		QAction *printAction;
		QAction *printPreviewAction;
		QAction *closeAction;
		QAction *exitAction;
		
		QAction *undoAction;
		QAction *redoAction;
		QAction *cutAction;
		QAction *copyAction;
		QAction *pasteAction;
		QAction *duplicateLineAction;
		QAction *selectAllAction;
		QAction *deselectAction;
		QAction *increaseIndentAction;
		QAction *decreaseIndentAction;
		QAction *preferencesAction;
		
		QAction *findAction;
		QAction *findNextAction;
		QAction *findPreviousAction;
		QAction *replaceAction;
		QAction *goToAction;
		
		QAction *previousBufferAction;
		QAction *nextBufferAction;
		QAction *moveBufferLeftAction;
		QAction *moveBufferRightAction;
		
		QAction *aboutQomposeAction;
		QAction *aboutQtAction;
		
		QomposeBuffer *editor;
		
		void initializeActions();
		void initializeMenus();
		
		QIcon getIconFromTheme(const QString &n) const;
		
	private slots:
		void doNew(bool c);
		void doOpen(bool c);
		void doRevert(bool c);
		void doSave(bool c);
		void doSaveAs(bool c);
		void doPrint(bool c);
		void doPrintPreview(bool c);
		void doClose(bool c);
		void doExit(bool c);
		void doUndo(bool c);
		void doRedo(bool c);
		void doCut(bool c);
		void doCopy(bool c);
		void doPaste(bool c);
		void doDuplicateLine(bool c);
		void doSelectAll(bool c);
		void doDeselect(bool c);
		void doIncreaseIndent(bool c);
		void doDecreaseIndent(bool c);
		void doPreferences(bool c);
		void doFind(bool c);
		void doFindNext(bool c);
		void doFindPrevious(bool c);
		void doReplace(bool c);
		void doGoTo(bool c);
		void doPreviousBuffer(bool c);
		void doNextBuffer(bool c);
		void doMoveBufferLeft(bool c);
		void doMoveBufferRight(bool c);
		void doAboutQompose(bool c);
		void doAboutQt(bool c);
};

#endif
