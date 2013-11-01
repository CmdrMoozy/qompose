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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)
PROJECT(qompose)

SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE OFF)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# Define what Qt dependencies we have.

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(ICU REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)
FIND_PACKAGE(Qt5PrintSupport REQUIRED)

# Set INCLUDE and C(XX)FLAGS

IF(NOT CMAKE_BUILD_TYPE)
	MESSAGE(STATUS "No CMAKE_BUILD_TYPE specified; defaulting to Release mode.")
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -ansi -pedantic -Wshadow -Wpointer-arith -Wcast-qual -std=c++0x -O0 -DQOMPOSE_DEBUG -DQT_NO_KEYWORDS")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -ansi -pedantic -Wshadow -Wpointer-arith -Wcast-qual -std=c++0x -fomit-frame-pointer -DQT_NO_KEYWORDS")

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
	src/dialogs/QomposeFindDialog.h
	src/dialogs/QomposeGoToDialog.h
	src/dialogs/QomposeReplaceDialog.h
	
	src/dialogs/preferences/QomposePreferencesDialog.h
	src/dialogs/preferences/QomposePreferencesListView.h
	src/dialogs/preferences/QomposePreferencesListModel.h
	
	src/dialogs/preferences/widgets/QomposeEditorPreferencesWidget.h
	src/dialogs/preferences/widgets/QomposeGeneralPreferencesWidget.h
	src/dialogs/preferences/widgets/QomposePreferencesWidget.h
	
	src/editor/QomposeBuffer.h
	src/editor/QomposeEditor.h
	
	src/gui/QomposeBufferWidget.h
	src/gui/QomposeColorPickerButton.h
	src/gui/QomposeFontPickerButton.h
	src/gui/QomposeGUIUtils.h
	src/gui/QomposeStatusBar.h
	
	src/gui/menus/QomposeMainMenu.h
	src/gui/menus/QomposeRecentMenu.h
	
	src/syntax/QomposeLexer.h
	src/syntax/QomposeSyntaxHighlighter.h
	
	src/util/QomposeFindQuery.h
	src/util/QomposeReplaceQuery.h
	src/util/QomposeSettings.h
	
)

SET(qompose_SOURCES
	
	src/Qompose.cpp
	src/QomposeApplication.cpp
	src/QomposeWindow.cpp
	
	src/dialogs/QomposeAboutDialog.cpp
	src/dialogs/QomposeEncodingDialog.cpp
	src/dialogs/QomposeFileDialog.cpp
	src/dialogs/QomposeFindDialog.cpp
	src/dialogs/QomposeGoToDialog.cpp
	src/dialogs/QomposeReplaceDialog.cpp
	
	src/dialogs/preferences/QomposePreferencesDialog.cpp
	src/dialogs/preferences/QomposePreferencesListView.cpp
	src/dialogs/preferences/QomposePreferencesListModel.cpp
	
	src/dialogs/preferences/widgets/QomposeEditorPreferencesWidget.cpp
	src/dialogs/preferences/widgets/QomposeGeneralPreferencesWidget.cpp
	src/dialogs/preferences/widgets/QomposePreferencesWidget.cpp
	
	src/editor/QomposeBuffer.cpp
	src/editor/QomposeEditor.cpp
	
	src/gui/QomposeBufferWidget.cpp
	src/gui/QomposeColorPickerButton.cpp
	src/gui/QomposeFontPickerButton.cpp
	src/gui/QomposeGUIUtils.cpp
	src/gui/QomposeStatusBar.cpp
	
	src/gui/menus/QomposeMainMenu.cpp
	src/gui/menus/QomposeRecentMenu.cpp
	
	src/syntax/QomposeLexer.cpp
	src/syntax/QomposeSyntaxHighlighter.cpp
	
	src/util/QomposeFindQuery.cpp
	src/util/QomposeReplaceQuery.cpp
	src/util/QomposeSettings.cpp
	
)

# Build our project!

QT5_ADD_RESOURCES(qompose_RESOURCES_RCC ${qompose_RESOURCES})

ADD_EXECUTABLE(Qompose ${qompose_RESOURCES_RCC} ${qompose_SOURCES})
TARGET_LINK_LIBRARIES(Qompose icuuc icui18n ${Qt5Widgets_LIBRARIES} ${Qt5Network_LIBRARIES} ${Qt5PrintSupport_LIBRARIES})

QT5_USE_MODULES(Qompose Widgets Network PrintSupport)
