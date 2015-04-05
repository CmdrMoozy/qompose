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

#ifdef _WIN32
#include <cstdint>
#endif

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

#ifdef _WIN32
/*!
 * This function sets the priority value for a specific thread. By default, the
 * calling thread is used.
 *
 * This function is a wrapper for Windows' SetThreadPriority function, except
 * it throws an exception on error instead of returning an error value.
 *
 * \param c The new priority value for the thread.
 * \param thread The thread whose priority should be changed.
 */
void setThreadPriority(uint32_t c, HANDLE thread = nullptr);

/*!
 * This function sets the priority value for a process (as well as all of its
 * threads). By default, the calling process is used.
 *
 * This function is a wrapper for Windows' SetPriorityClass function, except
 * it throws an exception on error instead of returning an error value.
 *
 * \param c The new priority value for the process.
 * \param process The process whose priority should be changed.
 */
void setPriorityClass(uint32_t c, HANDLE process = nullptr);
#endif
}
}

#endif
