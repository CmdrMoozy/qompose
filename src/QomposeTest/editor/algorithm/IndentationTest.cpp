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

#include <QString>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>

#include "QomposeCommon/editor/algorithm/Indentation.h"
#include "QomposeCommon/editor/algorithm/Movement.h"

namespace
{
constexpr std::size_t TEST_INDENTATION_WIDTH = 8;

// clang-format off
const QString TEST_DOCUMENT_CONTENTS_TABS(
	"/*!\n"
	" * This function performs the first step of decreaseSelectionIndent. Namely,\n"
	" * we try to remove at most one full indentation string from the beginning of\n"
	" * each block in the selection, and we return true if at least one block was\n"
	" * modified in this way.\n"
	" *\n"
	" * \\param cursor The cursor to operate on.\n"
	" * \\param state The cursor's original selection state.\n"
	" * \\param mode The indentation mode to use.\n"
	" * \\param width The indentation width to use.\n"
	" * \\return True if at least one block was successfully de-indented.\n"
	" */\n"
	"bool deindentSelection(\n"
	"        QTextCursor &cursor,\n"
	"        qompose::editor::algorithm::CursorSelectionState const &state,\n"
	"        qompose::IndentationMode mode, std::size_t width)\n"
	"{\n"
	"	bool foundIndent = false;\n"
	"	QString indentStr =\n"
	"	        qompose::editor::algorithm::getIndentationString(mode, width);\n\n"
	"	qompose::editor::algorithm::foreachBlock(\n"
	"	        cursor, state.startPosition, state.blockCount,\n"
	"	        [width, &indentStr, &foundIndent](QTextCursor &c)\n"
	"	        {\n"
	"		        // If this block starts with our indentation string, we\n"
	"		        // want to remove that one instance of it from the\n"
	"		        // block.\n"
	"		        if(static_cast<std::size_t>(c.block().length()) >=\n"
	"		           width)\n"
	"		        {\n"
	"			        c.movePosition(QTextCursor::NextCharacter,\n"
	"			                       QTextCursor::KeepAnchor, width);\n"
	"		        }\n\n"
	"		        if(c.hasSelection() && (c.selectedText() == indentStr))\n"
	"		        {\n"
	"			        c.removeSelectedText();\n"
	"			        foundIndent = true;\n"
	"		        }\n"
	"		});\n\n"
	"	return foundIndent;\n"
	"}\n"
);
// clang-format on

const QString TEST_DOCUMENT_CONTENTS_SPACES(
        QString(TEST_DOCUMENT_CONTENTS_TABS)
                .replace('\t', QString(TEST_INDENTATION_WIDTH, ' ')));

constexpr int TEST_DOCUMENT_BLOCK_COUNT = 45;

constexpr int INDENT_SELECTION_START_BLOCK = 25;
constexpr int INDENT_SELECTION_END_BLOCK = 39;
constexpr int INDENT_SELECTION_BLOCK_COUNT =
        INDENT_SELECTION_END_BLOCK - INDENT_SELECTION_START_BLOCK + 1;

enum class BlockRelativePosition
{
	START,
	MIDDLE,
	END
};

int blockOf(QTextDocument &document, int position)
{
	QTextCursor cursor(&document);
	cursor.setPosition(position, QTextCursor::MoveAnchor);
	return cursor.blockNumber();
}

bool inMiddleOfBlock(QTextDocument &document, int block, int position)
{
	if(blockOf(document, position) != block)
		return false;

	QTextCursor cursor(&document);
	cursor.setPosition(position, QTextCursor::MoveAnchor);
	return (!cursor.atBlockStart()) && (!cursor.atBlockEnd());
}

void goToBlock(QTextCursor &cursor, int block,
               BlockRelativePosition relativePosition, bool moveAnchor)
{
	QTextCursor positionFinder(cursor);
	qompose::editor::algorithm::goToBlock(positionFinder, block);
	switch(relativePosition)
	{
	case BlockRelativePosition::MIDDLE:
		positionFinder.movePosition(
		        QTextCursor::NextCharacter, QTextCursor::MoveAnchor,
		        positionFinder.block().length() / 2);
		break;

	case BlockRelativePosition::END:
		positionFinder.movePosition(QTextCursor::EndOfBlock,
		                            QTextCursor::MoveAnchor);
		break;

	default:
		break;
	}
	int position = positionFinder.position();

	cursor.setPosition(position, moveAnchor ? QTextCursor::MoveAnchor
	                                        : QTextCursor::KeepAnchor);
}

bool tryIndentSelection(QTextDocument const &document, QTextCursor &cursor,
                        qompose::IndentationMode mode)
{
	int originalCharacterCount = document.characterCount();
	qompose::editor::algorithm::increaseSelectionIndent(
	        cursor, mode, TEST_INDENTATION_WIDTH);
	int charactersChanged =
	        document.characterCount() - originalCharacterCount;
	int expectedChanged =
	        mode == qompose::IndentationMode::Tabs
	                ? INDENT_SELECTION_BLOCK_COUNT
	                : INDENT_SELECTION_BLOCK_COUNT * TEST_INDENTATION_WIDTH;
	return charactersChanged == expectedChanged;
}

bool postIndentSelectionCursorIsCorrect(QTextCursor const &c)
{
	QTextCursor cursor(c);
	int anchor = cursor.anchor();
	int position = cursor.position();

	// The cursor's selection should be forward; the anchor should come
	// before the position.
	if(anchor > position)
		return false;

	// The anchor should be at the start of the first block we indented.
	cursor.setPosition(anchor, QTextCursor::MoveAnchor);
	if(!cursor.atBlockStart() ||
	   (cursor.blockNumber() != INDENT_SELECTION_START_BLOCK))
	{
		return false;
	}

	// The position should be at the end of the last block we indented.
	cursor.setPosition(position, QTextCursor::MoveAnchor);
	if(!cursor.atBlockEnd() ||
	   (cursor.blockNumber() != INDENT_SELECTION_END_BLOCK))
	{
		return false;
	}

	// Looks like the cursor is correct!
	return true;
}
}

