# It gets inherited from the previous directory, hence it has to be removed explicitely
remove_definitions(-DBUILDING_TELEPATHY_QT4_LOGGER)
# We are building Telepathy-Qt4-Yell-Models
add_definitions(-DBUILDING_TELEPATHY_QT4_LOGGER_MODELS)

# Set source files
set(telepathy_qt4_logger_models_SRCS
    logger-conversation-model.cpp
)

set(telepathy_qt4_logger_models_MOC_SRCS
    logger-conversation-model.h
)

set(telepathy_qt4_logger_models_HEADERS
    Global
    global.h
    LoggerConversationModel
    logger-conversation-model.h
)

tpqt4_generate_mocs(${telepathy_qt4_logger_models_MOC_SRCS})

# Create the library
add_library(telepathy-qt4-logger-models STATIC ${telepathy_qt4_logger_models_SRCS} ${telepathy_qt4_logger_models_MOC_SRCS})
add_dependencies(telepathy-qt4-logger-models telepathy-qt4-logger)

# Link
target_link_libraries(telepathy-qt4-logger-models
    telepathy-qt4-logger
    #${QT_QTDBUS_LIBRARY}
    #${QT_QTCORE_LIBRARY}
    #${QT_QTDECLARATIVE_LIBRARY}
    #${GLIB2_LIBRARIES}
    #${GOBJECT_LIBRARIES}
    #${TELEPATHY_GLIB_LIBRARIES}
    #${TELEPATHY_LOGGER_LIBRARIES}
    #${TELEPATHY_QT4_LOGGER_LIBRARY}
    #${TELEPATHY_QT4_LIBRARIES}
    #${QTGLIB_LIBRARIES}
)

if (ENABLE_COMPILER_COVERAGE)
    target_link_libraries(telepathy-qt4-logger-models gcov)
endif (ENABLE_COMPILER_COVERAGE)

# Set the correct version number
set_target_properties(telepathy-qt4-logger-models PROPERTIES
	SOVERSION ${TP_QT4_LOGGER_ABI_VERSION}
	VERSION ${TP_QT4_LOGGER_LIBRARY_VERSION})

# Install the library - watch out for the correct components
if (WIN32)
    install(TARGETS telepathy-qt4-logger-models RUNTIME DESTINATION ${LIB_INSTALL_DIR} COMPONENT models
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT models_headers)
else (WIN32)
    install(TARGETS telepathy-qt4-logger-models LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT models
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT models_headers)
endif (WIN32)

# Install headers
install(FILES ${telepathy_qt4_logger_models_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/telepathy-1.0/TelepathyQt4Logger/Models
    COMPONENT models_headers)

