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

#ifndef INCLUDE_QOMPOSECOMMON_THREAD_UTILS_H
#define INCLUDE_QOMPOSECOMMON_THREAD_UTILS_H

namespace qompose
{
namespace thread_utils
{
#ifdef __linux__
/*!
 * This function is simply a syscall() wrapper for ioprio_get. See
 * IOPRIO_SET(2) for more information.
 *
 * Instead of returning an error value, an exception is thrown in case an error
 * occurs.
 *
 * \param which Defines how the who argument is interpreted.
 * \param who The thread(s) on which the system call will operate.
 * \return The highest ioprio value of any of the specified thread(s).
 */
int ioprio_get(int which, int who);

/*!
 * This function is simply a syscall() wrapper for ioprio_set. See
 * IOPRIO_SET(2) for more information.
 *
 * Instead of returning a success/error flag, an exception is thrown in case
 * an error occurs.
 *
 * \param which Defines how the who argument is interpreted.
 * \param who The thread(s) on which the system call will operate.
 * \param ioprio The new IO priority value for the specified thread(s).
 */
void ioprio_set(int which, int who, int ioprio);
#endif
}
}

#endif
