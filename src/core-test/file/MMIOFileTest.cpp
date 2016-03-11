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
#include <vector>

#include <bdrck/fs/TemporaryStorage.hpp>

#include "core/document/PieceTable.hpp"
#include "core/file/MMIOFile.hpp"
#include "core/string/Utf8Iterator.hpp"

TEST_CASE("Test MMIO file loading", "[InMemoryFile]")
{
	constexpr char const *TEST_CONTENTS = "this is a test file.";

	bdrck::fs::TemporaryStorage file(bdrck::fs::TemporaryStorageType::FILE);

	{
		std::ofstream out(file.getPath(),
		                  std::ios_base::out | std::ios_base::binary |
		                          std::ios_base::trunc);
		REQUIRE(out.is_open());
		out << TEST_CONTENTS;
	}

	qompose::core::file::MMIOFile loadedFile(file.getPath());
	auto begin = qompose::core::document::beginIteratorFrom(loadedFile);
	auto end = qompose::core::document::endIteratorFrom(loadedFile);

	using Character = qompose::core::string::Utf8Iterator::value_type;
	std::vector<Character> characters;
	std::copy(begin, end, std::back_inserter(characters));

	std::vector<Character> expectedCharacters;
	for(char const *it = TEST_CONTENTS;
	    it < TEST_CONTENTS + std::strlen(TEST_CONTENTS); ++it)
	{
		expectedCharacters.emplace_back(static_cast<Character>(*it));
	}

	CHECK(characters == expectedCharacters);
}
