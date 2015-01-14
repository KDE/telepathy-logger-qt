file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_gen")

set(TelepathyLoggerQt_CODEGEN_HEADERS
    call-event.h
    entity.h
    event.h
    log-manager.h
    log-walker.h
    text-event.h
    types.h)

set(TelepathyLoggerQt_CODEGEN_INCLUDES
    -Itelepathy-logger/call-event.h
    -Itelepathy-logger/entity.h
    -Itelepathy-logger/log-manager.h
    -Itelepathy-logger/log-walker.h
    -Itelepathy-logger/event.h
    -Itelepathy-logger/text-event.h
    -ITelepathyLoggerQt/CallEvent
    -ITelepathyLoggerQt/Entity
    -ITelepathyLoggerQt/Event
    -ITelepathyLoggerQt/LogManager
    -ITelepathyLoggerQt/LogWalker
    -ITelepathyLoggerQt/TextEvent
    -ITelepathyLoggerQt/Types
)

# Add command to generate gen.cpp using codegen
run_codegen("TelepathyLoggerQt" "${TelepathyLoggerQt_CODEGEN_INCLUDES}" "${TelepathyLoggerQt_CODEGEN_HEADERS}")

# Set the required flags found in TelepathyDefaults
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VISIBILITY_HIDDEN_FLAGS} ${COMPILER_COVERAGE_FLAGS} ${DEPRECATED_DECLARATIONS_FLAGS}")
set(LD_FLAGS "${LD_FLAGS} ${VISIBILITY_HIDDEN_FLAGS} ${COMPILER_COVERAGE_FLAGS} ${DEPRECATED_DECLARATIONS_FLAGS}")

# We are building telepathy-logger-qt
add_definitions(-DBUILDING_TELEPATHY_LOGGER_QT)
add_definitions (-DQT_NO_KEYWORDS)
add_definitions(-fPIC)

# Sources for Tp-Logger-Qt
set(telepathy_logger_qt_SRCS
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
)

