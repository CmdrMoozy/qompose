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

#include "QomposeTypes.h"

/*!
 * \brief This class extends QFileDialog with character encoding detection.
 */
class QomposeFileDialog : public QFileDialog
{
	public:
		QomposeFileDialog(QWidget *, Qt::WindowFlags);
		virtual ~QomposeFileDialog();

		static QomposeFileDescriptor getNullDescriptor();
		static QomposeFileDescriptor
			getPathDescriptor(const QString &);

		static QomposeFileDescriptor getOpenFileName(
			QWidget * = nullptr, const QString & = QString(),
			const QString & = QString(),
			const QString & = QString(), QString * = nullptr,
			QFileDialog::Options = nullptr);
		static QList<QomposeFileDescriptor> getOpenFileNames(
			QWidget * = nullptr, const QString & = QString(),
			const QString & = QString(),
			const QString & = QString(), QString * = nullptr,
			QFileDialog::Options = nullptr);

	private:
		static bool fileIsGood(const QString &, QWidget * = nullptr);
		static QString detectTextCodec(const QString &);
		static QString promptTextCodec(const QString &);
};

#endif
