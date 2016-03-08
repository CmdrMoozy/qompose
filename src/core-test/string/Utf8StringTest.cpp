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

#include <catch/catch.hpp>

#include <algorithm>
#include <cstddef>
#include <vector>
#include <experimental/optional>

#include "core/string/Utf8String.hpp"

TEST_CASE("Test default construction", "[Utf8String]")
{
	qompose::core::string::Utf8String str;
	CHECK(str.empty());
	CHECK(str.length() == 0);
	CHECK(str.size() == 0);
	CHECK(str.begin() == str.end());
	CHECK(str.rbegin() == str.rend());
}

TEST_CASE("Test empty string length", "[Utf8String]")
{
	qompose::core::string::Utf8String str("");
	CHECK(str.empty());
	CHECK(str.length() == 0);
	CHECK(str.size() == 0);
	CHECK(str.begin() == str.end());
	CHECK(str.rbegin() == str.rend());
}

TEST_CASE("Test ASCII string length and iterating", "[Utf8String]")
{
	constexpr char const *TEST_STRING = "this is a test string";
	constexpr std::size_t TEST_STRING_LENGTH = 21;

	qompose::core::string::Utf8String str(TEST_STRING);
	CHECK(!str.empty());
	CHECK(str.length() == TEST_STRING_LENGTH);
	CHECK(str.size() == TEST_STRING_LENGTH);

	std::vector<char> iterated;
	for(auto it = str.begin(); it != str.end(); ++it)
		iterated.emplace_back(static_cast<char>(*it));

	REQUIRE(iterated.size() == TEST_STRING_LENGTH);
	CHECK(std::equal(TEST_STRING, TEST_STRING + TEST_STRING_LENGTH,
	                 iterated.data()));

	std::vector<char> reverseIterated;
	for(auto it = str.rbegin(); it != str.rend(); ++it)
		reverseIterated.emplace_back(static_cast<char>(*it));

	REQUIRE(reverseIterated.size() == TEST_STRING_LENGTH);
	CHECK(std::equal(reverseIterated.begin(), reverseIterated.end(),
	                 iterated.rbegin()));
}

TEST_CASE("Test iterator definition", "[Utf8String]")
{
	static const std::vector<uint8_t> TEST_STRING{
	        0xCEU, 0xBAU, 0xE1U, 0xBDU, 0xB9U, 0xCFU,
	        0x83U, 0xCEU, 0xBCU, 0xCEU, 0xB5U};
	static const std::vector<uint32_t> TEST_DECODED_CHARACTERS{
	        0x3BAU, 0x1F79U, 0x3C3U, 0x3BCU, 0x3B5U};

	qompose::core::string::Utf8String str(
	        TEST_STRING.data(), TEST_STRING.data() + TEST_STRING.size());

	qompose::core::string::Utf8String::iterator it = str.begin();
	qompose::core::string::Utf8String::reverse_iterator rit = str.rbegin();

	// Test copy construction, copy assignment, equality / inequality,
	// and dereferenceability as an rvalue.
	{
		qompose::core::string::Utf8String::iterator copyA(it);
		CHECK(copyA == it);
		CHECK(!(copyA != it));
		CHECK(*copyA == *it);
		CHECK(*copyA.operator->() == *it.operator->());

		qompose::core::string::Utf8String::iterator copyB;
		CHECK(copyB != it);
		CHECK(!(copyB == it));
		copyB = it;
		CHECK(copyB == it);
		CHECK(!(copyB != it));
		CHECK(*copyB == *it);
		CHECK(*copyB.operator->() == *it.operator->());

		qompose::core::string::Utf8String::reverse_iterator revCopyA(
		        rit);
		CHECK(revCopyA == rit);
		CHECK(!(revCopyA != rit));
		CHECK(*revCopyA == *rit);
		CHECK(*revCopyA.operator->() == *rit.operator->());

		qompose::core::string::Utf8String::reverse_iterator revCopyB;
		CHECK(revCopyB != rit);
		CHECK(!(revCopyB == rit));
		revCopyB = rit;
		CHECK(revCopyB == rit);
		CHECK(!(revCopyB != rit));
		CHECK(*revCopyB == *rit);
		CHECK(*revCopyB.operator->() == *rit.operator->());
	}

	// Must be incrementable.
	CHECK(*it == TEST_DECODED_CHARACTERS[0]);
	++it;
	CHECK(*it == TEST_DECODED_CHARACTERS[1]);
	it++;
	CHECK(*it == TEST_DECODED_CHARACTERS[2]);
	CHECK(*rit ==
	      TEST_DECODED_CHARACTERS[TEST_DECODED_CHARACTERS.size() - 1]);
	++rit;
	CHECK(*rit ==
	      TEST_DECODED_CHARACTERS[TEST_DECODED_CHARACTERS.size() - 2]);
	rit++;
	CHECK(*rit ==
	      TEST_DECODED_CHARACTERS[TEST_DECODED_CHARACTERS.size() - 3]);

	// Must be default-constructible. End iterators are always equal.
	{
		qompose::core::string::Utf8String::iterator defaultIt;
		CHECK(defaultIt == str.end());

		qompose::core::string::Utf8String::reverse_iterator
		        defaultRevIt;
		CHECK(defaultRevIt == str.rend());
	}
}

