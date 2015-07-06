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

#include <cmath>
#include <list>
#include <memory>
#include <functional>

#include <QColor>
#include <QString>

#include "QomposeCommon/editor/Editor.h"

namespace
{
/*!
 * \brief This subclass exposes Editor functions useful for testing.
 */
class TestableEditor : public qompose::editor::Editor
{
public:
	TestableEditor() : qompose::editor::Editor(nullptr)
	{
	}

	virtual qreal contentMargin() const
	{
		return qompose::editor::Editor::contentMargin();
	}

	virtual qreal singleColumnWidth() const
	{
		return qompose::editor::Editor::singleColumnWidth();
	}

	virtual qreal columnOffset(int column) const
	{
		return qompose::editor::Editor::columnOffset(column);
	}

	virtual qreal wrapGuideOffset() const
	{
		return qompose::editor::Editor::wrapGuideOffset();
	}
};
}

TEST_CASE("Verify Editor default state", "[Editor]")
{
	auto editor = std::make_shared<qompose::editor::Editor>();

	REQUIRE(editor->isGutterVisible());
	REQUIRE(editor->font().family() == QString("Courier"));
	REQUIRE(editor->font().pointSize() == 11);
	REQUIRE(editor->font().pointSizeF() == Approx(11.0));
	REQUIRE(editor->fontZoom() == 0);
	REQUIRE(editor->fontZoomSize() == Approx(11.0));
	REQUIRE(editor->getIndentationWidth() == 8);
	REQUIRE(editor->getIndentationMode() == qompose::IndentationMode::Tabs);
	REQUIRE(editor->isWrapGuideVisible());
	REQUIRE(editor->getWrapGuideColumnWidth() == 100);
	REQUIRE(editor->getWrapGuideColor() == QColor(127, 127, 127));
	REQUIRE(editor->getEditorForeground() == QColor(0, 0, 0));
	REQUIRE(editor->getEditorBackground() == QColor(255, 255, 255));
	REQUIRE(editor->getCurrentLineColor() == QColor(70, 72, 61));
	REQUIRE(editor->getGutterForeground() == QColor(255, 255, 255));
	REQUIRE(editor->getGutterBackground() == QColor(0, 0, 0));
	REQUIRE(editor->getCurrentLine() == 1);
	REQUIRE(editor->getCurrentColumn() == 1);
}

TEST_CASE("Verify Editor font zooming functionality", "[Editor]")
{
	auto editor = std::make_shared<TestableEditor>();

	// Set a non-default wrap guide column width.

	REQUIRE(editor->getWrapGuideColumnWidth() == 100);
	editor->setWrapGuideColumnWidth(80);
	REQUIRE(editor->getWrapGuideColumnWidth() == 80);

	// Make sure that offsets are always evenly divisible by column widths.

	auto computeOffsetColumnRemainder = [&](int column) -> long double
	{
		long double offset = static_cast<long double>(
		        editor->columnOffset(column) - editor->contentMargin());
		long double single =
		        static_cast<long double>(editor->singleColumnWidth());
		return std::fmod(offset, single);
	};

	auto computeTestOffsetColumnRemainders = [&]() -> std::list<long double>
	{
		return {computeOffsetColumnRemainder(80),
		        computeOffsetColumnRemainder(40),
		        computeOffsetColumnRemainder(120),
		        computeOffsetColumnRemainder(31)};
	};

	for(const auto &remainder : computeTestOffsetColumnRemainders())
		REQUIRE(remainder == Approx(0.0));

	editor->setFontZoom(33);
	for(const auto &remainder : computeTestOffsetColumnRemainders())
		REQUIRE(remainder == Approx(0.0));

	editor->setFontZoom(-33);
	for(const auto &remainder : computeTestOffsetColumnRemainders())
		REQUIRE(remainder == Approx(0.0));

	editor->setFontZoom(331);
	for(const auto &remainder : computeTestOffsetColumnRemainders())
		REQUIRE(remainder == Approx(0.0));
}
