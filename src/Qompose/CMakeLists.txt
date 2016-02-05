set(Qompose_SOURCES

	Qompose.cpp

)

if(UNIX)
	ADD_EXECUTABLE(Qompose ${Qompose_SOURCES})
else()
	ADD_EXECUTABLE(Qompose WIN32 ${Qompose_SOURCES})
endif()

target_link_libraries(Qompose QomposeCommon ${qompose_LIBRARIES})
qt5_use_modules(Qompose Core Gui Widgets Network PrintSupport)
