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

cmake_minimum_required(VERSION 3.0.2)
project(Qompose)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Use the new behavior for COMPILE_DEFINITIONS_<Config> properties.
# See: http://www.cmake.org/cmake/help/v3.0/policy/CMP0043.html
cmake_policy(SET CMP0043 NEW)

# Automatically link to the static qtmain.lib on Windows.
# See: http://www.cmake.org/cmake/help/v3.0/policy/CMP0020.html
cmake_policy(SET CMP0020 NEW)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Allow for custom Qt prefixes, for e.g. developer builds.

if(CUSTOM_QT_DIR)
	message(STATUS "Using custom Qt prefix: ${CUSTOM_QT_DIR}")
	set(CMAKE_PREFIX_PATH ${CUSTOM_QT_DIR} ${CMAKE_PREFIX_PATH})
	set(QT_ROOT_DIR ${CUSTOM_QT_DIR})
	set(QT_QMAKE_EXECUTABLE ${CUSTOM_QT_DIR}/bin/qmake)
endif()

# Note which type of build we're performing.

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No CMAKE_BUILD_TYPE specified; defaulting to Release.")
	set(CMAKE_BUILD_TYPE "Release")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

# Define what Qt dependencies we have.

set(CMAKE_AUTOMOC ON)

find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Leveldb REQUIRED)
find_package(HUNSPELL REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

# Define CMake options.

option(USE_UNIT_TESTS "enable unit tests" ON)

# Setup our compile flags.

include(${CMAKE_SOURCE_DIR}/3rdparty/cmu/cmake/SetFlags.cmake)
cmuSetCompileFlags()

add_definitions(-DQT_NO_KEYWORDS)

if(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
	add_definitions(-DQOMPOSE_DEBUG)
endif()

include_directories("src")
include_directories(SYSTEM

	3rdparty/include
	${PROTOBUF_INCLUDE_DIRS}
	${CMAKE_BINARY_DIR}/src/core

)

# Define the libraries Qompose uses.

set(qompose_LIBRARIES

	${Qt5Core_LIBRARIES}
	${Qt5Gui_LIBRARIES}
	${Qt5Widgets_LIBRARIES}
	${Qt5Network_LIBRARIES}
	${Qt5PrintSupport_LIBRARIES}
	${PROTOBUF_LIBRARIES}
	${LEVELDB_LIBRARIES}
	${HUNSPELL_LIBRARIES}
	bdrck-config
	bdrck-fs
	bdrck-string
	bdrck-util

)

if(UNIX)
	set(qompose_LIBRARIES m ${qompose_LIBRARIES})
endif()

# Add the various targets.

add_subdirectory(3rdparty/bdrck)
add_subdirectory(src/QomposeCommon)
add_subdirectory(src/qompose)
add_subdirectory(src/core)

if(USE_UNIT_TESTS)
	enable_testing()
	add_subdirectory(src/QomposeTest)
	add_subdirectory(src/core-test)
endif()
