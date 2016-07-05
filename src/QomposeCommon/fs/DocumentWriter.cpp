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

#include "DocumentWriter.h"

#include <QIODevice>
#include <QRegExp>
#include <QStringList>
#include <QTextCodec>
#include <QTextDocument>

namespace qompose
{
DocumentWriter::DocumentWriter() : whitespaceTrimmed(false), stream()
{
}

DocumentWriter::DocumentWriter(QIODevice *d)
        : whitespaceTrimmed(false), stream()
{
	setDevice(d);
}

DocumentWriter::~DocumentWriter()
{
}

QIODevice *DocumentWriter::getDevice() const
{
	return stream.device();
}

void DocumentWriter::setDevice(QIODevice *d)
{
	stream.setDevice(d);
}

QTextCodec *DocumentWriter::getCodec() const
{
	return stream.codec();
}

void DocumentWriter::setCodec(QTextCodec *codec)
{
	stream.setCodec(codec);
}

bool DocumentWriter::isWhitespaceTrimmed() const
{
	return whitespaceTrimmed;
}

void DocumentWriter::setWhitespaceTrimmed(bool w)
{
	whitespaceTrimmed = w;
}

bool DocumentWriter::write(const QTextDocument *d)
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

QString DocumentWriter::trimWhitespace(const QString &s) const
{
	QString result("");

	QStringList lines =
	        s.split(QRegExp("\n|(\r\n)|\r"), QString::KeepEmptyParts);

	for(int i = 0; i < lines.size(); ++i)
	{
		int n = lines.at(i).size() - 1;
		while((n >= 0) && lines.at(i).at(n).isSpace())
			--n;

		result.append(lines.at(i).left(n + 1));

		if(i < lines.size() - 1)
			result.append('\n');
	}

	return result;
}
}
