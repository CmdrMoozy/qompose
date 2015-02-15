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

#include "HotkeyTest.h"

#include <QKeyEvent>

#include "QomposeCommon/util/Hotkey.h"

namespace qompose
{
namespace test
{
/*!
 * We implement our superclass's test() function to perform our various tests
 * against the Hotkey class.
 */
void HotkeyTest::test()
{
	testHotkeyConstruction();
	testHotkeyCopying();
	testHotkeyMatching();
}

/*!
 * This function tests that the various Hotkey constructors initialize
 * the resulting object's internal state properly.
 */
void HotkeyTest::testHotkeyConstruction()
{
	Hotkey a(Qt::Key_A);

	vrfy::assert::assertEquals(a.getKey(), Qt::Key_A);
	vrfy::assert::assertEquals(a.getKeyInteger(),
	                           static_cast<quint64>(Qt::Key_A));
	vrfy::assert::assertEquals(a.getRequiredModifiers(),
	                           Qt::KeyboardModifiers(Qt::NoModifier));
	vrfy::assert::assertEquals(a.getWhitelistedModifiers(),
	                           Qt::KeyboardModifiers(Qt::NoModifier));

	Hotkey b(Qt::Key_A, Qt::ControlModifier);

	vrfy::assert::assertEquals(b.getKey(), Qt::Key_A);
	vrfy::assert::assertEquals(b.getKeyInteger(),
	                           static_cast<quint64>(Qt::Key_A));
	vrfy::assert::assertEquals(b.getRequiredModifiers(),
	                           Qt::KeyboardModifiers(Qt::ControlModifier));
	vrfy::assert::assertEquals(b.getWhitelistedModifiers(),
	                           Qt::KeyboardModifiers(Qt::ControlModifier));

	Hotkey c(Qt::Key_A, Qt::ControlModifier, Qt::ShiftModifier);

	vrfy::assert::assertEquals(c.getKey(), Qt::Key_A);
	vrfy::assert::assertEquals(c.getKeyInteger(),
	                           static_cast<quint64>(Qt::Key_A));
	vrfy::assert::assertEquals(c.getRequiredModifiers(),
	                           Qt::KeyboardModifiers(Qt::ControlModifier));
	vrfy::assert::assertEquals(c.getWhitelistedModifiers(),
	                           Qt::ControlModifier | Qt::ShiftModifier);
}

/*!
 * This function tests that the assignment operator, copy constructor, and
 * equivalence operator from the Hotkey class work properly.
 */
void HotkeyTest::testHotkeyCopying()
{
	Hotkey a(Qt::Key_A, Qt::ControlModifier, Qt::ShiftModifier);
	Hotkey b(Qt::Key_B);

	vrfy::assert::assertTrue(!(a == b));

	Hotkey c(a);
	b = a;

	vrfy::assert::assertTrue(a == b);
	vrfy::assert::assertTrue(a == c);
}

/*!
 * This function tests that Hotkey's matching function works properly.
 */
void HotkeyTest::testHotkeyMatching()
{
	Hotkey a(Qt::Key_Enter, nullptr, ~Qt::KeyboardModifiers(nullptr));

	QKeyEvent aeA(QKeyEvent::KeyPress, Qt::Key_Enter, nullptr);
	QKeyEvent aeB(QKeyEvent::KeyPress, Qt::Key_Enter,
	              Qt::KeyboardModifiers(Qt::ShiftModifier));
	QKeyEvent aeC(QKeyEvent::KeyPress, Qt::Key_Enter,
	              ~Qt::KeyboardModifiers(nullptr));
	QKeyEvent aeD(QKeyEvent::KeyPress, Qt::Key_A, nullptr);

	vrfy::assert::assertEquals(a.matches(&aeA), 0);
	vrfy::assert::assertEquals(a.matches(&aeB), 1);
	vrfy::assert::assertEquals(a.matches(&aeC), 32);
	vrfy::assert::assertEquals(a.matches(&aeD), -1);

	Hotkey b(Qt::Key_Home);

	QKeyEvent beA(QKeyEvent::KeyPress, Qt::Key_Home, nullptr);
	QKeyEvent beB(QKeyEvent::KeyPress, Qt::Key_Home,
	              Qt::KeyboardModifiers(Qt::ShiftModifier));
	QKeyEvent beC(QKeyEvent::KeyPress, Qt::Key_Home,
	              ~Qt::KeyboardModifiers(nullptr));
	QKeyEvent beD(QKeyEvent::KeyPress, Qt::Key_A, nullptr);

	vrfy::assert::assertEquals(b.matches(&beA), 0);
	vrfy::assert::assertEquals(b.matches(&beB), -1);
	vrfy::assert::assertEquals(b.matches(&beC), -1);
	vrfy::assert::assertEquals(b.matches(&beD), -1);
}
}
}
