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

#include "FileDialog.h"

#include <cstddef>
#include <fstream>
#include <memory>

#include <QStringList>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextCodec>
#include <QByteArray>

#include <unicode/ucsdet.h>

#include "QomposeCommon/Defines.h"
#include "QomposeCommon/dialogs/EncodingDialog.h"

namespace qompose
{

/*!
 * This is our default constructor, which creates a new file dialog instance.
 *
 * \param p The parent widget for our dialog.
 * \param f The window flags to use for our dialog.
 */
FileDialog::FileDialog(QWidget *p, Qt::WindowFlags f)
	: QFileDialog(p, f)
{
}

/*!
 * This is our default destructor, which cleans up & destroys our dialog.
 */
FileDialog::~FileDialog()
{
}

/*!
 * This is a factory function, which creates a new "null" file descriptor and
 * returns it. "Null" file descriptors are those with the value QString() for
 * both properties.
 *
 * \return A newly-created null file descriptor.
 */
FileDescriptor FileDialog::getNullDescriptor()
{
	FileDescriptor desc = {
		QString(),
		QString()
	};

	return desc;
}

/*!
 * This function will create a file descriptor for the file at the given path.
 * The file's text codec will be detected or, failing that, the user will be
 * prompted to select the text codec to use.
 *
 * \param p The path to the file to create a descriptor for.
 * \return A file descriptor for the given path.
 */
FileDescriptor FileDialog::getPathDescriptor(const QString &p)
{
	FileDescriptor desc = {
		p,
		FileDialog::detectTextCodec(p)
	};

	if(desc.textCodec.isNull())
	{
		desc.textCodec = FileDialog::promptTextCodec(p);

		if(desc.textCodec.isNull())
			return FileDialog::getNullDescriptor();
	}

	return desc;
}

/*!
 * This is a static utility function which displays an instance of our dialog
 * for the purposes of opening a single file.
 *
 * \param p The parent widget to use for our dialog.
 * \param c The caption to use for the dialog prompt.
 * \param d The default directory to use for the dialog.
 * \param f The filter to use for the open dialog.
 * \param sf The selected filter to use for the open dialog.
 * \param o The set of extra options to use for the open dialog.
 * \return A valid file descriptor on "accept," or a null descriptor otherwise.
 */
FileDescriptor FileDialog::getOpenFileName(QWidget *p,
	const QString &c, const QString &d, const QString &f, QString *sf,
	QFileDialog::Options o)
{
	QString fileName = QFileDialog::getOpenFileName(p, c, d, f, sf, o);

	if(fileName.isNull())
		return FileDialog::getNullDescriptor();

	if(!FileDialog::fileIsGood(fileName, p))
		return FileDialog::getNullDescriptor();

	return FileDialog::getPathDescriptor(fileName);
}

/*!
 * This is a static utility function which displays an instance of our dialog
 * for the purposes of opening one or more files.
 *
 * \param p The parent widget to use for our dialog.
 * \param c The caption to use for the dialog prompt.
 * \param d The default directory to use for the dialog.
 * \param f The filter to use for the open dialog.
 * \param sf The selected filter to use for the open dialog.
 * \param o The set of extra options to use for the open dialog.
 * \return A list of file descriptors on "accept," or an empty list otherwise.
 */
QList<FileDescriptor> FileDialog::getOpenFileNames(QWidget *p,
	const QString &c, const QString &d, const QString &f, QString *sf,
	QFileDialog::Options o)
{
	QStringList files = QFileDialog::getOpenFileNames(p, c, d, f, sf, o);

	QList<FileDescriptor> ret;

	QStringList list = files;
	for(QStringList::Iterator it = list.begin(); it != list.end(); ++it)
	{
		if(!FileDialog::fileIsGood(*it, p))
			continue;

		FileDescriptor desc = FileDialog::getPathDescriptor(*it);

		if(desc.fileName.isNull())
			continue;

		ret.append(desc);
	}

	return ret;
}

/*!
 * This function tests whether or not the given file seems "good to go." This
 * includes whether or not it is a regular file, is readable, and is reasonably
 * sized (files larger than 5 MiB are considered unusually large).
 *
 * \param f The path to the file to inspect.
 * \param p The parent widget for any prompts we display.
 * \return True if the file looks good, or false otherwise.
 */
bool FileDialog::fileIsGood(const QString &f, QWidget *p)
{
	QFileInfo file(f);

	if( (!file.exists()) || (!file.isReadable()) )
		return false;

	if(file.size() > (5 * QMEGABYTE))
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
QString FileDialog::detectTextCodec(const QString &f)
{
	// Create our charset detector instance.

	UErrorCode err = U_ZERO_ERROR;

	std::shared_ptr<UCharsetDetector> detector(ucsdet_open(&err),
		[](UCharsetDetector *p)
		{
			ucsdet_close(p);
		});

	if(U_FAILURE(err))
		return QString();

	// Read 1 KiB from our file for testing.

	std::ifstream fs(f.toStdString().c_str());
	std::shared_ptr<char> buf(new char[1024], [](char *p)
	{
		delete[] p;
	});

	fs.read(buf.get(), 1024);
	size_t read = static_cast<size_t>(fs.gcount());

	fs.close();

	// Try to detect the charset.

	ucsdet_setText(detector.get(), buf.get(), read, &err);

	if(U_FAILURE(err))
		return QString();

	const UCharsetMatch *cs = ucsdet_detect(detector.get(), &err);

	if(U_FAILURE(err) || (cs == NULL))
		return QString();

	// Get the match confidence & charset name.

	int32_t confidence = ucsdet_getConfidence(cs, &err);

	if(U_FAILURE(err))
		return QString();

	const char *csn  = ucsdet_getName(cs, &err);

	if(U_FAILURE(err))
		return QString();

	QString name(csn);

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

/*!
 * This is a utility function which prompts the user to select a character encoding,
 * in the case where the proper encoding couldn't be autodetected for a file.
 *
 * \param f The path to the file to prompt about.
 * \return The user-selected character encoding for the given file.
 */
QString FileDialog::promptTextCodec(const QString &f)
{
	QFileInfo file(f);

	QString message = QString("The character encoding for '%1' couldn't be auto-detected. "
		"Which character encoding should be used to open it?").arg(file.fileName());

	return EncodingDialog::promptEncoding(nullptr, "UTF-8", message);
}

}
