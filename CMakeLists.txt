project(TelepathyLoggerQt)

cmake_minimum_required(VERSION 2.8.12)

# Making releases:
#   set the new version number:
#     odd minor -> development series
#     even minor -> stable series
#     increment micro for each release within a series
#   set nano_version to 0
#   make the release, tag it
#   set nano_version to 1
set(TP_LOGGER_QT_MAJOR_VERSION 0)
set(TP_LOGGER_QT_MINOR_VERSION 9)
set(TP_LOGGER_QT_MICRO_VERSION 0)
set(TP_LOGGER_QT_NANO_VERSION  0)

# This value contains the library's SOVERSION. This value is to be increased everytime an API/ABI break
# occurs, and will be used for the SOVERSION of the generated shared libraries.
set(TP_LOGGER_QT_ABI_VERSION 1)
# This variable is used for the library's long version. It is generated dynamically, so don't change its
# value! Change TP_LOGGER_QT_ABI_VERSION and TP_LOGGER_QT_*_VERSION instead.
if (${TP_LOGGER_QT_NANO_VERSION} EQUAL 0)
    set(TP_LOGGER_QT_LIBRARY_VERSION ${TP_LOGGER_QT_ABI_VERSION}.${TP_LOGGER_QT_MAJOR_VERSION}.${TP_LOGGER_QT_MINOR_VERSION}.${TP_LOGGER_QT_MICRO_VERSION})
else (${TP_LOGGER_QT_NANO_VERSION} EQUAL 0)
    set(TP_LOGGER_QT_LIBRARY_VERSION ${TP_LOGGER_QT_ABI_VERSION}.${TP_LOGGER_QT_MAJOR_VERSION}.${TP_LOGGER_QT_MINOR_VERSION}.${TP_LOGGER_QT_MICRO_VERSION}.${TP_QT_NANO_VERSION})
endif (${TP_LOGGER_QT_NANO_VERSION} EQUAL 0)

set(PACKAGE_NAME telepathy-logger-qt5)

if (${TP_LOGGER_QT_NANO_VERSION} EQUAL 0)
    set(PACKAGE_VERSION ${TP_LOGGER_QT_MAJOR_VERSION}.${TP_LOGGER_QT_MINOR_VERSION}.${TP_LOGGER_QT_MICRO_VERSION})
else (${TP_LOGGER_QT_NANO_VERSION} EQUAL 0)
    set(PACKAGE_VERSION ${TP_LOGGER_QT_MAJOR_VERSION}.${TP_LOGGER_QT_MINOR_VERSION}.${TP_LOGGER_QT_MICRO_VERSION}.${TP_LOGGER_QT_NANO_VERSION})
endif (${TP_LOGGER_QT_NANO_VERSION} EQUAL 0)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is
# checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# This file contains all the needed initialization macros
include(GNUInstallDirs)
include(TelepathyDefaults)
include(FeatureSummary)

# TelepathyLoggerQt specific defines needed to trigger deprecation warnings
if (CXX_DEPRECATED_DECLARATIONS)
    set(DEPRECATED_DECLARATIONS_FLAGS "${DEPRECATED_DECLARATIONS_FLAGS} -DTELEPATHY_LOGGER_QT_DEPRECATED_WARNINGS")
endif (CXX_DEPRECATED_DECLARATIONS)

# external dependencies

# Required dependencies
find_package(Qt5 REQUIRED)

set (TELEPATHYQT_MIN_VERSION "0.9.1")
find_package(TelepathyQt5 ${TELEPATHYQT_MIN_VERSION} REQUIRED)
set_package_properties(TelepathyQt5 PROPERTIES
                       DESCRIPTION "Qt bindings for Telepathy"
                       URL "http://telepathy.freedesktop.org"
                       TYPE REQUIRED)

# This file contains all macros used in the buildsystem
include(TpQtMacros)

include(Doxygen)
include(MacroWriteBasicCMakeVersionFile)

#add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(ENABLE_DEBUG_OUTPUT ON CACHE BOOL "If activated, compiles support for printing debug output to stderr")
if (ENABLE_DEBUG_OUTPUT)
    add_definitions(-DENABLE_DEBUG)
endif (ENABLE_DEBUG_OUTPUT)

# Find python version >= 2.5
find_package(PythonLibrary REQUIRED)
set(REQUIRED_PY 2.5)
if(${PYTHON_SHORT_VERSION} VERSION_GREATER ${REQUIRED_PY} OR ${PYTHON_SHORT_VERSION} VERSION_EQUAL ${REQUIRED_PY})
    message(STATUS "Python ${PYTHON_SHORT_VERSION} found")
else(${PYTHON_SHORT_VERSION} VERSION_GREATER ${REQUIRED_PY} OR ${PYTHON_SHORT_VERSION} VERSION_EQUAL ${REQUIRED_PY})
    message(SEND_ERROR "Python >= ${REQUIRED_PY} is required")
