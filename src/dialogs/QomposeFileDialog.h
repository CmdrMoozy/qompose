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

#ifndef INCLUDE_QOMPOSE_FILE_DIALOG_H
#define INCLUDE_QOMPOSE_FILE_DIALOG_H

#include <QFileDialog>
#include <QString>
#include <QList>

/*!
 * \brief This structure stores a filename and its detected character encoding.
 */
typedef struct QomposeFileDescriptor
{
	QString fileName;
	QString textCodec;
} QomposeFileDescriptor;

/*!
 * \brief This class extends QFileDialog with character encoding detection.
 */
class QomposeFileDialog : public QFileDialog
{
	public:
		QomposeFileDialog(QWidget *p, Qt::WindowFlags f);
		virtual ~QomposeFileDialog();

		static QomposeFileDescriptor getNullDescriptor();
		static QomposeFileDescriptor getPathDescriptor(const QString &p);

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
