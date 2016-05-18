set(QomposeTest_SOURCES

	FontMetricsTest.cpp
	QomposeTest.cpp

	dialogs/FindDialogTest.cpp
	dialogs/ReplaceDialogTest.cpp

	editor/EditorTest.cpp

	editor/algorithm/IndentationTest.cpp
	editor/algorithm/MovementTest.cpp

	hotkey/HotkeyMapTest.cpp
	hotkey/HotkeyTest.cpp

	settings/RoundTripTest.cpp
	settings/SettingsTest.cpp

	util/EncodingTest.cpp

)

add_executable(QomposeTest ${QomposeTest_SOURCES})
target_link_libraries(QomposeTest QomposeCommon ${qompose_LIBRARIES})
qt5_use_modules(QomposeTest Core Gui Widgets Network PrintSupport)

add_test(QomposeTest QomposeTest)
