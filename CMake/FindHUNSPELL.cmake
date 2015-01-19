find_path(HUNSPELL_INCLUDE_DIR hunspell.hxx

	/usr/local/include/hunspell
	/usr/local/include
	/usr/include/hunspell
	/usr/include

)

set(HUNSPELL_NAMES ${HUNSPELL_NAMES} hunspell)

find_library(HUNSPELL_LIBRARIES NAMES ${HUNSPELL_NAMES} PATHS

	/usr/lib
	/usr/local/lib

)

if(HUNSPELL_INCLUDE_DIR AND HUNSPELL_LIBRARIES)
	set(HUNSPELL_FOUND TRUE)
else()
	set(HUNSPELL_FOUND FALSE)
endif()

if(HUNSPELL_FOUND)
	if(NOT HUNSPELL_FIND_QUIETLY)
		message(STATUS "Found Hunspell: ${HUNSPELL_LIBRARIES}")
	endif()
else()
	if(HUNSPELL_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find Hunspell.")
	endif()
endif()

mark_as_advanced(

	HUNSPELL_LIBRARIES
	HNSPELL_INCLUDE_DIR

)