TEST_CASE("Test UTF8 string length and iterating", "[Utf8String]")
{
	static const std::vector<uint8_t> TEST_STRING{
	        0xCEU, 0xBAU, 0xE1U, 0xBDU, 0xB9U, 0xCFU,
	        0x83U, 0xCEU, 0xBCU, 0xCEU, 0xB5U};
	static const std::vector<uint32_t> TEST_DECODED_CHARACTERS{
	        0x3BAU, 0x1F79U, 0x3C3U, 0x3BCU, 0x3B5U};
	constexpr std::size_t TEST_STRING_LENGTH = 5;

	qompose::core::string::Utf8String str(
	        TEST_STRING.data(), TEST_STRING.data() + TEST_STRING.size());
	CHECK(!str.empty());
	CHECK(str.length() == TEST_STRING_LENGTH);
	CHECK(str.size() == TEST_STRING_LENGTH);

	std::vector<uint32_t> iterated;
	for(auto it = str.begin(); it != str.end(); ++it)
		iterated.push_back(*it);

	REQUIRE(iterated.size() == TEST_STRING_LENGTH);
	CHECK(std::equal(TEST_DECODED_CHARACTERS.begin(),
	                 TEST_DECODED_CHARACTERS.end(), iterated.begin()));

	std::vector<uint32_t> reverseIterated;
	for(auto it = str.rbegin(); it != str.rend(); ++it)
		reverseIterated.push_back(*it);

	REQUIRE(reverseIterated.size() == iterated.size());
	CHECK(std::equal(reverseIterated.begin(), reverseIterated.end(),
	                 iterated.rbegin()));
}

TEST_CASE("Test first possible UTF8 sequences of a length", "[Utf8String]")
{
	using TestCase = struct
	{
		std::vector<uint8_t> bytes;
		std::size_t expectedLength;
		std::vector<uint32_t> expectedCharacters;
	};

	static const std::vector<TestCase> TEST_CASES{
	        {{0x00U}, 1, {0x00000000U}},
	        {{0xC2U, 0x80U}, 1, {0x00000080U}},
	        {{0xE0U, 0xA0U, 0x80U}, 1, {0x00000800U}},
	        {{0xF0U, 0x90U, 0x80U, 0x80U}, 1, {0x00010000U}},
	        {{0xF8U, 0x88U, 0x80U, 0x80U, 0x80U}, 1, {0x00200000U}},
	        {{0xFCU, 0x84U, 0x80U, 0x80U, 0x80U, 0x80U}, 1, {0x04000000U}}};

	for(auto const &testCase : TEST_CASES)
	{
		std::experimental::optional<qompose::core::string::Utf8String>
		        str;
		REQUIRE_NOTHROW(str.emplace(testCase.bytes.data(),
		                            testCase.bytes.data() +
		                                    testCase.bytes.size()));
		CHECK((*str).empty() == (testCase.expectedLength == 0));
		CHECK((*str).length() == testCase.expectedLength);
		CHECK((*str).size() == testCase.expectedLength);

		std::vector<uint32_t> iterated;
		for(auto it = (*str).begin(); it != (*str).end(); ++it)
			iterated.emplace_back(*it);
		REQUIRE(iterated.size() == testCase.expectedCharacters.size());
		CHECK(std::equal(iterated.begin(), iterated.end(),
		                 testCase.expectedCharacters.begin()));
	}
}

