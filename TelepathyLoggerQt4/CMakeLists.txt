file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_gen")

set(TelepathyLoggerQt4_CODEGEN_HEADERS
    call-event.h
    entity.h
    event.h
    log-manager.h
    text-event.h
    types.h)

set(TelepathyLoggerQt4_CODEGEN_INCLUDES
    -Itelepathy-logger/call-event.h
    -Itelepathy-logger/entity.h
    -Itelepathy-logger/log-manager.h
    -Itelepathy-logger/event.h
    -Itelepathy-logger/text-event.h
    -ITelepathyLoggerQt4/CallEvent
    -ITelepathyLoggerQt4/Entity
    -ITelepathyLoggerQt4/Event
    -ITelepathyLoggerQt4/LogManager
    -ITelepathyLoggerQt4/TextEvent
    -ITelepathyLoggerQt4/Types
    -IQGlib/Quark)

# Add command to generate gen.cpp using codegen
run_codegen("TelepathyLoggerQt4" "${TelepathyLoggerQt4_CODEGEN_INCLUDES}" "${TelepathyLoggerQt4_CODEGEN_HEADERS}")

# Set the required flags found in TelepathyDefaults
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VISIBILITY_HIDDEN_FLAGS} ${COMPILER_COVERAGE_FLAGS} ${DEPRECATED_DECLARATIONS_FLAGS}")
set(LD_FLAGS "${LD_FLAGS} ${VISIBILITY_HIDDEN_FLAGS} ${COMPILER_COVERAGE_FLAGS} ${DEPRECATED_DECLARATIONS_FLAGS}")

# We are building telepathy-logger-qt4
add_definitions(-DBUILDING_TELEPATHY_LOGGER_QT4)
add_definitions (-DQT_NO_KEYWORDS)
add_definitions(-fPIC)

# Sources for Tp-Logger-Qt4
set(telepathy_logger_qt4_SRCS
    call-event.cpp
    entity.cpp
    event.cpp
    log-manager.cpp
    log-walker.cpp
    pending-dates.cpp
    pending-entities.cpp
    pending-events.cpp
    pending-clear.cpp
    pending-operation.cpp
    pending-search.cpp
    pending-log-walker-operation.cpp
    search-hit.cpp
    text-event.cpp
    utils.cpp
    init.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/gen.cpp)

# Exported headers for Tp-Logger-Qt4
set(telepathy_logger_qt4_HEADERS
    CallEvent
    Entity
    Event
    Global
    Init
    LogManager
    LogWalker
    PendingDates
    PendingEntities
    PendingEvents
    PendingOperation
    PendingSearch
    SearchHit
    TextEvent
    Types
    call-event.h
    entity.h
    event.h
    global.h
    init.h
    log-manager.h
    pending-dates.h
    pending-entities.h
    pending-events.h
    pending-operation.h
    pending-search.h
    search-hit.h
    text-event.h
    types.h
    )

# Headers file moc will be run on
set(telepathy_logger_qt4_MOC_SRCS
    pending-dates.h
    pending-entities.h
    pending-events.h
    pending-clear.h
    pending-operation.h
    pending-search.h
    pending-log-walker-operation.h
    )

# Generated headers which will be installed and exported
set(telepathy_logger_qt4_gen_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/_gen/cli-logger.h
    ${CMAKE_CURRENT_BINARY_DIR}/_gen/constants.h
    ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h)

# The escape character in MSVC is ^
if(MSVC)
    set(TYPES_INCLUDE ^<TelepathyLoggerQt4/Types^> )
    set(GLOBAL_INCLUDE ^<TelepathyLoggerQt4/Global^> )
    set(LOGGER_EXTRA_INCLUDES '^<TelepathyLoggerQt4/Types^>' )
else(MSVC)
    set(TYPES_INCLUDE '<TelepathyLoggerQt4/Types>' )
    set(GLOBAL_INCLUDE '<TelepathyLoggerQt4/Global>' )
    set(LOGGER_EXTRA_INCLUDES '<TelepathyLoggerQt4/Types>' )
