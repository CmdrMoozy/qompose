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

#include "QomposeDocumentWriter.h"

#include <QIODevice>
#include <QTextCodec>
#include <QTextDocument>
#include <QStringList>
#include <QRegExp>

/*!
 * This is our default constructor, which creates a new document writer
 * instance. Before writing any documents, one will need to set the QIODevice
 * with setDevice().
 */
QomposeDocumentWriter::QomposeDocumentWriter()
	: whitespaceTrimmed(false)
{
}

/*!
 * This constructor creates a new document writer, which is configured to write
 * to the given QIODevice.
 *
 * \param d The device to write to.
 */
QomposeDocumentWriter::QomposeDocumentWriter(QIODevice *d)
	: whitespaceTrimmed(false)
{
	setDevice(d);
}

/*!
 * This is our default destructor, which cleans up & destroys our object.
 */
QomposeDocumentWriter::~QomposeDocumentWriter()
{
}

/*!
 * This function returns a pointer to the QIODevice this writer is currently
 * configured to use when writing data.
 *
 * \return The device this object is currently using.
 */
QIODevice *QomposeDocumentWriter::getDevice() const
{
	return stream.device();
}

/*!
 * This function sets the QIODevice we will use for any subsequent write
 * requests.
 *
 * \param d The device to write to.
 */
void QomposeDocumentWriter::setDevice(QIODevice *d)
{
	stream.setDevice(d);
}

/*!
 * This function returns a pointer to the QTextCodec this writer will use to
 * encode data it writes.
 *
 * \return The text codec this object is currently using.
 */
QTextCodec *QomposeDocumentWriter::getCodec() const
{
	return stream.codec();
}

/*!
 * This function sets the QTextCodec we will use to encode text for any
 * subsequent write requests.
 *
 * \param codec The text codec to use for encoding.
 */
void QomposeDocumentWriter::setCodec(QTextCodec *codec)
{
	stream.setCodec(codec);
}

/*!
 * This function returns whether or not our object is currently configured to
 * trim trailing whitespace before writing documents.
 *
 * \return Whether or not this object will trim trailing whitespace.
 */
bool QomposeDocumentWriter::isWhitespaceTrimmed() const
{
	return whitespaceTrimmed;
}

/*!
 * This function sets whether or not this writer should trim all trailing
 * whitespace from the lines in a document before writing it.
 *
 * \param w Whether or not to trim trailing whitespace.
 */
void QomposeDocumentWriter::setWhitespaceTrimmed(bool w)
{
	whitespaceTrimmed = w;
}

/*!
 * This function writes the given document to our writer's current QIODevice,
 * using our current QTextCodec for text encoding.
 *
 * This is done by getting the document's plain text (via toPlainText()),
 * optionally processing it first, and then writing it to whatever QIODevice
 * we're configured to use.
 *
 * \param d The text document to write.
 * \return True on success, or false otherwise.
 */
bool QomposeDocumentWriter::write(const QTextDocument *d)
{
	// Make sure our device is good for writing.

	if(!getDevice()->isWritable() &&
		!getDevice()->open(QIODevice::WriteOnly))
	{
		return false;
	}

	// Get the contents of the document.

	QString contents = d->toPlainText();

	if(isWhitespaceTrimmed())
		contents = trimWhitespace(contents);

	// Write the contents to our text stream.

	stream << contents;

	getDevice()->close();

	return true;
}

/*!
 * This is a utility function which trims the trailing whitespace from each line
 * in the given string. The given string is split into lines ending with one of
 * \n, \r\n or \r, and then each line is stripped of trailing whitespace (i.e.,
 * any QChars where isSpace() returns true).
 *
 * Note that the given string isn't modified; a new string is returned instead.
 *
 * \param s The string to trim trailing whitespace from.
 * \return A copy of the given string, after processing.
 */
QString QomposeDocumentWriter::trimWhitespace(const QString &s) const
{
	QString result("");

	QStringList lines = s.split(QRegExp("\n|(\r\n)|\r"),
		QString::KeepEmptyParts);

	for(int i = 0; i < lines.size(); ++i)
	{
		int n = lines.at(i).size() - 1;
		while( (n >= 0) && lines.at(i).at(n).isSpace() )
			--n;

		result.append(lines.at(i).left(n + 1));

		if(i < lines.size() - 1)
			result.append('\n');
	}

	return result;
}
