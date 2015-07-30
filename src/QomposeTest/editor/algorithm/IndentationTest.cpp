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
const QString TEST_DOCUMENT_CONTENTS(
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

constexpr int TEST_DOCUMENT_BLOCK_COUNT = 45;

constexpr int INDENT_SELECTION_START_BLOCK = 25;
constexpr int INDENT_SELECTION_END_BLOCK = 39;
constexpr int INDENT_SELECTION_BLOCK_COUNT =
        INDENT_SELECTION_END_BLOCK - INDENT_SELECTION_START_BLOCK + 1;
}

TEST_CASE("Test increasing full forward selection tab indent", "[Indentation]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS);
	REQUIRE(TEST_DOCUMENT_BLOCK_COUNT == document.blockCount());

	QTextCursor cursor(&document);

	// Move the anchor to the start of the first block, and the cursor to
	// the end of the last block.
	qompose::editor::algorithm::goToBlock(cursor,
	                                      INDENT_SELECTION_END_BLOCK);
	cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
	int endPosition = cursor.position();
	qompose::editor::algorithm::goToBlock(cursor,
	                                      INDENT_SELECTION_START_BLOCK);
	cursor.setPosition(endPosition, QTextCursor::KeepAnchor);

	// Indent the blocks in the selection. Check that the expected number
	// of tabs were added to the document.
	int originalCharacterCount = document.characterCount();
	qompose::editor::algorithm::increaseSelectionIndent(
	        cursor, qompose::IndentationMode::Tabs, TEST_INDENTATION_WIDTH);
	int charactersAdded =
	        document.characterCount() - originalCharacterCount;
	CHECK(INDENT_SELECTION_BLOCK_COUNT == charactersAdded);

	// Verify that the new cursor anchor and position are correct.
	int anchorPosition = cursor.anchor();
	int cursorPosition = cursor.position();
	CHECK(anchorPosition <= cursorPosition);
	cursor.setPosition(anchorPosition, QTextCursor::MoveAnchor);
	CHECK(cursor.atBlockStart());
	CHECK(INDENT_SELECTION_START_BLOCK == cursor.blockNumber());
	cursor.setPosition(cursorPosition, QTextCursor::MoveAnchor);

	CHECK(cursor.atBlockEnd());
	CHECK(INDENT_SELECTION_END_BLOCK == cursor.blockNumber());
}

TEST_CASE("Test increasing partial forward selection tab indent",
          "[Indentation]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS);
	REQUIRE(TEST_DOCUMENT_BLOCK_COUNT == document.blockCount());

	QTextCursor cursor(&document);

	// Move the anchor to the middle of the first block, and the cursor to
	// the middle of the last block.
	qompose::editor::algorithm::goToBlock(cursor,
	                                      INDENT_SELECTION_END_BLOCK);
	cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor,
	                    std::max(1, cursor.block().length() / 2));
	REQUIRE(INDENT_SELECTION_END_BLOCK == cursor.blockNumber());
	REQUIRE(!cursor.atBlockStart());
	REQUIRE(!cursor.atBlockEnd());
	int endPosition = cursor.position();
	qompose::editor::algorithm::goToBlock(cursor,
	                                      INDENT_SELECTION_START_BLOCK);
	cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor,
	                    std::max(1, cursor.block().length() / 2));
	REQUIRE(INDENT_SELECTION_START_BLOCK == cursor.blockNumber());
	REQUIRE(!cursor.atBlockStart());
	REQUIRE(!cursor.atBlockEnd());
	cursor.setPosition(endPosition, QTextCursor::KeepAnchor);

	// Indent the blocks in the selection. Check that the expected number
	// of tabs were added to the document.
	int originalCharacterCount = document.characterCount();
	qompose::editor::algorithm::increaseSelectionIndent(
	        cursor, qompose::IndentationMode::Tabs, TEST_INDENTATION_WIDTH);
	int charactersAdded =
	        document.characterCount() - originalCharacterCount;
	CHECK(INDENT_SELECTION_BLOCK_COUNT == charactersAdded);

	// Verify that the new cursor anchor and position are correct.
	int anchorPosition = cursor.anchor();
	int cursorPosition = cursor.position();
	CHECK(anchorPosition <= cursorPosition);
	cursor.setPosition(anchorPosition, QTextCursor::MoveAnchor);
	CHECK(cursor.atBlockStart());
	CHECK(INDENT_SELECTION_START_BLOCK == cursor.blockNumber());
	cursor.setPosition(cursorPosition, QTextCursor::MoveAnchor);

	CHECK(cursor.atBlockEnd());
	CHECK(INDENT_SELECTION_END_BLOCK == cursor.blockNumber());
}