endif(MSVC)

# Generate the spec files
set(gen_stable_spec_xml ${CMAKE_CURRENT_BINARY_DIR}/_gen/stable-spec.xml)

tpqt_xincludator(stable-ifaces-includator ${CMAKE_CURRENT_SOURCE_DIR}/stable-interfaces.xml ${gen_stable_spec_xml})

add_custom_target(all-generated-sources)

tpqt_constants_gen(stable-constants ${gen_stable_spec_xml} ${CMAKE_CURRENT_BINARY_DIR}/_gen/constants.h
                   --namespace=Tpl
                   --define-prefix=TP_LOGGER_QT4_
                   --must-define=IN_TELEPATHY_LOGGER_QT4_HEADER
                   DEPENDS stable-ifaces-includator)

tpqt_types_gen(stable-typesgen ${gen_stable_spec_xml}
               ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h ${CMAKE_CURRENT_BINARY_DIR}/_gen/types-body.hpp
               Tpl TelepathyLoggerQt4/types.h TelepathyLoggerQt4/Types
               --must-define=IN_TELEPATHY_LOGGER_QT4_HEADER
               --visibility=TELEPATHY_LOGGER_QT4_EXPORT
               --extraincludes=${GLOBAL_INCLUDE}
               DEPENDS stable-constants)

# Add the generated types to the library's sources
list(APPEND telepathy_logger_qt4_SRCS ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h)
list(APPEND telepathy_logger_qt4_SRCS ${CMAKE_CURRENT_BINARY_DIR}/_gen/types-body.hpp)

# For each spec, generate a cli file and add it to the sources (including mocs).
set(SPECS
    logger)
foreach(spec ${SPECS})
    tpqt_xincludator(${spec}-spec-xincludator ${CMAKE_CURRENT_SOURCE_DIR}/${spec}.xml ${CMAKE_CURRENT_BINARY_DIR}/_gen/spec-${spec}.xml DEPENDS stable-typesgen)
    set(NEW_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/_gen/cli-${spec}-body.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/_gen/cli-${spec}.moc.hpp)
    list(APPEND telepathy_logger_qt4_SRCS ${NEW_FILES})
    list(APPEND telepathy_logger_qt4_generated_specs_mocs "moc-cli-${spec}.moc.hpp")
    set_source_files_properties(${NEW_FILES} PROPERTIES GENERATED true)
endforeach(spec ${SPECS})

#SET(telepathy_logger_qt4_generated_specs_mocs types.h)

# Use the client generator for generating headers out of specs
tpqt_client_generator(logger clientlogger TelepathyLoggerQt4/LogManager Tpl
                      --typesnamespace=Tpl
                      --realinclude=TelepathyLoggerQt4/log-manager.h
                      --prettyinclude=TelepathyLoggerQt4/LogManager
                      --must-define=IN_TELEPATHY_LOGGER_QT4_HEADER
                      --visibility=TELEPATHY_LOGGER_QT4_EXPORT
                      DEPENDS logger-spec-xincludator )

# Create the library
add_library(telepathy-logger-qt4 SHARED ${telepathy_logger_qt4_SRCS})

# generate client moc files
foreach(moc_src ${telepathy_logger_qt4_MOC_SRCS})
    set(generated_file _gen/${moc_src})
    string(REPLACE ".h" ".moc.hpp" generated_file ${generated_file})
    tpqt_generate_moc_i_target_deps(${CMAKE_CURRENT_SOURCE_DIR}/${moc_src} ${CMAKE_CURRENT_BINARY_DIR}/${generated_file}
                                    ${telepathy_logger_qt4_generated_specs_mocs})
    list(APPEND telepathy_logger_qt4_SRCS ${CMAKE_CURRENT_BINARY_DIR}/${generated_file})
    string(REPLACE ".h" ".moc.hpp" moc_src ${moc_src})
    add_dependencies(telepathy-logger-qt4 "moc-${moc_src}")
