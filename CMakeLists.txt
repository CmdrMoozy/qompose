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

CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
PROJECT(qompose)

SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE OFF)

# Use the new behavior for COMPILE_DEFINITIONS_<Config> properties.
# See: http://www.cmake.org/cmake/help/v3.0/policy/CMP0043.html
cmake_policy(SET CMP0043 NEW)

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

# Set INCLUDE and C(XX)FLAGS

IF(CMAKE_CXX_COMPILER_ID MATCHES GNU)
	SET(F_WARN "-pedantic -Wall -Wextra -Wcast-align -Wcast-qual")
	SET(F_WARN "${F_WARN} -Wctor-dtor-privacy -Wdisabled-optimization")
	SET(F_WARN "${F_WARN} -Wformat=2 -Winit-self -Wlogical-op")
	SET(F_WARN "${F_WARN} -Wmissing-declarations -Wmissing-include-dirs")
	SET(F_WARN "${F_WARN} -Wnoexcept -Wold-style-cast -Wshadow")
	SET(F_WARN "${F_WARN} -Woverloaded-virtual -Wredundant-decls")
	SET(F_WARN "${F_WARN} -Wsign-conversion -Wsign-promo -Wundef")
	SET(F_WARN "${F_WARN} -Wstrict-null-sentinel -Wstrict-overflow=5")
	SET(F_WARN "${F_WARN} -Wswitch-default -Weffc++ -Wuseless-cast")
	SET(F_WARN "${F_WARN} -Wzero-as-null-pointer-constant")

	SET(F_DBG "${F_WARN} -std=c++11 -O0")
	SET(F_REL "${F_WARN} -std=c++11 -fomit-frame-pointer")

	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${F_DBG}")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${F_REL}")
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES Clang)
	SET(F_WARN "-pedantic -Wall -Wextra -Wcast-align -Wcast-qual")
	SET(F_WARN "${F_WARN} -Wctor-dtor-privacy -Wdisabled-optimization")
	SET(F_WARN "${F_WARN} -Wformat=2 -Winit-self -Wstrict-overflows=5")
	SET(F_WARN "${F_WARN} -Wmissing-declarations -Wmissing-include-dirs")
	SET(F_WARN "${F_WARN} -Wold-style-cast -Wshadow -Wswitch-default")
	SET(F_WARN "${F_WARN} -Woverloaded-virtual -Wredundant-decls")
	SET(F_WARN "${F_WARN} -Wsign-conversion -Wsign-promo -Wundef")
	SET(F_WARN "${F_WARN} -Wint-to-void-pointer-cast -Weffc++")

	SET(F_DBG "${F_WARN} -std=c++11 -O0")
	SET(F_REL "${F_WARN} -std=c++11 -fomit-frame-pointer")

	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${F_DBG}")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${F_REL}")
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4 /WX /FIQomposeMSVCWarnings.h")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W4 /WX /FIQomposeMSVCWarnings.h")
ENDIF()

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

IF(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
	ADD_DEFINITIONS(-DQOMPOSE_DEBUG)
ENDIF()

INCLUDE_DIRECTORIES("src/QomposeCommon")

# Define the libraries Qompose uses.

SET(Qompose_LIBS

	QomposeCommon
	${ICU_LIBRARIES}
	${Qt5Core_LIBRARIES}
	${Qt5Widgets_LIBRARIES}
	${Qt5Network_LIBRARIES}
	${Qt5PrintSupport_LIBRARIES}

)

IF(UNIX)
	SET(Qompose_LIBS ${Qompose_LIBS} m)
ENDIF()

ADD_SUBDIRECTORY(src/QomposeCommon)
ADD_SUBDIRECTORY(src/Qompose)

IF(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
	ADD_SUBDIRECTORY(src/QomposeTest)
ENDIF()
