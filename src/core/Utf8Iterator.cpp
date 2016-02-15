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
/*
 * Part of our iterator class's invariant is that its internal pointers
 * must be non-nullptr. However, we want to allow empty strings to be
 * iterated properly, and in some cases these are represented by a pair
 * of null pointers. So, instead, we just replace those null pointers
 * with this placeholder empty string.
 */
constexpr char const *PLACEHOLDER_EMPTY_STRING = "";

constexpr uint8_t MAXIMUM_UTF8_BYTE_LENGTH = 6;

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

bool isEndPointer(uint8_t const *begin, uint8_t const *current,
                  uint8_t const *end)
{
	return current < begin || current >= end;
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
	assert(nlz <= MAXIMUM_UTF8_BYTE_LENGTH);

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
	assert(bytesUsed <= MAXIMUM_UTF8_BYTE_LENGTH);

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

void throwOnInvalidUtf8Character(uint32_t character, uint8_t bytesUsed)
{
	if(isOverlongUtf8Character(character, bytesUsed))
	{
		throw std::runtime_error(
		        "Encountered overlong UTF-8 character.");
	}

	if(isUtf16SurrogateCharacter(character))
	{
		throw std::runtime_error(
		        "Encountered invalid UTF-16 surrogate.");
	}

	if(isUtf8Noncharacter(character))
	{
		throw std::runtime_error("Encountered UTF-8 character that is "
		                         "permanently reserved for internal "
		                         "use.");
	}
}

/*!
 * Decodes the UTF8 character at the given position. If the current
 * position is nullptr, or is at or after end, then an "end" structure is
 * returned (namely, {0, nullptr, nullptr}).
 */
qompose::core::Utf8DecodeResult
decodeUtf8(uint8_t const *begin, uint8_t const *current, uint8_t const *end)
{
	assert(begin != nullptr);
	assert(current != nullptr);
	assert(end != nullptr);

	if(isEndPointer(begin, current, end))
		return {0, current, current};

	auto byteLength = getUtf8CharByteLength(*current);
	if(!byteLength)
	{
		throw std::runtime_error(
		        "Encountered invalid UTF-8 start byte.");
	}

	if(current + *byteLength > end)
		throw std::runtime_error("UTF-8 bytes ended prematurely.");

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

	throwOnInvalidUtf8Character(value, *byteLength);

	return {value, current, current + *byteLength};
}

/*!
 * Finds a pointer to the first byte of the UTF8 character which appears
 * strictly before the byte pointed to by current. If there is no such
 * byte at or after begin, then nullptr is returned instead.
 *
 * Note that, for bidirectional iteration, if current is the end pointer,
 * then a pointer to the last UTF-8 character is returned.
 */
uint8_t const *findPrevious(uint8_t const *begin, uint8_t const *current,
                            uint8_t const *end)
{
	assert(begin != nullptr);
	assert(current != nullptr);
	assert(end != nullptr);

	if(current <= begin)
		return begin - 1;

	if(current > end)
		current = end;
	for(uint8_t const *it = current - 1; it >= begin; --it)
	{
		if(!!getUtf8CharByteLength(*it))
			return it;
	}

	return begin - 1;
}
}

namespace qompose
{
namespace core
{
Utf8Iterator::Utf8Iterator(uint8_t const *b, uint8_t const *e, uint8_t const *c)
        : begin(b), end(e), value({0, nullptr, nullptr})
{
	if(begin == end)
	{
		begin = end = reinterpret_cast<uint8_t const *>(
		        PLACEHOLDER_EMPTY_STRING);
	}

	if(begin == nullptr || end == nullptr)
	{
		throw std::runtime_error(
		        "Cannot construct Utf8Iterator from invalid pointers.");
	}

	value = decodeUtf8(begin, c == nullptr ? begin : c, end);
}

Utf8Iterator::Utf8Iterator() : Utf8Iterator(nullptr, nullptr, nullptr)
{
}

bool Utf8Iterator::operator==(Utf8Iterator const &o) const
{
	if(isEndPointer(begin, value.current, end) &&
	   isEndPointer(o.begin, o.value.current, o.end))
	{
		// End pointers from any range are always equal.
		return true;
	}

	return value.current == o.value.current;
}

bool Utf8Iterator::operator!=(Utf8Iterator const &o) const
{
	return !(*this == o);
}

Utf8Iterator &Utf8Iterator::operator++()
{
	value = decodeUtf8(begin, value.currentEnd, end);
	return *this;
}

Utf8Iterator Utf8Iterator::operator++(int)
{
	++(*this);
	return *this;
}

Utf8Iterator &Utf8Iterator::operator--()
{
	value = decodeUtf8(begin, findPrevious(begin, value.current, end), end);
	return *this;
}

Utf8Iterator Utf8Iterator::operator--(int)
{
	--(*this);
	return *this;
}

Utf8Iterator::reference Utf8Iterator::operator*() const
{
	return value.value;
}

Utf8Iterator::pointer Utf8Iterator::operator->() const
{
	return &value.value;
}

Utf8ReverseIterator::Utf8ReverseIterator() : iterator()
{
}

Utf8ReverseIterator::Utf8ReverseIterator(Utf8Iterator const &o) : iterator(o)
{
	--iterator;
}

bool Utf8ReverseIterator::operator==(Utf8ReverseIterator const &o) const
{
	return iterator == o.iterator;
}

bool Utf8ReverseIterator::operator!=(Utf8ReverseIterator const &o) const
{
	return iterator != o.iterator;
}

Utf8ReverseIterator &Utf8ReverseIterator::operator++()
{
	--iterator;
	return *this;
}

Utf8ReverseIterator &Utf8ReverseIterator::operator++(int)
{
	iterator--;
	return *this;
}

Utf8ReverseIterator &Utf8ReverseIterator::operator--()
{
	++iterator;
	return *this;
}

Utf8ReverseIterator &Utf8ReverseIterator::operator--(int)
{
	iterator++;
	return *this;
}

Utf8ReverseIterator::reference Utf8ReverseIterator::operator*() const
{
	return *iterator;
}

Utf8ReverseIterator::pointer Utf8ReverseIterator::operator->() const
{
	return iterator.operator->();
}
}
}