TEST_CASE("Test increasing full forward selection space indent",
          "[Indentation]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS);
	REQUIRE(TEST_DOCUMENT_BLOCK_COUNT == document.blockCount());

	QTextCursor cursor(&document);

	// Move the anchor to the start of the first block, and the cursor to
	// the end of the last block.
	qompose::editor::algorithm::goToBlock(cursor,
	                                      INDENT_SELECTION_END_BLOCK);
	cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
	int endPosition = cursor.position();
	qompose::editor::algorithm::goToBlock(cursor,
	                                      INDENT_SELECTION_START_BLOCK);
	cursor.setPosition(endPosition, QTextCursor::KeepAnchor);

	// Indent the blocks in the selection. Check that the expected number
	// of tabs were added to the document.
	int originalCharacterCount = document.characterCount();
	qompose::editor::algorithm::increaseSelectionIndent(
	        cursor, qompose::IndentationMode::Spaces,
	        TEST_INDENTATION_WIDTH);
	int charactersAdded =
	        document.characterCount() - originalCharacterCount;
	CHECK((INDENT_SELECTION_BLOCK_COUNT * TEST_INDENTATION_WIDTH) ==
	      charactersAdded);

	// Verify that the new cursor anchor and position are correct.
	int anchorPosition = cursor.anchor();
	int cursorPosition = cursor.position();
	CHECK(anchorPosition <= cursorPosition);
	cursor.setPosition(anchorPosition, QTextCursor::MoveAnchor);
	CHECK(cursor.atBlockStart());
	CHECK(INDENT_SELECTION_START_BLOCK == cursor.blockNumber());
	cursor.setPosition(cursorPosition, QTextCursor::MoveAnchor);

	CHECK(cursor.atBlockEnd());
	CHECK(INDENT_SELECTION_END_BLOCK == cursor.blockNumber());
}

TEST_CASE("Test increasing partial forward selection space indent",
          "[Indentation]")
{
	QTextDocument document(TEST_DOCUMENT_CONTENTS);
	REQUIRE(TEST_DOCUMENT_BLOCK_COUNT == document.blockCount());

	QTextCursor cursor(&document);

	// Move the anchor to the middle of the first block, and the cursor to
	// the middle of the last block.
	qompose::editor::algorithm::goToBlock(cursor,
	                                      INDENT_SELECTION_END_BLOCK);
	cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor,
	                    std::max(1, cursor.block().length() / 2));
	REQUIRE(INDENT_SELECTION_END_BLOCK == cursor.blockNumber());
	REQUIRE(!cursor.atBlockStart());
	REQUIRE(!cursor.atBlockEnd());
	int endPosition = cursor.position();
	qompose::editor::algorithm::goToBlock(cursor,
	                                      INDENT_SELECTION_START_BLOCK);
	cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor,
	                    std::max(1, cursor.block().length() / 2));
	REQUIRE(INDENT_SELECTION_START_BLOCK == cursor.blockNumber());
	REQUIRE(!cursor.atBlockStart());
	REQUIRE(!cursor.atBlockEnd());
	cursor.setPosition(endPosition, QTextCursor::KeepAnchor);

	// Indent the blocks in the selection. Check that the expected number
	// of tabs were added to the document.
	int originalCharacterCount = document.characterCount();
	qompose::editor::algorithm::increaseSelectionIndent(
	        cursor, qompose::IndentationMode::Spaces,
	        TEST_INDENTATION_WIDTH);
	int charactersAdded =
	        document.characterCount() - originalCharacterCount;
	CHECK((INDENT_SELECTION_BLOCK_COUNT * TEST_INDENTATION_WIDTH) ==
	      charactersAdded);

	// Verify that the new cursor anchor and position are correct.
	int anchorPosition = cursor.anchor();
	int cursorPosition = cursor.position();
	CHECK(anchorPosition <= cursorPosition);
	cursor.setPosition(anchorPosition, QTextCursor::MoveAnchor);
	CHECK(cursor.atBlockStart());
	CHECK(INDENT_SELECTION_START_BLOCK == cursor.blockNumber());
	cursor.setPosition(cursorPosition, QTextCursor::MoveAnchor);

	CHECK(cursor.atBlockEnd());
	CHECK(INDENT_SELECTION_END_BLOCK == cursor.blockNumber());
}
