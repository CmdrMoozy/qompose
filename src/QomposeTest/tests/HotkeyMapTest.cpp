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

#include "HotkeyMapTest.h"

#include <functional>

#include <QKeyEvent>

#include "QomposeCommon/util/HotkeyMap.h"

namespace
{
enum class TestFunction
{
	DoNewline,
	DoTab,
	DecreaseSelectionIndent,
	DoMoveHome,
	DoSelectHome,
	DuplicateLine,
	ResetFontZoom,
	DoNoop,
	None
};

TestFunction LAST_EXECUTED = TestFunction::None;

void doNewline()
{
	LAST_EXECUTED = TestFunction::DoNewline;
}

void doTab()
{
	LAST_EXECUTED = TestFunction::DoTab;
}

void decreaseSelectionIndent()
{
	LAST_EXECUTED = TestFunction::DecreaseSelectionIndent;
}

void doMoveHome()
{
	LAST_EXECUTED = TestFunction::DoMoveHome;
}

void doSelectHome()
{
	LAST_EXECUTED = TestFunction::DoSelectHome;
}

void duplicateLine()
{
	LAST_EXECUTED = TestFunction::DuplicateLine;
}

void resetFontZoom()
{
	LAST_EXECUTED = TestFunction::ResetFontZoom;
}

void doNoop()
{
	LAST_EXECUTED = TestFunction::DoNoop;
}
}

