find_package(DWARF REQUIRED)

set(QomposeTest_SOURCES

	AssertionException.cpp
	AssertionException.h
	Test.cpp
	Test.h
	QomposeTest.cpp

	tests/FontMetricsTest.cpp
	tests/FontMetricsTest.h
	tests/HotkeyMapTest.cpp
	tests/HotkeyMapTest.h
	tests/HotkeyTest.cpp
	tests/HotkeyTest.h

)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/backward-cpp")
add_definitions(-DBACKWARD_HAS_DW=1)

add_executable(QomposeTest ${QomposeTest_SOURCES})
target_link_libraries(QomposeTest ${Qompose_LIBS} ${DWARF_LIBRARIES})

qt5_use_modules(QomposeTest Core Gui Widgets Network PrintSupport)
