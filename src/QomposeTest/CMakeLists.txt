set(QomposeTest_SOURCES

	FontMetricsTest.cpp
	QomposeTest.cpp

	editor/EditorTest.cpp

	editor/algorithm/IndentationTest.cpp
	editor/algorithm/MovementTest.cpp

	fs/MemoryMappedFileTest.cpp
	fs/TemporaryStorageTest.cpp

	hotkey/HotkeyMapTest.cpp
	hotkey/HotkeyTest.cpp

	settings/RoundTripTest.cpp

	util/EncodingTest.cpp

)

add_executable(QomposeTest ${QomposeTest_SOURCES})
target_link_libraries(QomposeTest ${Qompose_LIBS})

qt5_use_modules(QomposeTest Core Gui Widgets Network PrintSupport)

add_test(QomposeTest QomposeTest)