# Exported headers for Tp-Logger-Qt
set(telepathy_logger_qt_HEADERS
    CallEvent
    Entity
    Event
    Global
    Init
    LogManager
    LogWalker
    Object
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
    log-walker.h
    object.h
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
set(telepathy_logger_qt_MOC_SRCS
    pending-dates.h
    pending-entities.h
    pending-events.h
    pending-clear.h
    pending-operation.h
    pending-search.h
    pending-log-walker-operation.h
    )

# Generated headers which will be installed and exported
set(telepathy_logger_qt_gen_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/_gen/cli-logger.h
    ${CMAKE_CURRENT_BINARY_DIR}/_gen/constants.h
    ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h)

# The escape character in MSVC is ^
if(MSVC)
    set(TYPES_INCLUDE ^<TelepathyLoggerQt/Types^> )
    set(GLOBAL_INCLUDE ^<TelepathyLoggerQt/Global^> )
    set(LOGGER_EXTRA_INCLUDES '^<TelepathyLoggerQt/Types^>' )
else(MSVC)
    set(TYPES_INCLUDE '<TelepathyLoggerQt/Types>' )
    set(GLOBAL_INCLUDE '<TelepathyLoggerQt/Global>' )
    set(LOGGER_EXTRA_INCLUDES '<TelepathyLoggerQt/Types>' )
endif(MSVC)

# Generate the spec files
set(gen_stable_spec_xml ${CMAKE_CURRENT_BINARY_DIR}/_gen/stable-spec.xml)

tpqt_xincludator(stable-ifaces-includator ${CMAKE_CURRENT_SOURCE_DIR}/stable-interfaces.xml ${gen_stable_spec_xml})

add_custom_target(all-generated-sources)

tpqt_constants_gen(stable-constants ${gen_stable_spec_xml} ${CMAKE_CURRENT_BINARY_DIR}/_gen/constants.h
                   --namespace=Tpl
                   --define-prefix=TP_LOGGER_QT_
                   --must-define=IN_TELEPATHY_LOGGER_QT_HEADER
                   DEPENDS stable-ifaces-includator)

tpqt_types_gen(stable-typesgen ${gen_stable_spec_xml}
               ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h ${CMAKE_CURRENT_BINARY_DIR}/_gen/types-body.hpp
               Tpl TelepathyLoggerQt/types.h TelepathyLoggerQt/Types
               --must-define=IN_TELEPATHY_LOGGER_QT_HEADER
               --visibility=TELEPATHY_LOGGER_QT_EXPORT
               --extraincludes=${GLOBAL_INCLUDE}
               DEPENDS stable-constants)

# Add the generated types to the library's sources
list(APPEND telepathy_logger_qt_SRCS ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h)
list(APPEND telepathy_logger_qt_SRCS ${CMAKE_CURRENT_BINARY_DIR}/_gen/types-body.hpp)

# For each spec, generate a cli file and add it to the sources (including mocs).
set(SPECS
    logger)
foreach(spec ${SPECS})
    tpqt_xincludator(${spec}-spec-xincludator ${CMAKE_CURRENT_SOURCE_DIR}/${spec}.xml ${CMAKE_CURRENT_BINARY_DIR}/_gen/spec-${spec}.xml DEPENDS stable-typesgen)
    set(NEW_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/_gen/cli-${spec}-body.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/_gen/cli-${spec}.moc.hpp)
    list(APPEND telepathy_logger_qt_SRCS ${NEW_FILES})
    list(APPEND telepathy_logger_qt_generated_specs_mocs "moc-cli-${spec}.moc.hpp")
    set_source_files_properties(${NEW_FILES} PROPERTIES GENERATED true)
endforeach(spec ${SPECS})

#SET(telepathy_logger_qt_generated_specs_mocs types.h)

# Use the client generator for generating headers out of specs
tpqt_client_generator(logger clientlogger TelepathyLoggerQt/LogManager Tpl
                      --typesnamespace=Tpl
                      --realinclude=TelepathyLoggerQt/log-manager.h
                      --prettyinclude=TelepathyLoggerQt/LogManager
                      --must-define=IN_TELEPATHY_LOGGER_QT_HEADER
                      --visibility=TELEPATHY_LOGGER_QT_EXPORT
                      DEPENDS logger-spec-xincludator )

# Create the library
add_library(telepathy-logger-qt SHARED ${telepathy_logger_qt_SRCS})

# generate client moc files
foreach(moc_src ${telepathy_logger_qt_MOC_SRCS})
    set(generated_file _gen/${moc_src})
    string(REPLACE ".h" ".moc.hpp" generated_file ${generated_file})
    tpqt_generate_moc_i_target_deps(${CMAKE_CURRENT_SOURCE_DIR}/${moc_src} ${CMAKE_CURRENT_BINARY_DIR}/${generated_file}
                                    ${telepathy_logger_qt_generated_specs_mocs})
    list(APPEND telepathy_logger_qt_SRCS ${CMAKE_CURRENT_BINARY_DIR}/${generated_file})
    string(REPLACE ".h" ".moc.hpp" moc_src ${moc_src})
    add_dependencies(telepathy-logger-qt "moc-${moc_src}")
endforeach(moc_src ${telepathy_logger_qt_MOC_SRCS})

# Link
target_link_libraries(telepathy-logger-qt
LINK_PUBLIC
    Qt5::Core
    Qt5::DBus
    ${TELEPATHY_QT5_LIBRARIES}
LINK_PRIVATE
    ${GLIB2_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    -lgio-2.0
    ${QT_QTDBUS_LIBRARY}
    ${TELEPATHY_GLIB_LIBRARIES}
    ${TELEPATHY_LOGGER_LIBRARIES}
)

if (ENABLE_COMPILER_COVERAGE)
    target_link_libraries(telepathy-logger-qt gcov)
endif (ENABLE_COMPILER_COVERAGE)

# Set the correct version number
set_target_properties(telepathy-logger-qt PROPERTIES
                      SOVERSION ${TP_LOGGER_QT_ABI_VERSION}
                      VERSION ${TP_LOGGER_QT_LIBRARY_VERSION})

# Install header files
install(FILES ${telepathy_logger_qt_HEADERS}
        DESTINATION ${INCLUDE_DESTINATION}/telepathy-logger-0.2/TelepathyLoggerQt
        COMPONENT headers)
install(FILES ${telepathy_logger_qt_gen_HEADERS}
        DESTINATION ${INCLUDE_DESTINATION}/telepathy-logger-0.2/TelepathyLoggerQt/_gen
        COMPONENT headers)

# Install the library - watch out for the correct components
if (WIN32)
    install(TARGETS telepathy-logger-qt
            EXPORT  TelepathyLoggerQtTargets
            RUNTIME DESTINATION ${LIB_DESTINATION} COMPONENT mainlibrary
            ARCHIVE DESTINATION ${LIB_DESTINATION} COMPONENT headers)
else (WIN32)
    install(TARGETS telepathy-logger-qt
            EXPORT  TelepathyLoggerQtTargets
            LIBRARY DESTINATION ${LIB_DESTINATION} COMPONENT mainlibrary
            ARCHIVE DESTINATION ${LIB_DESTINATION} COMPONENT headers)
endif (WIN32)
target_include_directories(telepathy-logger-qt PUBLIC $<INSTALL_INTERFACE:${INCLUDE_DESTINATION}/telepathy-logger-0.2/> ${GLIB2_INCLUDE_DIR})
set(TELEPATHY_LOGGER_QT_DEFINITIONS  "-fexceptions")
target_compile_options(telepathy-logger-qt INTERFACE $<INSTALL_INTERFACE:${TELEPATHY_LOGGER_QT_DEFINITIONS}>)

# pkg-config file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TelepathyLoggerQt.pc.in ${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt.pc)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TelepathyLoggerQt-uninstalled.pc.in ${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt-uninstalled.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt.pc DESTINATION ${LIB_DESTINATION}/pkgconfig COMPONENT headers)

# cmake file

include(TpQtMacros)
# CMake Version and config files
include(MacroWriteBasicCMakeVersionFile)

configure_file(TelepathyLoggerQtConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQtConfig.cmake" @ONLY)

# this file is used by to check if the installed version can be used.
macro_write_basic_cmake_version_file(${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQtConfigVersion.cmake
                                     ${PACKAGE_VERSION})


install(EXPORT TelepathyLoggerQtTargets
        DESTINATION ${LIB_DESTINATION}/cmake/TelepathyLoggerQt
        COMPONENT headers)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQtConfigVersion.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQtConfig.cmake
        DESTINATION ${LIB_DESTINATION}/cmake/TelepathyLoggerQt
        COMPONENT headers)
