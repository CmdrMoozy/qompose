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

#include <QApplication>

#include <iostream>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <string>

#ifdef __GNUC__
	#include <cxxabi.h>
#endif

#include "QomposeTest/AssertionException.h"
#include "QomposeTest/Test.h"
#include "QomposeTest/tests/FontMetricsTest.h"
#include "QomposeTest/tests/HotkeyTest.h"
#include "QomposeTest/tests/HotkeyMapTest.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv, false);

	// Build the list of tests to run.

	std::vector<qompose::test::Test *> tests;
	std::vector<qompose::test::Test *>::iterator it;

	tests.push_back(new qompose::test::FontMetricsTest());
	tests.push_back(new qompose::test::HotkeyTest());
	tests.push_back(new qompose::test::HotkeyMapTest());

	// Execute each test.


	for(it = tests.begin(); it != tests.end(); ++it)
	{
		// Get this test object's type name.

		qompose::test::Test *test = *it;

		#ifdef __GNUC__
			int status;

			char *t = abi::__cxa_demangle(typeid(*test).name(),
				nullptr, nullptr, &status);

			std::string type = std::string(t);
			free(t);
		#else
			std::string type = std::string(typeid(*test).name());
		#endif

		// Try executing the test, catching all assertion exceptions.

		try
		{
			std::cout << type << "...\n";
			test->test();
		}
		catch(const qompose::test::AssertionException &e)
		{
			std::cout << "ASSERTION FAILED: " << type
				<< ": " << e.what() << "\n";

			std::cout << e.getStackTrace();
		}
	}

	// Clean up the tests.

	while(!tests.empty())
	{
		qompose::test::Test *t = tests.back();
		tests.pop_back();

		delete t;
	}

	return 0;
}
