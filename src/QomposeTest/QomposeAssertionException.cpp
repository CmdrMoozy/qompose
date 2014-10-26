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

#include "QomposeAssertionException.h"

#include "QomposeTest/QomposeTest.h"

/*!
 * This constructor creates a new assertion exception with an empty message.
 */
QomposeAssertionException::QomposeAssertionException() noexcept
	: message(""), trace("")
{
	trace = QomposeTest::getStackTrace();
}

/*!
 * This constructor creates a new assertion exception with the given message.
 *
 * \param w The exception message.
 */
QomposeAssertionException::QomposeAssertionException(
	const std::string &w) noexcept
	: message(w), trace("")
{
	trace = QomposeTest::getStackTrace();
}

/*!
 * This is a copy constructor, which creates a new exception which is identical
 * to the given other one.
 *
 * \param e The other exception to copy.
 */
QomposeAssertionException::QomposeAssertionException(
	const QomposeAssertionException &e) noexcept
{
	*this = e;
}

/*!
 * This is our default destructor, which cleans up and destroys this object.
 */
QomposeAssertionException::~QomposeAssertionException() noexcept
{
}

/*!
 * This is our assignment operator, which sets this exception equal to the given
 * other exception.
 *
 * \param e The other exception to set ourself equal to.
 * \return A reference to this, for operator chanining.
 */
QomposeAssertionException &QomposeAssertionException::operator=(
	const QomposeAssertionException &e) noexcept
{
	message = e.message;
	trace = e.trace;

	return *this;
}

/*!
 * This function returns this exception's message.
 *
 * \return This exception's message.
 */
const char *QomposeAssertionException::what() const noexcept
{
	return message.c_str();
}

/*!
 * This function returns, as a string, the rendered stack trace which was
 * generated in this object's constructor.
 *
 * \return This exception's stack trace.
 */
const std::string &QomposeAssertionException::getStackTrace() const noexcept
{
	return trace;
}