namespace qompose
{
namespace test
{
/*!
 * We implement our superclass's test() function to perform a test against the
 * HotkeyMap implementation. We'll setup a "typical" hotkey setup, and
 * then test a series of example QKeyEvents against it.
 */
void HotkeyMapTest::test()
{
	HotkeyMap hotkeys;

	// Enter

	hotkeys.addHotkey(
	        Hotkey(Qt::Key_Enter, nullptr, ~Qt::KeyboardModifiers(nullptr)),
	        std::bind(&doNewline));

	// Tab

	hotkeys.addHotkey(Hotkey(Qt::Key_Tab), std::bind(&doTab));

	// Shift + Tab

	hotkeys.addHotkey(Hotkey(Qt::Key_Tab, Qt::ShiftModifier),
	                  std::bind(&decreaseSelectionIndent));

	// Home

	hotkeys.addHotkey(Hotkey(Qt::Key_Home), std::bind(&doMoveHome));

	// Shift + Home

	hotkeys.addHotkey(Hotkey(Qt::Key_Home, Qt::ShiftModifier),
	                  std::bind(&doSelectHome));

	// Ctrl+D

	hotkeys.addHotkey(Hotkey(Qt::Key_D, Qt::ControlModifier),
	                  std::bind(&duplicateLine));

	// Ctrl+(Zero)

	hotkeys.addHotkey(Hotkey(Qt::Key_0, Qt::ControlModifier),
	                  std::bind(&resetFontZoom));

	// Ctrl+Shift+Left

	hotkeys.addHotkey(
	        Hotkey(Qt::Key_Left, Qt::ControlModifier | Qt::ShiftModifier),
	        std::bind(&doNoop));

	// Ctrl+Shift+Right

	hotkeys.addHotkey(
	        Hotkey(Qt::Key_Right, Qt::ControlModifier | Qt::ShiftModifier),
	        std::bind(&doNoop));

	// Ctrl+Insert

	hotkeys.addHotkey(Hotkey(Qt::Key_Insert, Qt::ControlModifier),
	                  std::bind(&doNoop));

	// Ctrl+K

	hotkeys.addHotkey(Hotkey(Qt::Key_K, Qt::ControlModifier),
	                  std::bind(&doNoop));

	// Shift+Insert

	hotkeys.addHotkey(Hotkey(Qt::Key_Insert, Qt::ShiftModifier),
	                  std::bind(&doNoop));

	// Shift+Delete

	hotkeys.addHotkey(Hotkey(Qt::Key_Delete, Qt::ShiftModifier),
	                  std::bind(&doNoop));

	// Test a series of example QKeyEvents against this hotkey mapping.

	QKeyEvent enterEvent(QKeyEvent::KeyPress, Qt::Key_Enter, nullptr);
	QKeyEvent tabEvent(QKeyEvent::KeyPress, Qt::Key_Tab, nullptr);
	QKeyEvent backtabEvent(QKeyEvent::KeyPress, Qt::Key_Tab,
	                       Qt::KeyboardModifiers(Qt::ShiftModifier));
	QKeyEvent homeEvent(QKeyEvent::KeyPress, Qt::Key_Home, nullptr);
	QKeyEvent selectHomeEvent(QKeyEvent::KeyPress, Qt::Key_Home,
	                          Qt::KeyboardModifiers(Qt::ShiftModifier));
	QKeyEvent duplicateEvent(QKeyEvent::KeyPress, Qt::Key_D,
	                         Qt::KeyboardModifiers(Qt::ControlModifier));
	QKeyEvent resetZoomEvent(QKeyEvent::KeyPress, Qt::Key_0,
	                         Qt::KeyboardModifiers(Qt::ControlModifier));
	QKeyEvent ctrlShiftLeftEvent(QKeyEvent::KeyPress, Qt::Key_Left,
	                             Qt::ControlModifier | Qt::ShiftModifier);
	QKeyEvent ctrlShiftRightEvent(QKeyEvent::KeyPress, Qt::Key_Right,
	                              Qt::ControlModifier | Qt::ShiftModifier);
	QKeyEvent ctrlInsertEvent(QKeyEvent::KeyPress, Qt::Key_Insert,
	                          Qt::KeyboardModifiers(Qt::ControlModifier));
	QKeyEvent ctrlKEvent(QKeyEvent::KeyPress, Qt::Key_K,
	                     Qt::KeyboardModifiers(Qt::ControlModifier));
	QKeyEvent shiftInsertEvent(QKeyEvent::KeyPress, Qt::Key_Insert,
	                           Qt::KeyboardModifiers(Qt::ShiftModifier));
	QKeyEvent shiftDeleteEvent(QKeyEvent::KeyPress, Qt::Key_Delete,
	                           Qt::KeyboardModifiers(Qt::ShiftModifier));

	QKeyEvent ctrlShiftTabEvent(QKeyEvent::KeyPress, Qt::Key_Tab,
	                            Qt::ControlModifier | Qt::ShiftModifier);
	QKeyEvent ctrlShiftHomeEvent(QKeyEvent::KeyPress, Qt::Key_Home,
	                             Qt::ControlModifier | Qt::ShiftModifier);
	QKeyEvent ctrlAEvent(QKeyEvent::KeyPress, Qt::Key_A,
	                     Qt::KeyboardModifiers(Qt::ControlModifier));

	auto testHotkey =
	        [&hotkeys](const QKeyEvent &event, TestFunction expected)
	{
		auto function = hotkeys.getHotkeyHandler(&event);

		if(function == nullptr)
			LAST_EXECUTED = TestFunction::None;
		else
			(*function)();

		vrfy::assert::assertEquals(LAST_EXECUTED, expected);
	};

	testHotkey(enterEvent, TestFunction::DoNewline);
	testHotkey(tabEvent, TestFunction::DoTab);
	testHotkey(backtabEvent, TestFunction::DecreaseSelectionIndent);
	testHotkey(homeEvent, TestFunction::DoMoveHome);
	testHotkey(selectHomeEvent, TestFunction::DoSelectHome);
	testHotkey(duplicateEvent, TestFunction::DuplicateLine);
	testHotkey(resetZoomEvent, TestFunction::ResetFontZoom);
	testHotkey(ctrlShiftLeftEvent, TestFunction::DoNoop);
	testHotkey(ctrlShiftRightEvent, TestFunction::DoNoop);
	testHotkey(ctrlInsertEvent, TestFunction::DoNoop);
	testHotkey(ctrlKEvent, TestFunction::DoNoop);
	testHotkey(shiftInsertEvent, TestFunction::DoNoop);
	testHotkey(shiftDeleteEvent, TestFunction::DoNoop);
	testHotkey(ctrlShiftTabEvent, TestFunction::None);
	testHotkey(ctrlShiftHomeEvent, TestFunction::None);
	testHotkey(ctrlAEvent, TestFunction::None);
}
}
}
