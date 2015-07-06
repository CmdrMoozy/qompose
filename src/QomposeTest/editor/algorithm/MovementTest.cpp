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

#include <QString>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>

#include "QomposeCommon/editor/algorithm/Movement.h"

namespace
{
const QString TEST_DOCUMENT_CONTENTS("Test\n"
                                     " \t \t \tTest\n"
                                     "     Test\n"
                                     "\t\t\t\tTest\n"
                                     "Test\t\tTest\n");

constexpr int TEST_DOCUMENT_BLOCK_COUNT = 6;
}

TEST_CASE("Test goToBlock() algorithm behavior", "[Movement]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS);

	REQUIRE(document.blockCount() == TEST_DOCUMENT_BLOCK_COUNT);

	{
		QTextCursor cursor(&document);
		qompose::editor::algorithm::goToBlock(cursor, -10);
		REQUIRE(cursor.block().blockNumber() == 0);
	}

	{
		QTextCursor cursor(&document);
		qompose::editor::algorithm::goToBlock(cursor, 10);
		REQUIRE(cursor.block().blockNumber() ==
		        TEST_DOCUMENT_BLOCK_COUNT - 1);
	}

	for(int i = 0; i < TEST_DOCUMENT_BLOCK_COUNT; ++i)
	{
		QTextCursor cursor(&document);
		qompose::editor::algorithm::goToBlock(cursor, i);
		REQUIRE(cursor.block().blockNumber() == i);
	}
}

TEST_CASE("Test home() algorithm behavior", "[Movement]")
{
}