endif(${PYTHON_SHORT_VERSION} VERSION_GREATER ${REQUIRED_PY} OR ${PYTHON_SHORT_VERSION} VERSION_EQUAL ${REQUIRED_PY})
set_package_properties(PythonLibrary PROPERTIES
                       DESCRIPTION "Python Library"
                       URL "http://www.python.org"
                       TYPE REQUIRED)

# Check for Qt Glib support
include(CheckCXXSourceCompiles)

set(CMAKE_REQUIRED_INCLUDES ${Qt5Core_INCLUDE_DIRS})
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
QT_GLIB_SUPPORT)
if (QT_GLIB_SUPPORT)
    message(STATUS "Qt was compiled with GLib support enabled")
else ()
    message(FATAL_ERROR "Qt was not compiled with GLib support enabled")
endif()

find_package(GLIB2 REQUIRED)
set_package_properties(GLIB2 PROPERTIES
                       DESCRIPTION "A library containing many useful C routines"
                       URL "http://developer.gnome.org/glib/"
                       TYPE REQUIRED)

find_package(GObject REQUIRED)
set_package_properties(GObject PROPERTIES
                       DESCRIPTION "GLib Object System"
                       URL "http://developer.gnome.org/glib/"
                       TYPE REQUIRED)

find_package(DBus REQUIRED)
set_package_properties(DBus PROPERTIES
                       DESCRIPTION "Inter-process communication system"
                       URL "http://dbus.freedesktop.org"
                       TYPE REQUIRED)

find_package(DBusGLib REQUIRED)
set_package_properties(DBusGLib PROPERTIES
                       DESCRIPTION "GLib bindings for DBus"
                       URL "http://dbus.freedesktop.org"
                       TYPE REQUIRED)

find_package(LibXml2 REQUIRED)
set_package_properties(LibXml2 PROPERTIES
                       DESCRIPTION "XML C parser and toolkit"
                       URL "http://www.xmlsoft.org"
                       TYPE REQUIRED)

find_package(QtGLib REQUIRED)
set_package_properties(QtGLib PROPERTIES
                       DESCRIPTION "Qt bindings for GLib, part of QtGStreamer"
                       URL "http://www.qt-project.org"
                       TYPE REQUIRED)

set(TELEPATHY_LOGGER_MIN_VERSION 0.8.0)
find_package(TelepathyLogger ${TELEPATHY_LOGGER_MIN_VERSION} REQUIRED)
set_package_properties(TelepathyLogger PROPERTIES
                       DESCRIPTION "Daemon for logging Telepathy conversations"
                       URL "http://telepathy.freedesktop.org"
                       TYPE REQUIRED)

add_definitions(-DTPLQT_TPL_VERSION_MAJOR=${TELEPATHY_LOGGER_VERSION_MAJOR})
add_definitions(-DTPLQT_TPL_VERSION_MINOR=${TELEPATHY_LOGGER_VERSION_MINOR})
add_definitions(-DTPLQT_TPL_VERSION_PATCH=${TELEPATHY_LOGGER_VERSION_PATCH})

#find_program(GLIB_GENMARSHAL glib-genmarshal)

# Find telepathy-glib
set(TELEPATHY_GLIB_MIN_VERSION 0.16.0)
find_package(TelepathyGlib ${TELEPATHY_GLIB_MIN_VERSION} REQUIRED)
set_package_properties(TelepathyGlib PROPERTIES
                       DESCRIPTION "Glib bindings for Telepathy"
                       URL "http://telepathy.freedesktop.org"
                       TYPE REQUIRED)

# Activate debug symbols generation
#set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_BINARY_DIR}
                    ${TELEPATHY_QT5_INCLUDE_DIR}
                    ${TELEPATHY_LOGGER_INCLUDE_DIR}
                    ${TELEPATHY_GLIB_INCLUDE_DIR}
                    ${GLIB2_INCLUDE_DIR}
                    ${DBUS_GLIB_INCLUDE_DIR})

# Search codegen to see if it's installed somewhere
#find_program(CODEGEN_BIN NAMES codegen PATHS /usr/bin /usr/local/bin ~/bin ./codegen ../codegen)

# Macro to run codegen from the subdirs
macro(run_codegen _dir_name _includes _headers)
    set(_prefixed_headers "")
    foreach(_header ${_headers})
        list(APPEND _prefixed_headers ${_dir_name}/${_header})
    endforeach()

    add_custom_command(OUTPUT gen.cpp
                       COMMAND codegen
                       ARGS ${_includes} ${_prefixed_headers}
                              > ${CMAKE_CURRENT_BINARY_DIR}/gen.cpp
                       DEPENDS codegen ${_headers}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)
endmacro()


# Add the source subdirectories
add_subdirectory(codegen)
add_subdirectory(TelepathyLoggerQt)
#add_subdirectory(tests)
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

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/FIXME.out
                   COMMAND egrep
                   ARGS -A 5 '[F]IXME|[T]ODO|[X]XX' ${CMAKE_SOURCE_DIR}/TelepathyLoggerQt/*.[ch]*
                        > FIXME.out || true)
add_custom_target(check-local DEPENDS ${CMAKE_BINARY_DIR}/FIXME.out)

include(TelepathyDist)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
