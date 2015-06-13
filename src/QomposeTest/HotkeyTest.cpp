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

#include <QKeyEvent>

#include "QomposeCommon/util/Hotkey.h"

TEST_CASE("Verify hotkey construction behavior", "[Hotkey]")
{
	qompose::Hotkey a(Qt::Key_A);

	REQUIRE(a.getKey() == Qt::Key_A);
	REQUIRE(a.getKeyInteger() == static_cast<quint64>(Qt::Key_A));
	REQUIRE(a.getRequiredModifiers() ==
	        Qt::KeyboardModifiers(Qt::NoModifier));
	REQUIRE(a.getWhitelistedModifiers() ==
	        Qt::KeyboardModifiers(Qt::NoModifier));

	qompose::Hotkey b(Qt::Key_A, Qt::ControlModifier);

	REQUIRE(b.getKey() == Qt::Key_A);
	REQUIRE(b.getKeyInteger() == static_cast<quint64>(Qt::Key_A));
	REQUIRE(b.getRequiredModifiers() ==
	        Qt::KeyboardModifiers(Qt::ControlModifier));
	REQUIRE(b.getWhitelistedModifiers() ==
	        Qt::KeyboardModifiers(Qt::ControlModifier));

	qompose::Hotkey c(Qt::Key_A, Qt::ControlModifier, Qt::ShiftModifier);

	REQUIRE(c.getKey() == Qt::Key_A);
	REQUIRE(c.getKeyInteger() == static_cast<quint64>(Qt::Key_A));
	REQUIRE(c.getRequiredModifiers() ==
	        Qt::KeyboardModifiers(Qt::ControlModifier));
	REQUIRE(c.getWhitelistedModifiers() ==
	        (Qt::ControlModifier | Qt::ShiftModifier));
}

TEST_CASE("Verify hotkey copying functionality", "[Hotkey]")
{
	qompose::Hotkey a(Qt::Key_A, Qt::ControlModifier, Qt::ShiftModifier);
	qompose::Hotkey b(Qt::Key_B);

	REQUIRE(!(a == b));

	qompose::Hotkey c(a);
	b = a;

	REQUIRE(a == b);
	REQUIRE(a == c);
}

TEST_CASE("Verify hotkey matching functionality", "[Hotkey]")
{
	qompose::Hotkey a(Qt::Key_Enter, nullptr,
	                  ~Qt::KeyboardModifiers(nullptr));

	QKeyEvent aeA(QKeyEvent::KeyPress, Qt::Key_Enter, nullptr);
	QKeyEvent aeB(QKeyEvent::KeyPress, Qt::Key_Enter,
	              Qt::KeyboardModifiers(Qt::ShiftModifier));
	QKeyEvent aeC(QKeyEvent::KeyPress, Qt::Key_Enter,
	              ~Qt::KeyboardModifiers(nullptr));
	QKeyEvent aeD(QKeyEvent::KeyPress, Qt::Key_A, nullptr);

	REQUIRE(a.matches(&aeA) == 0);
	REQUIRE(a.matches(&aeB) == 1);
	REQUIRE(a.matches(&aeC) == 32);
	REQUIRE(a.matches(&aeD) == -1);

	qompose::Hotkey b(Qt::Key_Home);

	QKeyEvent beA(QKeyEvent::KeyPress, Qt::Key_Home, nullptr);
	QKeyEvent beB(QKeyEvent::KeyPress, Qt::Key_Home,
	              Qt::KeyboardModifiers(Qt::ShiftModifier));
	QKeyEvent beC(QKeyEvent::KeyPress, Qt::Key_Home,
	              ~Qt::KeyboardModifiers(nullptr));
	QKeyEvent beD(QKeyEvent::KeyPress, Qt::Key_A, nullptr);

	REQUIRE(b.matches(&beA) == 0);
	REQUIRE(b.matches(&beB) == -1);
	REQUIRE(b.matches(&beC) == -1);
	REQUIRE(b.matches(&beD) == -1);
}
