include(${CMAKE_SOURCE_DIR}/CMake/CheckClassMemberFunction.cmake)

function(qomposePlatformChecks)
	message(${Qt5Widgets_INCLUDE_DIRS})

	set(CMAKE_REQUIRED_FLAGS
		${CMAKE_REQUIRED_FLAGS}
		"-fPIC -std=c++11"
	)

	set(CMAKE_REQUIRED_INCLUDES
		${CMAKE_REQUIRED_INCLUDES}
		${Qt5Core_INCLUDE_DIRS}
		${Qt5Gui_INCLUDE_DIRS}
		${Qt5Widgets_INCLUDE_DIRS}
		${Qt5Network_INCLUDE_DIRS}
		${Qt5PrintSupport_INCLUDE_DIRS}
	)

	check_class_member_function(QPlainTextEdit setTabStopWidthF QPlainTextEdit HAVE_QREAL_TAB_STOPS)
	configure_file(${CMAKE_SOURCE_DIR}/Config.h.in ${CMAKE_BINARY_DIR}/Config.h)
endfunction()
