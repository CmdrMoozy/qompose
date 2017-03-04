set(Qompose_SOURCES

	Qompose.cpp

)

if(UNIX)
	ADD_EXECUTABLE(qompose ${Qompose_SOURCES})
else()
	ADD_EXECUTABLE(qompose WIN32 ${Qompose_SOURCES})
endif()

target_link_libraries(qompose QomposeCommon ${qompose_LIBRARIES})
qt5_use_modules(qompose Core Gui Widgets Network PrintSupport)
