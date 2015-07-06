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

#include <vector>

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
		int position = cursor.position();
		cursor.movePosition(QTextCursor::StartOfBlock,
		                    QTextCursor::MoveAnchor);
		REQUIRE(cursor.position() == position);
	}

	{
		QTextCursor cursor(&document);
		qompose::editor::algorithm::goToBlock(cursor, 10);
		REQUIRE(cursor.block().blockNumber() ==
		        TEST_DOCUMENT_BLOCK_COUNT - 1);
		int position = cursor.position();
		cursor.movePosition(QTextCursor::StartOfBlock,
		                    QTextCursor::MoveAnchor);
		REQUIRE(cursor.position() == position);
	}

	for(int i = 0; i < TEST_DOCUMENT_BLOCK_COUNT; ++i)
	{
		QTextCursor cursor(&document);
		qompose::editor::algorithm::goToBlock(cursor, i);
		REQUIRE(cursor.block().blockNumber() == i);
		int position = cursor.position();
		cursor.movePosition(QTextCursor::StartOfBlock,
		                    QTextCursor::MoveAnchor);
		REQUIRE(cursor.position() == position);
	}
}

namespace
{
struct HomeTestDescriptor
{
	int block;
	int firstPosition;
	int secondPosition;

	HomeTestDescriptor(int b, int f, int s)
	        : block(b), firstPosition(f), secondPosition(s)
	{
	}
};

const std::vector<HomeTestDescriptor> HOME_TESTS = {
        HomeTestDescriptor(0, 0, 0), HomeTestDescriptor(1, 6, 0),
        HomeTestDescriptor(2, 5, 0), HomeTestDescriptor(3, 4, 0),
        HomeTestDescriptor(4, 0, 0), HomeTestDescriptor(5, 0, 0)};
}

TEST_CASE("Test home() algorithm behavior", "[Movement]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS);

	for(const auto &test : HOME_TESTS)
	{
		QTextCursor cursor(&document);
		qompose::editor::algorithm::goToBlock(cursor, test.block);
		int position = cursor.position();
		qompose::editor::algorithm::home(cursor, true);
		REQUIRE(cursor.position() == position);
		cursor.movePosition(QTextCursor::EndOfBlock,
		                    QTextCursor::MoveAnchor);
		qompose::editor::algorithm::home(cursor, true);
		REQUIRE(cursor.position() == position + test.firstPosition);
		qompose::editor::algorithm::home(cursor, true);
		REQUIRE(cursor.position() == position + test.secondPosition);
	}
}
