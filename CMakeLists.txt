cmake_minimum_required(VERSION 3.5)

project(TelepathyLoggerQt)

# ECM setup
find_package(ECM 1.6.0 CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} "${TelepathyLoggerQt_SOURCE_DIR}/cmake/modules")

include(GenerateExportHeader)
include(ECMGeneratePriFile)
include(ECMPackageConfigHelpers)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)

include(Doxygen)

set(TELEPATHY_LOGGER_QT_VERSION "0.9.80.0")

ecm_setup_version(${TELEPATHY_LOGGER_QT_VERSION} VARIABLE_PREFIX TelepathyLoggerQt
                  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt_version.h"
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQtConfigVersion.cmake"
                  SOVERSION 5
)

# TelepathyLoggerQt specific defines needed to trigger deprecation warnings
if (CXX_DEPRECATED_DECLARATIONS)
    set(DEPRECATED_DECLARATIONS_FLAGS "${DEPRECATED_DECLARATIONS_FLAGS} -DTELEPATHY_LOGGER_QT_DEPRECATED_WARNINGS")
endif (CXX_DEPRECATED_DECLARATIONS)

################# Find packages #####################
set(QT5_MIN_VERSION "5.2")
find_package(Qt5 ${QT5_MIN_VERSION} REQUIRED COMPONENTS Core DBus Network Xml)

set(TELEPATHY_QT_MIN_VERSION "0.9.1")
find_package(TelepathyQt5 ${TELEPATHY_QT_MIN_VERSION} REQUIRED)
set_package_properties(TelepathyQt5 PROPERTIES
                       DESCRIPTION "Qt bindings for Telepathy"
                       URL "https://telepathy.freedesktop.org"
                       TYPE REQUIRED)

find_package(GLIB2 REQUIRED)
set_package_properties(GLIB2 PROPERTIES
                       DESCRIPTION "A library containing many useful C routines"
                       URL "https://developer.gnome.org/glib/"
                       TYPE REQUIRED)

find_package(GObject REQUIRED)
set_package_properties(GObject PROPERTIES
                       DESCRIPTION "GLib Object System"
                       URL "https://developer.gnome.org/glib/"
                       TYPE REQUIRED)

find_package(DBus REQUIRED)
set_package_properties(DBus PROPERTIES
                       DESCRIPTION "Inter-process communication system"
                       URL "https://dbus.freedesktop.org"
                       TYPE REQUIRED)

find_package(DBusGLib REQUIRED)
set_package_properties(DBusGLib PROPERTIES
                       DESCRIPTION "GLib bindings for DBus"
                       URL "https://dbus.freedesktop.org"
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
                       URL "https://telepathy.freedesktop.org"
                       TYPE REQUIRED)

# Find python
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    find_package(PythonInterp 3 REQUIRED)
    set_package_properties(PythonInterp PROPERTIES
                           DESCRIPTION "Python Interpreter"
                           URL "https://www.python.org"
                           TYPE REQUIRED)

    # PythonInterp exposes PYTHON_EXECUTABLE on its own
else()
    find_package(Python3 REQUIRED)
    set_package_properties(Python3 PROPERTIES
                           DESCRIPTION "Python Interpreter"
                           URL "https://www.python.org"
                           TYPE REQUIRED)
    set(PYTHON_EXECUTABLE "${Python3_EXECUTABLE}")
endif()

# Find TelepathyLogger >= 0.8.0
set(TELEPATHY_LOGGER_MIN_VERSION 0.8.0)
find_package(TelepathyLogger ${TELEPATHY_LOGGER_MIN_VERSION} REQUIRED)
set_package_properties(TelepathyLogger PROPERTIES
                       DESCRIPTION "Daemon for logging Telepathy conversations"
                       URL "https://telepathy.freedesktop.org"
                       TYPE REQUIRED)
# Generate a helper include file that defines current TPL version
ecm_setup_version(${TELEPATHY_LOGGER_VERSION} VARIABLE_PREFIX TelepathyLogger
                  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/TelepathyLogger_version.h")

################# Targets ####################

# Prevent conflicts between Glib and Qt
add_definitions(-DQT_NO_KEYWORDS)


# Add the source subdirectories
add_subdirectory(TelepathyLoggerQt)
#add_subdirectory(tests)
#add_subdirectory(tools)


############### CMake Config Files ####################

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/TelepathyLoggerQt")

ecm_configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/TelepathyLoggerQt/TelepathyLoggerQtConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt/TelepathyLoggerQtConfig.cmake"
    INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt/TelepathyLoggerQtConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQtConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT DEVEL
)

install(EXPORT TelepathyLoggerQtTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE TelepathyLoggerQtTargets.cmake)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt_version.h"
    "${CMAKE_CURRENT_BINARY_DIR}/TelepathyLogger_version.h"
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/TelepathyLoggerQt"
    COMPONENT Devel)


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
