####################################################################################################################
#
# This is the CMake build file for Qompose. The build process can be controlled in several ways:
#
#     - To enable "release" or "debug" builds, CMake should be instantiated as follows:
#
#           cmake -DCMAKE_BUILD_TYPE=Release ..		(This is the default, if neither is selected.)
#           cmake -DCMAKE_BUILD_TYPE=Debug ..
#
####################################################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(qompose)

SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE OFF)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# Define what Qt dependencies we have.

FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(ICU REQUIRED)

SET(QT_USE_QTCORE TRUE)
SET(QT_USE_QTGUI TRUE)
SET(QT_USE_QTNETWORK TRUE)

# Set INCLUDE and C(XX)FLAGS

IF(NOT CMAKE_BUILD_TYPE)
	MESSAGE(STATUS "No CMAKE_BUILD_TYPE specified; defaulting to Release mode.")
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -ansi -pedantic -Wshadow -Wpointer-arith -Wcast-qual -std=c++0x -O0 -DQOMPOSE_DEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -ansi -pedantic -Wshadow -Wpointer-arith -Wcast-qual -std=c++0x -fomit-frame-pointer")

INCLUDE_DIRECTORIES("src")

# Define our project's source files.

SET(qompose_RESOURCES
	
	data.qrc
	icons.qrc
	
)

SET(qompose_HEADERS
	
	src/QomposeApplication.h
	src/QomposeWindow.h
	
	src/dialogs/QomposeAboutDialog.h
	src/dialogs/QomposeEncodingDialog.h
	src/dialogs/QomposeFileDialog.h
	
	src/dialogs/preferences/QomposePreferencesDialog.h
	src/dialogs/preferences/QomposePreferencesListView.h
	src/dialogs/preferences/QomposePreferencesListModel.h
	
	src/dialogs/preferences/widgets/QomposeEditorPreferencesWidget.h
	src/dialogs/preferences/widgets/QomposeGeneralPreferencesWidget.h
	src/dialogs/preferences/widgets/QomposePreferencesWidget.h
	
	src/editor/QomposeBuffer.h
	src/editor/QomposeEditor.h
	
	src/editor/highlighting/QomposeSyntaxHighlighter.h
	
	src/gui/QomposeBufferWidget.h
	src/gui/QomposeColorPickerButton.h
	src/gui/QomposeFontPickerButton.h
	src/gui/QomposeGUIUtils.h
	
	src/util/QomposeSettings.h
	
)

SET(qompose_MOC_HEADERS
	
	src/QomposeApplication.h
	src/QomposeWindow.h
	
	src/dialogs/QomposeAboutDialog.h
	src/dialogs/QomposeEncodingDialog.h
	
	src/dialogs/preferences/QomposePreferencesDialog.h
	
	src/editor/QomposeBuffer.h
	src/editor/QomposeEditor.h
	
	src/editor/highlighting/QomposeSyntaxHighlighter.h
	
	src/gui/QomposeBufferWidget.h
	src/gui/QomposeColorPickerButton.h
	src/gui/QomposeFontPickerButton.h
	
	src/util/QomposeSettings.h
	
)

SET(qompose_SOURCES
	
	src/Qompose.cpp
	src/QomposeApplication.cpp
	src/QomposeWindow.cpp
	
	src/dialogs/QomposeAboutDialog.cpp
	src/dialogs/QomposeEncodingDialog.cpp
	src/dialogs/QomposeFileDialog.cpp
	
	src/dialogs/preferences/QomposePreferencesDialog.cpp
	src/dialogs/preferences/QomposePreferencesListView.cpp
	src/dialogs/preferences/QomposePreferencesListModel.cpp
	
	src/dialogs/preferences/widgets/QomposeEditorPreferencesWidget.cpp
	src/dialogs/preferences/widgets/QomposeGeneralPreferencesWidget.cpp
	src/dialogs/preferences/widgets/QomposePreferencesWidget.cpp
	
	src/editor/QomposeBuffer.cpp
	src/editor/QomposeEditor.cpp
	
	src/editor/highlighting/QomposeSyntaxHighlighter.cpp
	
	src/gui/QomposeBufferWidget.cpp
	src/gui/QomposeColorPickerButton.cpp
	src/gui/QomposeFontPickerButton.cpp
	src/gui/QomposeGUIUtils.cpp
	
	src/util/QomposeSettings.cpp
	
)

# Build our project!

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

QT4_WRAP_CPP(qompose_MOC_SOURCES ${qompose_MOC_HEADERS})

QT4_ADD_RESOURCES(qompose_RESOURCES_RCC ${qompose_RESOURCES})

ADD_EXECUTABLE(Qompose ${qompose_RESOURCES_RCC} ${qompose_MOC_SOURCES} ${qompose_SOURCES})
TARGET_LINK_LIBRARIES(Qompose icuuc icui18n ${QT_LIBRARIES})
