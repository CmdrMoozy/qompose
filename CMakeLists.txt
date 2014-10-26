###############################################################################
#
# This is the CMake build file for Qompose. The build process can be controlled
# in several ways:
#
#     - To enable "release" or "debug" builds, CMake should be instantiated as:
#
#           cmake -DCMAKE_BUILD_TYPE=Release ..		(This is the default.)
#           cmake -DCMAKE_BUILD_TYPE=Debug ..
#
###############################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(qompose)

SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE OFF)

IF(CMAKE_MAJOR_VERSION GREATER 2)
	# Use the new behavior for COMPILE_DEFINITIONS_<Config> properties.
	# See: http://www.cmake.org/cmake/help/v3.0/policy/CMP0043.html
	cmake_policy(SET CMP0043 NEW)
ENDIF()

# Automatically link to the static qtmain.lib on Windows.
# See: http://www.cmake.org/cmake/help/v3.0/policy/CMP0020.html
cmake_policy(SET CMP0020 NEW)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# Allow for custom Qt prefixes, for e.g. developer builds.

IF(CUSTOM_QT_DIR)
	SET(CMAKE_PREFIX_PATH ${CUSTOM_QT_DIR} ${CMAKE_PREFIX_PATH})
	SET(QT_ROOT_DIR ${CUSTOM_QT_DIR})
	SET(QT_QMAKE_EXECUTABLE ${QT_ROOT_DIR}/bin/qmake)
ENDIF()

# Note which type of build we're performing.

IF(NOT CMAKE_BUILD_TYPE)
	MESSAGE(STATUS "No CMAKE_BUILD_TYPE specified; defaulting to Release.")
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

