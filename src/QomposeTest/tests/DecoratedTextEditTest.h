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

#ifndef INCLUDE_QOMPOSETEST_TESTS_DECORATED_TEXT_EDIT_TEST_H
#define INCLUDE_QOMPOSETEST_TESTS_DECORATED_TEXT_EDIT_TEST_H

#include <Vrfy/Vrfy.h>

namespace qompose
{
namespace test
{
/*!
 * \brief This class defines unit tests for our decorated text edit class.
 */
class DecoratedTextEditTest : public vrfy::Test
{
public:
	DecoratedTextEditTest() = default;
	virtual ~DecoratedTextEditTest() = default;

	virtual void test();
};
}
}

#endif
