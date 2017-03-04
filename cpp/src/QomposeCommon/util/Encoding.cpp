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

#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>

#include "core/file/MMIOFile.hpp"

namespace
{
/**
 * \brief A list of valid unicode BOM's, and their associated encodings.
 *
 * Note that the order matters here; some BOM's start with the same bytes
 * as others, so if they are checked in the wrong order we may mistake one
 * for another (e.g. UTF-32LE and UTF-16LE).
 */
const std::vector<std::pair<QString, std::vector<uint8_t>>> BOM_TYPES = {
        {QString("UTF-32BE"), {0x00, 0x00, 0xFE, 0xFF}},
        {QString("UTF-32LE"), {0xFF, 0xFE, 0x00, 0x00}},
        {QString("UTF-8"), {0xEF, 0xBB, 0xBF}},
        {QString("UTF-16BE"), {0xFE, 0xFF}},
        {QString("UTF-16LE"), {0xFF, 0xFE}}};

/**
 * Tests whether or not the given unicode BOM is present at the beginning of
 * the given file.
 *
 * \param bytes The BOM bytes to search for.
 * \param file The file whose contents will be examined.
 * \return Whether or not the given BOM is present in the given file.
 */
bool bomPresent(const std::vector<uint8_t> &bytes,
                const qompose::core::file::MMIOFile &file)
{
	if(file.size() < bytes.size())
		return false;
	return std::equal(bytes.data(), bytes.data() + bytes.size(),
	                  file.data());
}

/**
 * Tests whether or not the contents of a suspected UTF-8 codepoint are
 * valid. It is assumed that the caller has already verified the first byte
 * of the codepoint, which indicates the number of other bytes.
 *
 * If the codepoint is valid, then the given codepoint pointer will be moved
 * to the first byte after the codepoint being examined.
 *
 * \param codepoint A pointer to the first 0x10xxxxxx byte in the codepoint.
 * \param length The length of the codepoint, excluding the first byte.
 * \param end The last byte in the overall data stream being examined.
 * \return Whether or not the given codepoint is a valid codepoint.
 */
bool isValidUTF8CodePoint(const uint8_t *&codepoint, std::size_t length,
                          const uint8_t *end)
{
	if(length == 0)
		return false;
	const uint8_t *codepointEnd = codepoint + length;
	if(codepointEnd > end)
		return false;

	// Skip the first byte in the codepoint; the caller should have
	// already verified the first byte.
	++codepoint;

	for(const uint8_t *byte = codepoint; byte < codepointEnd; ++byte)
	{
		if((*byte & 0xC0) != 0x80)
			return false;
	}

	codepoint = codepointEnd;
	return true;
}

/**
 * Determine whether or not the contents of the given file are valid UTF-8
 * codepoints (which indicates that it is most likely a text file encoded
 * with UTF-8).
 *
 * \param file The file whose contents will be examined.
 * \return Whether or not the given file contains only valid UTF-8 codepoints.
 */
bool isValidUTF8(const qompose::core::file::MMIOFile &file)
{
	// UTF-8 code points can be up to six bytes long. They follow one of
	// the following formats:
	//
	// 0x0xxxxxxx
	// 0x110xxxxx 0x10xxxxxx
	// 0x1110xxxx 0x10xxxxxx 0x10xxxxxx
	// 0x11110xxx 0x10xxxxxx 0x10xxxxxx 0x10xxxxxx
	// 0x111110xx 0x10xxxxxx 0x10xxxxxx 0x10xxxxxx 0x10xxxxxx
	// 0x1111110x 0x10xxxxxx 0x10xxxxxx 0x10xxxxxx 0x10xxxxxx 0x10xxxxxx
	//
	// If we encounter any sequence of bytes which do not follow this
	// pattern, then we know that the given file contains non-UTF8 data.

	const uint8_t *byte = file.data();
	const uint8_t *end = file.data() + file.size();
	while(byte < end)
	{
		if((*byte & 0x80) == 0x00) // 0x0xxxxxxx (1 byte)
		{
			++byte;
		}
		else if((*byte & 0xE0) == 0xC0) // 0x110xxxxx (2 bytes)
		{
			if(!isValidUTF8CodePoint(byte, 2, end))
				return false;
		}
		else if((*byte & 0xF0) == 0xE0) // 0x1110xxxx (3 bytes)
		{
			if(!isValidUTF8CodePoint(byte, 3, end))
				return false;
		}
		else if((*byte & 0xF8) == 0xF0) // 0x11110xxx (4 bytes)
		{
			if(!isValidUTF8CodePoint(byte, 4, end))
				return false;
		}
		else if((*byte & 0xFC) == 0xF8) // 0x111110xx (5 bytes)
		{
			if(!isValidUTF8CodePoint(byte, 5, end))
				return false;
		}
		else if((*byte & 0xFE) == 0xFC) // 0x1111110x (6 bytes)
		{
			if(!isValidUTF8CodePoint(byte, 6, end))
				return false;
		}
		else
		{
			// Invalid first byte of sequence point.
			return false;
		}
	}

	return true;
}
}

namespace qompose
{
namespace encoding_utils
{
QString detectTextCodec(const QString &f)
{
	try
	{
		core::file::MMIOFile file(f.toStdString());

		// If the file is empty, just default to UTF-8.
		if(file.size() == 0)
			return QString("UTF-8");

		// First, test if a BOM is present in the given file.
		for(const auto &bomPair : BOM_TYPES)
		{
			if(bomPresent(bomPair.second, file))
				return bomPair.first;
		}

		// I guess there's no BOM; see if the bytes in this file are
		// valid UTF-8. Otherwise, we will just return no encoding.
		if(isValidUTF8(file))
			return QString("UTF-8");
		else
			return QString();
	}
	catch(...)
	{
	}

	return QString();
}
}
}
