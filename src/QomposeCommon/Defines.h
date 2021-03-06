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

#ifndef INCLUDE_QOMPOSECOMMON_DEFINES_H
#define INCLUDE_QOMPOSECOMMON_DEFINES_H

/*
 * Define our application's version.
 */
#define QOMPOSE_VERSION_MAJ 1
#define QOMPOSE_VERSION_MIN 0
#define QOMPOSE_VERSION_BUG 0

/*
 * Define a GUID to uniquely identify our application.
 */
#ifdef QOMPOSE_DEBUG
#define QOMPOSE_GUID ("cc0c9103-5fc0-45d3-ab0e-694d566f3e8d")
#else
#define QOMPOSE_GUID ("32e11710-e79f-4a3a-ba31-2ff06f1f86a0")
#endif

/*
 * Define a megabyte in bytes, for code readability.
 */
#define QMEGABYTE (1048576)

/*
 * Define our QLUNUSED macro, which is used on win32 to suppress the
 * "unreferenced local variable" warning (4101) in cases where we are
 * very intentionally NOT referencing said variable.
 */
#ifndef QLUNUSED
#ifdef _WIN32
#define QLUNUSED(x) x;
#else
#define QLUNUSED(x)
#endif
#endif

/*
 * Define our QUNUSED macro, which is used to suppress unused variable
 * warnings by indicating that the variable is intended to be unused.
 * Note that this only does anything significant for GNU compilers.
 */
#ifndef QUNUSED
#if defined(__GNUC__)
#define QUNUSED(x) UNUSED_##x __attribute__((unused))
#else
#define QUNUSED(x)
#endif
#endif

#endif
