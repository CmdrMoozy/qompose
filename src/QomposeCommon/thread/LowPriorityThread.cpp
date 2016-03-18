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

#include "LowPriorityThread.h"

#include <stdexcept>

#ifdef __linux__
#include <pthread.h>
#include <sched.h>
#endif

#include <QObject>
#include <QThread>

#include <bdrck/util/Error.hpp>

#include "QomposeCommon/thread/Utils.h"

namespace
{
/*!
 * This function sets the calling thread's CPU priority to its lowest value.
 */
void setLowCPUPriority()
{
#if defined(_WIN32)
	qompose::thread_utils::setThreadPriority(PROCESS_MODE_BACKGROUND_BEGIN);
#elif defined(__linux__)
	struct sched_param param;
	param.sched_priority = sched_get_priority_min(SCHED_OTHER);

	int ret = pthread_setschedparam(pthread_self(), SCHED_OTHER, &param);
	if(ret != 0)
		bdrck::util::error::throwErrnoError(ret);
#else
	static_assert(false, "CPU priority unsupported on this platform.");
#endif
}

/*!
 * This function sets the calling thread's IO priority to its lowest value.
 */
void setLowIOPriority()
{
#if defined(_WIN32)
	qompose::thread_utils::setThreadPriority(PROCESS_MODE_BACKGROUND_BEGIN);
#elif defined(__linux__)
	qompose::thread_utils::ioprio_set(
	        qompose::thread_utils::IOPrioWho::Process, 0,
	        qompose::thread_utils::ioprioPrioValue(
	                qompose::thread_utils::IOPrioClass::Idle, 0));
#else
	static_assert(false, "IO priority unsupported on this platform.");
#endif
}

/*!
 * This function sets the calling thread's priority to its lowest value for
 * all of the given priority types.
 *
 * \param types The types of priority to set.
 */
void setLowPriority(const std::initializer_list<qompose::PriorityType> &types)
{
	for(const auto &type : types)
	{
		switch(type)
		{
		case qompose::PriorityType::CPU:
			setLowCPUPriority();
			break;

		case qompose::PriorityType::IO:
			setLowIOPriority();
			break;
		}
	}
}
}

namespace qompose
{
/*!
 * \brief The QThread-based implementation of LowPriorityThread.
 */
class LowPriorityThreadImpl : public QThread
{
public:
	/*!
	 * This constructor creates a new low priority thread, which has low
	 * priority for all of the given priority types.
	 *
	 * \param t The low priority types for this thread.
	 */
	LowPriorityThreadImpl(const std::initializer_list<PriorityType> &t);

	virtual ~LowPriorityThreadImpl();

	/*!
	 * This function returns this thread's priority. This is
	 * LowestPriority, by definition.
	 *
	 * \return This thread's priority.
	 */
	virtual Priority priority() const;

	/*!
	 * We override setPriority() to enforce that it is not implemented for
	 * this QThread subclass. If this function is called, an exception is
	 * thrown and no action is taken.
	 */
	virtual void setPriority(Priority);

protected:
	/*!
	 * This QThread's run method simply sets the thread priority
	 * appropriately, and then begins event processing.
	 */
	virtual void run();

private:
	std::initializer_list<PriorityType> priorityTypes;
};

/*!
* This constructor creates a new low priority thread, which has low
* priority for all of the given priority types.
*
* \param t The low priority types for this thread.
*/
LowPriorityThreadImpl::LowPriorityThreadImpl(
        const std::initializer_list<PriorityType> &t)
        : QThread(), priorityTypes(t)
{
}

LowPriorityThreadImpl::~LowPriorityThreadImpl()
{
}

/*!
* This function returns this thread's priority. This is
* LowestPriority, by definition.
*
* \return This thread's priority.
*/
QThread::Priority LowPriorityThreadImpl::priority() const
{
	return QThread::LowestPriority;
}

/*!
* We override setPriority() to enforce that it is not implemented for
* this QThread subclass. If this function is called, an exception is
* thrown and no action is taken.
*/
void LowPriorityThreadImpl::setPriority(Priority)
{
	throw std::runtime_error(
	        "setPriority() isn't supported for this subclass.");
}

/*!
* This QThread's run method simply sets the thread priority
* appropriately, and then begins event processing.
*/
void LowPriorityThreadImpl::run()
{
	setLowPriority(priorityTypes);
	exec();
}

LowPriorityThread::LowPriorityThread(
        const std::initializer_list<PriorityType> &t)
        : thread(new LowPriorityThreadImpl(t))
{
}

LowPriorityThread::LowPriorityThread(PriorityType t)
        : thread(new LowPriorityThreadImpl({t}))
{
}

LowPriorityThread::~LowPriorityThread()
{
	join();
}

void LowPriorityThread::start()
{
	thread->start();
}

void LowPriorityThread::moveToThread(QObject *o)
{
	o->moveToThread(thread.get());
}

void LowPriorityThread::join()
{
	if(!thread->wait())
		throw std::runtime_error("Thread join failed.");
}
}
