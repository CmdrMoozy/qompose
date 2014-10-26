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

#include "QomposeFontMetricsTest.h"

#include "util/QomposeFontMetrics.h"

#include <QFont>

/*!
 * This function executes all of the tests this class defines.
 */
void QomposeFontMetricsTest::test()
{
	testIsMonospaced();
	testGetColumnWidth();
}

/*!
 * This function tests that our font metrics class' isMonospaced() function can
 * correctly identify monospaced and non-monospaced fonts.
 */
void QomposeFontMetricsTest::testIsMonospaced()
{
	// Test that we can identify monospaced fonts.

	QFont courier(QString("Courier New"), 12);
	QomposeFontMetrics courierMetrics(courier);

	QomposeTest::assertTrue(courier.exactMatch());
	QomposeTest::assertEquals(true, courierMetrics.isMonospaced());

	// Test that we can identify non-monospaced fonts.

	QFont times(QString("Times New Roman"), 12);
	QomposeFontMetrics timesMetrics(times);

	QomposeTest::assertTrue(times.exactMatch());
	QomposeTest::assertEquals(false, timesMetrics.isMonospaced());
}

/*!
 * This function tests that our font metrics class' getColumnWidthF() function
 * can correctly compute column widths.
 */
void QomposeFontMetricsTest::testGetColumnWidth()
{
	QFont courier(QString("Courier New"), 11);
	QomposeFontMetrics metrics(courier);

	QomposeTest::assertEquals(720.0, metrics.getColumnWidthF(80));
}
