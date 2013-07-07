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
