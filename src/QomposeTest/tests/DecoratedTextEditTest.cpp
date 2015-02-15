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

#include "DecoratedTextEditTest.h"

#include <memory>

#include <QColor>
#include <QString>

#include "QomposeCommon/editor/DecoratedTextEdit.h"

namespace
{
/*!
 * This test verifies that the default decorated text edit state is what we
 * expect it to be.
 */
void testDefaultState()
{
	std::shared_ptr<qompose::DecoratedTextEdit> editor(
	        new qompose::DecoratedTextEdit());

	vrfy::assert::assertTrue(editor->isGutterVisible());
	vrfy::assert::assertEquals(editor->font().family(), QString("Courier"));
	vrfy::assert::assertEquals(editor->font().pointSize(), 11);
	vrfy::assert::assertEqualsEpsilon(editor->font().pointSizeF(), 11.0,
	                                  10);
	vrfy::assert::assertEquals(editor->fontZoom(), 0);
	vrfy::assert::assertEqualsEpsilon(editor->fontZoomSize(), 11.0, 10);
	vrfy::assert::assertEquals(editor->getIndentationWidth(), 8);
	vrfy::assert::assertEquals(editor->getIndentationMode(),
	                           qompose::IndentationMode::Tabs);
	vrfy::assert::assertTrue(editor->isWrapGuideVisible());
	vrfy::assert::assertEquals(editor->getWrapGuideColumnWidth(), 100);
	vrfy::assert::assertEquals(editor->getWrapGuideColor(),
	                           QColor(127, 127, 127));
	vrfy::assert::assertEquals(editor->getEditorForeground(),
	                           QColor(0, 0, 0));
	vrfy::assert::assertEquals(editor->getEditorBackground(),
	                           QColor(255, 255, 255));
	vrfy::assert::assertEquals(editor->getCurrentLineColor(),
	                           QColor(70, 72, 61));
	vrfy::assert::assertEquals(editor->getGutterForeground(),
	                           QColor(255, 255, 255));
	vrfy::assert::assertEquals(editor->getGutterBackground(),
	                           QColor(0, 0, 0));
	vrfy::assert::assertEquals(editor->getCurrentLine(), 1);
	vrfy::assert::assertEquals(editor->getCurrentColumn(), 1);
}

void testFontZooming()
{
	std::shared_ptr<qompose::DecoratedTextEdit> editor(
	        new qompose::DecoratedTextEdit());
}
}

namespace qompose
{
namespace test
{
/*!
 * This function executes all of the various decorated text edit unit tests.
 */
void DecoratedTextEditTest::test()
{
	testDefaultState();
	testFontZooming();
}
}
}
