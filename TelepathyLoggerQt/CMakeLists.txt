include(TpQtMacros)
include(TpLoggerQtMacros)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_gen")

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
    object.cpp
)


####################### Codegen #######################

# The escape character in MSVC is ^
if(MSVC)
    set(TYPES_INCLUDE ^"types.h^" )
    set(GLOBAL_INCLUDE ^"global.h^" )
    set(LOGGER_EXTRA_INCLUDES ^"types.h^" )
else(MSVC)
    set(TYPES_INCLUDE '\"types.h\"' )
    set(GLOBAL_INCLUDE '\"global.h\"' )
    set(LOGGER_EXTRA_INCLUDES '\"types.h\"' )
endif(MSVC)


add_custom_target(all-generated-sources)

# Generate stable spec
set(gen_stable_spec_xml ${CMAKE_CURRENT_BINARY_DIR}/_gen/stable-spec.xml)
tpqt_xincludator(stable-ifaces-includator ${CMAKE_CURRENT_SOURCE_DIR}/stable-interfaces.xml
                 ${gen_stable_spec_xml})
tpqt_constants_gen(stable-constants ${gen_stable_spec_xml} ${CMAKE_CURRENT_BINARY_DIR}/_gen/constants.h
                   --namespace=Tpl
                   --define-prefix=TP_LOGGER_QT_
                   DEPENDS stable-ifaces-includator
)
tpqt_types_gen(stable-typesgen ${gen_stable_spec_xml}
               ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h ${CMAKE_CURRENT_BINARY_DIR}/_gen/types-body.hpp
               Tpl TelepathyLoggerQt/types.h TelepathyLoggerQt/Types
               --must-define=
               --visibility=TELEPATHY_LOGGER_QT_EXPORT
               --extraincludes=${GLOBAL_INCLUDE}
               DEPENDS stable-constants
)

# Generate logger spec
tpqt_xincludator(logger-ifaces-includator ${CMAKE_CURRENT_SOURCE_DIR}/logger.xml
                 ${CMAKE_CURRENT_BINARY_DIR}/_gen/spec-logger.xml
                 DEPENDS stable-typesgen
)
# define generate_cli-logger-body target
tpqt_client_generator(logger clientlogger log-manager.h Tpl
                      --typesnamespace=Tpl
                      --realinclude=log-manager.h
                      --prettyinclude=
                      --must-define=
                      --visibility=TELEPATHY_LOGGER_QT_EXPORT
                      DEPENDS logger-ifaces-includator
)

set(telepathy_logger_qt_GEN_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h
    ${CMAKE_CURRENT_BINARY_DIR}/_gen/cli-logger.h
)

# Run moc on the newly generated files
qt5_wrap_cpp(telepathy_logger_qt_SRCS ${telepathy_logger_qt_GEN_HEADERS})

################# Add target ##########################

add_library(telepathy-logger-qt SHARED
    ${telepathy_logger_qt_SRCS}
)
add_dependencies(telepathy-logger-qt generate_cli-logger-body)

generate_export_header(telepathy-logger-qt EXPORT_FILE_NAME TelepathyLoggerQt_export.h)

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


target_include_directories(telepathy-logger-qt PRIVATE
    $<BUILD_INTERFACE:${TelepathyLoggerQt_SOURCE_DIR}>
    $<BUILD_INTERFACE:${TelepathyLoggerQt_BINARY_DIR}>
    ${GLIB2_INCLUDE_DIR}
    ${DBUS_GLIB_INCLUDE_DIR}
    ${TELEPATHY_GLIB_INCLUDE_DIR}
    ${TELEPATHY_QT5_INCLUDE_DIR}
    ${TELEPATHY_LOGGER_INCLUDE_DIR}
)
target_include_directories(telepathy-logger-qt INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/TelepathyLoggerQt>
    ${GLIB2_INCLUDE_DIR}
    ${TELEPATHY_QT5_INCLUDE_DIR}
)


set(TELEPATHY_LOGGER_QT_DEFINITIONS  "-fexceptions")
target_compile_options(telepathy-logger-qt INTERFACE $<INSTALL_INTERFACE:${TELEPATHY_LOGGER_QT_DEFINITIONS}>)


# Set the correct version number
set_target_properties(telepathy-logger-qt PROPERTIES
    SOVERSION ${TelepathyLoggerQt_SOVERSION}
    VERSION ${TelepathyLoggerQt_VERSION_STRING}
    EXPORT_NAME TelepathyLoggerQt
)

tplqt_generate_headers(TelepathyLoggerQt_CamelCase_HEADERS
  HEADER_NAMES
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
  REQUIRED_HEADERS TelepathyLoggerQt_HEADERS
)

install(FILES
    ${TelepathyLoggerQt_HEADERS}
    ${TelepathyLoggerQt_CamelCase_HEADERS}
    "${CMAKE_CURRENT_BINARY_DIR}/TelepathyLoggerQt_export.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TelepathyLoggerQt
    COMPONENT Devel)

install(TARGETS telepathy-logger-qt
        EXPORT  TelepathyLoggerQtTargets
        ${INSTALL_TARGETS_DEFAULT_ARGS})