TEST_CASE("Test last possible UTF8 sequences of a length", "[Utf8String]")
{
	using TestCase = struct
	{
		std::vector<uint8_t> bytes;
		std::size_t expectedLength;
		std::vector<uint32_t> expectedCharacters;
	};

	static const std::vector<TestCase> TEST_CASES{
	        {{0x7FU}, 1, {0x0000007FU}},
	        {{0xDFU, 0xBFU}, 1, {0x000007FFU}},
	        /*
	        // Skip U+FFFF, because it is a noncharacter.
	        {
	                {0xEFU, 0xBFU, 0xBFU},
	                1,
	                {0x0000FFFFU}
	        },
	        */
	        {{0xF7U, 0xBFU, 0xBFU, 0xBFU}, 1, {0x001FFFFFU}},
	        {{0xFBU, 0xBFU, 0xBFU, 0xBFU, 0xBFU}, 1, {0x03FFFFFFU}},
	        {{0xFD, 0xBFU, 0xBFU, 0xBFU, 0xBFU, 0xBFU}, 1, {0x7FFFFFFFU}}};

	for(auto const &testCase : TEST_CASES)
	{
		std::experimental::optional<qompose::core::string::Utf8String>
		        str;
		REQUIRE_NOTHROW(str.emplace(testCase.bytes.data(),
		                            testCase.bytes.data() +
		                                    testCase.bytes.size()));
		CHECK((*str).empty() == (testCase.expectedLength == 0));
		CHECK((*str).length() == testCase.expectedLength);
		CHECK((*str).size() == testCase.expectedLength);

		std::vector<uint32_t> iterated;
		for(auto it = (*str).begin(); it != (*str).end(); ++it)
			iterated.emplace_back(*it);
		REQUIRE(iterated.size() == testCase.expectedCharacters.size());
		CHECK(std::equal(iterated.begin(), iterated.end(),
		                 testCase.expectedCharacters.begin()));
	}
}

TEST_CASE("Test other boundary conditions", "[Utf8String]")
{
	using TestCase = struct
	{
		std::vector<uint8_t> bytes;
		std::size_t expectedLength;
		std::vector<uint32_t> expectedCharacters;
	};

	static const std::vector<TestCase> TEST_CASES{
	        {{0xEDU, 0x9FU, 0xBFU}, 1, {0x0000D7FFU}},
	        {{0xEEU, 0x80U, 0x80U}, 1, {0x0000E000U}},
	        {{0xEFU, 0xBFU, 0xBDU}, 1, {0x0000FFFDU}},
	        /*
	        // Skip U+10FFFF, because it is a noncharacter.
	        {
	                {0xF4U, 0x8FU, 0xBFU, 0xBFU},
	                1,
	                {0x0010FFFFU}
	        },
	        */
	        {{0xF4U, 0x90U, 0x80U, 0x80U}, 1, {0x00110000U}}};

	for(auto const &testCase : TEST_CASES)
	{
		std::experimental::optional<qompose::core::string::Utf8String>
		        str;
		REQUIRE_NOTHROW(str.emplace(testCase.bytes.data(),
		                            testCase.bytes.data() +
		                                    testCase.bytes.size()));
		CHECK((*str).empty() == (testCase.expectedLength == 0));
		CHECK((*str).length() == testCase.expectedLength);
		CHECK((*str).size() == testCase.expectedLength);

		std::vector<uint32_t> iterated;
		for(auto it = (*str).begin(); it != (*str).end(); ++it)
			iterated.emplace_back(*it);
		REQUIRE(iterated.size() == testCase.expectedCharacters.size());
		CHECK(std::equal(iterated.begin(), iterated.end(),
		                 testCase.expectedCharacters.begin()));
	}
}

