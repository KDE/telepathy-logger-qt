project(TelepathyQt4Logger)

cmake_minimum_required(VERSION 2.6)

# CMake policies are used for backwards compatibilty. Setting a policy to a behavior lets newer
# CMake versions where some behaviors changed behave in a way or another. In our specific case,
# From CMake's documentation:
#
# In CMake 2.6.2 and below, CMake Policy settings in scripts loaded by
# the include() and find_package() commands would affect the includer.
# Explicit invocations of cmake_policy(PUSH) and cmake_policy(POP) were
# required to isolate policy changes and protect the includer.  While
# some scripts intend to affect the policies of their includer, most do
# not.  In CMake 2.6.3 and above, include() and find_package() by
# default PUSH and POP an entry on the policy stack around an included
# script, but provide a NO_POLICY_SCOPE option to disable it.  This
# policy determines whether or not to imply NO_POLICY_SCOPE for
# compatibility.  The OLD behavior for this policy is to imply
# NO_POLICY_SCOPE for include() and find_package() commands.  The NEW
# behavior for this policy is to allow the commands to do their default
# cmake_policy PUSH and POP.
#
# This policy was introduced in CMake version 2.6.3.  CMake version
# 2.8.2 warns when the policy is not set and uses OLD behavior.  Use the
# cmake_policy command to set it to OLD or NEW explicitly.
#
# Whenever our cmake_minimum_required version bumps up to 2.7 or 2.6.3, this policy setting can
# hence be removed.
if(POLICY CMP0011)
    cmake_policy(SET CMP0011 NEW)
endif(POLICY CMP0011)

# Making releases:
#   set the new version number:
#     odd minor -> development series
#     even minor -> stable series
#     increment micro for each release within a series
#   set nano_version to 0
#   make the release, tag it
#   set nano_version to 1
set(TP_QT4_LOGGER_MAJOR_VERSION 0)
set(TP_QT4_LOGGER_MINOR_VERSION 1)
set(TP_QT4_LOGGER_MICRO_VERSION 0)
set(TP_QT4_LOGGER_NANO_VERSION  0)

# This value contains the library's SOVERSION. This value is to be increased everytime an API/ABI break
# occurs, and will be used for the SOVERSION of the generated shared libraries.
set(TP_QT4_LOGGER_ABI_VERSION 0)
# This variable is used for the library's long version. It is generated dynamically, so don't change its
# value! Change TP_QT4_LOGGER_ABI_VERSION and TP_QT4_LOGGER_*_VERSION instead.
if (${TP_QT4_LOGGER_NANO_VERSION} EQUAL 0)
    set(TP_QT4_LOGGER_LIBRARY_VERSION ${TP_QT4_LOGGER_ABI_VERSION}.${TP_QT4_LOGGER_MAJOR_VERSION}.${TP_QT4_LOGGER_MINOR_VERSION}.${TP_QT4_LOGGER_MICRO_VERSION})
else (${TP_QT4_LOGGER_NANO_VERSION} EQUAL 0)
    set(TP_QT4_LOGGER_LIBRARY_VERSION ${TP_QT4_LOGGER_ABI_VERSION}.${TP_QT4_LOGGER_MAJOR_VERSION}.${TP_QT4_LOGGER_MINOR_VERSION}.${TP_QT4_LOGGER_MICRO_VERSION}.${TP_QT4_NANO_VERSION})
endif (${TP_QT4_LOGGER_NANO_VERSION} EQUAL 0)

set(PACKAGE_NAME telepathy-qt4-logger)

if (${TP_QT4_LOGGER_NANO_VERSION} EQUAL 0)
    set(PACKAGE_VERSION ${TP_QT4_LOGGER_MAJOR_VERSION}.${TP_QT4_LOGGER_MINOR_VERSION}.${TP_QT4_LOGGER_MICRO_VERSION})
else (${TP_QT4_LOGGER_NANO_VERSION} EQUAL 0)
    set(PACKAGE_VERSION ${TP_QT4_LOGGER_MAJOR_VERSION}.${TP_QT4_LOGGER_MINOR_VERSION}.${TP_QT4_LOGGER_MICRO_VERSION}.${TP_QT4_LOGGER_NANO_VERSION})
