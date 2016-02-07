set(qompose-core_SOURCES

	Utf8Iterator.cpp
	Utf8Iterator.hpp
	Utf8String.cpp
	Utf8String.hpp

)

add_library(qompose-core STATIC ${qompose-core_SOURCES})
target_link_libraries(qompose-core ${Qompose_LIBS})
