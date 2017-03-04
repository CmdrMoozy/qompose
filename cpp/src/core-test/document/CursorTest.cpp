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
#include <cstring>
#include <fstream>
#include <iterator>
#include <utility>
#include <vector>

#include <bdrck/fs/TemporaryStorage.hpp>

#include "core/document/Cursor.hpp"
#include "core/document/PieceTable.hpp"
#include "core/file/InMemoryFile.hpp"

TEST_CASE("Test PieceTable iteration with cursors", "[Cursor]")
{
	constexpr char const *TEST_CONTENTS = "this is a test file.";
	bdrck::fs::TemporaryStorage file(bdrck::fs::TemporaryStorageType::FILE);

	{
		std::ofstream out(file.getPath(),
		                  std::ios_base::out | std::ios_base::binary |
		                          std::ios_base::trunc);
		REQUIRE(out.is_open());
		out.write(TEST_CONTENTS, std::strlen(TEST_CONTENTS));
	}

	qompose::core::file::InMemoryFile inMemoryFile(file.getPath());
	qompose::core::document::PieceTable pieceTable(std::move(inMemoryFile));

	using Character =
	        qompose::core::document::PieceTable::iterator::value_type;
	std::vector<Character> characters;
	std::copy(pieceTable.begin(), pieceTable.end(),
	          std::back_inserter(characters));

	std::vector<Character> expectedCharacters;
	for(char const *it = TEST_CONTENTS;
	    it < TEST_CONTENTS + std::strlen(TEST_CONTENTS); ++it)
	{
		expectedCharacters.emplace_back(static_cast<Character>(*it));
	}

	CHECK(characters == expectedCharacters);
}

TEST_CASE("Test PieceTable iterator with reverse cursors", "[Cursor]")
{
	constexpr char const *TEST_CONTENTS = "this is a test file.";
	bdrck::fs::TemporaryStorage file(bdrck::fs::TemporaryStorageType::FILE);

	{
		std::ofstream out(file.getPath(),
		                  std::ios_base::out | std::ios_base::binary |
		                          std::ios_base::trunc);
		REQUIRE(out.is_open());
		out.write(TEST_CONTENTS, std::strlen(TEST_CONTENTS));
	}

	qompose::core::file::InMemoryFile inMemoryFile(file.getPath());
	qompose::core::document::PieceTable pieceTable(std::move(inMemoryFile));

	using Character =
	        qompose::core::document::PieceTable::iterator::value_type;
	std::vector<Character> characters;
	std::copy(pieceTable.rbegin(), pieceTable.rend(),
	          std::back_inserter(characters));

	std::vector<Character> expectedCharacters;
	for(char const *it = TEST_CONTENTS + std::strlen(TEST_CONTENTS) - 1;
	    it >= TEST_CONTENTS; --it)
	{
		expectedCharacters.emplace_back(static_cast<Character>(*it));
	}

	CHECK(characters == expectedCharacters);
}