TEST_CASE("Test increasing full forward selection tab indent", "[Indentation]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS_TABS);
	REQUIRE(TEST_DOCUMENT_BLOCK_COUNT == document.blockCount());

	QTextCursor cursor(&document);
	goToBlock(cursor, INDENT_SELECTION_START_BLOCK,
	          BlockRelativePosition::START, true);
	goToBlock(cursor, INDENT_SELECTION_END_BLOCK,
	          BlockRelativePosition::END, false);

	CHECK(tryIndentSelection(document, cursor,
	                         qompose::IndentationMode::Tabs));

	CHECK(postIndentSelectionCursorIsCorrect(cursor));
}

TEST_CASE("Test increasing partial forward selection tab indent",
          "[Indentation]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS_TABS);
	REQUIRE(TEST_DOCUMENT_BLOCK_COUNT == document.blockCount());

	QTextCursor cursor(&document);
	goToBlock(cursor, INDENT_SELECTION_START_BLOCK,
	          BlockRelativePosition::MIDDLE, true);
	REQUIRE(inMiddleOfBlock(document, INDENT_SELECTION_START_BLOCK,
	                        cursor.position()));
	goToBlock(cursor, INDENT_SELECTION_END_BLOCK,
	          BlockRelativePosition::MIDDLE, false);
	REQUIRE(inMiddleOfBlock(document, INDENT_SELECTION_END_BLOCK,
	                        cursor.position()));

	CHECK(tryIndentSelection(document, cursor,
	                         qompose::IndentationMode::Tabs));

	CHECK(postIndentSelectionCursorIsCorrect(cursor));
}

TEST_CASE("Test increasing full forward selection space indent",
          "[Indentation]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS_SPACES);
	REQUIRE(TEST_DOCUMENT_BLOCK_COUNT == document.blockCount());

	QTextCursor cursor(&document);
	goToBlock(cursor, INDENT_SELECTION_START_BLOCK,
	          BlockRelativePosition::START, true);
	goToBlock(cursor, INDENT_SELECTION_END_BLOCK,
	          BlockRelativePosition::END, false);

	CHECK(tryIndentSelection(document, cursor,
	                         qompose::IndentationMode::Tabs));

	CHECK(postIndentSelectionCursorIsCorrect(cursor));
}