endforeach(moc_src ${telepathy_logger_qt4_MOC_SRCS})

# Link
target_link_libraries(telepathy-logger-qt4
    ${QT_QTCORE_LIBRARY}
    ${GLIB2_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    -lgio-2.0
    ${QT_QTDBUS_LIBRARY}
    ${DBUS_LIBRARIES}
    ${LIBXML2_LIBRARIES}
    ${DBUS_GLIB_LIBRARIES}
    ${TELEPATHY_GLIB_LIBRARIES}
    ${TELEPATHY_LOGGER_LIBRARIES}
    ${TELEPATHY_QT4_LIBRARIES}
    ${QTGLIB_LIBRARIES})

if (ENABLE_COMPILER_COVERAGE)
    target_link_libraries(telepathy-logger-qt4 gcov)
endif (ENABLE_COMPILER_COVERAGE)

# Set the correct version number
set_target_properties(telepathy-logger-qt4 PROPERTIES
                      SOVERSION ${TP_LOGGER_QT4_ABI_VERSION}
                      VERSION ${TP_LOGGER_QT4_LIBRARY_VERSION})

# Install header files
install(FILES ${telepathy_logger_qt4_HEADERS}
        DESTINATION ${INCLUDE_INSTALL_DIR}/telepathy-logger-0.2/TelepathyLoggerQt4
        COMPONENT headers)
install(FILES ${telepathy_logger_qt4_gen_HEADERS}
        DESTINATION ${INCLUDE_INSTALL_DIR}/telepathy-logger-0.2/TelepathyLoggerQt4/_gen
        COMPONENT headers)

# Install the library - watch out for the correct components
if (WIN32)
    install(TARGETS telepathy-logger-qt4
            EXPORT  TelepathyLoggerQt4Targets
            RUNTIME DESTINATION ${LIB_INSTALL_DIR} COMPONENT mainlibrary
            ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT headers)
else (WIN32)
    install(TARGETS telepathy-logger-qt4
            EXPORT  TelepathyLoggerQt4Targets
            LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT mainlibrary
            ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT headers)
endif (WIN32)

# pkg-config file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TelepathyLoggerQt4.pc.in ${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt4.pc)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TelepathyLoggerQt4-uninstalled.pc.in ${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt4-uninstalled.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt4.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig COMPONENT headers)

# cmake file

include(TpQtMacros)
# CMake Version and config files
include(MacroWriteBasicCMakeVersionFile)


macro(make_install_path_absolute out in)
   if (IS_ABSOLUTE "${in}")
      set(${out} "${in}")
   else (IS_ABSOLUTE "${in}")
      set(${out} "\${TELEPATHY_LOGGER_QT4_INSTALL_DIR}/${in}")
   endif (IS_ABSOLUTE "${in}")
endmacro(make_install_path_absolute out in)


# all the following variables are put into TelepathyLoggerQt4Config.cmake, so
# they are usable by projects using TelepathyLoggerQt4.
make_install_path_absolute(TELEPATHY_LOGGER_QT4_INCLUDE_DIR  ${INCLUDE_INSTALL_DIR}/telepathy-logger-0.2)
set(TELEPATHY_LOGGER_QT4_DEFINITIONS  "-fexceptions")

configure_file(TelepathyLoggerQt4Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt4Config.cmake" @ONLY)

# this file is used by to check if the installed version can be used.
macro_write_basic_cmake_version_file(${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt4ConfigVersion.cmake
                                     ${PACKAGE_VERSION})


install(EXPORT TelepathyLoggerQt4Targets
        DESTINATION $${LIB_INSTALL_DIR}/cmake/TelepathyLoggerQt4
        COMPONENT headers)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt4ConfigVersion.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt4Config.cmake
        DESTINATION $${LIB_INSTALL_DIR}/cmake/TelepathyLoggerQt4
        COMPONENT headers)
