project(TelepathyLoggerQt4)

cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)

set(TP_LOGGER_QT4_MAJOR_VERSION 0)
set(TP_LOGGER_QT4_MINOR_VERSION 9)
set(TP_LOGGER_QT4_MICRO_VERSION 0)
set(TP_LOGGER_QT4_NANO_VERSION  0)
set(TP_LOGGER_QT4_ABI_VERSION 2)

set(TP_LOGGER_QT4_VERSION ${TP_LOGGER_QT4_MAJOR_VERSION}.${TP_LOGGER_QT4_MINOR_VERSION}.${TP_LOGGER_QT4_MICRO_VERSION})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(InstallSettings)
include(GenerateExportHeader)
include(MacroWriteBasicCMakeVersionFile)
include(TelepathyDefaults)
include(FeatureSummary)

# TelepathyLoggerQt4 specific defines needed to trigger deprecation warnings
if (CXX_DEPRECATED_DECLARATIONS)
    set(DEPRECATED_DECLARATIONS_FLAGS "${DEPRECATED_DECLARATIONS_FLAGS} -DTELEPATHY_LOGGER_QT4_DEPRECATED_WARNINGS")
endif (CXX_DEPRECATED_DECLARATIONS)

# This file contains all macros used in the buildsystem
include(TpQtMacros)
include(Doxygen)
include(QtMacros)

# external dependencies

# Required dependencies
# Find qt4 version >= 4.5
set (QT_MIN_VERSION "4.5.0")
find_package(Qt4 ${QT_MIN_VERSION} REQUIRED)
set_package_properties(Qt4 PROPERTIES
                       DESCRIPTION "Qt 4 Framework"
                       URL "http://www.qt-project.org"
                       TYPE REQUIRED)

set (TELEPATHYQT4_MIN_VERSION "0.9.1")
find_package(TelepathyQt4 ${TELEPATHYQT4_MIN_VERSION} REQUIRED)
set_package_properties(TelepathyQt4 PROPERTIES
                       DESCRIPTION "Qt bindings for Telepathy"
                       URL "http://telepathy.freedesktop.org"
                       TYPE REQUIRED)

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

# Find telepathy-glib
set(TELEPATHY_GLIB_MIN_VERSION 0.16.0)
find_package(TelepathyGlib ${TELEPATHY_GLIB_MIN_VERSION} REQUIRED)
set_package_properties(TelepathyGlib PROPERTIES
                       DESCRIPTION "Glib bindings for Telepathy"
                       URL "http://telepathy.freedesktop.org"
                       TYPE REQUIRED)

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


set(TELEPATHY_LOGGER_MIN_VERSION 0.8.0)
find_package(TelepathyLogger ${TELEPATHY_LOGGER_MIN_VERSION} REQUIRED)
set_package_properties(TelepathyLogger PROPERTIES
                       DESCRIPTION "Daemon for logging Telepathy conversations"
                       URL "http://telepathy.freedesktop.org"
                       TYPE REQUIRED)


#add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(ENABLE_DEBUG_OUTPUT ON CACHE BOOL "If activated, compiles support for printing debug output to stderr")
if (ENABLE_DEBUG_OUTPUT)
    add_definitions(-DENABLE_DEBUG)
endif (ENABLE_DEBUG_OUTPUT)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_BINARY_DIR}
                    ${QT_INCLUDES}
                    ${GLIB2_INCLUDE_DIR}
                    ${DBUS_GLIB_INCLUDE_DIR}
                    ${TELEPATHY_GLIB_INCLUDE_DIR}
                    ${TELEPATHY_QT4_INCLUDE_DIR}
                    ${TELEPATHY_LOGGER_INCLUDE_DIR}
)

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
add_subdirectory(TelepathyLoggerQt4)
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
                   ARGS -A 5 '[F]IXME|[T]ODO|[X]XX' ${CMAKE_SOURCE_DIR}/TelepathyLoggerQt4/*.[ch]*
                        > FIXME.out || true)
add_custom_target(check-local DEPENDS ${CMAKE_BINARY_DIR}/FIXME.out)

include(TelepathyDist)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
