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

#include "QomposeTest.h"

#include <cstdio>

#include "backward.hpp"

/*!
 * This function returns, as a string, a rendered stack trace. This stack trace
 * is altered to exclude any items before this program's main() function, as
 * well as this function.
 *
 * \return A full, rendered stack trace.
 */
std::string QomposeTest::getStackTrace()
{
	// Get a stack trace.

	backward::StackTrace trace;
	trace.load_here(100);

	// Prepare our stack trace pretty-printer.

	backward::Printer p;

	p.object = false;
	p.color = false;
	p.address = true;

	// Prepare a memstream to write the trace to.

	char *buf = NULL;
	size_t buflen;

	FILE *stream = open_memstream(&buf, &buflen);

	// Print the stack trace to our buffer.

	p.print(trace, stream);

	// Close the memory stream.

	fflush(stream);
	fclose(stream);

	// Convert to a C++ string, and free the buffer.

	std::string ret(buf, buflen);

	free(buf);
	buf = nullptr;

	// Done!

	return ret;
}

/*!
 * This function asserts that the given boolean expression is true.
 *
 * \param expr The expression to tst for being true.
 */
void QomposeTest::assertTrue(bool expr)
{
	if(!expr)
		throw QomposeAssertionException("Expected true, got false");
}

/*!
 * This is our default destructor, which cleans up and destroys this object.
 */
QomposeTest::~QomposeTest()
{
}
