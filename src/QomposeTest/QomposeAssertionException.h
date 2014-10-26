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

#ifndef INCLUDE_QOMPOSE_ASSERTION_EXCEPTION_H
#define INCLUDE_QOMPOSE_ASSERTION_EXCEPTION_H

#include <exception>
#include <string>

/*!
 * \brief This class is an exception to be thrown on a test assertion failure.
 */
class QomposeAssertionException : public std::exception
{
	public:
		QomposeAssertionException() noexcept;
		QomposeAssertionException(const std::string &w) noexcept;
		QomposeAssertionException(
			const QomposeAssertionException &e) noexcept;
		virtual ~QomposeAssertionException() noexcept;

		QomposeAssertionException &operator=(
			const QomposeAssertionException &e) noexcept;

		virtual const char *what() const noexcept;

		const std::string &getStackTrace() const noexcept;

	private:
		std::string message;
		std::string trace;
};

#endif