TEST_CASE("Test increasing partial forward selection space indent",
          "[Indentation]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS_SPACES);
	REQUIRE(TEST_DOCUMENT_BLOCK_COUNT == document.blockCount());

	QTextCursor cursor(&document);
	goToBlock(cursor, INDENT_SELECTION_START_BLOCK,
	          BlockRelativePosition::MIDDLE, true);
	REQUIRE(inMiddleOfBlock(document, INDENT_SELECTION_START_BLOCK,
	                        cursor.position()));
	goToBlock(cursor, INDENT_SELECTION_END_BLOCK,
	          BlockRelativePosition::MIDDLE, false);
	REQUIRE(inMiddleOfBlock(document, INDENT_SELECTION_END_BLOCK,
	                        cursor.position()));

	CHECK(tryIndentSelection(document, cursor,
	                         qompose::IndentationMode::Tabs));

	CHECK(postIndentSelectionCursorIsCorrect(cursor));
}

TEST_CASE("Test increasing full backward selection tab indent", "[Indentation]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS_TABS);
	REQUIRE(TEST_DOCUMENT_BLOCK_COUNT == document.blockCount());

	QTextCursor cursor(&document);
	goToBlock(cursor, INDENT_SELECTION_START_BLOCK,
	          BlockRelativePosition::END, true);
	goToBlock(cursor, INDENT_SELECTION_END_BLOCK,
	          BlockRelativePosition::START, false);

	CHECK(tryIndentSelection(document, cursor,
	                         qompose::IndentationMode::Tabs));

	CHECK(postIndentSelectionCursorIsCorrect(cursor));
}

TEST_CASE("Test increasing partial backward selection tab indent",
          "[Indentation]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS_TABS);
	REQUIRE(TEST_DOCUMENT_BLOCK_COUNT == document.blockCount());

	QTextCursor cursor(&document);
	goToBlock(cursor, INDENT_SELECTION_END_BLOCK,
	          BlockRelativePosition::MIDDLE, true);
	REQUIRE(inMiddleOfBlock(document, INDENT_SELECTION_END_BLOCK,
	                        cursor.position()));
	goToBlock(cursor, INDENT_SELECTION_START_BLOCK,
	          BlockRelativePosition::MIDDLE, false);
	REQUIRE(inMiddleOfBlock(document, INDENT_SELECTION_START_BLOCK,
	                        cursor.position()));

	CHECK(tryIndentSelection(document, cursor,
	                         qompose::IndentationMode::Tabs));

	CHECK(postIndentSelectionCursorIsCorrect(cursor));
}

TEST_CASE("Test increasing full backward selection space indent",
          "[Indentation]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS_SPACES);
	REQUIRE(TEST_DOCUMENT_BLOCK_COUNT == document.blockCount());

	QTextCursor cursor(&document);
	goToBlock(cursor, INDENT_SELECTION_START_BLOCK,
	          BlockRelativePosition::END, true);
	goToBlock(cursor, INDENT_SELECTION_END_BLOCK,
	          BlockRelativePosition::START, false);

	CHECK(tryIndentSelection(document, cursor,
	                         qompose::IndentationMode::Tabs));

	CHECK(postIndentSelectionCursorIsCorrect(cursor));
}

TEST_CASE("Test increasing partial backward selection space indent",
          "[Indentation]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS_SPACES);
	REQUIRE(TEST_DOCUMENT_BLOCK_COUNT == document.blockCount());

	QTextCursor cursor(&document);
	goToBlock(cursor, INDENT_SELECTION_END_BLOCK,
	          BlockRelativePosition::MIDDLE, true);
	REQUIRE(inMiddleOfBlock(document, INDENT_SELECTION_END_BLOCK,
	                        cursor.position()));
	goToBlock(cursor, INDENT_SELECTION_START_BLOCK,
	          BlockRelativePosition::MIDDLE, false);
	REQUIRE(inMiddleOfBlock(document, INDENT_SELECTION_START_BLOCK,
	                        cursor.position()));

	CHECK(tryIndentSelection(document, cursor,
	                         qompose::IndentationMode::Tabs));

	CHECK(postIndentSelectionCursorIsCorrect(cursor));
}
