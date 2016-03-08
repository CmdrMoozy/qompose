set(qompose-core_SOURCES

	document/Cursor.cpp
	document/Cursor.hpp
	document/Document.cpp
	document/Document.hpp
	document/DocumentHistory.cpp
	document/DocumentHistory.hpp
	document/PieceTable.cpp
	document/PieceTable.hpp

	string/Utf8Iterator.cpp
	string/Utf8Iterator.hpp
	string/Utf8String.cpp
	string/Utf8String.hpp
	string/Utf8StringRef.cpp
	string/Utf8StringRef.hpp

)

add_library(qompose-core STATIC ${qompose-core_SOURCES})
target_link_libraries(qompose-core ${Qompose_LIBS})
