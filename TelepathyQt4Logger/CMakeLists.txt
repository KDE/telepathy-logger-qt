file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_gen")

set(TelepathyQt4Logger_CODEGEN_HEADERS 
    entity.h
    event.h
    log-manager.h
    text-event.h
    types.h)

set(TelepathyQt4Logger_CODEGEN_INCLUDES
    -Igst/gst.h
    -IQGlib/Quark)

# Add command to generate gen.cpp using codegen
run_codegen("TelepathyQt4Logger" "${TelepathyQt4Logger_CODEGEN_INCLUDES}" "${TelepathyQt4Logger_CODEGEN_HEADERS}")

# Set the required flags found in TelepathyDefaults
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VISIBILITY_HIDDEN_FLAGS} ${COMPILER_COVERAGE_FLAGS} ${DEPRECATED_DECLARATIONS_FLAGS}")
set(LD_FLAGS "${LD_FLAGS} ${VISIBILITY_HIDDEN_FLAGS} ${COMPILER_COVERAGE_FLAGS} ${DEPRECATED_DECLARATIONS_FLAGS}")

# We are building telepathy-qt4-logger
add_definitions(-DBUILDING_TELEPATHY_QT4_LOGGER)
add_definitions (-DQT_NO_KEYWORDS)

# Sources for Tp-Qt4-Logger
set(telepathy_qt4_logger_SRCS
    entity.cpp
    event.cpp
    log-manager.cpp
    pending-dates.cpp
    pending-entities.cpp
    pending-events.cpp
    pending-operation.cpp
    pending-search.cpp
    pending-tpcontacts.cpp
    text-event.cpp
    utils.cpp)

# Exported headers for Tp-Qt4-Logger
set(telepathy_qt4_logger_HEADERS
    Constants
    Entity
    Event
    Global
    LogManager
    PendingDates
    PendingEntities
    PendingEvents
    PendingOperation
    TextEvent
    Types
    constants.h 
    entity.h
    event.h 
    global.h
    log-manager.h
    pending-dates.h
    pending-entities.h
    pending-events.h
    pending-operation.h
    pending-search.h
    pending-tpcontacts.h
    text-event.h
    types.h
    utils.h)

# Headers file moc will be run on
set(telepathy_qt4_logger_MOC_SRCS
    pending-dates.h
    pending-entities.h
    pending-events.h
    pending-operation.h
    pending-search.h
    pending-tpcontacts.h)

# Generated headers which will be installed and exported
#set(telepathy_qt4_logger_gen_HEADERS
#    ${CMAKE_CURRENT_BINARY_DIR}/_gen/constants.h
#    ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h)

# The escape character in MSVC is ^
if(MSVC)
    set(TYPES_INCLUDE ^<TelepathyQt4Logger/Types^> )
    set(GLOBAL_INCLUDE ^<TelepathyQt4Logger/Global^> )
    set(CHANNEL_EXTRA_INCLUDES '^<TelepathyQt4Logger/Types^>,^<TelepathyQt4/Channel^>' )
else(MSVC)
    set(TYPES_INCLUDE '<TelepathyQt4Logger/Types>' )
    set(GLOBAL_INCLUDE '<TelepathyQt4Logger/Global>' )
    set(CHANNEL_EXTRA_INCLUDES '<TelepathyQt4Logger/Types>,<TelepathyQt4/Channel>' )
endif(MSVC)

# Generate the spec files
#set(gen_stable_spec_xml ${CMAKE_CURRENT_BINARY_DIR}/_gen/stable-spec.xml)

#tpqt4_xincludator(stable-ifaces-includator ${CMAKE_CURRENT_SOURCE_DIR}/stable-interfaces.xml ${gen_stable_spec_xml})

#tpqt4_constants_gen(stable-constants ${gen_stable_spec_xml} ${CMAKE_CURRENT_BINARY_DIR}/_gen/constants.h
#                    --namespace=Tpl
#                    --define-prefix=TP_QT4_LOGGER_
#                    --must-define=IN_TELEPATHY_QT4_LOGGER_HEADER
#                    DEPENDS stable-ifaces-includator)

#tpqt4_types_gen(stable-typesgen ${gen_stable_spec_xml}
#                ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h ${CMAKE_CURRENT_BINARY_DIR}/_gen/types-body.hpp
#                Tpy TelepathyQt4Logger/types.h TelepathyQt4Logger/Types
#                --must-define=IN_TELEPATHY_QT4_LOGGER_HEADER
#                --visibility=TELEPATHY_QT4_LOGGER_EXPORT
#                --extraincludes=${GLOBAL_INCLUDE}
#                DEPENDS stable-constants)

# Add the generated types to the library's sources
#list(APPEND telepathy_qt4_logger_SRCS ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h)
#list(APPEND telepathy_qt4_logger_SRCS ${CMAKE_CURRENT_BINARY_DIR}/_gen/types-body.hpp)

