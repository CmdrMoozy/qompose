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

#ifndef INCLUDE_QOMPOSECOMMON_THREAD_IO_PRIORITY_DEFINES_H
#define INCLUDE_QOMPOSECOMMON_THREAD_IO_PRIORITY_DEFINES_H

namespace qompose
{
namespace thread_utils
{
#ifdef __linux__
/*!
 * \brief Defines the various IO priority classes available.
 */
enum class IOPrioClass
{
	None,
	RT,
	BE,
	Idle
};

/*!
 * \brief Defines how the "who" paramter to ioprio_*() will be interpreted.
 */
enum class IOPrioWho
{
	Process = 1,
	PGrp,
	User
};

/*!
 * The number of best-effort priority levels that are supported.
 */
constexpr int IOPRIO_BE_NR = 8;

/*!
 * \brief A fallback best-effort priority.
 */
constexpr int IOPRIO_NORM = 4;

/*!
 * \brief The total number of IO priority bits; room for 8 classes + data.
 */
constexpr int IOPRIO_BITS = 16;

/*!
 * \brief The number of bits in the data portion of an IO value.
 */
constexpr int IOPRIO_CLASS_SHIFT = 13;

/*!
 * \brief The mask for the data portion of an IO priority value.
 */
constexpr int IOPRIO_PRIO_MASK = (1UL << IOPRIO_CLASS_SHIFT) - 1;

/*!
 * This function returns the IOPrioClass portion of the given priority value.
 *
 * \param value The IO priority value to inspect.
 * \return The IOPrioClass portion of the given value.
 */
inline IOPrioClass ioprioPrioClass(int value)
{
	return static_cast<IOPrioClass>(value >> IOPRIO_CLASS_SHIFT);
}

/*!
 * This function returns the data portion of the given priority value.
 *
 * \param value The IO priority value to inspect.
 * \return The data portion of the given value.
 */
inline int ioprioPrioData(int value)
{
	return value & IOPRIO_PRIO_MASK;
}

/*!
 * This function returns an IO priority value which contains the given priority
 * class and data.
 *
 * \param c The IO priority class.
 * \param data The data.
 * \return The IO priority value containing the given information.
 */
inline int ioprioPrioValue(IOPrioClass c, int data)
{
	return (static_cast<int>(c) << IOPRIO_CLASS_SHIFT) | data;
}

/*!
 * This function returns whether or not the given IO priority value is a valid
 * one.
 *
 * \param value The IO priority value to inspect.
 * \return Whether or not the given value is valid.
 */
inline bool ioprioValid(int value)
{
	return ioprioPrioClass(value) != IOPrioClass::None;
}
#endif
}
}

#endif
