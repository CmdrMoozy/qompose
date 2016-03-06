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

#include <functional>

#include <QKeyEvent>

#include "QomposeCommon/hotkey/HotkeyMap.h"

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

TEST_CASE("Test HotkeyMap function lookup behavior", "[HotkeyMap]")
{
	qompose::HotkeyMap hotkeys;

	// Enter

	hotkeys.addHotkey(qompose::Hotkey(Qt::Key_Enter, nullptr,
	                                  ~Qt::KeyboardModifiers(nullptr)),
	                  std::bind(&doNewline));

	// Tab

	hotkeys.addHotkey(qompose::Hotkey(Qt::Key_Tab), std::bind(&doTab));

	// Shift + Tab

	hotkeys.addHotkey(qompose::Hotkey(Qt::Key_Tab, Qt::ShiftModifier),
	                  std::bind(&decreaseSelectionIndent));

	// Home

	hotkeys.addHotkey(qompose::Hotkey(Qt::Key_Home),
	                  std::bind(&doMoveHome));

	// Shift + Home

	hotkeys.addHotkey(qompose::Hotkey(Qt::Key_Home, Qt::ShiftModifier),
	                  std::bind(&doSelectHome));

	// Ctrl+D

	hotkeys.addHotkey(qompose::Hotkey(Qt::Key_D, Qt::ControlModifier),
	                  std::bind(&duplicateLine));

	// Ctrl+(Zero)

	hotkeys.addHotkey(qompose::Hotkey(Qt::Key_0, Qt::ControlModifier),
	                  std::bind(&resetFontZoom));

	// Ctrl+Shift+Left

	hotkeys.addHotkey(
	        qompose::Hotkey(Qt::Key_Left,
	                        Qt::ControlModifier | Qt::ShiftModifier),
	        std::bind(&doNoop));

	// Ctrl+Shift+Right

	hotkeys.addHotkey(
	        qompose::Hotkey(Qt::Key_Right,
	                        Qt::ControlModifier | Qt::ShiftModifier),
	        std::bind(&doNoop));

	// Ctrl+Insert

	hotkeys.addHotkey(qompose::Hotkey(Qt::Key_Insert, Qt::ControlModifier),
	                  std::bind(&doNoop));

	// Ctrl+K

	hotkeys.addHotkey(qompose::Hotkey(Qt::Key_K, Qt::ControlModifier),
	                  std::bind(&doNoop));

	// Shift+Insert

	hotkeys.addHotkey(qompose::Hotkey(Qt::Key_Insert, Qt::ShiftModifier),
	                  std::bind(&doNoop));

	// Shift+Delete

	hotkeys.addHotkey(qompose::Hotkey(Qt::Key_Delete, Qt::ShiftModifier),
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

	auto updateLastExecuted =
	        [&hotkeys](const QKeyEvent &event) -> TestFunction
	{
		auto function = hotkeys.getHotkeyHandler(&event);

		if(function == nullptr)
			LAST_EXECUTED = TestFunction::None;
		else
			(*function)();

		return LAST_EXECUTED;
	};

	REQUIRE(updateLastExecuted(enterEvent) == TestFunction::DoNewline);
	REQUIRE(updateLastExecuted(tabEvent) == TestFunction::DoTab);
	REQUIRE(updateLastExecuted(backtabEvent) ==
	        TestFunction::DecreaseSelectionIndent);
	REQUIRE(updateLastExecuted(homeEvent) == TestFunction::DoMoveHome);
	REQUIRE(updateLastExecuted(selectHomeEvent) ==
	        TestFunction::DoSelectHome);
	REQUIRE(updateLastExecuted(duplicateEvent) ==
	        TestFunction::DuplicateLine);
	REQUIRE(updateLastExecuted(resetZoomEvent) ==
	        TestFunction::ResetFontZoom);
	REQUIRE(updateLastExecuted(ctrlShiftLeftEvent) == TestFunction::DoNoop);
	REQUIRE(updateLastExecuted(ctrlShiftRightEvent) ==
	        TestFunction::DoNoop);
	REQUIRE(updateLastExecuted(ctrlInsertEvent) == TestFunction::DoNoop);
	REQUIRE(updateLastExecuted(ctrlKEvent) == TestFunction::DoNoop);
	REQUIRE(updateLastExecuted(shiftInsertEvent) == TestFunction::DoNoop);
	REQUIRE(updateLastExecuted(shiftDeleteEvent) == TestFunction::DoNoop);
	REQUIRE(updateLastExecuted(ctrlShiftTabEvent) == TestFunction::None);
	REQUIRE(updateLastExecuted(ctrlShiftHomeEvent) == TestFunction::None);
	REQUIRE(updateLastExecuted(ctrlAEvent) == TestFunction::None);
}
