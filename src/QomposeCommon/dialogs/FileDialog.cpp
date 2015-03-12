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

#include <QStringList>
#include <QFileInfo>
#include <QMessageBox>

#include "QomposeCommon/Defines.h"
#include "QomposeCommon/dialogs/EncodingDialog.h"
#include "QomposeCommon/util/Encoding.h"

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
	FileDescriptor desc = {p, encoding_utils::detectTextCodec(p)};

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