endif (${TP_QT4_LOGGER_NANO_VERSION} EQUAL 0)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is
# checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Default build type is RelWithDebInfo for release versions and Debug for developement
# versions
if(NOT CMAKE_BUILD_TYPE)
    if(TP_QT4_LOGGER_NANO_VERSION EQUAL 0)
        set(CMAKE_BUILD_TYPE RelWithDebInfo)
    else(TP_QT4_LOGGER_NANO_VERSION EQUAL 0)
        set(CMAKE_BUILD_TYPE Debug)
    endif(TP_QT4_LOGGER_NANO_VERSION EQUAL 0)
endif(NOT CMAKE_BUILD_TYPE)

# This file contains all the needed initialization macros
include(TelepathyDefaults)

# TelepathyQt4Logger specific defines needed to trigger deprecation warnings
if (CXX_DEPRECATED_DECLARATIONS)
    set(DEPRECATED_DECLARATIONS_FLAGS "${DEPRECATED_DECLARATIONS_FLAGS} -DTELEPATHY_QT4_LOGGER_DEPRECATED_WARNINGS")
endif (CXX_DEPRECATED_DECLARATIONS)

# This file contains all macros used in the buildsystem
include(TpQt4Macros)

include(Doxygen)
include(MacroLogFeature)

# external dependencies

# Required dependencies
# Find qt4 version >= 4.5
set (QT_MIN_VERSION "4.5.0")
find_package(Qt4 REQUIRED)
find_package(TelepathyQt4 REQUIRED)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(ENABLE_DEBUG_OUTPUT ON CACHE BOOL "If activated, compiles support for printing debug output to stderr")
if (ENABLE_DEBUG_OUTPUT)
    add_definitions(-DENABLE_DEBUG)
endif (ENABLE_DEBUG_OUTPUT)

# Check for Qt4 Glib support
include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_INCLUDES ${QT_INCLUDES})
set(CMAKE_REQUIRED_DEFINITIONS "")
set(CMAKE_REQUIRED_FLAGS "")

CHECK_CXX_SOURCE_COMPILES("
#include <QtCore/QtGlobal>
int main()
{
#if defined(QT_NO_GLIB)
#error \"Qt was compiled with Glib disabled\"
#endif
return 0;
}"
QT4_GLIB_SUPPORT)
macro_log_feature(QT4_GLIB_SUPPORT "Qt4 Glib Support"
                  "QtCore library using Glib's main event loop"
                  "http://qt.nokia.com/" FALSE ""
                  "Needed, together with Telepathy-Glib, to build most of the unit tests")

find_package(GLIB2 REQUIRED)
find_package(GObject REQUIRED)
find_package(DBus REQUIRED)
find_package(DBusGLib REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(QtGLib REQUIRED)
find_package(TelepathyLogger REQUIRED)

#find_program(GLIB_GENMARSHAL glib-genmarshal)

# Find telepathy-glib
set(TELEPATHY_GLIB_MIN_VERSION 0.13.0)
find_package(TelepathyGlib REQUIRED)

# Activate debug symbols generation
#set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_BINARY_DIR}
                    ${QT_INCLUDES}
                    ${TELEPATHY_QT4_INCLUDE_DIR}
                    ${TELEPATHY_LOGGER_INCLUDE_DIR}
                    ${GLIB2_INCLUDE_DIR}
                    ${DBUS_GLIB_INCLUDE_DIR}
                    ${QTGLIB_INCLUDE_DIR})

# Add the source subdirectories
add_subdirectory(TelepathyQt4Logger)
add_subdirectory(tests)
#add_subdirectory(tools)

# Generate config.h
configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/config.h)

# Create the uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Display the feature log
macro_display_feature_log()

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/FIXME.out
                   COMMAND egrep
                   ARGS -A 5 '[F]IXME|[T]ODO|[X]XX' ${CMAKE_SOURCE_DIR}/TelepathyQt4Logger/*.[ch]*
                        > FIXME.out || true)
add_custom_target(check-local DEPENDS ${CMAKE_BINARY_DIR}/FIXME.out)

include(TelepathyDist)
