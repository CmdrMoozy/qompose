##############################################################################
#
# This is the CMake build file for Qompose. The build process can be controlled
# in several ways:
#
#     - To enable "release" or "debug" builds, CMake should be instantiated as:
#
#           cmake -DCMAKE_BUILD_TYPE=Release ..		(This is the default.)
#           cmake -DCMAKE_BUILD_TYPE=Debug ..
#
##############################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)
PROJECT(qompose)

SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE OFF)

# Use the new behavior for COMPILE_DEFINITIONS_<Config> target properties.
# See: http://www.cmake.org/cmake/help/v3.0/policy/CMP0043.html
cmake_policy(SET CMP0043 NEW)

# Automatically link to the static qtmain.lib on Windows.
# See: http://www.cmake.org/cmake/help/v3.0/policy/CMP0020.html
cmake_policy(SET CMP0020 NEW)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# Note which type of build we're performing.

IF(NOT CMAKE_BUILD_TYPE)
	MESSAGE(STATUS "No CMAKE_BUILD_TYPE specified; defaulting to Release.")
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

STRING(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

# Set the Qt prefix we're going to use, if a custom one was requested.

IF(CUSTOM_QT_DIR)
	SET(CMAKE_PREFIX_PATH ${CUSTOM_QT_DIR} ${CMAKE_PREFIX_PATH})
	SET(QT_ROOT_DIR ${CUSTOM_QT_DIR})
	SET(QT_QMAKE_EXECUTABLE ${QT_ROOT_DIR}/bin/qmake)
ENDIF()

# Define what Qt dependencies we have.

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(ICU COMPONENTS data uc i18n REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)
FIND_PACKAGE(Qt5PrintSupport REQUIRED)

IF(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
	FIND_PACKAGE(DWARF REQUIRED)
ENDIF()

# Set INCLUDE and C(XX)FLAGS

IF(UNIX)
	SET(F_DBG "-Wall -Wextra -ansi -pedantic -Wshadow -Wpointer-arith -Wcast-qual")
	SET(F_DBG "${F_DBG} -std=c++11 -O0")

	SET(F_REL "-Wall -Wextra -ansi -pedantic -Wshadow -Wpointer-arith -Wcast-qual")
	SET(F_REL "${F_REL} -std=c++11 -fomit-frame-pointer")

	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${F_DBG}")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${F_REL}")
ELSE()
	# Use a static MSVC runtime on Windows.
	
        SET(CMAKE_CXX_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
        SET(CMAKE_CXX_FLAGS_MINSIZEREL_INIT "/MT /O1 /Ob1 /D NDEBUG")
        SET(CMAKE_CXX_FLAGS_RELEASE_INIT "/MT /O2 /Ob2 /D NDEBUG")
        SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /O2 /Ob1 /D NDEBUG")
ENDIF()

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

IF(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
	ADD_DEFINITIONS(-DQOMPOSE_DEBUG)
	ADD_DEFINITIONS(-DBACKWARD_HAS_DW=1)
ENDIF()

INCLUDE_DIRECTORIES("src" "backward-cpp")

# Define Qompose's source files.

SET(qompose_RESOURCES

	data.qrc
	icons.qrc

)

SET(qompose_HEADERS

	src/QomposeApplication.h
	src/QomposeTypes.h
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
	src/dialogs/preferences/widgets/QomposeOpenSavePreferencesWidget.h
	src/dialogs/preferences/widgets/QomposePreferencesScrollArea.h
	src/dialogs/preferences/widgets/QomposePreferencesWidget.h

	src/editor/QomposeBuffer.h
	src/editor/QomposeDecoratedTextEdit.h
	src/editor/QomposeEditor.h
	src/editor/QomposeGutter.h

	src/gui/QomposeBufferWidget.h
	src/gui/QomposeColorPickerButton.h
	src/gui/QomposeFontPickerButton.h
	src/gui/QomposeGUIUtils.h
	src/gui/QomposeNotificationLabel.h
	src/gui/QomposeStatusBar.h

	src/gui/menus/QomposeMainMenu.h
	src/gui/menus/QomposeRecentMenu.h

	src/syntax/QomposeLexer.h
	src/syntax/QomposeSyntaxHighlighter.h

	src/util/QomposeDocumentWriter.h
	src/util/QomposeFindQuery.h
	src/util/QomposeFontMetrics.h
	src/util/QomposeHotkey.h
	src/util/QomposeHotkeyMap.h
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
	src/dialogs/preferences/widgets/QomposeOpenSavePreferencesWidget.cpp
	src/dialogs/preferences/widgets/QomposePreferencesScrollArea.cpp
	src/dialogs/preferences/widgets/QomposePreferencesWidget.cpp

	src/editor/QomposeBuffer.cpp
	src/editor/QomposeDecoratedTextEdit.cpp
	src/editor/QomposeEditor.cpp
	src/editor/QomposeGutter.cpp

	src/gui/QomposeBufferWidget.cpp
	src/gui/QomposeColorPickerButton.cpp
	src/gui/QomposeFontPickerButton.cpp
	src/gui/QomposeGUIUtils.cpp
	src/gui/QomposeNotificationLabel.cpp
	src/gui/QomposeStatusBar.cpp

	src/gui/menus/QomposeMainMenu.cpp
	src/gui/menus/QomposeRecentMenu.cpp

	src/syntax/QomposeLexer.cpp
	src/syntax/QomposeSyntaxHighlighter.cpp

	src/util/QomposeDocumentWriter.cpp
	src/util/QomposeFindQuery.cpp
	src/util/QomposeFontMetrics.cpp
	src/util/QomposeHotkey.cpp
	src/util/QomposeReplaceQuery.cpp
	src/util/QomposeSettings.cpp

)

# Define our test suite's sources.

SET(qompose_test_HEADERS

	src/test/QomposeAssertionException.h
	src/test/QomposeTest.h

	src/test/util/QomposeFontMetricsTest.h
	src/test/util/QomposeHotkeyTest.h
	src/test/util/QomposeHotkeyMapTest.h

	src/util/QomposeFontMetrics.h
	src/util/QomposeHotkey.h
	src/util/QomposeHotkeyMap.h

)

SET(qompose_test_SOURCES

	src/test/QomposeAssertionException.cpp
	src/test/QomposeTest.cpp
	src/test/QomposeTestSuite.cpp

	src/test/util/QomposeFontMetricsTest.cpp
	src/test/util/QomposeHotkeyTest.cpp
	src/test/util/QomposeHotkeyMapTest.cpp

	src/util/QomposeFontMetrics.cpp
	src/util/QomposeHotkey.cpp

)

# Build our project!

QT5_ADD_RESOURCES(qompose_RESOURCES_RCC ${qompose_RESOURCES})

SET(Qompose_LIBS
	
	${ICU_LIBRARIES}
	${Qt5Core_LIBRARIES}
	${Qt5Widgets_LIBRARIES}
	${Qt5Network_LIBRARIES}
	${Qt5PrintSupport_LIBRARIES}
	
)

IF(UNIX)
	SET(Qompose_LIBS ${Qompose_LIBS} m)
ELSE()
	LINK_DIRECTORIES(
		${CMAKE_PREFIX_PATH}/lib
		${CMAKE_PREFIX_PATH}/lib64
		${CMAKE_PREFIX_PATH}/plugins/platforms
	)
	
	SET(Qompose_LIBS
		
		ws2_32
		imm32
		winmm
		Qt5PlatformSupport
		Qt5WinExtras
		qwindows
		opengl32
		qtharfbuzzng
		${Qompose_LIBS}
		
	)
ENDIF()

ADD_EXECUTABLE(Qompose ${qompose_RESOURCES_RCC} ${qompose_SOURCES})
TARGET_LINK_LIBRARIES(Qompose ${Qompose_LIBS})

IF(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
	ADD_EXECUTABLE(QomposeTest ${qompose_test_SOURCES})
	TARGET_LINK_LIBRARIES(QomposeTest ${Qompose_LIBS} ${DWARF_LIBRARIES})
ENDIF()

QT5_USE_MODULES(Qompose Widgets Network PrintSupport)
