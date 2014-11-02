FIND_PACKAGE(DWARF REQUIRED)

SET(QomposeTest_SOURCES

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

INCLUDE_DIRECTORIES(SYSTEM "${CMAKE_SOURCE_DIR}/backward-cpp")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src")

ADD_DEFINITIONS(-DBACKWARD_HAS_DW=1)

ADD_EXECUTABLE(QomposeTest ${QomposeTest_SOURCES})
TARGET_LINK_LIBRARIES(QomposeTest ${Qompose_LIBS} ${DWARF_LIBRARIES})

QT5_USE_MODULES(QomposeTest Core Widgets Network PrintSupport)
