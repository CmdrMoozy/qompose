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

#include <QString>
#include <QTextStream>

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
	/*!
	 * This is our default constructor, which creates a new document writer
	 * instance. Before writing any documents, one will need to set the
	 * QIODevice with setDevice().
	 */
	DocumentWriter();

	/*!
	 * This constructor creates a new document writer, which is configured
	 * to write to the given QIODevice.
	 *
	 * \param d The device to write to.
	 */
	DocumentWriter(QIODevice *d);

	DocumentWriter(const DocumentWriter &) = delete;

	virtual ~DocumentWriter();

	DocumentWriter &operator=(const DocumentWriter &) = delete;

	/*!
	 * This function returns a pointer to the QIODevice this writer is
	 * currently configured to use when writing data.
	 *
	 * \return The device this object is currently using.
	 */
	QIODevice *getDevice() const;

	/*!
	 * This function sets the QIODevice we will use for any subsequent
	 * write requests.
	 *
	 * \param d The device to write to.
	 */
	void setDevice(QIODevice *d);

	/*!
	 * This function returns a pointer to the QTextCodec this writer will
	 * use to encode data it writes.
	 *
	 * \return The text codec this object is currently using.
	 */
	QTextCodec *getCodec() const;

	/*!
	 * This function sets the QTextCodec we will use to encode text for any
	 * subsequent write requests.
	 *
	 * \param codec The text codec to use for encoding.
	 */
	void setCodec(QTextCodec *codec);

	/*!
	 * This function returns whether or not our object is currently
	 * configured to trim trailing whitespace before writing documents.
	 *
	 * \return Whether or not this object will trim trailing whitespace.
	 */
	bool isWhitespaceTrimmed() const;

	/*!
	 * This function sets whether or not this writer should trim all
	 * trailing whitespace from the lines in a document before writing it.
	 *
	 * \param w Whether or not to trim trailing whitespace.
	 */
	void setWhitespaceTrimmed(bool w);

	/*!
	 * This function writes the given document to our writer's current
	 * QIODevice, using our current QTextCodec for text encoding.
	 *
	 * This is done by getting the document's plain text (via
	 * toPlainText()), optionally processing it first, and then writing it
	 * to whatever QIODevice we're configured to use.
	 *
	 * \param d The text document to write.
	 * \return True on success, or false otherwise.
	 */
	bool write(const QTextDocument *d);

private:
	bool whitespaceTrimmed;

	QTextStream stream;

	/*!
	 * This is a utility function which trims the trailing whitespace from
	 * each line in the given string. The given string is split into lines
	 * ending with one of the various platform-dependent newlines, and
	 * then each line is stripped of trailing whitespace (i.e., any
	 * QChars where isSpace() returns true).
	 *
	 * Note that the given string isn't modified; a new string is returned
	 * instead.
	 *
	 * \param s The string to trim trailing whitespace from.
	 * \return A copy of the given string, after processing.
	 */
	QString trimWhitespace(const QString &s) const;
};
}

#endif
