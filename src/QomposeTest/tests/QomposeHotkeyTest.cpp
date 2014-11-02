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

#include "QomposeHotkeyTest.h"

#include <QKeyEvent>

#include "util/QomposeHotkey.h"

/*!
 * This is our default constructor, which creates a new insance of our test.
 */
QomposeHotkeyTest::QomposeHotkeyTest()
{
}

/*!
 * This is our default destructor, which cleans up an destroys this test
 * instance.
 */
QomposeHotkeyTest::~QomposeHotkeyTest()
{
}

/*!
 * We implement our superclass's test() function to perform our various tests
 * against the QomposeHotkey class.
 */
void QomposeHotkeyTest::test()
{
	testHotkeyConstruction();
	testHotkeyCopying();
	testHotkeyMatching();
}

/*!
 * This function tests that the various QomposeHotkey constructors initialize
 * the resulting object's internal state properly.
 */
void QomposeHotkeyTest::testHotkeyConstruction()
{
	QomposeHotkey a(Qt::Key_A);

	QomposeTest::assertEquals(a.getKey(), Qt::Key_A);
	QomposeTest::assertEquals(a.getKeyInteger(),
		static_cast<quint64>(Qt::Key_A));
	QomposeTest::assertEquals(a.getRequiredModifiers(),
		Qt::KeyboardModifiers(Qt::NoModifier));
	QomposeTest::assertEquals(a.getWhitelistedModifiers(),
		Qt::KeyboardModifiers(Qt::NoModifier));

	QomposeHotkey b(Qt::Key_A, Qt::ControlModifier);

	QomposeTest::assertEquals(b.getKey(), Qt::Key_A);
	QomposeTest::assertEquals(b.getKeyInteger(),
		static_cast<quint64>(Qt::Key_A));
	QomposeTest::assertEquals(b.getRequiredModifiers(),
		Qt::KeyboardModifiers(Qt::ControlModifier));
	QomposeTest::assertEquals(b.getWhitelistedModifiers(),
		Qt::KeyboardModifiers(Qt::ControlModifier));

	QomposeHotkey c(Qt::Key_A, Qt::ControlModifier, Qt::ShiftModifier);

	QomposeTest::assertEquals(c.getKey(), Qt::Key_A);
	QomposeTest::assertEquals(c.getKeyInteger(),
		static_cast<quint64>(Qt::Key_A));
	QomposeTest::assertEquals(c.getRequiredModifiers(),
		Qt::KeyboardModifiers(Qt::ControlModifier));
	QomposeTest::assertEquals(c.getWhitelistedModifiers(),
		Qt::ControlModifier | Qt::ShiftModifier);
}

/*!
 * This function tests that the assignment operator, copy constructor, and
 * equivalence operator from the QomposeHotkey class work properly.
 */
void QomposeHotkeyTest::testHotkeyCopying()
{
	QomposeHotkey a(Qt::Key_A, Qt::ControlModifier, Qt::ShiftModifier);
	QomposeHotkey b(Qt::Key_B);

	QomposeTest::assertTrue(!(a == b));

	QomposeHotkey c(a);
	b = a;

	QomposeTest::assertTrue(a == b);
	QomposeTest::assertTrue(a == c);
}

/*!
 * This function tests that QomposeHotkey's matching function works properly.
 */
void QomposeHotkeyTest::testHotkeyMatching()
{
	QomposeHotkey a(Qt::Key_Enter, nullptr,
		~Qt::KeyboardModifiers(nullptr));

	QKeyEvent aeA(QKeyEvent::KeyPress, Qt::Key_Enter, nullptr);
	QKeyEvent aeB(QKeyEvent::KeyPress, Qt::Key_Enter,
		Qt::KeyboardModifiers(Qt::ShiftModifier));
	QKeyEvent aeC(QKeyEvent::KeyPress, Qt::Key_Enter,
		~Qt::KeyboardModifiers(nullptr));
	QKeyEvent aeD(QKeyEvent::KeyPress, Qt::Key_A, nullptr);

	QomposeTest::assertEquals(a.matches(&aeA), 0);
	QomposeTest::assertEquals(a.matches(&aeB), 1);
	QomposeTest::assertEquals(a.matches(&aeC), 32);
	QomposeTest::assertEquals(a.matches(&aeD), -1);

	QomposeHotkey b(Qt::Key_Home);

	QKeyEvent beA(QKeyEvent::KeyPress, Qt::Key_Home, nullptr);
	QKeyEvent beB(QKeyEvent::KeyPress, Qt::Key_Home,
		Qt::KeyboardModifiers(Qt::ShiftModifier));
	QKeyEvent beC(QKeyEvent::KeyPress, Qt::Key_Home,
		~Qt::KeyboardModifiers(nullptr));
	QKeyEvent beD(QKeyEvent::KeyPress, Qt::Key_A, nullptr);

	QomposeTest::assertEquals(b.matches(&beA), 0);
	QomposeTest::assertEquals(b.matches(&beB), -1);
	QomposeTest::assertEquals(b.matches(&beC), -1);
	QomposeTest::assertEquals(b.matches(&beD), -1);
}
