set(qompose-core_SOURCES

	config/Configuration.cpp
	config/Configuration.hpp

	document/Cursor.cpp
	document/Cursor.hpp
	document/Document.cpp
	document/Document.hpp
	document/DocumentHistory.cpp
	document/DocumentHistory.hpp
	document/PieceTable.cpp
	document/PieceTable.hpp

	file/InMemoryFile.cpp
	file/InMemoryFile.hpp
	file/MMIOFile.cpp
	file/MMIOFile.hpp

	string/Utf8Iterator.cpp
	string/Utf8Iterator.hpp
	string/Utf8String.cpp
	string/Utf8String.hpp
	string/Utf8StringRef.cpp
	string/Utf8StringRef.hpp

)

add_definitions(-DPROTOBUF_INLINE_NOT_IN_HEADERS=0)
file(GLOB proto_FILES "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto")
PROTOBUF_GENERATE_CPP(proto_SOURCES proto_HEADERS ${proto_FILES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(qompose-core STATIC ${qompose-core_SOURCES} ${proto_SOURCES} ${proto_HEADERS})
target_link_libraries(qompose-core ${qompose_LIBRARIES})
