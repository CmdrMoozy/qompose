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

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <random>
#include <utility>
#include <vector>

#include <QByteArray>
#include <QColor>
#include <QDataStream>
#include <QFont>

#include "QomposeCommon/settings/Deserializer.h"
#include "QomposeCommon/settings/Serializer.h"

namespace
{
template <typename T> std::pair<T, T> performRoundTripTest(T const &v)
{
	qompose::settings::Serializer<T> serializer;
	qompose::settings::Deserializer<T> deserializer;
	return std::make_pair(v, deserializer(serializer(v)));
}

bool almostEqual(double a, double b)
{
	double max = std::max({1.0, a, b});
	return std::abs(a - b) <= std::numeric_limits<double>::epsilon() * max;
}
}

TEST_CASE("Test integer round tripping", "[Settings]")
{
	constexpr std::initializer_list<uint64_t> TEST_CASES{
	        0, std::numeric_limits<uint64_t>::max() / 2,
	        std::numeric_limits<uint64_t>::max()};

	for(auto const &v : TEST_CASES)
	{
		auto pair = performRoundTripTest(v);
		CHECK(almostEqual(pair.first, pair.second));
	}
}

TEST_CASE("Test floating point round tripping", "[Settings]")
{
	constexpr std::initializer_list<double> TEST_CASES{0.0, -12345.54321,
	                                                   12345.54321};

	for(auto const &v : TEST_CASES)
	{
		auto pair = performRoundTripTest(v);
		CHECK(std::abs(pair.first - pair.second) <=
		      std::numeric_limits<double>::epsilon());
	}
}

TEST_CASE("Test boolean round tripping", "[Settings]")
{
	constexpr std::initializer_list<bool> TEST_CASES{true, false};

	for(auto const &v : TEST_CASES)
	{
		auto pair = performRoundTripTest(v);
		CHECK(pair.first == pair.second);
	}
}

TEST_CASE("Test QColor round tripping", "[Settings]")
{
	const std::vector<QColor> TEST_CASES{
	        QColor(0, 0, 0), QColor(255, 255, 255), QColor(12, 34, 56, 78)};

	for(auto const &v : TEST_CASES)
	{
		auto pair = performRoundTripTest(v);
		CHECK(pair.first == pair.second);
	}
}

TEST_CASE("Test QByteArray round tripping", "[Settings]")
{
	std::random_device rd;
	std::mt19937_64 engine(rd());
	std::uniform_int_distribution<uint64_t> dist;

	QByteArray v;
	QDataStream stream(&v, QIODevice::WriteOnly);
	for(int i = 0; i < 10; ++i)
	{
		uint64_t rv = dist(engine);
		stream.writeBytes(reinterpret_cast<char const *>(&rv),
		                  sizeof(uint64_t));
	}

	auto pair = performRoundTripTest(v);
	bool success = pair.first == pair.second;
	CHECK(success);
}

TEST_CASE("Test QFont round tripping", "[Settings]")
{
	const std::vector<QFont> TEST_CASES{
	        QFont(), QFont("Helvetica"),
	        QFont("Times New Roman", 24, 100, true)};

	for(auto const &v : TEST_CASES)
	{
		auto pair = performRoundTripTest(v);
		CHECK(pair.first == pair.second);
	}
}
