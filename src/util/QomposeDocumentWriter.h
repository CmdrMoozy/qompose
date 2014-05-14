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

#ifndef INCLUDE_QOMPOSE_DOCUMENT_WRITER_H
#define INCLUDE_QOMPOSE_DOCUMENT_WRITER_H

#include <QTextStream>
#include <QString>

class QIODevice;
class QTextCodec;
class QTextDocument;

/*!
 * \brief This class encapsulates code to write QDocuments to QIODevices.
 */
class QomposeDocumentWriter
{
	public:
		QomposeDocumentWriter();
		QomposeDocumentWriter(QIODevice *d);
		virtual ~QomposeDocumentWriter();

		QIODevice *getDevice() const;
		void setDevice(QIODevice *d);

		QTextCodec *getCodec() const;
		void setCodec(QTextCodec *codec);

		bool isWhitespaceTrimmed() const;
		void setWhitespaceTrimmed(bool w);

		bool write(const QTextDocument *d);

	private:
		bool whitespaceTrimmed;

		QTextStream stream;

		QString trimWhitespace(const QString &s) const;
};

#endif