STRING(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

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
	SET(F_DBG "-Wall -Wextra -ansi -pedantic -Wshadow -Wpointer-arith")
	SET(F_DBG "${F_DBG} -Wcast-qual -std=c++11 -O0")

	SET(F_REL "-Wall -Wextra -ansi -pedantic -Wshadow -Wpointer-arith")
	SET(F_REL "${F_REL} -Wcast-qual -std=c++11 -fomit-frame-pointer")

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

INCLUDE_DIRECTORIES("src/libqompose")

# Define Qompose's source files.

SET(qompose_RESOURCES

	data.qrc
	icons.qrc

)

SET(libqompose_SOURCES

	src/libqompose/QomposeApplication.cpp
	src/libqompose/QomposeApplication.h
	src/libqompose/QomposeDefines.h
	src/libqompose/QomposeTypes.h
	src/libqompose/QomposeWindow.cpp
	src/libqompose/QomposeWindow.h

	src/libqompose/dialogs/QomposeAboutDialog.cpp
	src/libqompose/dialogs/QomposeAboutDialog.h
	src/libqompose/dialogs/QomposeEncodingDialog.cpp
	src/libqompose/dialogs/QomposeEncodingDialog.h
	src/libqompose/dialogs/QomposeFileDialog.cpp
	src/libqompose/dialogs/QomposeFileDialog.h
	src/libqompose/dialogs/QomposeFindDialog.cpp
	src/libqompose/dialogs/QomposeFindDialog.h
	src/libqompose/dialogs/QomposeGoToDialog.cpp
	src/libqompose/dialogs/QomposeGoToDialog.h
	src/libqompose/dialogs/QomposeReplaceDialog.cpp
	src/libqompose/dialogs/QomposeReplaceDialog.h

	src/libqompose/dialogs/preferences/QomposePreferencesDialog.cpp
	src/libqompose/dialogs/preferences/QomposePreferencesDialog.h
	src/libqompose/dialogs/preferences/QomposePreferencesListModel.cpp
	src/libqompose/dialogs/preferences/QomposePreferencesListModel.h
	src/libqompose/dialogs/preferences/QomposePreferencesListView.cpp
	src/libqompose/dialogs/preferences/QomposePreferencesListView.h

	src/libqompose/dialogs/preferences/widgets/QomposeEditorPreferencesWidget.cpp
	src/libqompose/dialogs/preferences/widgets/QomposeEditorPreferencesWidget.h
	src/libqompose/dialogs/preferences/widgets/QomposeGeneralPreferencesWidget.cpp
	src/libqompose/dialogs/preferences/widgets/QomposeGeneralPreferencesWidget.h
	src/libqompose/dialogs/preferences/widgets/QomposeOpenSavePreferencesWidget.cpp
	src/libqompose/dialogs/preferences/widgets/QomposeOpenSavePreferencesWidget.h
	src/libqompose/dialogs/preferences/widgets/QomposePreferencesScrollArea.cpp
	src/libqompose/dialogs/preferences/widgets/QomposePreferencesScrollArea.h
	src/libqompose/dialogs/preferences/widgets/QomposePreferencesWidget.cpp
	src/libqompose/dialogs/preferences/widgets/QomposePreferencesWidget.h

	src/libqompose/editor/QomposeBuffer.cpp
	src/libqompose/editor/QomposeBuffer.h
	src/libqompose/editor/QomposeDecoratedTextEdit.cpp
	src/libqompose/editor/QomposeDecoratedTextEdit.h
	src/libqompose/editor/QomposeEditor.cpp
	src/libqompose/editor/QomposeEditor.h
	src/libqompose/editor/QomposeGutter.cpp
	src/libqompose/editor/QomposeGutter.h

	src/libqompose/gui/QomposeBufferWidget.cpp
	src/libqompose/gui/QomposeBufferWidget.h
	src/libqompose/gui/QomposeColorPickerButton.cpp
	src/libqompose/gui/QomposeColorPickerButton.h
	src/libqompose/gui/QomposeFontPickerButton.cpp
	src/libqompose/gui/QomposeFontPickerButton.h
	src/libqompose/gui/QomposeGUIUtils.cpp
	src/libqompose/gui/QomposeGUIUtils.h
	src/libqompose/gui/QomposeNotificationLabel.cpp
	src/libqompose/gui/QomposeNotificationLabel.h
	src/libqompose/gui/QomposeStatusBar.cpp
	src/libqompose/gui/QomposeStatusBar.h

	src/libqompose/gui/menus/QomposeMainMenu.cpp
	src/libqompose/gui/menus/QomposeMainMenu.h
	src/libqompose/gui/menus/QomposeRecentMenu.cpp
	src/libqompose/gui/menus/QomposeRecentMenu.h

	src/libqompose/syntax/QomposeLexer.cpp
	src/libqompose/syntax/QomposeLexer.h
	src/libqompose/syntax/QomposeSyntaxHighlighter.cpp
	src/libqompose/syntax/QomposeSyntaxHighlighter.h

	src/libqompose/util/QomposeDocumentWriter.cpp
	src/libqompose/util/QomposeDocumentWriter.h
	src/libqompose/util/QomposeFindQuery.cpp
	src/libqompose/util/QomposeFindQuery.h
	src/libqompose/util/QomposeFontMetrics.cpp
	src/libqompose/util/QomposeFontMetrics.h
	src/libqompose/util/QomposeHotkey.cpp
	src/libqompose/util/QomposeHotkey.h
	src/libqompose/util/QomposeHotkeyMap.h
	src/libqompose/util/QomposeReplaceQuery.cpp
	src/libqompose/util/QomposeReplaceQuery.h
	src/libqompose/util/QomposeSettings.cpp
	src/libqompose/util/QomposeSettings.h

)

SET(Qompose_SOURCES

	src/Qompose/Qompose.cpp

)

SET(QomposeTest_SOURCES

	src/QomposeTest/QomposeAssertionException.cpp
	src/QomposeTest/QomposeAssertionException.h
	src/QomposeTest/QomposeTest.cpp
	src/QomposeTest/QomposeTest.h
	src/QomposeTest/QomposeTestSuite.cpp

	src/QomposeTest/util/QomposeFontMetricsTest.cpp
	src/QomposeTest/util/QomposeFontMetricsTest.h
	src/QomposeTest/util/QomposeHotkeyMapTest.cpp
	src/QomposeTest/util/QomposeHotkeyMapTest.h
	src/QomposeTest/util/QomposeHotkeyTest.cpp
	src/QomposeTest/util/QomposeHotkeyTest.h

)

# Build our project!

QT5_ADD_RESOURCES(qompose_RESOURCES_RCC ${qompose_RESOURCES})

SET(Qompose_LIBS

	qompose
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

ADD_LIBRARY(qompose STATIC ${qompose_RESOURCES_RCC} ${libqompose_SOURCES})

ADD_EXECUTABLE(Qompose ${Qompose_SOURCES})
TARGET_LINK_LIBRARIES(Qompose ${Qompose_LIBS})

IF(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
	ADD_EXECUTABLE(QomposeTest ${QomposeTest_SOURCES})
	TARGET_LINK_LIBRARIES(QomposeTest ${Qompose_LIBS} ${DWARF_LIBRARIES})
	SET_PROPERTY(TARGET QomposeTest PROPERTY INCLUDE_DIRECTORIES
		"${CMAKE_SOURCE_DIR}/backward-cpp"
		"${CMAKE_SOURCE_DIR}/src"
		"${CMAKE_SOURCE_DIR}/src/libqompose"
	)
ENDIF()

QT5_USE_MODULES(qompose Core Widgets Network PrintSupport)
QT5_USE_MODULES(Qompose Core Widgets Network PrintSupport)
QT5_USE_MODULES(QomposeTest Core Widgets Network PrintSupport)
