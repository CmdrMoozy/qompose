#ifndef INCLUDE_QOMPOSE_FILE_DIALOG_H
#define INCLUDE_QOMPOSE_FILE_DIALOG_H

#include <QFileDialog>
#include <QString>
#include <QList>

typedef struct QomposeFileDescriptor
{
	QString fileName;
	QString textCodec;
} QomposeFileDescriptor;

class QomposeFileDialog : public QFileDialog
{
	public:
		QomposeFileDialog(QWidget *p, Qt::WindowFlags f);
		virtual ~QomposeFileDialog();
		
		static QomposeFileDescriptor getNullDescriptor();
		
		static QomposeFileDescriptor getOpenFileName(QWidget *p = 0,
			const QString &c = QString(), const QString &d = QString(),
			const QString &f = QString(), QString *sf = 0, QFileDialog::Options o = 0);
		static QList<QomposeFileDescriptor> getOpenFileNames(QWidget *p = 0,
			const QString &c = QString(), const QString &d = QString(),
			const QString &f = QString(), QString *sf = 0, QFileDialog::Options o = 0);
		
	private:
		static bool fileIsGood(const QString &f, QWidget *p = 0);
		static QString detectTextCodec(const QString &f);
		static QString promptTextCodec(const QString &f);
};

#endif
