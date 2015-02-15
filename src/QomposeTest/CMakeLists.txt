find_package(DWARF REQUIRED)

set(QomposeTest_SOURCES

	QomposeTest.cpp

	tests/DecoratedTextEditTest.cpp
	tests/DecoratedTextEditTest.h
	tests/FontMetricsTest.cpp
	tests/FontMetricsTest.h
	tests/HotkeyMapTest.cpp
	tests/HotkeyMapTest.h
	tests/HotkeyTest.cpp
	tests/HotkeyTest.h

)

include_directories(${CMAKE_SOURCE_DIR}/vrfy/src)

add_executable(QomposeTest ${QomposeTest_SOURCES})
target_link_libraries(QomposeTest ${Qompose_LIBS} ${Vrfy_LIBS})

qt5_use_modules(QomposeTest Core Gui Widgets Network PrintSupport)
