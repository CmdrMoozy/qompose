set(QomposeTest_SOURCES

	DecoratedTextEditTest.cpp
	FontMetricsTest.cpp
	QomposeTest.cpp

	editor/algorithm/MovementTest.cpp

	hotkey/HotkeyMapTest.cpp
	hotkey/HotkeyTest.cpp

)

add_executable(QomposeTest ${QomposeTest_SOURCES})
target_link_libraries(QomposeTest ${Qompose_LIBS})

qt5_use_modules(QomposeTest Core Gui Widgets Network PrintSupport)

add_test(QomposeTest QomposeTest)
