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

#ifndef INCLUDE_QOMPOSECOMMON_THREAD_LOW_PRIORITY_THREAD_H
#define INCLUDE_QOMPOSECOMMON_THREAD_LOW_PRIORITY_THREAD_H

#include <initializer_list>
#include <memory>

class QObject;

namespace qompose
{
class LowPriorityThreadImpl;

/*!
 * \brief Defines the types of thread priority which can be changed.
 */
enum class PriorityType
{
	CPU,
	IO
};

/*!
 * \brief This class implements a low-priority thread.
 *
 * This class can be used to execute some code in a thread which is marked as
 * having low CPU or IO priority, so as to not interfere with other processes
 * on the system.
 *
 * It's worth noting that the priority of this thread generally can't be raised
 * without superuser permissions, so now API is provided to do so.
 */
class LowPriorityThread
{
public:
	/*!
	 * This constructor creates a new low priority thread, which has low
	 * priority for all of the given priority types.
	 *
	 * \param t The low priority types for this thread.
	 */
	explicit LowPriorityThread(
	        const std::initializer_list<PriorityType> &t);

	/*!
	 * This constructor creates a new low priority thread, which has low
	 * priority for the given priority type.
	 *
	 * \param t The low priority type for this thread.
	 */
	LowPriorityThread(PriorityType t);

	LowPriorityThread(const LowPriorityThread &) = delete;

	/*!
	 * This destructor automatically join()'s the thread to ensure it stops
	 * before destructing.
	 */
	~LowPriorityThread();

	LowPriorityThread &operator=(const LowPriorityThread &) = delete;

	/*!
	 * This function moves the given QObject to this thread for event
	 * processing.
	 *
	 * \param o The object to move.
	 */
	void moveToThread(QObject *o);

	/*!
	 * This function waits for this thread to stop running. If something
	 * goes wrong, an exception will be thrown.
	 */
	void join();

private:
	std::unique_ptr<LowPriorityThreadImpl> thread;
};
}

#endif
