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

#include <QFont>

#include "QomposeCommon/util/FontMetrics.h"

namespace
{
QFont getExactFont(const std::vector<QString> &fonts)
{
	for(const auto &font : fonts)
	{
		QFont fontObj(font, 12);
		if(fontObj.exactMatch())
			return fontObj;
	}

	throw std::runtime_error("Getting exact font failed.");
}

QFont getMonospacedFont()
{
	return getExactFont(
	        {QString("Courier New"), QString("Bitstream Vera Mono"),
	         QString("Consolas"), QString("DejaVu Sans Mono"),
	         QString("Droid Sans Mono"), QString("Liberation Mono"),
	         QString("Lucida Typewriter")});
}

QFont getNonMonospacedFont()
{
	return getExactFont({QString("Times New Roman"),
	                     QString("DejaVu Serif"), QString("Droid Serif"),
	                     QString("Liberation Serif"), QString("Arial"),
	                     QString("DejaVu Sans"), QString("Droid Sans"),
	                     QString("Lucida Sans"), QString("Tahoma"),
	                     QString("Trebuchet"), QString("Verdana")});
}
}

TEST_CASE("Test isMonospaced() function behavior", "[FontMetrics]")
{
	// Test that we can identify monospaced fonts.

	QFont mono = getMonospacedFont();
	qompose::FontMetrics monoMetrics(mono);
	REQUIRE(mono.exactMatch());
	REQUIRE(monoMetrics.isMonospaced());

	// Test that we can identify non-monospaced fonts.

	QFont nonMono = getNonMonospacedFont();
	qompose::FontMetrics nonMonoMetrics(nonMono);
	REQUIRE(nonMono.exactMatch());
	REQUIRE_FALSE(nonMonoMetrics.isMonospaced());
}

TEST_CASE("Test getColumnWidthF() function behavior", "[FontMetrics]")
{
	QFont courier(QString("Courier New"), 11);
	qompose::FontMetrics metrics(courier);

	REQUIRE(720.0 == metrics.getColumnWidthF(80));
}
