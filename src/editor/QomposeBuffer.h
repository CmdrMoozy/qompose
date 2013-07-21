#ifndef INCLUDE_QOMPOSE_BUFFER_H
#define INCLUDE_QOMPOSE_BUFFER_H

#include "editor/QomposeEditor.h"

#include "dialogs/QomposeFileDialog.h"

class QomposeBuffer : public QomposeEditor
{
	Q_OBJECT
	
	public:
		QomposeBuffer(QWidget *p = NULL);
		virtual ~QomposeBuffer();
		
		void open(const QomposeFileDescriptor &f);
		
		QString getTitle() const;
		QString getPath() const;
		
		bool hasBeenSaved() const;
		bool isModified() const;
		
	signals:
		void titleChanged(const QString &);
		void pathChanged(const QString &);
};

#endif