# For each spec, generate a cli file and add it to the sources (including mocs).
#set(SPECS
#    channel)
#foreach(spec ${SPECS})
#    tpqt4_xincludator(${spec}-spec-xincludator ${CMAKE_CURRENT_SOURCE_DIR}/${spec}.xml ${CMAKE_CURRENT_BINARY_DIR}/_gen/spec-${spec}.xml
#                      DEPENDS stable-typesgen)
#    set(NEW_FILES
#        ${CMAKE_CURRENT_BINARY_DIR}/_gen/cli-${spec}-body.hpp
#        ${CMAKE_CURRENT_BINARY_DIR}/_gen/cli-${spec}.moc.hpp)
#    list(APPEND telepathy_qt4_logger_SRCS ${NEW_FILES})
#    list(APPEND telepathy_qt4_logger_generated_specs_mocs "moc-cli-${spec}.moc.hpp")
#    set_source_files_properties(${NEW_FILES} PROPERTIES GENERATED true)
#endforeach(spec ${SPECS})

SET(telepathy_qt4_logger_generated_specs_mocs types.h)

# Use the client generator for generating headers out of specs
#tpqt4_client_generator(${gen_stable_spec_xml} channel clientchannel TelepathyQt4Logger/channel.h TelepathyQt4Logger/Channel Tpl::Client Tpl IN_TELEPATHY_QT4_LOGGER_HEADER TELEPATHY_QT4_LOGGER_EXPORT --mainiface=Tp::Client::ChannelInterface --extraincludes=${CHANNEL_EXTRA_INCLUDES} DEPENDS channel-spec-xincludator)

# Create the library
add_library(telepathy-qt4-logger STATIC ${telepathy_qt4_logger_SRCS})

# generate client moc files
foreach(moc_src ${telepathy_qt4_logger_MOC_SRCS})
    set(generated_file _gen/${moc_src})
    string(REPLACE ".h" ".moc.hpp" generated_file ${generated_file})
    tpqt4_generate_moc_i_target_deps(${CMAKE_CURRENT_SOURCE_DIR}/${moc_src} ${CMAKE_CURRENT_BINARY_DIR}/${generated_file}
                                     ${telepathy_qt4_logger_generated_specs_mocs})
    list(APPEND telepathy_qt4_logger_SRCS ${CMAKE_CURRENT_BINARY_DIR}/${generated_file})
    string(REPLACE ".h" ".moc.hpp" moc_src ${moc_src})
    add_dependencies(telepathy-qt4-logger "moc-${moc_src}")
endforeach(moc_src ${telepathy_qt4_logger_MOC_SRCS})

# Link
target_link_libraries(telepathy-qt4-logger
    ${QT_QTCORE_LIBRARY}
    ${GLIB2_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    #${QT_QTDBUS_LIBRARY}
    #${DBUS_LIBRARIES}
    #${LIBXML2_LIBRARIES}
    #${DBUS_GLIB_LIBRARIES}
    ${TELEPATHY_GLIB_LIBRARIES}
    ${TELEPATHY_LOGGER_LIBRARIES}
    ${TELEPATHY_QT4_LIBRARIES}
    ${QTGLIB_LIBRARIES})

if (ENABLE_COMPILER_COVERAGE)
    target_link_libraries(telepathy-qt4-logger gcov)
endif (ENABLE_COMPILER_COVERAGE)

# Set the correct version number
set_target_properties(telepathy-qt4-logger PROPERTIES
                      SOVERSION ${TP_QT4_LOGGER_ABI_VERSION}
                      VERSION ${TP_QT4_LOGGER_LIBRARY_VERSION})

# Install header files
install(FILES ${telepathy_qt4_logger_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/telepathy-1.0/TelepathyQt4Logger COMPONENT headers)
install(FILES ${telepathy_qt4_logger_gen_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/telepathy-1.0/TelepathyQt4Logger/_gen COMPONENT headers)

# Install the library - watch out for the correct components
if (WIN32)
    install(TARGETS telepathy-qt4-logger RUNTIME DESTINATION ${LIB_INSTALL_DIR} COMPONENT mainlibrary
                                  ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT headers)
else (WIN32)
    install(TARGETS telepathy-qt4-logger LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT mainlibrary
                                  ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT headers)
endif (WIN32)

# pkg-config file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TelepathyQt4Logger.pc.in ${CMAKE_CURRENT_BINARY_DIR}/TelepathyQt4Logger.pc)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TelepathyQt4Logger-uninstalled.pc.in ${CMAKE_CURRENT_BINARY_DIR}/TelepathyQt4Logger-uninstalled.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/TelepathyQt4Logger.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig COMPONENT headers)
