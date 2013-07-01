#ifndef INCLUDE_QOMPOSE_ENCODING_DIALOG_H
#define INCLUDE_QOMPOSE_ENCODING_DIALOG_H

#include <QDialog>
#include <QString>

class QGridLayout;
class QLabel;
class QListWidget;
class QPushButton;

class QomposeEncodingDialog : public QDialog
{
	Q_OBJECT
	
	public:
		QomposeEncodingDialog(QWidget *p = 0, Qt::WindowFlags f = 0,
			const QString &d = "UTF-8",
			const QString &m = tr("Which character encoding should be used?"),
			const QString &t = tr("Select Character Encoding"));
		virtual ~QomposeEncodingDialog();
		
		static QString promptEncoding(QWidget *p = 0, const QString &d = "UTF-8",
			const QString &m = tr("Which character encoding should be used?"),
			const QString &t = tr("Select Character Encoding"));
		
		QString getSelectedEncoding() const;
		
	private:
		QString encoding;
		
		QGridLayout *layout;
		QLabel *messageLabel;
		QListWidget *encodingList;
		
		QWidget *buttonsWidget;
		QGridLayout *buttonsLayout;
		QPushButton *selectButton;
		QPushButton *cancelButton;
		
	private slots:
		void doSelectClicked(bool c);
		void doCancelClicked(bool c);
};

#endif
