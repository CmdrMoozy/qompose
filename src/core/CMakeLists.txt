set(qompose-core_SOURCES

	Utf8Iterator.cpp
	Utf8Iterator.hpp
	Utf8String.cpp
	Utf8String.hpp

	document/Cursor.cpp
	document/Cursor.hpp
	document/Document.cpp
	document/Document.hpp
	document/DocumentHistory.cpp
	document/DocumentHistory.hpp
	document/PieceTable.cpp
	document/PieceTable.hpp

)

add_library(qompose-core STATIC ${qompose-core_SOURCES})
target_link_libraries(qompose-core ${Qompose_LIBS})
