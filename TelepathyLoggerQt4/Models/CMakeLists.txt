# It gets inherited from the previous directory, hence it has to be removed explicitely
remove_definitions(-DBUILDING_TELEPATHY_LOGGER_QT4)
# We are building Telepathy-Qt4-Yell-Models
add_definitions(-DBUILDING_TELEPATHY_LOGGER_QT4_MODELS)

# Set source files
set(telepathy_logger_qt4_models_SRCS
    logger-conversation-model.cpp
)

set(telepathy_logger_qt4_models_MOC_SRCS
    logger-conversation-model.h
)

set(telepathy_logger_qt4_models_HEADERS
    Global
    global.h
    LoggerConversationModel
    logger-conversation-model.h
)

tpqt4_generate_mocs(${telepathy_logger_qt4_models_MOC_SRCS})

# Create the library
add_library(telepathy-logger-qt4-models STATIC ${telepathy_logger_qt4_models_SRCS} ${telepathy_logger_qt4_models_MOC_SRCS})
add_dependencies(telepathy-logger-qt4-models telepathy-logger-qt4)

# Include directories
include_directories(${TELEPATHY_QT4_YELL_INCLUDE_DIR})

# Link
target_link_libraries(telepathy-logger-qt4-models
    telepathy-logger-qt4
    #${QT_QTDBUS_LIBRARY}
    #${QT_QTCORE_LIBRARY}
    #${QT_QTDECLARATIVE_LIBRARY}
    #${GLIB2_LIBRARIES}
    #${GOBJECT_LIBRARIES}
    #${TELEPATHY_GLIB_LIBRARIES}
    #${TELEPATHY_LOGGER_LIBRARIES}
    #${TELEPATHY_LOGGER_QT4_LIBRARY}
    #${TELEPATHY_QT4_LIBRARIES}
    #${TELEPATHY_QT4_YELL_LIBRARIES}
    #${QTGLIB_LIBRARIES}
)

if (ENABLE_COMPILER_COVERAGE)
    target_link_libraries(telepathy-logger-qt4-models gcov)
endif (ENABLE_COMPILER_COVERAGE)

# Set the correct version number
set_target_properties(telepathy-logger-qt4-models PROPERTIES
	SOVERSION ${TP_LOGGER_QT4_ABI_VERSION}
	VERSION ${TP_LOGGER_QT4_LIBRARY_VERSION})

# Install the library - watch out for the correct components
if (WIN32)
    install(TARGETS telepathy-logger-qt4-models RUNTIME DESTINATION ${LIB_INSTALL_DIR} COMPONENT models
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT models_headers)
else (WIN32)
    install(TARGETS telepathy-logger-qt4-models LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT models
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT models_headers)
endif (WIN32)

# Install headers
install(FILES ${telepathy_logger_qt4_models_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/telepathy-logger-0.2/TelepathyLoggerQt4/Models
    COMPONENT models_headers)

