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

#ifndef INCLUDE_QOMPOSECOMMON_UTIL_DOCUMENT_WRITER_H
#define INCLUDE_QOMPOSECOMMON_UTIL_DOCUMENT_WRITER_H

#include <QTextStream>
#include <QString>

class QIODevice;
class QTextCodec;
class QTextDocument;

namespace qompose
{
/*!
 * \brief This class encapsulates code to write QDocuments to QIODevices.
 */
class DocumentWriter
{
public:
	DocumentWriter();
	DocumentWriter(QIODevice *);
	virtual ~DocumentWriter();

	QIODevice *getDevice() const;
	void setDevice(QIODevice *);

	QTextCodec *getCodec() const;
	void setCodec(QTextCodec *);

	bool isWhitespaceTrimmed() const;
	void setWhitespaceTrimmed(bool);

	bool write(const QTextDocument *);

private:
	bool whitespaceTrimmed;

	QTextStream stream;

	QString trimWhitespace(const QString &) const;
};
}

#endif
