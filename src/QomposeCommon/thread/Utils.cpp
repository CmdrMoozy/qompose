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

#include "Utils.h"

#include <stdexcept>

#ifdef __linux__
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#include <sys/syscall.h>
#endif

#include "QomposeCommon/util/Errno.h"
#include "QomposeCommon/util/Windows.h"

namespace qompose
{
namespace thread_utils
{
#ifdef __linux__
int ioprio_get(int which, int who)
{
	int r = syscall(SYS_ioprio_get, which, who);
	if(r == -1)
		util::throwErrnoError();
	return r;
}

void ioprio_set(int which, int who, int ioprio)
{
	int r = syscall(SYS_ioprio_set, which, who, ioprio);
	if(r != 0)
		util::throwErrnoError();
}
#endif

#ifdef _WIN32
void setThreadPriority(uint32_t c, HANDLE thread)
{
	if(thread == nullptr)
		thread = GetCurrentThread();

	bool ret = SetThreadPriority(thread, static_cast<DWORD>(c)) != 0;
	if(!ret)
		util::throwLastWindowsError();
}

void setPriorityClass(uint32_t c, HANDLE process)
{
	if(process == nullptr)
		process = GetCurrentProcess();

	bool ret = SetPriorityClass(process, static_cast<DWORD>(c)) != 0;
	if(!ret)
		util::throwLastWindowsError();
}
#endif
}
}
