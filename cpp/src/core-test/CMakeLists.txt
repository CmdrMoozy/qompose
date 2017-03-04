set(qompose-core-test_SOURCES

	qompose-core-test.cpp

	document/CursorTest.cpp

	file/InMemoryFileTest.cpp
	file/MMIOFileTest.cpp

	string/Utf8StringTest.cpp

)

add_executable(qompose-core-test ${qompose-core-test_SOURCES})
target_link_libraries(qompose-core-test qompose-core)
add_test(qompose-core-test qompose-core-test)
