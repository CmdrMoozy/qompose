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
FileDialog::FileDialog(QWidget *p, Qt::WindowFlags f) : QFileDialog(p, f)
{
}

FileDescriptor FileDialog::getNullDescriptor()
{
	FileDescriptor desc = {QString(), QString()};

	return desc;
}

FileDescriptor FileDialog::getPathDescriptor(const QString &p)
{
	FileDescriptor desc = {p, FileDialog::detectTextCodec(p)};

	if(desc.textCodec.isNull())
	{
		desc.textCodec = FileDialog::promptTextCodec(p);

		if(desc.textCodec.isNull())
			return FileDialog::getNullDescriptor();
	}

	return desc;
}

FileDescriptor FileDialog::getOpenFileName(QWidget *p, const QString &c,
                                           const QString &d, const QString &f,
                                           QString *sf, QFileDialog::Options o)
{
	QString fileName = QFileDialog::getOpenFileName(p, c, d, f, sf, o);

	if(fileName.isNull())
		return FileDialog::getNullDescriptor();

	if(!FileDialog::fileIsGood(fileName, p))
		return FileDialog::getNullDescriptor();

	return FileDialog::getPathDescriptor(fileName);
}

QList<FileDescriptor> FileDialog::getOpenFileNames(QWidget *p, const QString &c,
                                                   const QString &d,
                                                   const QString &f,
                                                   QString *sf,
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

bool FileDialog::fileIsGood(const QString &f, QWidget *p)
{
	QFileInfo file(f);

	if((!file.exists()) || (!file.isReadable()))
		return false;

	if(file.size() > (5 * QMEGABYTE))
	{
		QString text =
		        QString("The file '%1' is relatively large. Do you "
		                "really want to open it?").arg(file.fileName());

		QMessageBox::StandardButton result = QMessageBox::question(
		        p, "Qompose - Large File", text,
		        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

		if(result != QMessageBox::Yes)
			return false;
	}

	return true;
}

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

	const char *csn = ucsdet_getName(cs, &err);

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

QString FileDialog::promptTextCodec(const QString &f)
{
	QFileInfo file(f);

	QString message =
	        QString("The character encoding for '%1' couldn't be "
	                "auto-detected. "
	                "Which character encoding should be used to open it?")
	                .arg(file.fileName());

	return EncodingDialog::promptEncoding(nullptr, "UTF-8", message);
}
}
