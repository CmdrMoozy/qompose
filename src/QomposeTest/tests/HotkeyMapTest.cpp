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

#include <QKeyEvent>

#include "QomposeCommon/util/QomposeHotkeyMap.h"

namespace qompose
{
namespace test
{

/*!
 * This is our default constructor, which creates a new insance of our test.
 */
HotkeyMapTest::HotkeyMapTest()
{
}

/*!
 * This is our default destructor, which cleans up an destroys this test
 * instance.
 */
HotkeyMapTest::~HotkeyMapTest()
{
}

/*!
 * We implement our superclass's test() function to perform a test against the
 * QomposeHotkeyMap implementation. We'll setup a "typical" hotkey setup, and
 * then test a series of example QKeyEvents against it.
 */
void HotkeyMapTest::test()
{
	QomposeHotkeyMap<int *> hotkeys;

	int doNewline = 0;
	int doTab = 0;
	int decreaseSelectionIndent = 0;
	int doMoveHome = 0;
	int doSelectHome = 0;
	int duplicateLine = 0;
	int resetFontZoom = 0;
	int doNoop = 0;

	// Enter

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_Enter, nullptr,
		~Qt::KeyboardModifiers(nullptr)), &doNewline);

	// Tab

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_Tab), &doTab);

	// Shift + Tab

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_Tab, Qt::ShiftModifier),
		&decreaseSelectionIndent);

	// Home

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_Home),
		&doMoveHome);

	// Shift + Home

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_Home, Qt::ShiftModifier),
		&doSelectHome);

	// Ctrl+D

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_D, Qt::ControlModifier),
		&duplicateLine);

	// Ctrl+(Zero)

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_0, Qt::ControlModifier),
		&resetFontZoom);

	// Ctrl+Shift+Left

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_Left, Qt::ControlModifier |
		Qt::ShiftModifier), &doNoop);

	// Ctrl+Shift+Right

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_Right, Qt::ControlModifier |
		Qt::ShiftModifier), &doNoop);

	// Ctrl+Insert

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_Insert, Qt::ControlModifier),
		&doNoop);

	// Ctrl+K

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_K, Qt::ControlModifier),
		&doNoop);

	// Shift+Insert

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_Insert, Qt::ShiftModifier),
		&doNoop);

	// Shift+Delete

	hotkeys.addHotkey(QomposeHotkey(Qt::Key_Delete, Qt::ShiftModifier),
		&doNoop);

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
	QKeyEvent ctrkKEvent(QKeyEvent::KeyPress, Qt::Key_K,
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

	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&enterEvent), &doNewline);
	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&tabEvent), &doTab);
	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&backtabEvent), &decreaseSelectionIndent);
	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&homeEvent), &doMoveHome);
	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&selectHomeEvent), &doSelectHome);
	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&duplicateEvent), &duplicateLine);
	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&resetZoomEvent), &resetFontZoom);
	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&ctrlShiftLeftEvent), &doNoop);
	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&ctrlShiftRightEvent), &doNoop);
	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&ctrlInsertEvent), &doNoop);
	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&ctrkKEvent), &doNoop);
	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&shiftInsertEvent), &doNoop);
	Test::assertEquals(*hotkeys.getHotkeyHandler(
		&shiftDeleteEvent), &doNoop);

	Test::assertEquals<int * const *>(hotkeys.getHotkeyHandler(
		&ctrlShiftTabEvent), nullptr);
	Test::assertEquals<int * const *>(hotkeys.getHotkeyHandler(
		&ctrlShiftHomeEvent), nullptr);
	Test::assertEquals<int * const *>(hotkeys.getHotkeyHandler(
		&ctrlAEvent), nullptr);
}

}
}
