find_package(DWARF REQUIRED)

set(QomposeTest_SOURCES

	QomposeAssertionException.cpp
	QomposeAssertionException.h
	QomposeTest.cpp
	QomposeTest.h
	QomposeTestSuite.cpp

	tests/QomposeFontMetricsTest.cpp
	tests/QomposeFontMetricsTest.h
	tests/QomposeHotkeyMapTest.cpp
	tests/QomposeHotkeyMapTest.h
	tests/QomposeHotkeyTest.cpp
	tests/QomposeHotkeyTest.h

)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/backward-cpp")
include_directories("${CMAKE_SOURCE_DIR}/src")

add_definitions(-DBACKWARD_HAS_DW=1)

add_executable(QomposeTest ${QomposeTest_SOURCES})
target_link_libraries(QomposeTest ${Qompose_LIBS} ${DWARF_LIBRARIES})

qt5_use_modules(QomposeTest Core Gui Widgets Network PrintSupport)
