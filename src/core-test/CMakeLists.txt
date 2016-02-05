set(qompose-core-test_SOURCES

	qompose-core-test.cpp

	Utf8StringTest.cpp

)

add_executable(qompose-core-test ${qompose-core-test_SOURCES})
target_link_libraries(qompose-core-test qompose-core ${Qompose_LIBS})
add_test(qompose-core-test qompose-core-test)
