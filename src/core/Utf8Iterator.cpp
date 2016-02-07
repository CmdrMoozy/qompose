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

#include "Utf8Iterator.hpp"

#include <cassert>
#include <climits>
#include <stdexcept>
#include <experimental/optional>

namespace
{
constexpr qompose::core::Utf8DecodeResult END_UTF8_DECODE_RESULT = {0, nullptr,
                                                                    nullptr};

template <typename UIntT> UIntT numberOfLeadingZeros(UIntT x)
{
	constexpr auto BIT_SIZE = sizeof(UIntT) * CHAR_BIT;
	if(x == 0)
		return BIT_SIZE;

	UIntT n = 1;
	auto shift = BIT_SIZE / 2;
	while(shift < BIT_SIZE - 1)
	{
		if((x >> shift) == 0)
		{
			n += BIT_SIZE - shift;
			x <<= BIT_SIZE - shift;
		}
		shift += ((BIT_SIZE - shift) / 2);
	}
	n -= (x >> (BIT_SIZE - 1));
	return n;
}

std::experimental::optional<uint8_t> getUtf8CharByteLength(uint8_t const &byte)
{
	// clang-format off
	/*
	 * An array which maps a numberOfLeadingZeros(~byte) count (the
	 * index) to a UTF8 character byte count (the value).
	 */
	constexpr uint8_t NLZ_TO_BYTE_LENGTH_MAP[] = {
	        1, // 0xxxxxxx
	        0, // 10xxxxxx (not a valid first UTF8 byte)
	        2, // 110xxxxx
	        3, // 1110xxxx
	        4, // 11110xxx
	        5, // 111110xx
	        6  // 1111110x
	};
	// clang-format on
	constexpr uint8_t NLZ_TO_BYTE_LENGTH_MAP_LENGTH =
	        sizeof(NLZ_TO_BYTE_LENGTH_MAP) /
	        sizeof(NLZ_TO_BYTE_LENGTH_MAP[0]);

	uint8_t nlz = numberOfLeadingZeros<uint8_t>(~byte);
	if(nlz >= NLZ_TO_BYTE_LENGTH_MAP_LENGTH)
		return std::experimental::nullopt;

	uint8_t bytes = NLZ_TO_BYTE_LENGTH_MAP[nlz];
	if(bytes == 0)
		return std::experimental::nullopt;
	return bytes;
}

/*!
 * Return a mask which has only the actual code point bits from the given
 * first UTF8 byte set. Assumes that the given byte is a valid first UTF8
 * byte; it is up to the caller to ensure that this is the case.
 *
 * \param byte The byte to compute a mask for.
 * \return The code point mask for the given byte.
 */
uint8_t getUtf8FirstByteMask(uint8_t const &byte)
{
	uint8_t nlz = numberOfLeadingZeros<uint8_t>(~byte);
	assert(nlz <= 6);

	uint8_t mask = 1;
	mask <<= 8 - nlz - 1;
	mask -= 1;
	return mask;
}

/*!
 * Returns true if the given UTF-8 character can be expressed with fewer
 * UTF-8 bytes than the given bytesUsed number.
 */
bool isOverlongUtf8Character(uint32_t character, uint8_t bytesUsed)
{
	assert(bytesUsed <= 6);

	// clang-format off
	/*
	 * An array which maps a UTF-8 byte count to a number of real code
	 * point bits available in that number of bytes.
	 */
	constexpr uint8_t BITS_AVAILABLE_IN_BYTES[] = {
	        0,
	        7,      // 0xxxxxxx
	        5 + 6,  // 110xxxxx ...
	        4 + 12, // 1110xxxx ...
	        3 + 18, // 11110xxx ...
	        2 + 24, // 111110xx ...
	        1 + 30  // 1111110x ...
	};
	// clang-format on

	uint32_t characterBits = 32 - numberOfLeadingZeros<uint32_t>(character);
	assert(characterBits <= 31);
	for(std::size_t bytes = 1; bytes <= 6; ++bytes)
	{
		if(BITS_AVAILABLE_IN_BYTES[bytes] >= characterBits)
		{
			return bytes < bytesUsed;
		}
	}

	// Execution will never get here, because characterBits is <= the
	// largest value in BITS_AVAILABLE_IN_BYTES.
	assert(false);
}

bool isUtf16SurrogateCharacter(uint32_t character)
{
	return character >= 0x0000D800U && character <= 0x0000DFFFU;
}

bool isUtf8Noncharacter(uint32_t character)
{
	if(character >= 0x0000FDD0U && character <= 0x0000FDEF)
		return true;

	if(character == 0x0000FFFEU || character == 0x0000FFFF)
		return true;

	for(uint32_t plane = 0x00000000U; plane <= 0x00100000U;
	    plane += 0x00010000U)
	{
		if(character == (plane | 0x0000FFFEU) ||
		   character == (plane | 0x0000FFFFU))
		{
			return true;
		}
	}

	return false;
}

/*!
 * Decodes the UTF8 character at the given position. If the current
 * position is nullptr, or is at or after end, then an "end" structure is
 * returned (namely, {0, nullptr, nullptr}).
 */
qompose::core::Utf8DecodeResult decodeUtf8(uint8_t const *current,
                                           uint8_t const *end)
{
	/*
	 * Make sure that the pointers are valid, that this is a valid UTF8
	 * character, and that the entire character is in-bounds.
	 */
	if(current == nullptr || end == nullptr || current >= end)
		return END_UTF8_DECODE_RESULT;
	auto byteLength = getUtf8CharByteLength(*current);
	if(!byteLength)
	{
		throw std::runtime_error(
		        "Encountered invalid UTF-8 start byte.");
	}
	if(current + *byteLength > end)
	{
		throw std::runtime_error("UTF-8 bytes ended prematurely.");
	}

	uint32_t value = static_cast<uint32_t>(*current &
	                                       getUtf8FirstByteMask(*current));
	for(auto it = current + 1; it < current + *byteLength; ++it)
	{
		if((*it & 0xC0U) != 0x80U)
		{
			throw std::runtime_error(
			        "Encountered invalid UTF-8 continuation byte.");
		}
		value <<= 6;
		value |= *it & 0x3FU;
	}

	if(isOverlongUtf8Character(value, *byteLength))
	{
		throw std::runtime_error(
		        "Encountered overlong UTF-8 character.");
	}

	if(isUtf16SurrogateCharacter(value))
	{
		throw std::runtime_error(
		        "Encountered invalid UTF-16 surrogate.");
	}

	if(isUtf8Noncharacter(value))
	{
		throw std::runtime_error("Encountered UTF-8 character that is "
		                         "permanently reserved for internal "
		                         "use.");
	}

	return {value, current, current + *byteLength};
}

/*!
 * Finds a pointer to the first byte of the UTF8 character which appears
 * strictly before the byte pointed to by current. If there is no such
 * byte at or after begin, then nullptr is returned instead.
 */
uint8_t const *findPrevious(uint8_t const *begin, uint8_t const *current)
{
	if(begin == nullptr || current == nullptr)
		return nullptr;
	assert(current >= begin);

	for(uint8_t const *it = current - 1; it > begin; --it)
	{
		if(!!getUtf8CharByteLength(*it))
			return it;
	}

	return nullptr;
}
}