TEST_CASE("Test malformed UTF8 sequences", "[Utf8String]")
{
	static const std::vector<std::vector<uint8_t>> TEST_CASES{
	        // Unexpected continuation bytes:
	        {0x80U},
	        {0xBFU},
	        {0x80U, 0xBFU},
	        {0x80U, 0xBFU, 0x80U},
	        {0x80U, 0xBFU, 0x80U, 0xBFU},
	        {0x80U, 0xBFU, 0x80U, 0xBFU, 0x80U},
	        {0x80U, 0xBFU, 0x80U, 0xBFU, 0x80U, 0xBFU},
	        {0x80U, 0xBFU, 0x80U, 0xBFU, 0x80U, 0xBFU, 0x80U},

	        // Sequence of all 64 possible continuation bytes
	        // (0x80 to 0xBF):
	        {0x80U, 0x81U, 0x82U, 0x83U, 0x84U, 0x85U, 0x86U, 0x87U, 0x88U,
	         0x89U, 0x8AU, 0x8BU, 0x8CU, 0x8DU, 0x8EU, 0x8FU, 0x90U, 0x91U,
	         0x92U, 0x93U, 0x84U, 0x95U, 0x96U, 0x97U, 0x98U, 0x99U, 0x9AU,
	         0x9BU, 0x9CU, 0x9DU, 0x9EU, 0x9FU, 0xA0U, 0xA1U, 0xA2U, 0xA3U,
	         0xA4U, 0xA5U, 0xA6U, 0xA7U, 0xA8U, 0xA9U, 0xAAU, 0xABU, 0xACU,
	         0xADU, 0xAEU, 0xAFU, 0xB0U, 0xB1U, 0xB2U, 0xB3U, 0xB4U, 0xB5U,
	         0xB6U, 0xB7U, 0xB8U, 0xB9U, 0xBAU, 0xBBU, 0xBCU, 0xBDU, 0xBEU,
	         0xBFU},

	        // All 32 first bytes of 2-byte sequences (0xC0 to 0xDF),
	        // each followed by a space character:
	        {0xC0U, 0x20U, 0xC1U, 0x20U, 0xC2U, 0x20U, 0xC3U, 0x20U, 0xC4U,
	         0x20U, 0xC5U, 0x20U, 0xC6U, 0x20U, 0xC7U, 0x20U, 0xC8U, 0x20U,
	         0xC9U, 0x20U, 0xCAU, 0x20U, 0xCBU, 0x20U, 0xCCU, 0x20U, 0xCDU,
	         0x20U, 0xCEU, 0x20U, 0xCFU, 0x20U, 0xD0U, 0x20U, 0xD1U, 0x20U,
	         0xD2U, 0x20U, 0xD3U, 0x20U, 0xD4U, 0x20U, 0xD5U, 0x20U, 0xD6U,
	         0x20U, 0xD7U, 0x20U, 0xD8U, 0x20U, 0xD9U, 0x20U, 0xDAU, 0x20U,
	         0xDBU, 0x20U, 0xDCU, 0x20U, 0xDDU, 0x20U, 0xDEU, 0x20U, 0xDFU,
	         0x20U},

	        // All 16 first bytes of 3-byte sequences (0xE0 to 0xEF),
	        // each followed by a space character:
	        {0xE0U, 0x20U, 0xE1U, 0x20U, 0xE2U, 0x20U, 0xE3U, 0x20U, 0xE4U,
	         0x20U, 0xE5U, 0x20U, 0xE6U, 0x20U, 0xE7U, 0x20U, 0xE8U, 0x20U,
	         0xE9U, 0x20U, 0xEAU, 0x20U, 0xEBU, 0x20U, 0xECU, 0x20U, 0xEDU,
	         0x20U, 0xEEU, 0x20U, 0xEFU, 0x20U},

	        // All 8 first bytes of 4-byte sequences (0xF0 to 0xF7),
	        // each followed by a space character:
	        {0xF0U, 0x20U, 0xF1U, 0x20U, 0xF2U, 0x20U, 0xF3U, 0x20U, 0xF4U,
	         0x20U, 0xF5U, 0x20U, 0xF6U, 0x20U, 0xF7U, 0x20U},

	        // All 4 first bytes of 5-byte sequences (0xF8 to 0xFB),
	        // each followed by a space character:
	        {0xF8U, 0x20U, 0xF9U, 0x20U, 0xFAU, 0x20U, 0xFBU, 0x20U},

	        // All 2 first bytes of 6-byte sequences (0xFC to 0xFD),
	        // each followed by a space character:
	        {0xFCU, 0x20U, 0xFDU, 0x20U},

	        // Valid sequences with last byte missing:
	        {0xC0U},
	        {0xDFU},
	        {0xE0U, 0x80U},
	        {0xEFU, 0xBFU},
	        {0xF0U, 0x80U, 0x80U},
	        {0xF7U, 0xBFU, 0xBFU},
	        {0xF8U, 0x80U, 0x80U, 0x80U},
	        {0xFBU, 0xBFU, 0xBFU, 0xBFU},
	        {0xFCU, 0x80U, 0x80U, 0x80U, 0x80U},
	        {0xFDU, 0xBFU, 0xBFU, 0xBFU, 0xBFU},

	        // Valid sequences with last byte missing, concatenated:
	        {0xC0U, 0xE0U, 0x80U, 0xF0U, 0x80U, 0x80U, 0xF8U, 0x80U, 0x80U,
	         0x80U, 0xFCU, 0x80U, 0x80U, 0x80U, 0x80U, 0xDFU, 0xEFU, 0xBFU,
	         0xF7U, 0xBFU, 0xBFU, 0xFBU, 0xBFU, 0xBFU, 0xBFU, 0xFDU, 0xBFU,
	         0xBFU, 0xBFU, 0xBFU},

	        // Bytes which cannot appear in a correct UTF-8 string:
	        {0xFEU},
	        {0xFFU},
	        {0xFEU, 0xFEU, 0xFFU, 0xFFU},

	        // Overlong representations of the ASCII character "/":
	        {0xC0U, 0xAFU},
	        {0xE0U, 0x80U, 0xAFU},
	        {0xF0U, 0x80U, 0x80U, 0xAFU},
	        {0xF8U, 0x80U, 0x80U, 0x80U, 0xAFU},
	        {0xFCU, 0x80U, 0x80U, 0x80U, 0x80U, 0xAFU},

	        // The highest Unicode value that is still an overlong
	        // sequence, for each byte length:
	        {0xC1U, 0xBFU},
	        {0xE0U, 0x9FU, 0xBFU},
	        {0xF0U, 0x8FU, 0xBFU, 0xBFU},
	        {0xF8U, 0x87U, 0xBFU, 0xBFU, 0xBFU},
	        {0xFCU, 0x83U, 0xBFU, 0xBFU, 0xBFU, 0xBFU},

	        // Overlong representations of the NUL character:
	        {0xC0U, 0x80U},
	        {0xE0U, 0x80U, 0x80U},
	        {0xF0U, 0x80U, 0x80U, 0x80U},
	        {0xF8U, 0x80U, 0x80U, 0x80U, 0x80U},
	        {0xFCU, 0x80U, 0x80U, 0x80U, 0x80U, 0x80U},

	        // Non-ISO-10646 single UTF-16 surrogates:
	        {0xEDU, 0xA0U, 0x80U},
	        {0xEDU, 0xADU, 0xBFU},
	        {0xEDU, 0xAEU, 0x80U},
	        {0xEDU, 0xAFU, 0xBFU},
	        {0xEDU, 0xB0U, 0x80U},
	        {0xEDU, 0xBEU, 0x80U},
	        {0xEDU, 0xBFU, 0xBFU},

	        // Non-ISO-10646 paired UTF-16 surrogates:
	        {0xEDU, 0xA0U, 0x80U, 0xEDU, 0xB0U, 0x80U},
	        {0xEDU, 0xA0U, 0x80U, 0xEDU, 0xBFU, 0xBFU},
	        {0xEDU, 0xADU, 0xBFU, 0xEDU, 0xB0U, 0x80U},
	        {0xEDU, 0xADU, 0xBFU, 0xEDU, 0xBFU, 0xBFU},
	        {0xEDU, 0xAEU, 0x80U, 0xEDU, 0xB0U, 0x80U},
	        {0xEDU, 0xAEU, 0x80U, 0xEDU, 0xBFU, 0xBFU},
	        {0xEDU, 0xAFU, 0xBFU, 0xEDU, 0xB0U, 0x80U},
	        {0xEDU, 0xAFU, 0xBFU, 0xEDU, 0xBFU, 0xBFU},

	        // Problematic reserved code points / noncharacters:
	        {0xEFU, 0xBFU, 0xBEU},
	        {0xEFU, 0xBFU, 0xBFU},
	        {0xEFU, 0xB7U, 0x90U, 0xEFU, 0xB7U, 0x91U, 0xEFU, 0xB7U, 0x92U,
	         0xEFU, 0xB7U, 0x93U, 0xEFU, 0xB7U, 0x94U, 0xEFU, 0xB7U, 0x95U,
	         0xEFU, 0xB7U, 0x96U, 0xEFU, 0xB7U, 0x97U, 0xEFU, 0xB7U, 0x98U,
	         0xEFU, 0xB7U, 0x99U, 0xEFU, 0xB7U, 0x9AU, 0xEFU, 0xB7U, 0x9BU,
	         0xEFU, 0xB7U, 0x9CU, 0xEFU, 0xB7U, 0x9DU, 0xEFU, 0xB7U, 0x9EU,
	         0xEFU, 0xB7U, 0x9FU, 0xEFU, 0xB7U, 0xA0U, 0xEFU, 0xB7U, 0xA1U,
	         0xEFU, 0xB7U, 0xA2U, 0xEFU, 0xB7U, 0xA3U, 0xEFU, 0xB7U, 0xA4U,
	         0xEFU, 0xB7U, 0xA5U, 0xEFU, 0xB7U, 0xA6U, 0xEFU, 0xB7U, 0xA7U,
	         0xEFU, 0xB7U, 0xA8U, 0xEFU, 0xB7U, 0xA9U, 0xEFU, 0xB7U, 0xAAU,
	         0xEFU, 0xB7U, 0xABU, 0xEFU, 0xB7U, 0xACU, 0xEFU, 0xB7U, 0xADU,
	         0xEFU, 0xB7U, 0xAEU, 0xEFU, 0xB7U, 0xAFU},
	        {0xF0U, 0x9FU, 0xBFU, 0xBEU, 0xF0U, 0x9FU, 0xBFU, 0xBFU, 0xF0U,
	         0xAFU, 0xBFU, 0xBEU, 0xF0U, 0xAFU, 0xBFU, 0xBFU, 0xF0U, 0xBFU,
	         0xBFU, 0xBEU, 0xF0U, 0xBFU, 0xBFU, 0xBFU, 0xF1U, 0x8FU, 0xBFU,
	         0xBEU, 0xF1U, 0x8FU, 0xBFU, 0xBFU, 0xF1U, 0x9FU, 0xBFU, 0xBEU,
	         0xF1U, 0x9FU, 0xBFU, 0xBFU, 0xF1U, 0xAFU, 0xBFU, 0xBEU, 0xF1U,
	         0xAFU, 0xBFU, 0xBFU, 0xF1U, 0xBFU, 0xBFU, 0xBEU, 0xF1U, 0xBFU,
	         0xBFU, 0xBFU, 0xF2U, 0x8FU, 0xBFU, 0xBEU, 0xF2U, 0x8FU,
	         0xBFU}};

	for(auto const &testCase : TEST_CASES)
	{
		qompose::core::string::Utf8String str(
		        testCase.data(), testCase.data() + testCase.size());
		CHECK_THROWS(str.length());
	}
}
