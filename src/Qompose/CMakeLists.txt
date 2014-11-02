SET(Qompose_SOURCES

	Qompose.cpp

)

IF(UNIX)
	ADD_EXECUTABLE(Qompose ${Qompose_SOURCES})
ELSE()
	ADD_EXECUTABLE(Qompose WIN32 ${Qompose_SOURCES})
ENDIF()

TARGET_LINK_LIBRARIES(Qompose ${Qompose_LIBS})

QT5_USE_MODULES(Qompose Core Widgets Network PrintSupport)