namespace qompose
{
namespace core
{
Utf8Iterator::Utf8Iterator()
        : begin(nullptr), end(nullptr), value({0, nullptr, nullptr})
{
}

Utf8Iterator::Utf8Iterator(uint8_t const *b, uint8_t const *e)
        : begin(b), end(e), value(decodeUtf8(b, e))
{
}

bool Utf8Iterator::operator==(Utf8Iterator const &o) const
{
	return value.current == o.value.current;
}

bool Utf8Iterator::operator!=(Utf8Iterator const &o) const
{
	return !(*this == o);
}

Utf8Iterator &Utf8Iterator::operator++()
{
	value = decodeUtf8(value.currentEnd, end);
	return *this;
}

Utf8Iterator Utf8Iterator::operator++(int) const
{
	Utf8Iterator other(*this);
	++other;
	return other;
}

Utf8Iterator &Utf8Iterator::operator--()
{
	value = decodeUtf8(findPrevious(begin, value.current), end);
	return *this;
}

Utf8Iterator Utf8Iterator::operator--(int) const
{
	Utf8Iterator other(*this);
	--other;
	return other;
}

Utf8Iterator::reference Utf8Iterator::operator*() const
{
	return value.value;
}

Utf8Iterator::pointer Utf8Iterator::operator->() const
{
	return &value.value;
}
}
}
