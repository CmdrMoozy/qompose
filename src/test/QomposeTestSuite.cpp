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

#include <iostream>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <string>

#ifdef __GNUC__
	#include <cxxabi.h>
#endif

#include "test/QomposeAssertionException.h"
#include "test/QomposeTest.h"
#include "test/util/QomposeHotkeyMapTest.h"

int main(void)
{
	// Build the list of tests to run.

	std::vector<QomposeTest *> tests;

	tests.push_back(new QomposeHotkeyMapTest());

	// Execute each test.

	for(std::vector<QomposeTest *>::iterator it = tests.begin();
		it != tests.end(); ++it)
	{
		// Get this test object's type name.

		QomposeTest *test = *it;

		#ifdef __GNUC__
			int status;

			char *t = abi::__cxa_demangle(typeid(*test).name(),
				0, 0, &status);

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
		catch(const QomposeAssertionException &e)
		{
			std::cout << "ASSERTION FAILED: " << type
				<< ": " << e.what() << "\n";

			std::cout << e.getStackTrace();
		}
	}
}
