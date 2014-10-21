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

#ifndef INCLUDE_QOMPOSE_TEST_H
#define INCLUDE_QOMPOSE_TEST_H

#include <string>

#include "test/QomposeAssertionException.h"

/*!
 * \brief This class implements a generic unit testing superclass.
 */
class QomposeTest
{
	public:
		static std::string getStackTrace();

		static void assertTrue(bool expr);

		/*!
		 * This function asserts that the two given items are equal,
		 * using the given comparator. The comparator should implement
		 * an operator(), which returns true if the two items are equal,
		 * or false otherwise.
		 *
		 * \param a The first item to compare.
		 * \param b The second item to compare.
		 * \param comp The comparator to use to compare the items.
		 */
		template <typename T, class Compare>
		static void assertEquals(const T &a, const T &b, Compare comp)
		{
			if(!comp(a, b))
				throw QomposeAssertionException(
					"Expected equality, got inequality");
		}

		/*!
		 * This function asserts that the two given items are equal,
		 * using their operator==.
		 *
		 * \param a The first item to compare.
		 * \param b The second item to compare.
		 */
		template <typename T>
		static void assertEquals(const T &a, const T &b)
		{
			struct comparator {
				bool operator()(const T &ca, const T &cb)
				{
					return ca == cb;
				}
			} comp;

			assertEquals(a, b, comp);
		}

		virtual ~QomposeTest();

		virtual void test() = 0;
};

#endif
