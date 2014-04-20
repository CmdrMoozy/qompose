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

#include "test/QomposeAssertionException.h"

#ifdef __unix__
	#include <unistd.h>
#endif

#ifdef __GNUC__
	#include <cstdio>
	#include <cstdlib>
	#include <sstream>

	#include <cxxabi.h>
	#include <execinfo.h>
#endif

/*!
 * This function will return the absolute path to the currently running program
 * (i.e., this program).
 *
 * \return The path to this program.
 */
std::string QomposeTest::getProgramPath()
{
	#ifdef __unix__
		char p[1024];

		ssize_t s = readlink("/proc/self/exe", p, 1023);
		p[s] = '\0';

		std::string path;

		if(s != -1)
			path = std::string(p);

		return path;
	#else
		return std::string();
	#endif
}

/*!
 * This function returns, as a string, a rendered stack trace. This stack trace
 * is altered to exclude any items before this program's main() function, as
 * well as this function.
 *
 * \return A full, rendered stack trace.
 */
std::string QomposeTest::getStackTrace()
{
	#if defined(__GNUC__) && defined(__unix__)
		std::stringstream buf;

		void *trace[100];
		int traceSize;

		traceSize = backtrace(trace, 100);

		for(int i = 2; i < (traceSize - 2); ++i)
		{
			char syscom[1024];
			snprintf(syscom, 1024, "addr2line %p -e %s", trace[i],
				QomposeTest::getProgramPath().c_str());

			FILE *proc = popen(syscom, "r");

			if(proc)
			{
				buf << "[trace] " << i - 1 << " ";

				char pbuf[256];

				while(fgets(pbuf, 256, proc))
					buf << pbuf;

				pclose(proc);
			}
		}

		return buf.str();
	#else
		return std::string();
	#endif
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
