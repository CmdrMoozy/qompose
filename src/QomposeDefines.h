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

#ifndef INCLUDE_QOMPOSE_DEFINES_H
#define INCLUDE_QOMPOSE_DEFINES_H
	
	#define QOMPOSE_GUID ("32e11710-e79f-4a3a-ba31-2ff06f1f86a0")
	
	/*
	 * Define our QLUNUSED macro, which is used on win32 to suppress the "unreferenced local
	 * variable" warning (4101) in cases where we are very intentionally NOT referencing said
	 * variable.
	 */
	#ifndef QLUNUSED
		#ifdef _WIN32
			#define QLUNUSED(x) x;
		#else
			#define QLUNUSED(x)
		#endif
	#endif

	// Define our QUNUSED() macro
	#ifndef QUNUSED
		#if defined(__GNUC__)
			#define QUNUSED(x) UNUSED_ ## x __attribute__((unused))
		#else
			#define QUNUSED(x)
		#endif
	#endif

#endif
