set(QomposeTest_SOURCES

	DecoratedTextEditTest.cpp
	FontMetricsTest.cpp
	HotkeyMapTest.cpp
	HotkeyTest.cpp
	QomposeTest.cpp

)

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/3rdparty/catch/single_include)

add_executable(QomposeTest ${QomposeTest_SOURCES})
target_link_libraries(QomposeTest ${Qompose_LIBS})

qt5_use_modules(QomposeTest Core Gui Widgets Network PrintSupport)

add_test(QomposeTest QomposeTest)
