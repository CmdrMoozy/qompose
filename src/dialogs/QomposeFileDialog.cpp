#include "QomposeFileDialog.h"

#include <cstdlib>
#include <cstdio>

#include <QStringList>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextCodec>
#include <QByteArray>

#include <unicode/ucsdet.h>

#include "dialogs/QomposeEncodingDialog.h"

QomposeFileDialog::QomposeFileDialog(QWidget *p, Qt::WindowFlags f)
	: QFileDialog(p, f)
{
}

QomposeFileDialog::~QomposeFileDialog()
{
}

QomposeFileDescriptor QomposeFileDialog::getNullDescriptor()
{
	QomposeFileDescriptor desc;
	
	desc.fileName = QString();
	desc.textCodec = QString();
	
	return desc;
}

QomposeFileDescriptor QomposeFileDialog::getOpenFileName(QWidget *p,
	const QString &c, const QString &d, const QString &f, QString *sf,
	QFileDialog::Options o)
{
	QString fileName = QFileDialog::getOpenFileName(p, c, d, f, sf, o);
	
	if(fileName.isNull())
		return QomposeFileDialog::getNullDescriptor();
	
	if(!QomposeFileDialog::fileIsGood(fileName, p))
		return QomposeFileDialog::getNullDescriptor();
	
	QomposeFileDescriptor desc;
	desc.fileName = fileName;
	desc.textCodec = QomposeFileDialog::detectTextCodec(fileName);
	
	if(desc.textCodec.isNull())
	{
		desc.textCodec = QomposeFileDialog::promptTextCodec(fileName);
		
		if(desc.textCodec.isNull())
			return QomposeFileDialog::getNullDescriptor();
	}
	
	return desc;
}

QList<QomposeFileDescriptor> QomposeFileDialog::getOpenFileNames(QWidget *p,
	const QString &c, const QString &d, const QString &f, QString *sf,
	QFileDialog::Options o)
{
	QStringList files = QFileDialog::getOpenFileNames(p, c, d, f, sf, o);
	
	QList<QomposeFileDescriptor> ret;
	
	QStringList list = files;
	for(QStringList::Iterator it = list.begin(); it != list.end(); ++it)
	{
		if(!QomposeFileDialog::fileIsGood(*it, p))
			continue;
		
		QomposeFileDescriptor desc;
		desc.fileName = (*it);
		desc.textCodec = QomposeFileDialog::detectTextCodec(*it);
		
		if(desc.textCodec.isNull())
		{
			desc.textCodec = QomposeFileDialog::promptTextCodec(*it);
			
			if(desc.textCodec.isNull())
				continue;
		}
		
		ret.append(desc);
	}
	
	return ret;
}

bool QomposeFileDialog::fileIsGood(const QString &f, QWidget *p)
{
	QFileInfo file(f);
	
	if( (!file.exists()) || (!file.isReadable()) )
		return false;
	
	if(file.size() > 5242880)
	{
		QString text = QString("The file '%1' is relatively large. Do you really want to open it?")
			.arg(file.fileName());
		
		QMessageBox::StandardButton result = QMessageBox::question(p,
			"Qompose - Large File", text, QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No);
		
		if(result != QMessageBox::Yes)
			return false;
	}
	
	return true;
}

/*!
 * This function attempts to detet the character encoding in the specified file.
 * The character encoding will be returned as a string which can be used with
 * QTextCodec's codecForName function.
 *
 * If the character encoding cannot be determined, or if some other error occurs,
 * then we will return a null QString instead.
 *
 * \param f The path to the file whose encoding will be detected.
 * \return The encoding the given file seems to be using.
 */
QString QomposeFileDialog::detectTextCodec(const QString &f)
{
	// Create our charset detector instance.
	
	UErrorCode err = U_ZERO_ERROR;
	UCharsetDetector *detector = ucsdet_open(&err);
	
	if(U_FAILURE(err))
		return QString();
	
	// Read 1 KiB from our file for testing.
	
	FILE *fs = fopen(f.toStdString().c_str(), "r");
	char *buf = (char *) malloc(sizeof(char) * 1024);
	
	size_t read = fread(buf, sizeof(char), 1024, fs);
	
	fclose(fs);
	
	// Try to detect the charset.
	
	ucsdet_setText(detector, buf, read, &err);
	
	if(U_FAILURE(err))
	{
		free(buf);
		ucsdet_close(detector);
		
		return QString();
	}
	
	const UCharsetMatch *cs = ucsdet_detect(detector, &err);
	
	if(U_FAILURE(err) || (cs == NULL))
	{
		free(buf);
		ucsdet_close(detector);
		
		return QString();
	}
	
	// Get the match confidence & charset name.
	
	int32_t confidence = ucsdet_getConfidence(cs, &err);
	
	if(U_FAILURE(err))
	{
		free(buf);
		ucsdet_close(detector);
		
		return QString();
	}
	
	const char *csn  = ucsdet_getName(cs, &err);
	
	if(U_FAILURE(err))
	{
		free(buf);
		ucsdet_close(detector);
		
		return QString();
	}
	
	QString name(csn);
	
	free(buf);
	ucsdet_close(detector);
	
	// Assuming we're confident enough, return the match.
	
	if(confidence >= 10)
	{
		// Make sure the codec we detected is a valid QTextCodec.
		
		QList<QByteArray> codecs = QTextCodec::availableCodecs();
		
		for(int i = 0; i < codecs.size(); ++i)
		{
			if(QString(codecs.at(i)) == name)
			{
				return name;
			}
		}
		
		return QString();
	}
	else
	{
		return QString();
	}
}

QString QomposeFileDialog::promptTextCodec(const QString &f)
{
	QFileInfo file(f);
	
	QString message = QString("The character encoding for '%1' couldn't be auto-detected. "
		"Which character encoding should be used to open it?").arg(file.fileName());
	
	return QomposeEncodingDialog::promptEncoding(0, "UTF-8", message);
}
