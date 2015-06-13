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

#include "Encoding.h"

#include <fstream>
#include <memory>

#include <QList>
#include <QByteArray>
#include <QTextCodec>

#include <unicode/ucsdet.h>

namespace qompose
{
namespace encoding_utils
{
QString detectTextCodec(const QString &f)
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
}
}
